#include<mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/utils.h"

void dijkstra(int to[], int costs[], int localsize, int n, int n_col_per_proc, int offsets[], MPI_Comm comm){
    int rank;
    MPI_Comm_rank(comm, &rank);
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
    int* globalptr_to, *globalptr_costs, *globalptr_first = NULL;
    
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

    int* localdata_to = malloc(sizeof(int) * localsize);
    int* localdata_costs = malloc(sizeof(int) * localsize);

    /*
    int MPI_Alltoallw(const void *sendbuf, const int sendcounts[],
                  const int sdispls[], const MPI_Datatype sendtypes[],
                  void *recvbuf, const int recvcounts[], const int rdispls[],
                  const MPI_Datatype recvtypes[], MPI_Comm comm)*/

    MPI_Alltoallw(globalptr_to, sendcounts, senddispls, sendtypes,
                  &(localdata_to[0]), recvcounts, recvdispls, recvtypes, 
                  MPI_COMM_WORLD);
    
    MPI_Alltoallw(globalptr_costs, sendcounts, senddispls, sendtypes,
                  &(localdata_costs[0]), recvcounts, recvdispls, recvtypes, 
                  MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    dijkstra(localdata_to, localdata_costs, localsize, n, n_col_per_proc, offsets, MPI_COMM_WORLD);

    for (int proc=0; proc<world_size; proc++) {
        if (proc == world_rank) {
            printf("\nRANK %d\n", proc);
            printf("To: ");
            printarray(localdata_to, localsize);
            printf("Costs: ");
            printarray(localdata_costs, localsize);
        }
        MPI_Barrier(MPI_COMM_WORLD);            
    }
    
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