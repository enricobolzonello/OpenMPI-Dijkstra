#ifndef MATRIXUTILS
#define MATRIXUTILS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define NOT_VISITED 0
#define VISITED 1
#define INFTY INT_MAX

int** alloc2d(int n, int m);
void free2d(int **array);
int inputRoadNet_getN(FILE* f);
void edgelist_matrix(int** matrix, FILE* f, bool costs);
int random_getN(FILE* f);
void initalizeMatrix(int** matrix,int val, int n, int m);
void printarray(int* a, int size, char* label);
void printdists(int global_dist[], int n);
void printmatrix(int** matrix, int n, int m);

#endif