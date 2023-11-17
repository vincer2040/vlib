#include "../src/vlib.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_ht) {
    int a0 = 5, a1 = 7, a2 = 9, a3 = 11;
    int* get;
    ht ht = ht_new(sizeof(int));
    ht_insert(&ht, "a0", 2, &a0, NULL);
    ht_insert(&ht, "a1", 2, &a1, NULL);
    ht_insert(&ht, "a2", 2, &a2, NULL);
    ht_insert(&ht, "a3", 2, &a3, NULL);

    ck_assert_uint_eq(ht_len(&ht), 4);

    get = ht_get(&ht, "a0", 2);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 5);

    get = ht_get(&ht, "a1", 2);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 7);

    get = ht_get(&ht, "a2", 2);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 9);

    get = ht_get(&ht, "a3", 2);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 11);

    get = ht_get(&ht, "a4", 2);
    ck_assert_ptr_null(get);

    ck_assert_int_eq(ht_delete(&ht, "a0", 2, NULL, NULL), 0);
    ck_assert_int_eq(ht_delete(&ht, "a1", 2, NULL, NULL), 0);

    ck_assert_uint_eq(ht_len(&ht), 2);

    get = ht_get(&ht, "a0", 2);
    ck_assert_ptr_null(get);

    get = ht_get(&ht, "a1", 2);
    ck_assert_ptr_null(get);

    get = ht_get(&ht, "a2", 2);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 9);

    get = ht_get(&ht, "a3", 2);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 11);

    get = ht_get(&ht, "a4", 2);
    ck_assert_ptr_null(get);

    ht_free(&ht, NULL, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("ht test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_ht);
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
