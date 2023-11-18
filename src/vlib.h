/**
 * Filename:    vlib.h
 * Author:      Vince Ronnebaum (ronnebaumvince AT gmail.com)
 * License:     MIT
 * Description: This file contains public types and functions for the library.
 *
 *              Data structures:
 *              - String (vstr.c)
 *              - Vector (vec.c)
 *              - Doubly linked list (list.c)
 *              - Priority Queue (Min-heap) (pq.c)
 *              - Hashtable (ht.c)
 *
 *              Algorithms:
 *              - binary search (binary_search.c)
 *              - bubble sort (bubble_sort.c)
 *              - quick sort (quick_sort.c)
 */
#ifndef __VLIB_H__
#define __VLIB_H__

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/**
 * a macro for swapping two elements byte by byte, useful when you don't want
 * to allocate a temporary variable
 */
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

/**
 * a macro for printing debug messages to stdout. This will print the
 * arguments as well as the file and line number that the macro is invoked
 */
#define dbgf(...)                                                              \
    do {                                                                       \
        printf("%s:%d ", __FILE__, __LINE__);                                  \
        printf(__VA_ARGS__);                                                   \
        fflush(stdout);                                                        \
    } while (0)

/**
 * Utility function types used in a number of data structures and algorithms
 * within this library
 */

/**
 * utility compare function type
 */
typedef int CmpFn(void* a, void* b);
/**
 * Utility free function type
 */
typedef void FreeFn(void* ptr);

/**
 * @brief utility function for putting random bytes into a buffer of len length
 * @param p the buffer to put the random bytes into
 * @param len the available length of p
 */
void get_random_bytes(uint8_t* p, size_t len);

/**
 * @brief binary search implementation
 * @param arr the array to search in
 * @param needle the element to search for
 * @param len the number of elements in the array
 * @param data_size the size of each element of the array
 * @param fn the compare function to determine if needle is equal to an element
 * @returns 0 when needle is found, -1 when needle is not found
 */
int binary_search(void* arr, void* needle, size_t len, size_t data_size,
                  CmpFn* fn);
/**
 * @brief bubble sort implementation
 * @param arr the arary to sort
 * @param len the number of elements in the array
 * @param data_size the size of each element in the arary
 * @param fn the comparison function to determine if elements are equal to, less
 * than, or greater than each other
 */
void bubble_sort(void* arr, size_t len, size_t data_size, CmpFn* fn);
/**
 * @brief quick sort implementation
 * @param arr the arary to sort
 * @param len the number of elements in the array
 * @param data_size the size of each element in the arary
 * @param fn the comparison function to determine if elements are equal to, less
 * than, or greater than each other
 */
void quick_sort(void* arr, size_t len, size_t data_size, CmpFn* fn);

#define VSTR_MAX_SMALL_SIZE 23
#define VSTR_MAX_LARGE_SIZE (((uint64_t)(1)) << 56)

/**
 * @brief an optimized small string representation
 *
 * This struct is used in vstr to represent small strings on the stack rather
 * than on the heap.
 */
typedef struct {
    char data[VSTR_MAX_SMALL_SIZE]; /* the string itself with max len of 23 */
} vstr_sm;

/**
 * @brief a string representation for large strings
 *
 * This struct is used in vstr to represent larger strings whose data is
 * stored on the heap. The length field is truncated to 56 bits in order to
 * fit 23 bytes so that the small strings data can be stored in the vstr
 * struct
 */
typedef struct __attribute__((__packed__)) {
    char* data;      /* the string */
    size_t cap;      /* the capacity of the data buffer - how much is allocated,
                        including null term */
    size_t len : 56; /* the length of the string - how much of the buffer is
                        used */
} vstr_lg;

/**
 * @brief a string representation that is optimized for small strings
 *
 * The main data of this struct is a union of the vstr_sm struct and
 * the vstr_lg struct. Both structs are 23 bytes. One more byte is used to
 * determine if the string contained in the structure is a small string or a
 * large string.
 *
 * When the string is small, it's length must be less than or equal to
 * VSTR_MAX_SMALL_SIZE, which is 23. Note that the null terminator will not be
 * stored in the buffer in the vstr_sm struct when the buffer is full - it will
 * be represented in memory in the last bit of this struct. This is possible
 * because the is_large field will be 0 when it is a small string, and
 * small_avail will be 0 when the buffer is full. Because the is_large and
 * small_avail fields make up one total byte, this will act as the null
 * terminator for the string. This allows us to store up to 23 bytes in a small
 * string. Any string that is larger will be allocated on the heap and stored in
 * the vstr_lg struct.
 */
typedef struct {
    union {
        vstr_sm sm;
        vstr_lg lg;
    } str_data; /* union of the struct that contains the data */
    uint8_t
        is_large : 1; /* determine if we are a small (0) or large (1) string */
    uint8_t small_avail : 7; /* the amount of space available when the string is
                             a small string */
} vstr;

/**
 * @breif create a new vstr. Default initial type is a small string
 * @returns a vstr struct
 */
vstr vstr_new(void);
/**
 * @brief create a new vstr from a traditional C string.
 * @param cstr the C style string. Must be null terminated (strlen is called to
 * get the length)
 * @returns a vstr struct representing cstr
 */
vstr vstr_from(const char* cstr);
/**
 * @brief get the length of a vstr
 * @param s pointer to the vstr
 * @returns length of the vstr
 */
size_t vstr_len(vstr* s);
/**
 * @brief return the buffer contained in a vstr
 * @param s pointer to the vstr
 * @returns the buffer contained in the vstr
 */
char* vstr_data(vstr* s);
/**
 * @brief compare two vstr's
 * @param a vstr to compare
 * @param b vstr to compare
 * @returns 0 if they are equal, non zero if they are not
 */
int vstr_cmp(vstr* a, vstr* b);
/**
 * @brief append a char to a vstr
 * @param s the vstr to append to
 * @param c the char to append
 * @returns 0 on succes, -1 on failure
 */
int vstr_push_char(vstr* s, char c);
/**
 * @brief free a vstr
 * @param s the vstr to free
 */
void vstr_free(vstr* s);

/**
 * @brief a vector representation
 *
 * This structure represents a dynamically sized array. Data added to the
 * vector is stored in the same memory allocation as seen by the use
 * of the flexible array member
 *
 * Available operations:
 *      - push (vec_push)
 *      - pop (vec_pop)
 *      - get at (vec_get_at)
 *      - remove at (vec_remove_at)
 *      - linear find (vec_find)
 *      - binary search (vec_binary_search)
 *      - bubble sort (vec_bubble_sort)
 *      - quick sort (vec_quick_sort)
 */
typedef struct {
    size_t len; /* the number of items in the vector. Also used as an insertion
                   point */
    size_t cap; /* the number of slots available in the vector. cap * data_size
                   = the amount allocated for data */
    size_t data_size;     /* the size of a single element in the vector */
    unsigned char data[]; /* FAM of the data stored in the vector */
} vec;

/**
 * @brief allocate a new vector
 * @param data_size the size of a single element in the vector
 * @returns a newly allocated vector
 */
vec* vec_new(size_t data_size);
/**
 * @brief allocate a vector with a specified initial capacity rather than the
 * default
 * @param data_size the size of a single element in the vector
 * @param capacity the initial capacity of the vector
 * @returns allocate vector with capacity
 */
vec* vec_new_with_capacity(size_t data_size, size_t capacity);
/**
 * @brief append an element to the vector
 * @param vec the vector to append to
 * @param data the data to append
 * @returns 0 on success, -1 on failure
 */
int vec_push(vec** vec, void* data);
/**
 * @brief remove the last element from the vector
 * @param vec the vector to remove from
 * @param out where the element is copied to
 * @returns 0 on success, -1 on failure
 */
int vec_pop(vec* vec, void* out);
/**
 * @brief get the element at a specific index in the vector
 * @param vec the vector to retrieve the data from
 * @param idx the index of the data to retrieve
 * @returns pointer to the data on success, NULL on failure
 */
void* vec_get_at(vec* vec, size_t idx);
/**
 * @brief remove an element from the vector at a specific index
 * @param vec the vector to remove from
 * @param idx the index of the element to remove
 * @param out where the removed data is copied to
 * @returns 0 on success, -1 on failure
 */
int vec_remove_at(vec* vec, size_t idx, void* out);
/**
 * @brief find an element using linear search
 * @param vec the vector to find the data in
 * @param cmp_data what to compare the elements against. passed as first
 * argument to CmpFn
 * @param out where to copy the found data
 * @param fn the comparison function to determine if elements are equal
 * @returns index of found element on success, -1 on failure
 */
ssize_t vec_find(vec* vec, void* cmp_data, void* out, CmpFn* fn);
/**
 * @brief free the whole vector
 * @param vec the vector to free
 * @param fn free callback function to free each element if necessary. If null,
 * it is ignored
 */
void vec_free(vec* vec, FreeFn* fn);
/**
 * @brief compute binary search on vector for an item
 * @param vec the vector to search in
 * @param needle the data to search for. passed as first argument to CmpFn
 * @param fn the comparison function to determine if elements are equal
 * @returns 0 on found, -1 on not found
 */
int vec_binary_search(vec* vec, void* needle, CmpFn* fn);
/**
 * @brief sort the vector using bubble sort
 * @param vec the vector to sort
 * @praram fn the comparison function used to determine if elements are equal,
 * less than, or greater than
 */
void vec_bubble_sort(vec* vec, CmpFn* fn);
/**
 * @brief sort the vector using quick sort
 * @param vec the vector to sort
 * @praram fn the comparison function used to determine if elements are equal,
 * less than, or greater than
 */
void vec_quick_sort(vec* vec, CmpFn* fn);

/**
 * @brief node structure used in doubly linked list
 *
 * data is stored in the same memory allocation as the node for better cache
 */
typedef struct node {
    struct node* prev;    /* the previous node in the list */
    struct node* next;    /* the next node in the list */
    unsigned char data[]; /* FAM containing the data in this node */
} node;

/**
 * @brief allocate a new node
 * @param data the data to store in this node
 * @param data_size the size of the data
 * @returns allocated node on success, NULL on failure
 */
node* node_new(void* data, size_t data_size);
/**
 * @brief free an allocated node
 * @param node the node to free
 * @param fn optional callback function to free the data stored in the node. If
 * null, it is ignored
 */
void node_free(node* node, FreeFn* fn);

/**
 * @brief doubly linked list implementation
 *
 * available operations:
 *      - get length (list_len)
 *      - append (list_append)
 *      - prepend (list_prepend)
 *      - insert at (list_insert_at)
 *      - remove (list_remove)
 *      - remove at (list_remove_at)
 *      - get at (list_get)
 */
typedef struct {
    struct node* head; /* the beginning of the list */
    struct node* tail; /* the end of the list */
    size_t data_size;  /* the size of the data stored in the nodes */
    size_t len;        /* the number of nodes in the list */
} list;

/**
 * @brief create a new list
 * @param data_size the size of the data to store in the nodes
 * @returns the newly created list
 */
list list_new(size_t data_size);
/**
 * @brief get the number of items in the list
 * @param list the list to get the length of
 * @returns number of the items in the list
 */
size_t list_len(list* list);
/**
 * @brief insert data at a specific index
 * @param list the list to insert to
 * @param data the data to insert
 * @param idx the index to insert at
 * @returns 0 on success, -1 on failure
 */
int list_insert_at(list* list, void* data, size_t idx);
/**
 * @brief remove a specific item from a list
 * @param list the list to remove from
 * @param data the data to compare the node's data to
 * @param out where the removed data is copied to
 * @param cmp_fn comparison function used to determine if node's data is equal
 * to param data
 * @returns 0 on success, -1 on failure
 */
int list_remove(list* list, void* data, void* out, CmpFn* cmp_fn);
/**
 * @brief remove an item from a specific index
 * @param list the list to remove from
 * @param idx the index to remove
 * @param out where the removed data is copied to
 * @returns 0 on success, -1 on failure
 */
int list_remove_at(list* list, size_t idx, void* out);
/**
 * @brief append data to end of the list
 * @brief list the list to append
 * @param data the data to append
 * @returns 0 on success, -1 on failure
 */
int list_append(list* list, void* data);
/**
 * @brief prepend data to front of the list
 * @param list the list to prepend
 * @param data the data to prepend
 * @returns 0 on success, -1 on failure
 */
int list_prepend(list* list, void* data);
/**
 * @brief get an item from the list at a specific index
 * @param list the list to get from
 * @param idx the index of the data to get
 * @returns pointer to data on success, NULL on failure
 */
void* list_get(list* list, size_t idx);
/**
 * @brief free the while list
 * @param list the list to free
 * @param fn optional callback function to free the data stored in each node. If
 * null it is ignored
 */
void list_free(list* list, FreeFn* fn);

/**
 * @brief priority queue implementation
 *
 * Available operations:
 *      - insert (pq_insert)
 *      - delete/poll (pq_delete)
 */
typedef struct {
    size_t len;       /* the number of elements in the pq*/
    size_t cap;       /* the number of available slots cap * data_size = amount
                         allocated for data */
    size_t data_size; /* the size of the elements stored */
    unsigned char data[]; /* FAM storing the data */
} pq;

/**
 * @brief allocate a new pq
 * @param data_size the size of the data to be stored
 * @returns newly allocated pq
 */
pq* pq_new(size_t data_size);
/**
 * @brief insert data into priority queue
 * @param pq the pq to insert into
 * @param value the value to insert
 * @param fn the comparison function to determine if elements are equal, less
 * than, or greater than each other
 * @returns 0 on success, -1 on failure
 */
int pq_insert(pq** pq, void* value, CmpFn* fn);
/**
 * @brief delete/poll from priority queue
 * @param pq the pq to delete/poll from
 * @param out where the removed item is copied to
 * @param fn the comparison function to determine if elements are equal, less
 * than, or greater than each other
 * @returns 0 on success, -1 on failure
 */
int pq_delete(pq* pq, void* out, CmpFn* fn);
/**
 * @brief free the while priority queue
 * @param pq the pq to free
 * @param fn optional callback function to free each element in the qp. If null,
 * it is ignored
 */
void pq_free(pq* pq, FreeFn* fn);

/**
 * entry in the hashtable. defined in ht.c
 */
struct ht_entry;

typedef struct {
    size_t len;
    size_t cap;
    struct ht_entry** entries;
} ht_bucket;

#define HT_SEED_SIZE 16

/**
 * @brief hashtable implementation
 *
 * siphash 1-2 is used to hash the keys
 *
 * Available operations:
 *      - insert (ht_insert)
 *      - get (ht_get)
 *      - delete (ht_delete)
 */
typedef struct {
    size_t len;       /* the number of entries in the table */
    size_t cap;       /* the number of slots available in the table */
    size_t data_size; /* the size of the data in the table */
    unsigned char seed[HT_SEED_SIZE]; /* seed used to hash the keys*/
    ht_bucket* buckets; /* slots of the table */
} ht;

/**
 * @brief create a new hashtable
 * @param data_size the size of the data to store
 * @return hashtable
 */
ht ht_new(size_t data_size);
/**
 * @brief get the number of entries in a table
 * @param ht the table to get the number of entries in
 * @returns number of entries in the table
 */
size_t ht_len(ht* ht);
/**
 * @brief insert a value into the table
 * @param ht the table to insert into
 * @param key the key associated with the value
 * @param key_len the size of the key
 * @param value the value to insert
 * @param fn optional callback function to free the old value if one is
 * overwritten. If null, it is ignored
 * @returns 0 on success, -1 on failure
 */
int ht_insert(ht* ht, void* key, size_t key_len, void* value, FreeFn* fn);
/**
 * @brief retrieve an entry from the table
 * @param ht the table to retrieve from
 * @param key the key of the value to get
 * @param key_len the size of the key
 * @returns pointer to value on success, NULL on failure
 */
void* ht_get(ht* ht, void* key, size_t key_len);
/**
 * @brief remove an entry from the table
 * @param ht the table to remove from
 * @param key the key to remove
 * @param key_len the size of the key
 * @param free_key optional callback function to free the key. If null, it is
 * ignored
 * @param free_val optional callback function to free the value. If null, it is
 * ignored
 * @returns 0 on success, -1 on failure
 */
int ht_delete(ht* ht, void* key, size_t key_len, FreeFn* free_key,
              FreeFn* free_val);
/**
 * @brief free the whole table
 * @param ht the table to free
 * @param free_key optional callback function to free the keys. If null, it is
 * ignored
 * @param free_val optional callback function to free the values. If null, it is
 * ignored
 */
void ht_free(ht* ht, FreeFn* free_key, FreeFn* free_val);

#endif /*__VLIB_H__*/
