#include "../src/vlib.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setup(vec** vec) {
    int a0 = 0, a1 = 1, a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    vec_push(vec, &a0);
    vec_push(vec, &a1);
    vec_push(vec, &a2);
    vec_push(vec, &a3);
    vec_push(vec, &a4);
    vec_push(vec, &a5);
}

int cmp(void* a, void* b) {
    int ai = *(int*)a;
    int bi = *(int*)b;
    return ai - bi;
}

START_TEST(test_push_pop) {
    vec* vec = vec_new(sizeof(int));
    int out;
    setup(&vec);

    vec_pop(vec, &out);
    ck_assert_int_eq(out, 5);

    vec_pop(vec, &out);
    ck_assert_int_eq(out, 4);

    vec_pop(vec, &out);
    ck_assert_int_eq(out, 3);

    vec_pop(vec, &out);
    ck_assert_int_eq(out, 2);

    vec_pop(vec, &out);
    ck_assert_int_eq(out, 1);

    vec_pop(vec, &out);
    ck_assert_int_eq(out, 0);

    ck_assert_int_eq(vec_pop(vec, &out), -1);

    vec_free(vec, NULL);
}
END_TEST

START_TEST(test_get_at) {
    vec* vec = vec_new(sizeof(int));
    int* out;
    setup(&vec);

    out = vec_get_at(vec, 0);
    ck_assert_int_eq(*out, 0);

    out = vec_get_at(vec, 1);
    ck_assert_int_eq(*out, 1);

    out = vec_get_at(vec, 2);
    ck_assert_int_eq(*out, 2);

    out = vec_get_at(vec, 3);
    ck_assert_int_eq(*out, 3);

    out = vec_get_at(vec, 4);
    ck_assert_int_eq(*out, 4);

    out = vec_get_at(vec, 5);
    ck_assert_int_eq(*out, 5);

    out = vec_get_at(vec, 6);
    ck_assert_ptr_null(out);

    vec_free(vec, NULL);
}
END_TEST

START_TEST(test_remove_at) {
    vec* vec = vec_new(sizeof(int));
    int out;
    setup(&vec);

    vec_remove_at(vec, 0, &out);
    ck_assert_int_eq(out, 0);

    vec_remove_at(vec, 0, &out);
    ck_assert_int_eq(out, 1);

    vec_remove_at(vec, 0, &out);
    ck_assert_int_eq(out, 2);

    vec_remove_at(vec, 0, &out);
    ck_assert_int_eq(out, 3);

    vec_remove_at(vec, 0, &out);
    ck_assert_int_eq(out, 4);

    vec_remove_at(vec, 0, &out);
    ck_assert_int_eq(out, 5);

    ck_assert_int_eq(vec_remove_at(vec, 0, &out), -1);

    vec_free(vec, NULL);
}
END_TEST

START_TEST(test_invalid_capacity) {
    vec* vec1 = vec_new_with_capacity(sizeof(int), 1);
    vec* vec2 = vec_new_with_capacity(sizeof(int), 0);
    ck_assert_ptr_nonnull(vec1);
    ck_assert_ptr_nonnull(vec2);
}
END_TEST

START_TEST(test_vec_find) {
    vec* vec = vec_new(sizeof(int));
    int cmp_against0 = 3, cmp_against1 = 10;
    int out;
    ssize_t find;
    setup(&vec);

    find = vec_find(vec, &cmp_against0, &out, cmp);
    ck_assert_int_eq(find, 3);
    ck_assert_int_eq(out, 3);

    find = vec_find(vec, &cmp_against1, &out, cmp);
    ck_assert_int_eq(find, -1);
    vec_free(vec, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("vec_test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_push_pop);
    tcase_add_test(tc_core, test_get_at);
    tcase_add_test(tc_core, test_remove_at);
    tcase_add_test(tc_core, test_vec_find);
    tcase_add_test(tc_core, test_invalid_capacity);
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
