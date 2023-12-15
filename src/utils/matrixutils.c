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

int getN(FILE* f){
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

    return nodes;
}

void inputRoadNet_matrix(int** matrix, FILE* f, bool costs){
    char line[100];

    //skip one (two? dont know why) line(s)
    fscanf(f, "%*[^\n]\n");
    fscanf(f, "%*[^\n]\n");

    int u,v;
    int i=0;
    while(fgets(line, sizeof(line), f) != NULL){
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