#ifndef UTILS
#define UTILS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "star.h"

#define NOT_VISITED 0
#define VISITED 1
#define NO_CONN -1

struct Graph
{
    int N;  // number of nodes
    struct star* s; // data structure representing the graph

    int* L; // shortest path length for each node
    int* pred; // pred[i] saves the predecessor of node i
    int* flag; // flag the nodes the algorithm has flag
};

void generateEmptyGraph(int N, int E, struct Graph* G);
void printGraph(struct Graph* G);
void inputRoadNet(struct Graph* G, char** argv, bool cost);
void freeGraph(struct Graph* G);
void printarray(int* a, int size);
void initializeGraph(struct Graph* G, int N, int E, int first[], int to[], int cost[], int offset);

#endif