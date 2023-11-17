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

#define avl_empty NULL

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef int CmpFn(void* a, void* b);
typedef void FreeFn(void* ptr);
typedef void PrintFn(void* ptr);

void get_random_bytes(uint8_t* p, size_t len);

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
void vec_quick_sort(vec* vec, CmpFn* fn);

int binary_search(void* arr, void* needle, size_t len, size_t data_size,
                  CmpFn* fn);
void bubble_sort(void* vec, size_t len, size_t data_size, CmpFn* fn);
void quick_sort(void* arr, size_t len, size_t data_size, CmpFn* fn);

struct node;

typedef struct {
    struct node* head;
    struct node* tail;
    size_t data_size;
    size_t len;
} list;

list list_new(size_t data_size);
size_t list_len(list* list);
int list_insert_at(list* list, void* data, size_t idx);
int list_remove(list* list, void* data, void* out, CmpFn* cmp_fn);
int list_remove_at(list* list, size_t idx, void* out);
int list_append(list* list, void* data);
int list_prepend(list* list, void* data);
void* list_get(list* list, size_t idx);
void list_free(list* list, FreeFn* fn);

typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    unsigned char data[];
} pq;

pq* pq_new(size_t data_size);
int pq_insert(pq** pq, void* value, CmpFn* fn);
int pq_delete(pq* pq, void* out, CmpFn* fn);
void pq_free(pq* pq, FreeFn* fn);

struct ht_entry;

#define HT_SEED_SIZE 16

typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    unsigned char seed[HT_SEED_SIZE];
    struct ht_entry** buckets;
} ht;

ht ht_new(size_t data_size);
size_t ht_len(ht* ht);
int ht_insert(ht* ht, void* key, size_t key_len, void* value, FreeFn* fn);
void* ht_get(ht* ht, void* key, size_t key_len);
int ht_delete(ht* ht, void* key, size_t key_len, FreeFn* free_key,
              FreeFn* free_val);
void ht_free(ht* ht, FreeFn* free_key, FreeFn* free_val);

#endif /*__VLIB_H__*/
