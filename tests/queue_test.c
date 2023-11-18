#include "../src/vlib.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_queue) {
    int a0 = 5, a1 = 7, a2 = 9, a3 = 11;
    int out, *peek;
    queue q = queue_new(sizeof(int));

    queue_enque(&q, &a0);
    queue_enque(&q, &a1);
    queue_enque(&q, &a2);

    queue_deque(&q, &out);
    ck_assert_int_eq(out, 5);
    ck_assert_uint_eq(queue_len(&q), 2);

    queue_enque(&q, &a3);

    queue_deque(&q, &out);
    ck_assert_int_eq(out, 7);

    queue_deque(&q, &out);
    ck_assert_int_eq(out, 9);

    peek = queue_peek(&q);
    ck_assert_ptr_nonnull(peek);
    ck_assert_int_eq(*peek, 11);

    queue_deque(&q, &out);
    ck_assert_int_eq(out, 11);

    ck_assert_int_eq(queue_deque(&q, &out), -1);
    ck_assert_uint_eq(queue_len(&q), 0);

    queue_enque(&q, &a0);

    peek = queue_peek(&q);
    ck_assert_ptr_nonnull(peek);
    ck_assert_int_eq(*peek, 5);

    queue_deque(&q, &out);
    ck_assert_int_eq(out, 5);

    queue_free(&q, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("queue");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_queue);
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
