#include "../utils/matrixutils.h"
#include <time.h>

void dijkstra(int** matrix, int source, int n, int* L, int* pred){
    int min;
    int h;
    int i,j;
    int* flag;

    // initialize minimum cost found by Dijkstra with the cost from `s` to `i`
    for(i=0; i<n; i++){
        L[i] = matrix[source][i];
        pred[i] = source;
    }

    // initialize flag
    flag = calloc(n, sizeof(int));

    flag[source] = VISITED;
    L[source] = 0;

    // select a new arc
    for(i=0; i<n; i++){
        min = INFTY;
        // identify h = argmin{L[j] : j not in S}
        for(j=0; j<n; j++){
            if(flag[j] == NOT_VISITED && L[j] < min){
                min = L[j];
                h = j;
            }
        }

        flag[h] = VISITED;

        // update the minimum costs
        for(j=0; j<n; j++){
            if (matrix[h][j] != INFTY){
                if(flag[j] == NOT_VISITED && L[h] + matrix[h][j] < L[j]){
                    L[j] = L[h] + matrix[h][j];
                    pred[j] = h;
                }
            }
        }
    }
}

int main(int argc, char *argv[]){
    int n;
    int** M;
    int* dist, *pred;
    FILE* f = NULL;
    uintmax_t start, end;

    if(argc != 2){
        perror("must provide the file location argument");
        return 0;
    }

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
    edgelist_matrix(M, f, true);

    fclose(f);


    // print matrix
    // printmatrix(M, n, n);

    dist = (int*)malloc(sizeof(int) * n);
    pred = (int*)malloc(sizeof(int) * n);

    start = clock();

    dijkstra(M, 0, n, dist, pred);

    end = clock();

    printf("\nRESULTS:\n");
    printdists(dist, n);
    printarray(pred, n, "pred");
    printf("\n");

    printf("execution time: %8.6f seconds\n", (double)(end-start)/CLOCKS_PER_SEC);

    free(dist);
    free(pred);
    free2d(M);
}