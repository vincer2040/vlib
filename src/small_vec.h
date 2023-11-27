#ifndef __SMALL_VEC_H__

#define __SMALL_VEC_H__

#include <memory.h>
#include <stddef.h>

#define _small_vec_t(type, name, capacity)                                     \
    typedef struct {                                                           \
        size_t len;                                                            \
        size_t cap;                                                            \
        type data[capacity];                                                   \
    } small_vec_##name##_t;

#define _small_vec_init(name)                                                  \
    small_vec_##name##_t small_vec_init_##name(void) {                         \
        small_vec_##name##_t vec = {0};                                        \
        vec.cap = sizeof vec.data / sizeof vec.data[0];                        \
        return vec;                                                            \
    }

#define _small_vec_len(name)                                                   \
    size_t small_vec_##name##_len(small_vec_##name##_t* vec) {                 \
        return vec->len;                                                       \
    }

#define _small_vec_push(type, name)                                            \
    int small_vec_##name##_push(small_vec_##name##_t* vec, type data) {        \
        if (vec->len == vec->cap) {                                            \
            return -1;                                                         \
        }                                                                      \
        vec->data[vec->len] = data;                                            \
        vec->len++;                                                            \
        return 0;                                                              \
    }

#define _small_vec_pop(type, name)                                             \
    int small_vec_##name##_pop(small_vec_##name##_t* vec, type* out) {         \
        size_t pop_idx;                                                        \
        if (vec->len == 0) {                                                   \
            return -1;                                                         \
        }                                                                      \
        pop_idx = vec->len - 1;                                                \
        memcpy(out, &(vec->data[pop_idx]), sizeof(type));                      \
        memset(&(vec->data[pop_idx]), 0, sizeof(type));                        \
        vec->len--;                                                            \
        return 0;                                                              \
    }

#define _small_vec_get_at(type, name)                                          \
    type* small_vec_##name##_get_at(small_vec_##name##_t* vec, size_t idx) {   \
        if (idx >= vec->len) {                                                 \
            return NULL;                                                       \
        }                                                                      \
        return &(vec->data[idx]);                                              \
    }

#define _small_vec_remove_at(type, name)                                       \
    int small_vec_##name##_remove_at(small_vec_##name##_t* vec, size_t idx,    \
                                     type* out) {                              \
        size_t new_len;                                                        \
        if (idx >= vec->len) {                                                 \
            return -1;                                                         \
        }                                                                      \
        memcpy(out, &(vec->data[idx]), sizeof(type));                          \
        if (idx == vec->len - 1) {                                             \
            memset(&(vec->data[idx]), 0, sizeof(type));                        \
            vec->len--;                                                        \
            return 0;                                                          \
        }                                                                      \
        new_len = vec->len - 1;                                                \
        memmove(&(vec->data[idx]), &(vec->data[idx + 1]),                      \
                (new_len - idx) * sizeof(type));                               \
        memset(&(vec->data[new_len]), 0, sizeof(type));                        \
        vec->len--;                                                            \
        return 0;                                                              \
    }

#define SMALL_VEC_INIT(type, name, capacity)                                   \
    _small_vec_t(type, name, capacity) _small_vec_init(name)                   \
        _small_vec_len(name) _small_vec_push(type, name)                       \
            _small_vec_pop(type, name) _small_vec_get_at(type, name)           \
                _small_vec_remove_at(type, name)

#define small_vec_t(name) small_vec_##name##_t
#define small_vec_init(name) small_vec_init_##name()
#define small_vec_len(name, vec) small_vec_##name##_len(vec)
#define small_vec_push(name, vec, data) small_vec_##name##_push(vec, data)
#define small_vec_pop(name, vec, out) small_vec_##name##_pop(vec, out)
#define small_vec_get_at(name, vec, idx) small_vec_##name##_get_at(vec, idx)
#define small_vec_remove_at(name, vec, idx, out)                               \
    small_vec_##name##_remove_at(vec, idx, out)

#endif /* __SMALL_VEC_H__ */
