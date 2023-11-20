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

static binary_tree tree_test(void);

START_TEST(test_bfs) {
    binary_tree t = tree_test();
    int a0 = 45, a1 = 7, a2 = 69;
    ck_assert_int_eq(bt_bfs(&t, &a0, cmp), true);
    ck_assert_int_eq(bt_bfs(&t, &a1, cmp), true);
    ck_assert_int_eq(bt_bfs(&t, &a2, cmp), false);
    binary_tree_free(&t, NULL);
}
END_TEST

static binary_tree tree_test(void) {
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
    s = suite_create("bfs");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_bfs);
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
