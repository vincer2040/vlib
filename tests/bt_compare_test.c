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
static binary_tree tree2_test(void);

START_TEST(cmp_test) {
    binary_tree t1 = tree_test();
    binary_tree t2 = tree_test();
    binary_tree t3 = tree2_test();

    ck_assert_int_eq(bt_compare(&t1, &t2, cmp), true);
    ck_assert_int_eq(bt_compare(&t1, &t3, cmp), false);
    binary_tree_free(&t1, NULL);
    binary_tree_free(&t2, NULL);
    binary_tree_free(&t3, NULL);
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

static binary_tree tree2_test(void) {
    binary_tree tree = { 0 };
    int a0 = 20, a1 = 50, a2 = 30, a3 = 45, a4 = 49, a5 = 29, a6 = 21, a7 = 10, a8 = 15, a9 = 5, a10 = 7;
    size_t data_size = sizeof(int);
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
    binary_node* n10 = binary_node_new(&a10, data_size);
    n9->right = n10;
    n7->left = n9;
    n7->right = n8;
    n0->left = n7;
    n5->left = n6;
    n1->left = n5;
    n3->right = n4;
    n2->right = n3;
    n1->left = n2;
    n0->right = n1;
    tree.root = n0;
    tree.num_el = 11;
    return tree;
}

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("bt compare");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, cmp_test);
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
