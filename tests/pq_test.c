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

START_TEST(test_pq) {
    pq* pq = pq_new(sizeof(int));
    int a = 5, b = 3, c = 69, d = 420, e = 4, f = 1, g = 8, h = 7;
    int out;
    pq_insert(&pq, &a, cmp);
    pq_insert(&pq, &b, cmp);
    pq_insert(&pq, &c, cmp);
    pq_insert(&pq, &d, cmp);
    pq_insert(&pq, &e, cmp);
    pq_insert(&pq, &f, cmp);
    pq_insert(&pq, &g, cmp);
    pq_insert(&pq, &h, cmp);

    ck_assert_uint_eq(pq->len, 8);

    pq_delete(pq, &out, cmp);
    ck_assert_int_eq(out, 1);

    pq_delete(pq, &out, cmp);
    ck_assert_int_eq(out, 3);

    pq_delete(pq, &out, cmp);
    ck_assert_int_eq(out, 4);

    pq_delete(pq, &out, cmp);
    ck_assert_int_eq(out, 5);

    ck_assert_uint_eq(pq->len, 4);

    pq_delete(pq, &out, cmp);
    ck_assert_int_eq(out, 7);

    pq_delete(pq, &out, cmp);
    ck_assert_int_eq(out, 8);

    pq_delete(pq, &out, cmp);
    ck_assert_int_eq(out, 69);

    pq_delete(pq, &out, cmp);
    ck_assert_int_eq(out, 420);

    ck_assert_uint_eq(pq->len, 0);

    pq_free(pq, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("pq test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_pq);
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
