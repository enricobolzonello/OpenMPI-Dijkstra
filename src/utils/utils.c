#include "utils.h"

// generate an empty graph given the number of nodes `N` and number of edges `E`
// initializes an empty graph struct with `flag` array initialzied with INFTY 
// and empty `star`
void generateEmptyGraph(int N, int E, struct Graph* G)
{
	G->N = N;

    // initialize lengths (filled with infty)
	G->L = (int*)malloc(sizeof(int) * G->N );
	if(G->L == NULL){ 
        perror("malloc error on L"); 
        exit(EXIT_FAILURE); 
    }

    for(int i=0; i < G->N; i++){
        G->L[i] = INFTY;
    }

    // initialize flags (by default it is filled with zeros)
	G->flag = (int*)malloc(sizeof(int) * G->N);
	if(G->flag == NULL){ 
        perror("malloc error on flag");
        exit(EXIT_FAILURE); 
    }

    // initialize star
    struct star* s = malloc(sizeof(struct star));
    G->s = s;
    initStar(G->s, E, N);
}

// parsing input from file as in RoadNet dataset format (https://snap.stanford.edu/data/#road)
// inputs a graph struct and the file path
void inputRoadNet(struct Graph* G, char** argv)
{
    FILE *f;
    int nodes, edges;
    char line[100];

    f = fopen(argv[1], "r");
    if(f == NULL){
        perror("cannot open file");
        exit(EXIT_FAILURE);
    }
    printf("File opened!\n");

    //skip two lines
    fscanf(f, "%*[^\n]\n");
    fscanf(f, "%*[^\n]\n");
    //read the number of nodes and edges
    fscanf(f, "%*[^:]:");
    fscanf(f, "%d", &nodes);
    fscanf(f, "%*[^:]:");
    fscanf(f, "%d", &edges);

    printf("nodes: %d\tedges: %d\n", nodes, edges);

    generateEmptyGraph(nodes,edges, G);

    //skip one (two? dont know why) line(s)
    fscanf(f, "%*[^\n]\n");
    fscanf(f, "%*[^\n]\n");

    int u,v;
    while(fgets(line, sizeof(line), f) != NULL){
        sscanf(line, "%d\t%d\n", &u, &v);
        setStar(G->s, u, v, 1);
    }
    printf("File scanned successfully!\n");
}

void freeGraph(struct Graph* G){
    free(G->flag);
    free(G->L);
    freeStar(G->s);
}

void printarray(int* a, int size){
    for(int i=0; i<size; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}