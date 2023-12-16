#include "matrixutils.h"

int** alloc2d(int n, int m){
    int *data = (int *)malloc(n*m*sizeof(int));
    int **array = (int **)calloc(n, sizeof(int *));
    for (int i=0; i<n; i++)
        array[i] = &(data[i*m]);

    return array;
}

void free2d(int **array) {
    free(array[0]);
    free(array);
}

int inputRoadNet_getN(FILE* f){
    int nodes, edges;
    //skip two lines
    fscanf(f, "%*[^\n]\n");
    fscanf(f, "%*[^\n]\n");
    //read the number of nodes and edges
    fscanf(f, "%*[^:]:");
    fscanf(f, "%d", &nodes);
    fscanf(f, "%*[^:]:");
    fscanf(f, "%d", &edges);

    printf("nodes: %d\tedges: %d\n", nodes, edges);

    fscanf(f, "%*[^\n]\n");

    return nodes;
}

int random_getN(FILE* f){
    int nodes, edges;
    //read the number of nodes and edges
    fscanf(f, "%*[^:]:");
    fscanf(f, "%d", &nodes);
    fscanf(f, "%*[^:]:");
    fscanf(f, "%d", &edges);

    printf("nodes: %d\tedges: %d\n", nodes, edges);

    return nodes;
}

void edgelist_matrix(int** matrix, FILE* f, bool costs){
    char* line = NULL;
    size_t len = 0;

    int u,v;
    int i=0;
    getline(&line, &len, f); // skip empty line
    while (getline(&line, &len, f) != -1){
        u = 0;
        v = 0;

        if(i%100000 == 0){
            printf("parsed %d edges\n", i);
        }
        if(costs){
            int cost;
            sscanf(line, "%d\t%d\t%d\n", &u, &v, &cost);
            matrix[v][u] = cost;
            matrix[u][v] = cost;
        }else{
            sscanf(line, "%d\t%d\n", &u, &v);
            matrix[v][u] = 1;
            matrix[u][v] = 1;
        }
        i++;
    }
    printf("parsed %d edges\n", i);
    printf("File scanned successfully!\n\n");
}

void initalizeMatrix(int** matrix, int val, int n, int m){
    int i,j;
    for(i=0; i<n; i++){
        for(j=0; j<m; j++){
            matrix[i][j] = val;
        }
    }
}

void printarray(int* a, int size, char* label){
    printf("%s:\t", label);
    for(int i=0; i<size; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

void printdists(int global_dist[], int n)
{
    int v;

    printf("  v    dist 0->v\n");
    printf("----   ---------\n");

    for (v = 1; v < n; v++)
    {
        if (global_dist[v] == INFTY)
        {
            printf("%3d       %5s\n", v, "inf");
        }
        else
            printf("%3d       %4d\n", v, global_dist[v]);
    }
    printf("\n");
}

void printmatrix(int** matrix, int n, int m){
    int row, columns;
    for (row = 0; row < n; row++)
    {
        for (columns = 0; columns < m; columns++)
        {
            int val = matrix[row][columns];
            if (val == INFTY)
            {
                printf("%s\t", "inf");
            }
            else
            {
                printf("%d\t", val);
            }
        }
        printf("\n");
    }
}