#include "vlib.h"
#include <stddef.h>

void bubble_sort(void* vec, size_t len, size_t data_size, CmpFn* fn) {
    unsigned char* ucv = vec;
    size_t i;
    for (i = 0; i < len; ++i) {
        size_t j;
        for (j = 0; j < len - 1 - i; ++j) {
            void* a = ucv + (j * data_size);
            void* b = ucv + ((j + 1) * data_size);
            int cmp = fn(a, b);
            if (cmp > 0) {
                swap(a, b, data_size);
            }
        }
    }
}
