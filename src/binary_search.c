#include "vlib.h"
#include <stddef.h>

int binary_search(void* arr, void* needle, size_t len, size_t data_size,
                  CmpFn* fn) {
    size_t lo = 0;
    size_t hi = len;
    unsigned char* uca = arr;
    do {
        size_t m = lo + (hi - lo) / 2;
        void* at = uca + (m * data_size);
        int cmp = fn(at, needle);
        if (cmp == 0) {
            return 0;
        } else if (cmp > 0) {
            hi = m;
        } else {
            lo = m + 1;
        }
    } while (lo < hi);
    return -1;
}
