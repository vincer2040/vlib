#include "../src/vlib.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(void* a, void* b) {
    int ai = *(int*)a;
    int bi = *(int*)b;
    return ai - bi;
}

START_TEST(tree_test) {
    int a0 = 1, a1 = 69, a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    int not_in = 1337;
    tree t = tree_new(sizeof(int));
    ck_assert_int_eq(tree_insert(&t, &a0, NULL, cmp), 0);
    ck_assert_int_eq(tree_insert(&t, &a1, &a0, cmp), 0);
    ck_assert_int_eq(tree_insert(&t, &a2, &a1, cmp), 0);
    ck_assert_int_eq(tree_insert(&t, &a3, &a1, cmp), 0);
    ck_assert_int_eq(tree_insert(&t, &a4, &a3, cmp), 0);
    ck_assert_int_eq(tree_insert(&t, &a5, &a2, cmp), 0);
    ck_assert_int_eq(tree_has(&t, &a5, cmp), true);
    ck_assert_int_eq(tree_has(&t, &not_in, cmp), false);
    ck_assert_int_eq(tree_delete(&t, &a5, cmp, NULL), 0);
    ck_assert_int_eq(tree_delete(&t, &a3, cmp, NULL), 0);
    ck_assert_int_eq(tree_delete(&t, &not_in, cmp, NULL), -1);
    ck_assert_int_eq(tree_has(&t, &a0, cmp), true);
    ck_assert_int_eq(tree_has(&t, &a5, cmp), false);
    ck_assert_int_eq(tree_has(&t, &a3, cmp), false);
    tree_free(&t, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("tree");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, tree_test);
    suite_add_tcase(s, tc_core);
    return s;
}

int main() {
    int number_failed;
    Suite* s;
    SRunner* sr;
    s = ht_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
