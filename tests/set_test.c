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

START_TEST(set_test) {
    set s = set_new(cmp);
    int a0 = 5, a1 = 7, a2 = 9, a3 = 11, a4 = 69;
    set_insert(&s, &a0, sizeof(int));
    set_insert(&s, &a1, sizeof(int));
    set_insert(&s, &a2, sizeof(int));
    set_insert(&s, &a3, sizeof(int));
    set_insert(&s, &a4, sizeof(int));

    ck_assert_int_eq(set_insert(&s, &a0, sizeof(int)), -1);

    ck_assert_uint_eq(set_len(&s), 5);

    ck_assert_int_eq(set_has(&s, &a0, sizeof(int)), true);
    ck_assert_int_eq(set_has(&s, &a1, sizeof(int)), true);
    ck_assert_int_eq(set_has(&s, &a2, sizeof(int)), true);
    ck_assert_int_eq(set_has(&s, &a3, sizeof(int)), true);
    ck_assert_int_eq(set_has(&s, &a4, sizeof(int)), true);

    ck_assert_int_eq(set_delete(&s, &a0, sizeof(int), NULL), 0);
    ck_assert_int_eq(set_delete(&s, &a0, sizeof(int), NULL), -1);
    ck_assert_int_eq(set_delete(&s, &a1, sizeof(int), NULL), 0);

    ck_assert_int_eq(set_has(&s, &a0, sizeof(int)), false);
    ck_assert_int_eq(set_has(&s, &a1, sizeof(int)), false);

    ck_assert_uint_eq(set_len(&s), 3);
    set_free(&s, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("set");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, set_test);
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
