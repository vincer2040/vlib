#include "../src/vlib.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static binary_tree tree(void);

START_TEST(test_pre_oder) {
    binary_tree t = tree();
    int exps[] = {
        20, 10, 5, 7, 15, 50, 30, 29, 45, 100,
    };
    size_t i, len = sizeof exps / sizeof exps[0];
    vec* v = bt_pre_order(&t);

    for (i = 0; i < len; ++i) {
        int* cur = vec_get_at(v, i);
        int exp = exps[i];
        ck_assert_ptr_nonnull(cur);
        ck_assert_int_eq(*cur, exp);
    }
    vec_free(v, NULL);
    binary_tree_free(&t, NULL);
}
END_TEST

START_TEST(test_in_order) {
    binary_tree t = tree();
    int exps[] = {
        5, 7, 10, 15, 20, 29, 30, 45, 50, 100,
    };
    size_t i, len = sizeof exps / sizeof exps[0];
    vec* v = bt_in_order(&t);

    for (i = 0; i < len; ++i) {
        int* cur = vec_get_at(v, i);
        int exp = exps[i];
        ck_assert_ptr_nonnull(cur);
        ck_assert_int_eq(*cur, exp);
    }
    vec_free(v, NULL);
    binary_tree_free(&t, NULL);
}
END_TEST

START_TEST(test_post_order) {
    binary_tree t = tree();
    int exps[] = {
        7, 5, 15, 10, 29, 45, 30, 100, 50, 20,
    };
    size_t i, len = sizeof exps / sizeof exps[0];
    vec* v = bt_post_order(&t);

    for (i = 0; i < len; ++i) {
        int* cur = vec_get_at(v, i);
        int exp = exps[i];
        ck_assert_ptr_nonnull(cur);
        ck_assert_int_eq(*cur, exp);
    }
    vec_free(v, NULL);
    binary_tree_free(&t, NULL);
}
END_TEST

static binary_tree tree(void) {
    binary_tree tree = {0};
    size_t data_size = sizeof(int);
    int a0 = 20, a1 = 50, a2 = 100, a3 = 30, a4 = 45, a5 = 29, a6 = 10, a7 = 15,
        a8 = 5, a9 = 7;
    binary_node* n0 = binary_node_new(&a0, data_size);
    binary_node* n1 = binary_node_new(&a1, data_size);
    binary_node* n2 = binary_node_new(&a2, data_size);
    binary_node* n3 = binary_node_new(&a3, data_size);
    binary_node* n4 = binary_node_new(&a4, data_size);
    binary_node* n5 = binary_node_new(&a5, data_size);
    binary_node* n6 = binary_node_new(&a6, data_size);
    binary_node* n7 = binary_node_new(&a7, data_size);
    binary_node* n8 = binary_node_new(&a8, data_size);
    binary_node* n9 = binary_node_new(&a9, data_size);
    n3->left = n5;
    n3->right = n4;
    n1->right = n2;
    n1->left = n3;
    n0->right = n1;
    n8->right = n9;
    n6->left = n8;
    n6->right = n7;
    n0->left = n6;
    tree.root = n0;
    tree.data_size = sizeof(int);
    tree.num_el = 10;
    return tree;
}

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("tree traversal");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_pre_oder);
    tcase_add_test(tc_core, test_in_order);
    tcase_add_test(tc_core, test_post_order);
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
