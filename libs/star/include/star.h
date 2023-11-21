// Star Data Structure
//
// Bolzonello Enrico
// enrico.bolzonello@studenti.unipd.it
//
// Custom data structure to represent graph nodes and edges with its cost.
// The edges are assumed to be directional and with non-negative costs, due to Dijkstra's algorithm limitations
// 
// Algorithm: 
// The struct star represents the data structure.
// The array 'first' represents the starting node as the array index and the starting index in arrays 'to' and 'cost' 
// The array 'to' values represent the ending node, array 'costs' stores costs as the name says
// ex: first = [0, 2, 5, 6, 6]
//     to = [1, 2, 2, 3, 4, 4]
//     node 0 is connected to 1 and 2, node 1 is connected to 2,3 and 4 and node 3 is connected to node 4
//
// Notes:
// 1) Negative costs are not modeled
// 2) Nodes are directional, cannot model bi-directional nodes with one connection (needs a connection forward and backwards)
// 3) It is assumed that nodes are represented by a consecutive sequence of integers starting from an initial value 'offset'
//
// Future Development(s):
// - Mapping function to represent generic nodes in the data structure
#ifndef STAR
#define STAR

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define INFTY INT_MAX

struct star {
    int* first;
    int* to;
    int* cost;
    int n_edges;
    int n_nodes;
    int offset;
};

int* connected;

// ----- INITIALIZER -----
void initStar(struct star* s, int e, int n, int offset);
void initStar_arr(struct star* s, int e, int n, int first[], int to[], int costs[]);

// ----- GETTERS -----
int getNumEdges(struct star* s);
int getNumNodes(struct star* s);
int getNumEdgesFrom(struct star* s, int from_edge);
int* getAllEdgesFrom(struct star* s, int from_edge);

int getCost(struct star* s, int from_edge, int to_edge);

// ----- SETTER -----
void setStar(struct star* s, int from_edge, int to_edge, int cost);

// ----- DEBUG -----
void printStar(struct star* s);

void freeStar(struct star* s);

#endif