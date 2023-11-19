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

START_TEST(avl_test) {
    avl_tree tree = avl_tree_new(sizeof(int));
    vec* v;
    int a0 = 10, a1 = 20, a2 = 30, a3 = 40, a4 = 50, a5 = 25;
    int exps[] = {10, 20, 25, 30, 40, 50};
    size_t i, len = sizeof exps / sizeof exps[0];
    avl_insert(&tree, &a0, cmp);
    avl_insert(&tree, &a1, cmp);
    avl_insert(&tree, &a2, cmp);
    avl_insert(&tree, &a3, cmp);
    avl_insert(&tree, &a4, cmp);
    avl_insert(&tree, &a5, cmp);
    v = avl_in_order(&tree);
    for (i = 0; i < len; ++i) {
        int* cur = vec_get_at(v, i);
        int exp = exps[i];
        ck_assert_ptr_nonnull(cur);
        ck_assert_int_eq(*cur, exp);
    }
    vec_free(v, NULL);
    avl_tree_free(&tree, NULL);
}
END_TEST

START_TEST(avl_delete_test) {
    avl_tree tree = avl_tree_new(sizeof(int));
    vec *v, *v2;
    int a0 = 9, a1 = 5, a2 = 10, a3 = 0, a4 = 6, a5 = 11, a6 = -1, a7 = 1,
        a8 = 2, not_in = 69;
    int exps[] = {-1, 0, 1, 2, 5, 6, 9, 10, 11};
    int exps_after_delete[] = {-1, 0, 1, 2, 5, 6, 9, 11};
    size_t i, len = sizeof exps / sizeof exps[0],
              len2 = sizeof exps_after_delete / sizeof exps_after_delete[0];
    avl_insert(&tree, &a0, cmp);
    avl_insert(&tree, &a1, cmp);
    avl_insert(&tree, &a2, cmp);
    avl_insert(&tree, &a3, cmp);
    avl_insert(&tree, &a4, cmp);
    avl_insert(&tree, &a5, cmp);
    avl_insert(&tree, &a6, cmp);
    avl_insert(&tree, &a7, cmp);
    avl_insert(&tree, &a8, cmp);
    v = avl_in_order(&tree);
    for (i = 0; i < len; ++i) {
        int* cur = vec_get_at(v, i);
        int exp = exps[i];
        ck_assert_ptr_nonnull(cur);
        ck_assert_int_eq(*cur, exp);
    }
    vec_free(v, NULL);
    ck_assert_int_eq(avl_delete(&tree, &a2, cmp, NULL), 0);
    ck_assert_int_eq(avl_delete(&tree, &not_in, cmp, NULL), -1);
    v2 = avl_in_order(&tree);

    for (i = 0; i < len2; ++i) {
        int* cur = vec_get_at(v2, i);
        int exp = exps_after_delete[i];
        ck_assert_ptr_nonnull(cur);
        ck_assert_int_eq(*cur, exp);
    }
    vec_free(v2, NULL);
    avl_tree_free(&tree, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("avl tree");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, avl_test);
    tcase_add_test(tc_core, avl_delete_test);
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
