#include "vlib.h"

static ssize_t partition(void* arr, ssize_t lo, ssize_t hi, size_t data_size,
                         CmpFn* fn) {
    ssize_t idx = lo - 1;
    unsigned char* uca = arr;
    void *idx_val, *pivot = uca + (hi * data_size);
    size_t i;

    for (i = lo; i < hi; ++i) {
        void* at = uca + (i * data_size);
        int cmp = fn(at, pivot);
        if (cmp <= 0) {
            void* a;
            idx++;
            a = uca + (idx * data_size);
            swap(a, at, data_size);
        }
    }

    idx++;
    idx_val = uca + (idx * data_size);
    swap(idx_val, pivot, data_size);
    return idx;
}

static void qs(void* arr, ssize_t lo, ssize_t hi, size_t data_size, CmpFn* fn) {
    ssize_t pivot_idx;
    while (lo < hi) {
        pivot_idx = partition(arr, lo, hi, data_size, fn);
        if ((pivot_idx - lo) < (hi - pivot_idx)) {
            qs(arr, lo, pivot_idx - 1, data_size, fn);
            lo = pivot_idx + 1;
        } else {
            qs(arr, pivot_idx + 1, hi, data_size, fn);
            hi = pivot_idx - 1;
        }
    }
}

void quick_sort(void* arr, size_t len, size_t data_size, CmpFn* fn) {
    qs(arr, 0, len - 1, data_size, fn);
}
