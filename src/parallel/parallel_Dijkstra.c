#include<mpi.h>
#include <stdio.h>
#include <stdlib.h>

int* inputRoadNet(char** argv, int* n)
{
    FILE *f;
    int nodes, edges;
    char line[100];

    f = fopen(argv[1], "r");
    if(f == NULL){
        perror("cannot open file");
        exit(EXIT_FAILURE);
    }
    printf("File opened!\n");

    //skip two lines
    fscanf(f, "%*[^\n]\n");
    fscanf(f, "%*[^\n]\n");
    //read the number of nodes and edges
    fscanf(f, "%*[^:]:");
    fscanf(f, "%d", &nodes);
    fscanf(f, "%*[^:]:");
    fscanf(f, "%d", &edges);

    printf("nodes: %d\tedges: %d\n", nodes, edges);
    *n = nodes;

    //skip one (two? dont know why) line(s)
    fscanf(f, "%*[^\n]\n");
    fscanf(f, "%*[^\n]\n");

    int* mat = malloc(nodes * nodes * sizeof(int));

    int u,v,cost;
    while(fgets(line, sizeof(line), f) != NULL){
        sscanf(line, "%d\t%d\t%d\n", &u, &v, &cost);
        mat[u*nodes + v] = cost;
    }
    printf("File scanned successfully!\n");
    return mat;
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
    int n_col_per_proc;
    struct star* s = NULL;
    int* sizes;
    int * globalptr = NULL;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int proc0_subsizes[world_size];

    if(world_rank == 0){
        s = malloc(sizeof(struct star));
        initStar(s, 8, 6);
        setStar(s, 0, 1, 3);
        setStar(s, 0, 2, 2);
        setStar(s, 1, 3, 5);
        setStar(s, 2, 3, 7);
        setStar(s, 2, 4, 3);
        setStar(s, 3, 5, 4);
        setStar(s, 4, 5, 1);
        setStar(s, 5, 4, 8);
        if (s->n_nodes % world_size != 0){
            perror("Error: number of processors must be dividable by the number of nodes");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_UNKNOWN);
        }
        n_col_per_proc = s->n_nodes / world_size; // condition: number of processors must be a divider to number of nodes (for now)

        int temp;
        for(int proc=0; proc<world_size; proc++){
            temp = calculateLocalSize(s, proc * n_col_per_proc, proc * n_col_per_proc + n_col_per_proc); 
            proc0_subsizes[proc] = temp;
            MPI_Send(&temp, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
        globalptr = &(s->to[0]);
        printf("<--- STAR --->");
        printStar(s);
    }

    MPI_Recv(&localsize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("rank :%d local size: %d\n, ", world_rank, localsize);

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
        MPI_Datatype arraytypes[world_size];
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

    int* localdata = malloc(sizeof(int) * localsize);

    /*
    int MPI_Alltoallw(const void *sendbuf, const int sendcounts[],
                  const int sdispls[], const MPI_Datatype sendtypes[],
                  void *recvbuf, const int recvcounts[], const int rdispls[],
                  const MPI_Datatype recvtypes[], MPI_Comm comm)*/

    MPI_Alltoallw(globalptr, sendcounts, senddispls, sendtypes,
                  &(localdata[0]), recvcounts, recvdispls, recvtypes, 
                  MPI_COMM_WORLD);

    for (int proc=0; proc<world_size; proc++) {
        if (proc == world_rank) {
            printf("\nRank %d:\n", proc);
            printarray(localdata, localsize);
        }
        MPI_Barrier(MPI_COMM_WORLD);            
    }
    
    MPI_Finalize();
    return 0;
}