#ifndef __VLIB_H__
#define __VLIB_H__

#include <stddef.h>
#include <sys/types.h>

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

#endif /*__VLIB_H__*/
