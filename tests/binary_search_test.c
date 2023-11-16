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

START_TEST(test_binary_search) {
    vec* vec = vec_new(sizeof(int));
    int a0 = 1, a1 = 3, a2 = 4, a3 = 69, a4 = 71, a5 = 81, a6 = 90, a7 = 99,
        a8 = 420, a9 = 1337, a10 = 69420;
    int n0 = 69, n1 = 1336, n2 = 69420, n3 = 69421, n4 = 1, n5 = 0;
    vec_push(&vec, &a0);
    vec_push(&vec, &a1);
    vec_push(&vec, &a2);
    vec_push(&vec, &a3);
    vec_push(&vec, &a4);
    vec_push(&vec, &a5);
    vec_push(&vec, &a6);
    vec_push(&vec, &a7);
    vec_push(&vec, &a8);
    vec_push(&vec, &a9);
    vec_push(&vec, &a10);
    ck_assert_int_eq(vec_binary_search(vec, &n0, cmp), 0);
    ck_assert_int_eq(vec_binary_search(vec, &n1, cmp), -1);
    ck_assert_int_eq(vec_binary_search(vec, &n2, cmp), 0);
    ck_assert_int_eq(vec_binary_search(vec, &n3, cmp), -1);
    ck_assert_int_eq(vec_binary_search(vec, &n4, cmp), 0);
    ck_assert_int_eq(vec_binary_search(vec, &n5, cmp), -1);
    vec_free(vec, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("vec binary search");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_binary_search);
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
