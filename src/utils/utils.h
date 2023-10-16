#ifndef UTILS
#define UTILS

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "star.h"

#define INFTY INT_MAX
#define NOT_VISITED 0
#define VISITED 1
#define NO_CONN -1

struct Graph
{
    int N;  // number of nodes
    struct star* s; // data structure representing the graph

    int* L; // shortest path length for each node
    int* flag; // flag the nodes the algorithm has flag
};

void generateEmptyGraph(int N, int E, struct Graph* G);
void enableDebug(long N);
void printGraph(struct Graph* G);
void inputRoadNet(struct Graph* G, char** argv, bool cost);
void freeGraph(struct Graph* G);
void printarray(int* a, int size);

#endif