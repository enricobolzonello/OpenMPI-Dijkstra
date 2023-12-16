#include <mpi.h>
#include "../utils/matrixutils.h"

// TODO: include custom source
void dijkstra(int** localdata, int n, int n_col_per_proc, int* L, int* pred, MPI_Comm comm)
{
    int rank, i, j, min, h;
    int localmin[2];
    int globalmin[2];
    MPI_Comm_rank(comm, &rank);

    int* flag;
    flag = calloc(n_col_per_proc, sizeof(int));

    // initialization of the first node
    if (rank == 0)
    {
        flag[0] = VISITED;
    }

    for (i = 0; i < n - 1; i++)
    {
        min = INFTY;
        h = -1;
        // identify h = argmin{L[j] : j not in S}
        // finds the minimum in the vertices assigned to the process
        for (j = 0; j < n_col_per_proc; j++)
        {
            if (flag[j] == NOT_VISITED && L[j] < min)
            {
                min = L[j];
                h = j;
            }
        }

        localmin[0] = min;
        localmin[1] = (h == -1) ? -1 : rank * n_col_per_proc + h;

        // as described here: https://rookiehpc.org/mpi/docs/mpi_minloc/index.html
        MPI_Allreduce(localmin, globalmin, 1, MPI_2INT, MPI_MINLOC, comm);

        // check if the vertex belongs to the assigned nodes of the process
        // if so, mark the node as VISITED
        if (globalmin[1] / n_col_per_proc == rank)
        {
            h = globalmin[1] % n_col_per_proc;
            flag[h] = VISITED;
        }

        // update the minimum costs
        for (j = 0; j < n_col_per_proc; j++)
        {
            if (localdata[globalmin[1]][j] != INFTY)
            {
                if (flag[j] == NOT_VISITED && globalmin[0] + localdata[globalmin[1]][j] < L[j])
                {
                    L[j] = globalmin[0] + localdata[globalmin[1]][j];
                    pred[j] = globalmin[1];
                }
            }
        }
    }
}

int main(int argc, char *argv[]){
    int world_rank, world_size;
    int** M = NULL;
    int** localdata = NULL;
    int n_col_per_proc, n;
    void* sendptr = NULL;
    int *localdist, *localpred;
    int* globaldist, *globalpred;
    double t0, t1;

    MPI_Datatype sendvec = NULL, sendtype = NULL, recvec, rectype;

    FILE* f = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_rank == 0){
        f = fopen(argv[1], "r");
        if(f == NULL){
            perror("cannot open file");
            exit(EXIT_FAILURE);
        }
        printf("File opened!\n");

        n = random_getN(f);
        printf("n: %d\n", n);
        M = alloc2d(n, n);
        initalizeMatrix(M, INFTY, n, n);
        sendptr = &(M[0][0]);
        edgelist_matrix(M, f, true);

        fclose(f);

        // print matrix
        // printmatrix(M, n, n);

        if(n % world_size != 0){
            perror("Error: number of processors must be divisible by the number of nodes");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_UNKNOWN);
        }

        n_col_per_proc = n / world_size;

        globaldist = malloc(n * sizeof(int));
        globalpred = malloc(n * sizeof(int));
    }

    MPI_Bcast(&n_col_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    localdata = alloc2d(n, n_col_per_proc);

    // create MPI datatype for scattering matrix columns

    t0 = MPI_Wtime();

    // create send type
    if (world_rank == 0)
    {

        //
        // |<----->|  block length = 1
        //
        // +-------+-------+---+---+---+-------+-------+---+---+---+-------+-------+---+---+---+
        // | [0,0] | [0,1] |   |   |   | [0,1] | [1,1] |   |   |   | [0,2] | [1,2] |   |   |   |
        // +-------+-------+---+---+---+-------+-------+---+---+---+-------+-------+---+---+---+
        //
        // |<------------------------->| stride = n
        //
        //
        MPI_Type_vector(n, 1, n, MPI_INT, &sendvec);

        MPI_Type_commit(&sendvec);
        MPI_Type_create_resized(sendvec, 0, 1 * sizeof(int), &sendtype);
    }

    MPI_Type_vector(n, 1, n_col_per_proc, MPI_INT, &recvec);
    MPI_Type_commit(&recvec);
    MPI_Type_create_resized(recvec, 0, 1 * sizeof(int), &rectype);
    MPI_Type_commit(&rectype);

    MPI_Scatter(sendptr, n_col_per_proc, sendtype, &(localdata[0][0]), n_col_per_proc, rectype, 0, MPI_COMM_WORLD);

    localdist = (int*)malloc(sizeof(int) * n_col_per_proc);
    localpred = (int*)malloc(sizeof(int) * n_col_per_proc);

    // initalize localdist for dijkstra
    // cost of each vertex from 0
    for(int i=0; i < n_col_per_proc; i++){
        localdist[i] = localdata[0][i];
        localpred[i] = 0;
    }

    dijkstra(localdata, n, n_col_per_proc, localdist, localpred, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(localdist, n_col_per_proc, MPI_INT, globaldist, n_col_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(localpred, n_col_per_proc, MPI_INT, globalpred, n_col_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    t1 = MPI_Wtime();

    if(world_rank == 0){
        printf("\nRESULTS:\n");
        printdists(globaldist, n);
        printarray(globalpred, n, "pred");

        printf("\n");
        //time = 1.e5 * (t1 - t0);
        printf ("execution time: %8.6f seconds\n", t1-t0 );
    }

    // free all dynamically allocated resources
    if(world_rank == 0){
        free2d(M);
        free(globaldist);
        free(globalpred);
    }
    free2d(localdata);
    free(localdist);
    free(localpred);

    MPI_Finalize();
    return 0;
}