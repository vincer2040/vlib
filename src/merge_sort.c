#include "vlib.h"
#include <assert.h>
#include <memory.h>

static void ms(void* arr, size_t l, size_t r, size_t data_size,
               unsigned char* tmp, CmpFn* fn);
static void merge(void* arr, size_t l, size_t m, size_t r, size_t data_size,
                  unsigned char* tmp, CmpFn* fn);

void merge_sort(void* arr, size_t len, size_t data_size, CmpFn* fn) {
    unsigned char* tmp_arr = calloc(data_size, len);
    assert(tmp_arr != NULL);
    ms(arr, 0, len - 1, data_size, tmp_arr, fn);
    free(tmp_arr);
}

static void ms(void* arr, size_t l, size_t r, size_t data_size,
               unsigned char* tmp, CmpFn* fn) {
    size_t m;
    unsigned char* uca = arr;
    if (l < r) {
        m = l + (r - l) / 2;
        ms(arr, l, m, data_size, tmp, fn);
        ms(arr, m + 1, r, data_size, tmp, fn);
        if (fn(uca + (m * data_size), uca + ((m + 1) * data_size)) > 0) {
            merge(arr, l, m, r, data_size, tmp, fn);
        }
    }
}

static void merge(void* arr, size_t l, size_t m, size_t r, size_t data_size,
                  unsigned char* tmp, CmpFn* fn) {
    size_t i, j, k;
    size_t n1 = m - l + 1;
    size_t n2 = r - m;
    unsigned char* uca = arr;
    unsigned char* larr = tmp;
    unsigned char* rarr = larr + (n1 * data_size);

    memcpy(larr, uca + (l * data_size), n1 * data_size);
    memcpy(rarr, uca + ((m + 1) * data_size), n2 * data_size);
    i = 0;
    j = 0;
    k = l;

    while ((i < n1) && (j < n2)) {
        if (fn(larr + (i * data_size), rarr + (j * data_size)) <= 0) {
            memcpy(uca + (k * data_size), larr + (i * data_size), data_size);
            i++;
        } else {
            memcpy(uca + (k * data_size), rarr + (j * data_size), data_size);
            j++;
        }
        k++;
    }

    if (i < n1) {
        memcpy(uca + (k * data_size), larr + (i * data_size),
               (n1 - i) * data_size);
    }
    if (j < n2) {
        memcpy(uca + (k * data_size), rarr + (j * data_size),
               (n2 - j) * data_size);
    }
}
