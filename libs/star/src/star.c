#include "../include/star.h"

// ------- HELPER FUNCTIONS -------

// helper function for the partial sort to swap two elements in the array

void swap_s(int *xp, int *yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// selection partial sort; sorts in ascended order elements in the subset [start, end]
#define swap swap_s
void partialSort_s(struct star* s, int start, int end){
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = start; i <= end; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j <= end; j++){
            if ((s->to[j]) < (s->to[min_idx]))
                min_idx = j;
        }
 
        // Swap the found minimum element with the first element
        if(min_idx != i){
            swap(&s->to[min_idx], &s->to[i]);
            swap(&s->cost[min_idx], &s->cost[i]);
        }
    }
}
#undef swap

int endIndex(struct star* s, int index){
    int end = s->n_nodes; // in case there is no more out-connected nodes, return the length of the 'to' list

    int found = 0;
    int i = index + 1;
    while(!found && i<s->n_nodes){
        if(connected[i] != 0){
            end = s->first[i];
            found = 1;
        }
        i=i+1;
    }

    return end;
}

// ------- STAR DATA STRUCTURE -------

void initStar(struct star* s, int e, int n, int offset){
    if(n < 1){
        perror("star.initStar: number of nodes is zero or negative");
        exit(EXIT_FAILURE);
    }

    if(e < 1){
        perror("star.initStar: number of edges is zero or negative");
        exit(EXIT_FAILURE);
    }

    s->n_edges = e;
    s->n_nodes = n;
    s->offset = offset;
    s->first = (int*)malloc(sizeof(int) * n);
    if(s->first == NULL){
        perror("star.initStar: error in 'first' memory allocation");    // LCOV_EXCL_LINE
        exit(EXIT_FAILURE);                                             // LCOV_EXCL_LINE
    }
    s->to = (int*)malloc(sizeof(int) * e);
    if(s->to == NULL){
        perror("star.initStar: error in 'to' memory allocation");       // LCOV_EXCL_LINE
        exit(EXIT_FAILURE);                                             // LCOV_EXCL_LINE
    }
    s->cost = (int*)malloc(sizeof(int) * e);
    if(s->cost == NULL){
        perror("star.initStar: error in 'cost' memory allocation");     // LCOV_EXCL_LINE
        exit(EXIT_FAILURE);                                             // LCOV_EXCL_LINE
    }

    connected = (int*)calloc(n, sizeof(int));
}

// TODO: offset is wrong
void initStar_arr(struct star* s, int e, int n, int first[], int to[], int costs[], int offset){
    initStar(s, e, n, offset);

    s->first = first;
    s->to = to;
    s->cost = costs;
}

int getNumEdges(struct star* s){
    return s->n_edges;
}

int getNumNodes(struct star* s){
    return s->n_nodes;
}

int getNumEdgesFrom(struct star* s, int from_edge){
    if(from_edge - s->offset +1 == s->n_nodes){
        return s->n_edges - s->first[from_edge - s->offset];
    }
    return s->first[from_edge - s->offset + 1] - s->first[from_edge - s->offset];
}

// TODO: change
int getCost(struct star* s, int from_edge, int to_edge){
    if(from_edge < s->first[0] || from_edge > s->first[s->n_nodes-1]){
        return INFTY;
    }

    int offset_start = s->first[from_edge - s->offset];
    int offset_end = from_edge - s->offset + 1 == s->n_nodes ? s->n_edges : s->first[from_edge - s->offset + 1];
    for(int i = offset_start; i < offset_end; i++){
        if(s->to[i] == to_edge){
            return s->cost[i];
        }
    }
    return INFTY;
}

int* getAllEdgesFrom(struct star* s, int from_edge){
    int* ret = malloc(getNumEdgesFrom(s, from_edge));
    int i=0;
    int end = from_edge - s->offset + 1 == s->n_nodes ? s->n_edges : s->first[from_edge - s->offset + 1];
    for(int j=s->first[from_edge - s->offset]; j<end; j++){
        ret[i++] = s->to[j];
    }
    return ret;
}

#define partialSort partialSort_s
void setStar(struct star* s, int from_edge, int to_edge, int cost){
    if(from_edge < 0 || from_edge - s->offset >= s->n_nodes){
        perror("star.setStar: from_edge out of index");
        exit(EXIT_FAILURE);
    }

    if(to_edge < 0 || to_edge - s->offset > s->n_edges){
        perror("star.setStar: to_edge out of index");
        exit(EXIT_FAILURE);
    }

    if(cost <= 0){
        perror("star.setStar: cost must be greater than zero");
        exit(EXIT_FAILURE);
    }

    // TODO: not right
    // check if the edge is already set
    if(getCost(s, from_edge, to_edge) != INFTY){
        perror("star.setStar: edge is already set");
        exit(EXIT_FAILURE);
    }

    // insert to_edge in the sorted subset of ending edges which starting edges is from_edge
    // shift all the elements starting from the next subset to the right by one
    int k = from_edge - s->offset + 1 == s->n_nodes ? s->n_edges-1 : s->first[from_edge - s->offset + 1];
    memmove(&(s->to[k+1]), &(s->to[k]), (s->n_edges - k-1)*sizeof(int));
    memmove(&(s->cost[k+1]), &(s->cost[k]), (s->n_edges - k-1)*sizeof(int));
    // insert element (to_edge and cost) in place of the first shifted element
    s->to[k] = to_edge;
    s->cost[k] = cost;
    // increment the offset of all elements after from_edge
    for(int i=from_edge - s->offset + 1; i<s->n_nodes; i++){
        s->first[i] = s->first[i] + 1;
    }
    // sort the elements from first[from_edge] to k
    // to values are sorted at increasing values
    // costs follows the order of to values
    partialSort(s, s->first[from_edge - s->offset], k);
    // mark the node as connected
    connected[from_edge] = 1;
}
#undef partialSort

// LCOV_EXCL_START
void printStar(struct star* s){
    int i;
    printf("offset: %d\n", s->offset);

    printf("\n--- Print Star ---\n\n");
    printf("First: ");
    for(i=0; i<s->n_nodes; i++){
        printf("%d ", s->first[i]);
    }
    printf("\n");

    printf("To:    ");
    for(i=0; i<s->n_edges; i++){
        printf("%d ", s->to[i]);
    }
    printf("\n");
    printf("Costs:    ");
    for(i=0; i<s->n_edges; i++){
        printf("%d ", s->cost[i]);
    }
    printf("\n\n");

    printf("Arcs:\n");

    for(i=s->offset; i<s->n_nodes+s->offset; i++){
        int* to = getAllEdgesFrom(s, i);
        for(int j=0; j<getNumEdgesFrom(s, i); j++){
            printf("%d ---> %d (cost: %d)\n", i, to[j], getCost(s, i, to[j]));
        }
    }
    printf("\n--- End Print Star ---\n\n");
}
// LCOV_EXCL_STOP

void freeStar(struct star* s){
    free(s->first);
    free(s->to);
    free(s->cost);
    return;
}