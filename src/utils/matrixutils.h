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
int getN(FILE* f);
void inputRoadNet_matrix(int** matrix, FILE* f, bool costs);
void initalizeMatrix(int** matrix,int val, int n, int m);

#endif