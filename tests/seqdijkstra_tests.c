#include <check.h>
#include "../src/sequential/sequential_Dijkstra.h"

START_TEST (test_seqdijkstra_generateEmptyGraph) {
    struct Graph G;
    generateEmptyGraph(3, 2, &G);
    ck_assert_int_eq(G.N, 3);
    ck_assert_int_eq(G.s->n_edges, 2);
    ck_assert_int_eq(G.s->n_nodes, 3);
    freeGraph(&G);
}

/*
0	1
0	2
0	3
1	5
2	4
3	5
4	5
5	0
*/
START_TEST (test_seqdijkstra_simpleexample) {
    struct Graph G;
    generateEmptyGraph(6, 8, &G);
    setStar(G.s, 0, 1, 1);
    setStar(G.s, 0, 2, 1);
    setStar(G.s, 0, 3, 1);
    setStar(G.s, 1, 5, 1);
    setStar(G.s, 2, 4, 1);
    setStar(G.s, 3, 5, 1);
    setStar(G.s, 4, 5, 1);
    setStar(G.s, 5, 0, 1);
}

Suite * seqdijkstra_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Dijkstra");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_seqdijkstra_generateEmptyGraph);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = seqdijkstra_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}