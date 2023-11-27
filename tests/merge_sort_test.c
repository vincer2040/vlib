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

START_TEST(test_merge_sort) {
    vec* vec = vec_new(sizeof(int));
    int a0 = 9, a1 = 3, a2 = 7, a3 = 4, a4 = 69, a5 = 420, a6 = 42;
    int exps[] = {3, 4, 7, 9, 42, 69, 420};
    size_t i, len = sizeof exps / sizeof exps[0];
    vec_push(&vec, &a0);
    vec_push(&vec, &a1);
    vec_push(&vec, &a2);
    vec_push(&vec, &a3);
    vec_push(&vec, &a4);
    vec_push(&vec, &a5);
    vec_push(&vec, &a6);

    vec_merge_sort(vec, cmp);
    for (i = 0; i < len; ++i) {
        int* at = vec_get_at(vec, i);
        int exp = exps[i];
        ck_assert_ptr_nonnull(at);
        ck_assert_int_eq(exp, *at);
    }

    vec_free(vec, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("merge sort");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_merge_sort);
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
