#include "../src/vlib.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

START_TEST(lru_test) {
    int a0 = 69, a1 = 420, a2 = 1337, a3 = 69420;
    lru l = lru_new(3, sizeof(int), NULL);
    int* get;

    ck_assert_ptr_null(lru_get(&l, "foo", 3));

    lru_update(&l, "foo", 3, &a0, NULL);
    get = lru_get(&l, "foo", 3);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 69);

    lru_update(&l, "bar", 3, &a1, NULL);
    get = lru_get(&l, "bar", 3);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 420);

    lru_update(&l, "baz", 3, &a2, NULL);
    get = lru_get(&l, "baz", 3);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 1337);

    lru_update(&l, "ball", 4, &a3, NULL);
    get = lru_get(&l, "ball", 4);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 69420);

    ck_assert_ptr_null(lru_get(&l, "foo", 3));

    get = lru_get(&l, "bar", 3);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 420);

    lru_update(&l, "foo", 3, &a0, NULL);

    get = lru_get(&l, "bar", 3);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 420);

    lru_update(&l, "foo", 3, &a0, NULL);
    get = lru_get(&l, "foo", 3);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 69);

    ck_assert_ptr_null(lru_get(&l, "baz", 3));

    lru_free(&l, NULL, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("lru");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, lru_test);
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
