/**
 * Filename:    vlib.h
 * Author:      Vince Ronnebaum (ronnebaumvince AT gmail.com)
 * License:     MIT
 * Description: This file contains public types and functions for the library.
 *
 *              Data structures:
 *              - String (vstr.c)
 *              - Vector (vec.c)
 *              - Queue (queue.c)
 *              - Doubly linked list (list.c)
 *              - Priority Queue (Min-heap) (pq.c)
 *              - Hashtable (ht.c)
 *              - avl tree (avl_tree.c)
 *              - generic tree (tree.c)
 *              - set (set.c)
 *
 *              Algorithms:
 *              - binary search (binary_search.c)
 *              - bubble sort (bubble_sort.c)
 *              - insertion sort (insertion_sort.c)
 *              - merge sort (merge_sort.c)
 *              - quick sort (quick_sort.c)
 *              - binary tree traversal (pre, in, and post order)
 *              - breadth first search on binary tree (bt_bfs.c)
 *              - binary tree comparison (bt_compare.c)
 */
#ifndef __VLIB_H__
#define __VLIB_H__

#include "small_vec.h"
#include <stdbool.h>
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
 * @brief insertion sort implementation
 * @param arr the array to sort
 * @param len the number of elements in the array
 * @param data_size the size of each element in the arary
 * @param fn the comparison function to determine if elements are equal to, less
 * than, or greater than each other
 */
void insertion_sort(void* arr, size_t len, size_t data_size, CmpFn* fn);
/**
 * @brief merge sort implementation
 * @param arr the array to sort
 * @param len the number of elements in the array
 * @param data_size the size of each element in the arary
 * @param fn the comparison function to determine if elements are equal to, less
 * than, or greater than each other
 */
void merge_sort(void* arr, size_t len, size_t data_size, CmpFn* fn);
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
#define VSTR_MAX_LARGE_SIZE ((((uint64_t)(1)) << 56) - 1)

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
 * @brief create a new vstr with capacity len + 1
 * @param len the length to reserve
 * @returns a vstr
 */
vstr vstr_new_len(size_t len);
/**
 * @brief create a new vstr from a traditional C string.
 * @param cstr the C style string. Must be null terminated (strlen is called to
 * get the length)
 * @returns a vstr struct representing cstr
 */
vstr vstr_from(const char* cstr);
/**
 * @brief create a new vstr from a traditional C string of length len
 * @param cstr the C style string
 * @param len the length of the C str
 * @returns a vstr struct representing cstr
 */
vstr vstr_from_len(const char* cstr, size_t len);
/**
 * @brief create a vstr from a formatted string
 * @param fmt the format
 * @param ... the argument list used in format
 * @returns vstr
 */
vstr vstr_format(const char* fmt, ...);
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
const char* vstr_data(vstr* s);
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
 * @brief append a string to a vstr
 * @param s the vstr to append to
 * @param str the string to append
 * @returns 0 on success, -1 on failure
 */
int vstr_push_string(vstr* s, const char* str);
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
 *      - insertion sort (vec_insertion_sort)
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
 * @brief sort the vector using insertion sort
 * @param vec the vector to sort
 * @praram fn the comparison function used to determine if elements are equal,
 * less than, or greater than
 */
void vec_insertion_sort(vec* vec, CmpFn* fn);
/**
 * @brief sort the vector using merge sort
 * @param vec the vector to sort
 * @praram fn the comparison function used to determine if elements are equal,
 * less than, or greater than
 */
void vec_merge_sort(vec* vec, CmpFn* fn);
/**
 * @brief sort the vector using quick sort
 * @param vec the vector to sort
 * @praram fn the comparison function used to determine if elements are equal,
 * less than, or greater than
 */
void vec_quick_sort(vec* vec, CmpFn* fn);

/**
 * @brief iterator over a vector
 *
 * use iter.cur to get the current element of iteration. If it is null,
 * iteration is complete
 */
typedef struct {
    void* cur;  /* the current element of iteration. NULL if iterator is done */
    void* next; /* the next element of iteration */
    size_t next_idx; /* the index of the next element of iteration */
    size_t end_idx;  /* the last index of iteration */
    vec* vec;        /* the vector being iterated over */
} vec_iter;

/**
 * @bried create a new iterator
 * @param vec the vector to iterate over
 * @returns vec_iter
 */
vec_iter vec_iter_new(vec* vec);
/**
 * @brief get the next element of iteration
 * @param iter the iterator to iterate
 */
void vec_iter_next(vec_iter* iter);

/**
 * node of the queue
 */
struct qnode;

/**
 * @brief queue data stucture
 *
 * Available operations:
 *      - enque (queue_enque)
 *      - peek (queue_peek);
 *      - get length (queue_len)
 *      - deque (queue_deque)
 */
typedef struct {
    size_t len;         /* the length of the queue */
    size_t data_size;   /* the size of the data in the nodes */
    struct qnode* head; /* the first element of the queue */
    struct qnode* tail; /* the last element of the queue */
} queue;

/**
 * @brief create a new queue
 * @param data_size the size of the data stored in the queue
 * @returns newly created queue
 */
queue queue_new(size_t data_size);
/**
 * @brief get the number of nodes in the queue
 * @param q the q to get the length of
 * @returns number of nodes in the queue
 */
size_t queue_len(queue* q);
/**
 * @brief peek at the head node's data
 * @param q the queue to peek
 * @returns the head's data, NULL if length of queue is 0
 */
void* queue_peek(queue* q);
/**
 * @brief enque data into the queue
 * @param q the queue to enque data into
 * @param data the data to enque
 * @returns 0 on succes, -1 on failure
 */
int queue_enque(queue* q, void* data);
/**
 * @brief deque from the queue
 * @param q the queue to deque from
 * @param out where to copy the deque'd data into
 * @returns 0 on success, -1 on failure
 */
int queue_deque(queue* q, void* out);
/**
 * @brief free the queue
 * @param q the queue to free
 * @param fn optional callback function to free the data in the qnodes. If null,
 * it is ignored
 */
void queue_free(queue* q, FreeFn* fn);

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
 * @param list the list to append
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
 * @brief node of a binary tree
 */
typedef struct binary_node {
    struct binary_node* left;  /* left child */
    struct binary_node* right; /* right child */
    unsigned char data[];      /* data stored in the node */
} binary_node;

/**
 * @brief allocate a new node
 * @param data the data to store in the node
 * @param data_size the size of the data to store
 * @returns newly create node on success, NULL on failure
 */
binary_node* binary_node_new(void* data, size_t data_size);

/**
 * @brief binary tree structure
 *
 * contains the root node, the size of the data in the nodes,
 * and the number of elements in the tree
 */
typedef struct {
    size_t num_el;     /* the number of nodes in the tree */
    size_t data_size;  /* size of the data in the nodes */
    binary_node* root; /* root of the tree */
} binary_tree;

/**
 * @brief free a binary tree
 * @param tree the tree to free
 * @param fn optional callback function to free the data in the nodes. If null
 * it is ignored
 */
void binary_tree_free(binary_tree* tree, FreeFn* fn);
/**
 * @brief walk the tree in pre order
 * @param tree the tree to walk
 * @returns vector of the data in pre order, NULL on failure
 */
vec* bt_pre_order(binary_tree* tree);
/**
 * @brief walk the tree in order
 * @param tree the tree to walk
 * @returns vector of the data in order, NULL on failure
 */
vec* bt_in_order(binary_tree* tree);
/**
 * @brief walk the tree in post order
 * @param tree the tree to walk
 * @returns vector of the data in post order, NULL on failure
 */
vec* bt_post_order(binary_tree* tree);
/**
 * @brief search for a value in binary tree using breadth first search
 * @param tree the tree to search
 * @param needle the value to search for
 * @param fn comparison function to tell if needle and current value are equal
 * @returns true on found, false on not found
 */
bool bt_bfs(binary_tree* tree, void* needle, CmpFn* fn);
/**
 * @brief compare two binary trees
 * @param a binary tree to compare
 * @param b binary tree to compare
 * @param fn comparison function to tell if data in a is equal to data in b
 * @returns true if equal, false if not equal
 */
bool bt_compare(binary_tree* a, binary_tree* b, CmpFn* fn);

/**
 * node in avl_tree struct
 */
struct avl_node;

/**
 * @brief avl tree data structure
 *
 * Available operations
 *      - insert (avl_insert)
 *      - delete (avl_delete)
 *      - pre order traversal (avl_pre_order)
 *      - in order traversal (avl_in_order)
 *      - post order traversal (avl_post_order)
 */
typedef struct {
    size_t num_el;
    size_t key_size;
    struct avl_node* root;
} avl_tree;

/**
 * @brief create a new avl tree
 * @param key_size the size of the key stored in the tree's nodes
 * @returns avl_tree
 */
avl_tree avl_tree_new(size_t key_size);
/**
 * @brief insert a key into the tree
 * @param tree the tree to insert into
 * @param key the key to insert
 * @param fn comparison function to determine if keys are equal, less than, or
 * greater than
 * @returns 0 on success, -1 on failure
 */
int avl_insert(avl_tree* tree, void* key, CmpFn* fn);
/**
 * @brief delete a key from the tree
 * @param tree the tree to delete from
 * @param key the key to delete
 * @param cmp_fn comparison function to determine if keys are equal, less than,
 * or greater than
 * @param free_fn optional callback function to free the key in the deleted
 * node. If null, it is ignored
 * @returns 0 on success, -1 on failure
 */
int avl_delete(avl_tree* tree, void* key, CmpFn* cmp_fn, FreeFn* free_fn);
/**
 * @brief pre order traversal of the tree
 * @param tree the tree to do the traversal
 * @returns vector of keys in pre order, NULL on failure
 */
vec* avl_pre_order(avl_tree* tree);
/**
 * @brief in order traversal of the tree
 * @param tree the tree to do the traversal
 * @returns vector of keys in  order, NULL on failure
 */
vec* avl_in_order(avl_tree* tree);
/**
 * @brief post order traversal of the tree
 * @param tree the tree to do the traversal
 * @returns vector of keys in post order, NULL on failure
 */
vec* avl_post_order(avl_tree* tree);
/**
 * @brief free the tree
 * @param fn optional callback function to free the data in the nodes. If null,
 * it is ignored
 */
void avl_tree_free(avl_tree* tree, FreeFn* fn);

/**
 * node in tree data structure
 */
struct tree_node;

/**
 * @brief a generic tree structure
 *
 * Available operations
 *      - depth first find (tree_depth_first_find)
 *      - breadth first find (tree_breadth_first_find)
 *      - depth first insert (tree_depth_first_insert)
 *      - breadth first insert (tree_breadth_first_insert)
 *      - depth first delete (tree_depth_first_delete)
 *      - breadth first delete (tree_breadth_first_delete)
 */
typedef struct {
    size_t num_el;          /* number of elements in the tree */
    size_t key_size;        /* size of the keys in the tree */
    struct tree_node* root; /* root node */
} tree;

/**
 * @brief create a new tree
 * @param key_size size of the key in the tree
 * @returns newly created tree
 */
tree tree_new(size_t key_size);
/**
 * @brief determine if a key is in the tree using depth first search
 * @param tree the tree to search in
 * @param key the key to search for
 * @param fn comparison function to determine if keys are equal
 * @returns true on found, false on not found
 */
bool tree_depth_first_find(tree* tree, void* key, CmpFn* fn);
/**
 * @brief determine if a key is in the tree using breadth first search
 * @param tree the tree to search in
 * @param key the key to search for
 * @param fn comparison function to determine if keys are equal
 * @returns true on found, false on not found
 */
bool tree_breadth_first_find(tree* tree, void* key, CmpFn* fn);
/**
 * @brief insert a key into the tree using depth first search
 * @param tree the tree to insert into
 * @param key the key to insert
 * @param par_key the parent key to insert the child into
 * @param fn comparison function to determine if keys are equal
 * @returns 0 on success, -1 on failure
 */
int tree_depth_first_insert(tree* tree, void* key, void* par_key, CmpFn* fn);
/**
 * @brief insert a key into the tree using breadth first search
 * @param tree the tree to insert into
 * @param key the key to insert
 * @param par_key the parent key to insert the child into
 * @param fn comparison function to determine if keys are equal
 * @returns 0 on success, -1 on failure
 */
int tree_breadth_first_insert(tree* tree, void* key, void* par_key, CmpFn* fn);
/**
 * @brief delete a key from the tree using depth first search
 * @param tree the tree to delete from
 * @param key the key to delete
 * @param cmp_fn comparison function to determine if keys are equal
 * @param free_fn option callback function to free the key in the deleted node.
 * If null, it is ignored
 * @returns 0 on success, -1 on failure
 */
int tree_depth_first_delete(tree* tree, void* key, CmpFn* cmp_fn,
                            FreeFn* free_fn);
/**
 * @brief delete a key from the tree using breadth first search
 * @param tree the tree to delete from
 * @param key the key to delete
 * @param cmp_fn comparison function to determine if keys are equal
 * @param free_fn option callback function to free the key in the deleted node.
 * If null, it is ignored
 * @returns 0 on success, -1 on failure
 */
int tree_breadth_first_delete(tree* tree, void* key, CmpFn* cmp_fn,
                              FreeFn* free_fn);
/**
 * @brief free the whole tree
 * @param tree the tree to free
 * @param fn option callback function to free the key in the deleted node. If
 * null, it is ignored
 */
void tree_free(tree* tree, FreeFn* fn);

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
 * @brief an entry in the hashtable
 *
 * the key is padded to ensure that data is on an 8 byte aligned address
 *
 * Memory layout:
 *  --------------------------------
 * | key_len | key + padding | data |
 *  --------------------------------
 */
typedef struct ht_entry {
    size_t key_len;
    unsigned char data[];
} ht_entry;
ht_entry* ht_entry_new(void* key, size_t key_len, void* data, size_t data_size);
void ht_entry_free(ht_entry* entry, FreeFn* free_key, FreeFn* free_val);

typedef struct {
    size_t len;
    size_t cap;
    struct ht_entry** entries;
} ht_bucket;

#define HT_SEED_SIZE 16
#define HT_INITIAL_CAP 32
#define HT_BUCKET_INITIAL_CAP 2

/**
 * @brief hashtable implementation
 *
 * siphash 1-2 is used to hash the keys
 *
 * Available operations:
 *      - len (ht_len)
 *      - has (ht_has)
 *      - insert (ht_insert)
 *      - try insert (ht_try_insert)
 *      - get (ht_get)
 *      - delete (ht_delete)
 */
typedef struct {
    size_t len;       /* the number of entries in the table */
    size_t cap;       /* the number of slots available in the table */
    size_t data_size; /* the size of the data in the table */
    CmpFn* cmp_key;   /* optional function to compare keys. If null, memcmp is
                         used */
    unsigned char seed[HT_SEED_SIZE]; /* seed used to hash the keys*/
    ht_bucket* buckets;               /* slots of the table */
} ht;

/**
 * @brief create a new hashtable
 * @param data_size the size of the data to store
 * @param cmp_key optional function to compare keys
 * @return hashtable
 */
ht ht_new(size_t data_size, CmpFn* cmp_key);
/**
 * @brief get the number of entries in a table
 * @param ht the table to get the number of entries in
 * @returns number of entries in the table
 */
size_t ht_len(ht* ht);
/**
 * @brief check if key is in the table
 * @param ht the ht to search in
 * @param key the key to search for
 * @param key_len the size of the key
 * @returns true on found, false on not found
 */
bool ht_has(ht* ht, void* key, size_t key_len);
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
 * @brief try to insert a value into the table. If key is already in table,
 * don't insert
 * @param ht the table to insert into
 * @param key the key to insert
 * @param key_len the size of the key
 * @param value the value to insert
 * @returns 0 on success, -1 on failure
 */
int ht_try_insert(ht* ht, void* key, size_t key_len, void* value);
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

/**
 * @brief set data structure
 *
 * Available operations:
 *      - len (set_len)
 *      - has (set_has)
 *      - insert (set_insert)
 *      - delete (set_delete)
 */
typedef struct {
    size_t len;     /* the number of elements in the set */
    size_t cap;     /* the number of buckets in the set */
    CmpFn* cmp_key; /* optional function to compare keys. If null, memcmp is
                       used */
    unsigned char seed[HT_SEED_SIZE]; /* seed used to hash the keys */
    ht_bucket* buckets;               /* slots of the table */
} set;

/**
 * @brief create a new set
 * @param cmp_key optional key comparison function
 * @returns newly created set
 */
set set_new(CmpFn* cmp_key);
/**
 * @brief get the number of elements in the set
 * @param set the set to get the number of elements in
 * @returns number of elements in the set
 */
size_t set_len(set* set);
/**
 * @brief check if a key is in the set
 * @param set the set to search in
 * @param key the key to search for
 * @param key_len the size of the key
 * @returns true on found, false on not found
 */
bool set_has(set* set, void* key, size_t key_len);
/**
 * @brief insert a key in the set
 * @param set the set to insert into
 * @param key the key to insert
 * @param key_len the size of the key
 * @returns 0 on success, -1 on failure
 */
int set_insert(set* set, void* key, size_t key_len);
/**
 * @brief delete a key from the set
 * @param set the set to delete from
 * @param key the key to delete
 * @param key_len the size of the key
 * @param free_fn optional callback function to free the key in the set. if
 * null, it is ignored
 * @returns 0 on success, -1 on failure
 */
int set_delete(set* set, void* key, size_t key_len, FreeFn* free_fn);
/**
 * @brief free the whole set
 * @param set the set to free
 * @param free_fn optional callback function to free the key in the set. if
 * null, it is ignored
 */
void set_free(set* set, FreeFn* free_fn);

/**
 * @brief an lru data structure
 *
 * Available operations
 *
 *      - update (lru_update)
 *      - get (lru_get)
 */
typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    node* head;
    node* tail;
    ht lookup;
    ht reverse_lookup;
} lru;

/**
 * @brief create a new lru
 * @param cap the capacity of the lru
 * @param data_size size of data stored in lru
 * @param cmp_keys optional key comparison function
 * @returns newly created lru
 */
lru lru_new(size_t cap, size_t data_size, CmpFn* cmp_keys);
/**
 * @brief update a key in the lru
 * @param l the lru to update in
 * @param key the key to update
 * @param key_len the size of the key
 * @param value the value to update with
 * @param fn optional callback function to free the old value
 * @returns 0 on success, -1 on failure
 */
int lru_update(lru* l, void* key, size_t key_len, void* value, FreeFn* fn);
/**
 * @brief get a value from the lru
 * @param l the lru to get from
 * @param key the key to get
 * @param key_len the size of the key
 * @returns value on success, NULL on failure
 */
void* lru_get(lru* l, void* key, size_t key_len);
/**
 * @brief free the lru
 * @param l the lru to free
 * @param free_key optional callback function to free the key
 * @param free_val optional callback function to free the value
 */
void lru_free(lru* l, FreeFn* free_key, FreeFn* free_val);

#endif /*__VLIB_H__*/
