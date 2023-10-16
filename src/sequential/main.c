#include "sequential_Dijkstra.h"

int main(int argc, char *argv[]){
    struct Graph G;
    
    if(argc != 2){
        perror("must provide the file location argument");
        return 0;
    }

    inputRoadNet(&G, argv, false);

    printStar(G.s);

    dijkstra(&G, 0);

    char *b;
	b = malloc(G.N * 5);
	if(b == NULL) {perror("malloc"); exit(EXIT_FAILURE); }
	sprintf(b,"\nLowest distances: \tL=[");
	for(int j = 0; j<G.N; j++){
		sprintf(&b[strlen(b)], "%d,", G.L[j]);
	}
	printf("%s]\n", b);

    freeGraph(&G);
    return 0;
}