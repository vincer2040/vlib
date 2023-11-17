#include "../src/vlib.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(void* a, void* b) {
    int ai = *((int*)a);
    int bi = *((int*)b);
    return ai - bi;
}

START_TEST(test_list) {
    list l = list_new(sizeof(int));
    int out, a0 = 5, a1 = 7, a2 = 9, a3 = 11;
    int *get;
    list_append(&l, &a0);
    list_append(&l, &a1);
    list_append(&l, &a2);

    get = list_get(&l, 2);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 9);

    ck_assert_int_eq(list_remove_at(&l, 1, &out), 0);
    ck_assert_int_eq(out, 7);

    ck_assert_uint_eq(list_len(&l), 2);

    list_append(&l, &a3);
    ck_assert_int_eq(list_remove_at(&l, 1, &out), 0);
    ck_assert_int_eq(out, 9);

    ck_assert_int_eq(list_remove(&l, &a2, &out, cmp), -1);

    ck_assert_int_eq(list_remove_at(&l, 0, &out), 0);
    ck_assert_int_eq(out, 5);

    ck_assert_int_eq(list_remove_at(&l, 0, &out), 0);
    ck_assert_int_eq(out, 11);

    ck_assert_uint_eq(list_len(&l), 0);

    list_prepend(&l, &a0);
    list_prepend(&l, &a1);
    list_prepend(&l, &a2);

    get = list_get(&l, 2);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 5);

    get = list_get(&l, 0);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 9);

    ck_assert_int_eq(list_remove(&l, &a2, &out, cmp), 0);
    printf("%d\n", out);
    ck_assert_int_eq(out, 9);
    ck_assert_uint_eq(list_len(&l), 2);

    get = list_get(&l, 0);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 7);

    list_free(&l, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("list");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_list);
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
