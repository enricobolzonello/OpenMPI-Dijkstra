#include "../utils/matrixutils.h"

void dijkstra(int** matrix, int source, int n, int* L, int* pred){
    int min;
    int h;
    int i,j;
    int* flag;

    // initialize minimum cost found by Dijkstra with the cost from `s` to `i`
    for(i=0; i<n; i++){
        L[i] = matrix[source][i];
        pred[i] = source
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
            if(flag[j] == NOT_VISITED && L[h] + matrix[h][j] < L[j]){
                L[j] = L[h] + matrix[h][j];
                pred[j] = h;
            }
        }
    }
}

int main(int argc, char *argv[]){

}