#include "sequential_Dijkstra.h"

// implementation of Dijkstra's algorithm (pseduo-code in Matteo Fischetti's Introduction to Mathematical Optimization)
// inputs a graph structure and a source node
// modifies the `L` array in the struct with the minimum cost to `i` node
// TODO: integrate the solution in a new star structure so the path can be saved
void dijkstra(struct Graph* G, int source)
{
    int min;
    int h;
    int i,j;

    // initialize minimum cost found by Dijkstra with the cost from `s` to `i`
    for(i=0; i<G->N; i++){
        if(getCost(G->s, source,i) != -1){
            G->L[i] = getCost(G->s, source, i);
        }
    }

    G->flag[source] = VISITED;
    G->L[source] = 0;

    // select a new arc
    for(i=0; i<G->N-1; i++){
        min = INFTY;
        // identify h = argmin{L[j] : j not in S}
        for(j=0; j<G->N; j++){
            if(G->flag[j] == NOT_VISITED && G->L[j] < min){
                min = G->L[j];
                h = j;
            }
        }

        G->flag[h] = VISITED;

        // update the minimum costs
        for(j=0; j<G->N; j++){
            if(getCost(G->s, h, j) != NO_CONN){
                if(G->flag[j] == NOT_VISITED && G->L[h] + getCost(G->s, h, j) < G->L[j]){
                    G->L[j] = G->L[h] + getCost(G->s, h, j);
                }
            } 
        }
    }

}