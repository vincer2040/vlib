#include "../src/small_vec.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SMALL_VEC_INIT(int, my_vec, 32);

START_TEST(small_vec_test) {
    int out;
    int* get;

    small_vec_t(my_vec) vec = small_vec_init(my_vec);

    small_vec_push(my_vec, &vec, 1);
    small_vec_push(my_vec, &vec, 2);
    small_vec_push(my_vec, &vec, 3);
    small_vec_push(my_vec, &vec, 4);
    small_vec_push(my_vec, &vec, 5);
    small_vec_push(my_vec, &vec, 6);
    small_vec_push(my_vec, &vec, 7);

    ck_assert_uint_eq(small_vec_len(my_vec, &vec), 7);

    get = small_vec_get_at(my_vec, &vec, 0);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 1);

    small_vec_pop(my_vec, &vec, &out);
    ck_assert_int_eq(out, 7);

    ck_assert_uint_eq(small_vec_len(my_vec, &vec), 6);

    get = small_vec_get_at(my_vec, &vec, 5);
    ck_assert_ptr_nonnull(get);
    ck_assert_int_eq(*get, 6);

    get = small_vec_get_at(my_vec, &vec, 10);
    ck_assert_ptr_null(get);

    small_vec_remove_at(my_vec, &vec, 1, &out);
    ck_assert_int_eq(out, 2);

    ck_assert_uint_eq(small_vec_len(my_vec, &vec), 5);

    get = small_vec_get_at(my_vec, &vec, 1);
    ck_assert_ptr_nonnull(get);
    ck_assert_uint_eq(*get, 3);
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;
    s = suite_create("small vec");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, small_vec_test);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite* s;
    SRunner* sr;
    s = suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
    return 0;
}
