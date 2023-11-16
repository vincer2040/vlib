#ifndef __VLIB_H__
#define __VLIB_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define swap(a, b, size)                                                       \
    do {                                                                       \
        size_t x;                                                              \
        unsigned char* uca = ((unsigned char*)a);                              \
        unsigned char* ucb = ((unsigned char*)b);                              \
        for (x = 0; x < size; ++x) {                                           \
            unsigned char tmp = uca[x];                                        \
            uca[x] = ucb[x];                                                   \
            ucb[x] = tmp;                                                      \
        }                                                                      \
    } while (0)

typedef int CmpFn(void* a, void* b);
typedef void FreeFn(void* ptr);

typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    unsigned char data[];
} vec;

vec* vec_new(size_t data_size);
vec* vec_new_with_capacity(size_t data_size, size_t capacity);
int vec_push(vec** vec, void* data);
int vec_pop(vec* vec, void* out);
void* vec_get_at(vec* vec, size_t idx);
int vec_remove_at(vec* vec, size_t idx, void* out);
ssize_t vec_find(vec* vec, void* cmp_data, void* out, CmpFn* fn);
void vec_free(vec* vec, FreeFn* fn);

int vec_binary_search(vec* vec, void* needle, CmpFn* fn);

void vec_bubble_sort(vec* vec, CmpFn* fn);

#endif /*__VLIB_H__*/
