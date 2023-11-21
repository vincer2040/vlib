#include "vlib.h"
#include <memory.h>

void insertion_sort(void* arr, size_t len, size_t data_size, CmpFn* fn) {
    size_t i;
    unsigned char* uca = arr;
    void* key = malloc(data_size);
    memset(key, 0, data_size);
    for (i = 1; i < len; ++i) {
        size_t j = i - 1;
        memcpy(key, uca + (i * data_size), data_size);
        while ((j >= 0) && (fn(uca + (j * data_size), key) > 0)) {
            memmove(uca + ((j + 1) * data_size), uca + (j * data_size), data_size);
            j--;
        }
        memcpy(uca + ((j + 1) * data_size), key, data_size);
    }

    free(key);
}
