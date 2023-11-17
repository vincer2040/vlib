#include "../src/vlib.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_vstr_push_char) {
    vstr s = vstr_new();
    const char* data;
    size_t i;
    for (i = 0; i < 23; ++i) {
        vstr_push_char(&s, 'a');
    }

    ck_assert_uint_eq(vstr_len(&s), 23);
    ck_assert_uint_eq(s.is_large, 0);
    data = vstr_data(&s);
    ck_assert_str_eq(data, "aaaaaaaaaaaaaaaaaaaaaaa");

    vstr_push_char(&s, 'b');
    ck_assert_uint_eq(vstr_len(&s), 24);
    ck_assert_uint_eq(s.is_large, 1);
    data = vstr_data(&s);
    ck_assert_str_eq(data, "aaaaaaaaaaaaaaaaaaaaaaab");

    vstr_push_char(&s, 'b');
    ck_assert_uint_eq(vstr_len(&s), 25);
    ck_assert_uint_eq(s.is_large, 1);
    data = vstr_data(&s);
    ck_assert_str_eq(data, "aaaaaaaaaaaaaaaaaaaaaaabb");
    vstr_free(&s);
}
END_TEST

START_TEST(test_vstr_from_sm) {
    vstr s = vstr_from("aaaaaaaaaaaaaaaaaaaaaaa");
    char* data;
    ck_assert_uint_eq(s.is_large, 0);
    ck_assert_uint_eq(vstr_len(&s), 23);
    data = vstr_data(&s);
    ck_assert_str_eq(data, "aaaaaaaaaaaaaaaaaaaaaaa");

    vstr_push_char(&s, 'b');
    ck_assert_uint_eq(s.is_large, 1);
    ck_assert_uint_eq(vstr_len(&s), 24);
    data = vstr_data(&s);
    ck_assert_str_eq(data, "aaaaaaaaaaaaaaaaaaaaaaab");

    vstr_free(&s);
}
END_TEST

START_TEST(test_vstr_from_lg) {
    vstr s = vstr_from("aaaaaaaaaaaaaaaaaaaaaaabb");
    char* data;
    ck_assert_uint_eq(s.is_large, 0);
    ck_assert_uint_eq(vstr_len(&s), 25);
    data = vstr_data(&s);
    ck_assert_str_eq(data, "aaaaaaaaaaaaaaaaaaaaaaabb");
    vstr_free(&s);
}
END_TEST

START_TEST(test_vstr_cmp) {
    vstr a, b, c, d, e, f;
    int cmp;
    a = vstr_from("aaaaaaaaaa");
    b = vstr_from("aaaaaaaaaaa");
    cmp = vstr_cmp(&a, &b);
    ck_assert_int_gt(cmp, 0);

    cmp = vstr_cmp(&b, &a);
    ck_assert_int_lt(cmp, 0);

    c = vstr_from("aaaaaaaaaa");
    d = vstr_from("aaaaaaaaaa");
    cmp = vstr_cmp(&c, &d);
    ck_assert_int_eq(cmp, 0);

    e = vstr_from("aaaaaaaaaa");
    f = vstr_from("bbbbbbbbbb");
    cmp = vstr_cmp(&e, &f);
    ck_assert_int_lt(cmp, 0);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("vstr test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_vstr_push_char);
    tcase_add_test(tc_core, test_vstr_from_sm);
    tcase_add_test(tc_core, test_vstr_cmp);
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