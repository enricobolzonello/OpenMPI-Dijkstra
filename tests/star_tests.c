#include <check.h>
#include "../libs/star/include/star.h"

// covers initialization of star with valid number of edges and number of nodes
START_TEST (test_star_init){
    struct star s;
    initStar(&s, 8, 7, 0);
    ck_assert_int_eq(getNumEdges(&s), 8);
    ck_assert_int_eq(getNumNodes(&s), 7);
    freeStar(&s);
} END_TEST

// covers initialization of star with 0 nodes
START_TEST (test_star_init_nonodes){
    struct star s;
    initStar(&s, 6, 0, 0);
    freeStar(&s);
} END_TEST

// covers initialization of star with 0 edges
START_TEST (test_star_init_noedges){
    struct star s;
    initStar(&s, 0, 2, 0);
    freeStar(&s);
} END_TEST

// covers setStar with negative cost
START_TEST (test_star_set_negativecost){
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 1);
    setStar(&s, 0, 2, -1);
    freeStar(&s);
}

// covers setStar with two valid edges
// edge 0->1 with cost 10
// edge 0->2 with cost 7
START_TEST (test_star_set){
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 2, 7);
    ck_assert_int_eq(s.to[0], 1);
    ck_assert_int_eq(s.to[1], 2);
    ck_assert_int_eq(s.cost[0], 10);
    ck_assert_int_eq(s.cost[1], 7);
    freeStar(&s);
} END_TEST

// covers setStar with valid edges
// aims at testing the correct sorting 
// first insert edges from 0 to 2 and 3 and then insert edge to 1
// the 'to' and 'cost' elements should be sorted based on 'to' order
START_TEST (test_star_set_sort) {
    struct star s;
    initStar(&s, 3, 4, 0);
    setStar(&s, 0, 2, 6);
    setStar(&s, 0, 3, 2);
    setStar(&s, 0, 1, 7);
    ck_assert_int_eq(s.to[0], 1);
    ck_assert_int_eq(s.cost[0], 7);
    ck_assert_int_eq(s.to[1], 2);
    ck_assert_int_eq(s.cost[1], 6);
    ck_assert_int_eq(s.to[2], 3);
    ck_assert_int_eq(s.cost[2], 2);
    freeStar(&s);
}

// covers setStar with invalid 'from' edge
START_TEST (test_star_set_invalidfrom){
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 3, 2, 7);
    freeStar(&s);
} END_TEST

// covers setStar with invalid 'to' edge
START_TEST (test_star_set_invalidto){
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 3, 7);
    freeStar(&s);
} END_TEST

// covers getStar with two valid edges
// edge 0->1 with cost 10
// edge 0->2 with cost 7
START_TEST (test_star_get){
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 2, 7);
    ck_assert_int_eq(getCost(&s, 0, 1), 10);
    ck_assert_int_eq(getCost(&s, 0, 2), 7);
    freeStar(&s);    
} END_TEST

// covers getStar with invalid 'from' edge
START_TEST (test_star_get_invalidfrom){
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 2, 7);
    ck_assert_int_eq(getCost(&s, 3, 1), INFTY);
    freeStar(&s); 
}

// covers getStar with invalid 'to' edge
START_TEST (test_star_get_invalidto){
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 2, 7);
    ck_assert_int_eq(getCost(&s, 0, 3), INFTY);
    freeStar(&s); 
}

// covers getStar with an invalid connection
// should return -1
START_TEST (test_star_get_invalidedge) {
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 2, 7);
    ck_assert_int_eq(getCost(&s, 1, 2), INFTY);
    freeStar(&s);
}

// covers getNumEdgesFrom with a non-ending node
START_TEST (test_star_getNumEdgesFrom) {
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 2, 7);
    ck_assert_int_eq(getNumEdgesFrom(&s, 0), 2);
    freeStar(&s);
}

// covers getNumEdgesFrom with an ending node
START_TEST (test_star_getNumEdgesFrom_end) {
    struct star s;
    initStar(&s, 4, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 2, 7);
    setStar(&s, 2, 1, 3);
    setStar(&s, 2, 0, 5);
    ck_assert_int_eq(getNumEdgesFrom(&s, 2), 2);
    freeStar(&s);
}

// covers getAllEdges from
START_TEST (test_star_getAllEdgesFrom) {
    struct star s;
    initStar(&s, 2, 3, 0);
    setStar(&s, 0, 1, 10);
    setStar(&s, 0, 2, 7);
    int* a = getAllEdgesFrom(&s, 0);
    ck_assert_int_eq(a[0], 1);
    ck_assert_int_eq(a[1], 2);
    freeStar(&s);
}

START_TEST (test_star_offset) {
    struct star s;
    initStar(&s, 4, 4, 2);
    setStar(&s, 2, 3, 1);
    setStar(&s, 2, 4, 3);
    setStar(&s, 3, 4, 3);
    setStar(&s, 4, 5, 7);

    ck_assert_int_eq(getNumEdgesFrom(&s, 2), 2);

    int* a = getAllEdgesFrom(&s, 2);
    ck_assert_int_eq(a[0], 3);
    ck_assert_int_eq(a[1], 4);

    ck_assert_int_eq(getCost(&s, 2, 3), 1);
    ck_assert_int_eq(getCost(&s, 4, 5), 7);
}

Suite * star_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_errors;

    s = suite_create("Star");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_star_init);
    tcase_add_test(tc_core, test_star_set);
    tcase_add_test(tc_core, test_star_set_sort);
    tcase_add_test(tc_core, test_star_get);
    tcase_add_test(tc_core, test_star_get_invalidedge);
    tcase_add_test(tc_core, test_star_getNumEdgesFrom);
    tcase_add_test(tc_core, test_star_getNumEdgesFrom_end);
    tcase_add_test(tc_core, test_star_getAllEdgesFrom);
    tcase_add_test(tc_core, test_star_offset);
    suite_add_tcase(s, tc_core);

    tc_errors = tcase_create("Errors");
    tcase_add_exit_test(tc_errors, test_star_init_nonodes, EXIT_FAILURE);
    tcase_add_exit_test(tc_errors, test_star_init_noedges, EXIT_FAILURE);
    tcase_add_exit_test(tc_errors, test_star_set_negativecost, EXIT_FAILURE);
    tcase_add_exit_test(tc_errors, test_star_set_invalidfrom, EXIT_FAILURE);
    tcase_add_exit_test(tc_errors, test_star_set_invalidto, EXIT_FAILURE);
    tcase_add_test(tc_errors, test_star_get_invalidfrom);
    tcase_add_test(tc_errors, test_star_get_invalidto);
    suite_add_tcase(s, tc_errors);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = star_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}