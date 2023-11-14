#include<mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/utils.h"

void Print_dists(int global_dist[], int n) {
    int v;

    printf("  v    dist 0->v\n");
    printf("----   ---------\n");

    for (v = 1; v < n; v++) {
        if (global_dist[v] == INFTY) {
            printf("%3d       %5s\n", v, "inf");
        }
        else
            printf("%3d       %4d\n", v, global_dist[v]);
        }
    printf("\n");
}

// TODO: integrate custom source node
void dijkstra(struct Graph* G_local, int source, int n, MPI_Comm comm){
    int rank, i, j, min, h;
    int localmin[2];
    int globalmin[2];
    MPI_Comm_rank(comm, &rank);

    printf("rank: %d\there 1\n", rank);

    MPI_Barrier(MPI_COMM_WORLD);

    // initialize minimum cost found by Dijkstra with the cost from `s` to `i`
    for(i=0; i<G_local->N; i++){
        if(getCost(G_local->s, source, i) != -1){
            G_local->L[i] = getCost(G_local->s, source, i);
        }
    }

    // initialization of the first node
    if(rank == 0){
        G_local->flag[0] =  VISITED;
    }

    printf("rank: %d\there\n", rank);

    MPI_Barrier(MPI_COMM_WORLD);

    for(i=0; i < n - 1; i++){
        min = INFTY;
        h = -1;
        // identify h = argmin{L[j] : j not in S}
        // finds the minimum in the vertices assigned to the process
        for(j=0; j< G_local->N; j++){
            if(G_local->flag[j] == NOT_VISITED && G_local->L[j] < min){
                min = G_local->L[j];
                h = j;
            }
        }

        localmin[0] = min;
        localmin[1] = (h == -1) ? -1 : rank * G_local->N + h;

        // as described here: https://rookiehpc.org/mpi/docs/mpi_minloc/index.html
        MPI_Allreduce(localmin, globalmin, 1, MPI_2INT, MPI_MINLOC, comm);

        // check if the vertex belongs to the assigned nodes of the process
        // if so, mark the node as VISITED
        if(globalmin[1] / G_local->N == rank){
            h = globalmin[1] % G_local->N;
            G_local->flag[h] = VISITED;
        }

        // update the minimum costs
        for(j=0; j<G_local->N; j++){
            if(h != -1 && getCost(G_local->s, h, j) != NO_CONN){
                if(G_local->flag[j] == NOT_VISITED && G_local->L[h] + getCost(G_local->s, h, j) < G_local->L[j]){
                    G_local->L[j] = G_local->L[h] + getCost(G_local->s, h, j);
                }
            } 
        }
    }
}

int calculateLocalSize(struct star* s, int start_node, int end_node){
    int total = 0;
    for(int i=start_node; i<end_node; i++){
        total += getNumEdgesFrom(s, i);
    }
    return total;
}

// NOTE: number of nodes divisible by number of processes
int main(int argc, char *argv[]){
    int world_rank, world_size;
    int localsize;
    int n_col_per_proc, n;
    struct Graph* G;
    struct Graph* local_G;
    int* globalptr_to, *globalptr_costs, *globalptr_first = NULL;
    int* globaldist = NULL;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int proc0_subsizes[world_size];
    MPI_Datatype arraytypes[world_size];

    if(world_rank == 0){
        G = malloc(sizeof(struct Graph));

        inputRoadNet(G, argv, false);
        n = G->s->n_nodes;

        if (n % world_size != 0){
            perror("Error: number of processors must be divisible by the number of nodes");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_UNKNOWN);
        }
        n_col_per_proc = n / world_size;

        int temp;
        for(int proc=0; proc<world_size; proc++){
            temp = calculateLocalSize(G->s, proc * n_col_per_proc, proc * n_col_per_proc + n_col_per_proc); 
            proc0_subsizes[proc] = temp;
            MPI_Send(&temp, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
        globalptr_to = &(G->s->to[0]);
        globalptr_costs = &(G->s->cost[0]);
        globalptr_first = &(G->s->first[0]);
        printf("<--- STAR --->");
        printStar(G->s);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(&n_col_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("rank: %d n_col:%d n:%d\n", world_rank, n_col_per_proc, n);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Recv(&localsize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("rank :%d local size: %d\n, ", world_rank, localsize);
    MPI_Barrier(MPI_COMM_WORLD);

    int offsets[n_col_per_proc];
    MPI_Scatter(globalptr_first, n_col_per_proc, MPI_INT, &offsets, n_col_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
    printf("rank: %d offsets: ", world_rank);
    printarray(offsets, n_col_per_proc);
    MPI_Barrier(MPI_COMM_WORLD);

    int sendcounts[ world_size ];
    int senddispls[ world_size ];
    MPI_Datatype sendtypes[world_size];
    int recvcounts[ world_size ];
    int recvdispls[ world_size ];
    MPI_Datatype recvtypes[world_size];

    for (int proc=0; proc<world_size; proc++) {
        recvcounts[proc] = 0;
        recvdispls[proc] = 0;
        recvtypes[proc] = MPI_INT;

        sendcounts[proc] = 0;
        senddispls[proc] = 0;
        sendtypes[proc] = MPI_INT;
    }
    recvcounts[0] = localsize;
    recvdispls[0] = 0;


    if (world_rank == 0) {
        int subsize[1];
        int start[1] = {0};
        for(int i=0; i<world_size; i++){
            subsize[0] = proc0_subsizes[i];
            MPI_Type_create_subarray(1, proc0_subsizes, subsize, start, MPI_ORDER_C, MPI_INT, &arraytypes[i]);
            MPI_Type_commit(&arraytypes[i]);
        }
    
        int disp = 0;
        for(int proc=0; proc<world_size; proc++){
            sendcounts[proc] = 1; 

            senddispls[proc] = disp*sizeof(int);
            disp += proc0_subsizes[proc];

            sendtypes[proc] = arraytypes[proc];
        }
    }            

    // SEND AND RECEIVE DATA
    int* localdata_to = malloc(sizeof(int) * localsize);
    int* localdata_costs = malloc(sizeof(int) * localsize);
    int* localdata_first = malloc(sizeof(int) * n_col_per_proc);

    MPI_Alltoallw(globalptr_to, sendcounts, senddispls, sendtypes,
                  &(localdata_to[0]), recvcounts, recvdispls, recvtypes, 
                  MPI_COMM_WORLD);
    
    MPI_Alltoallw(globalptr_costs, sendcounts, senddispls, sendtypes,
                  &(localdata_costs[0]), recvcounts, recvdispls, recvtypes, 
                  MPI_COMM_WORLD);
    
    MPI_Scatter(globalptr_first, n_col_per_proc, MPI_INT, &(localdata_first[0]), n_col_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    // CREATE A NEW STRUCT WITH THE DATA
    local_G = malloc(sizeof(struct Graph));
    initializeGraph(local_G, n_col_per_proc, localsize, localdata_first, localdata_to, localdata_costs);

    MPI_Barrier(MPI_COMM_WORLD);

    for (int proc=0; proc<world_size; proc++) {
        if (proc == world_rank) {
            printf("\nRANK %d\n", proc);
            printGraph(local_G);
        }
        MPI_Barrier(MPI_COMM_WORLD);            
    }

    // RUN DIJKSTRA
    dijkstra(local_G, 0, n, MPI_COMM_WORLD);

    // GATHER THE RESULTS
    // TODO: throws segmentation fault here
    // probably because the receive count is not correct (variable for each process)
    MPI_Gather(local_G->L, G->N, MPI_INT, globaldist, G->N, MPI_INT, 0, MPI_COMM_WORLD);

    // PRINT RESULTS
    if(world_rank == 0){
        Print_dists(globaldist, n);
    }
    
    // FREE ALL RESOURCES
    free(localdata_costs);
    free(localdata_to);
    if(world_rank == 0){
        for(int proc=0; proc<world_size; proc++){
            MPI_Type_free(&arraytypes[proc]);
        }
        freeStar(G->s);
        free(G);
    }
    MPI_Finalize();
    return 0;
}