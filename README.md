# vlib

A pure C data structure and and algorithms library

The goal of this project is to provide reusable, fast, and efficient
data structures

## Data structures included

- [String](#string)
- [Vector](#vector)
- [Queue](#queue)
- [Doubly Linked List](#doubly-linked-list)
- [Priotity Queue](#priority-queue)
- [Hashtable](#hashtable)
- [AVL Tree](#avl-tree)
- [Generic Tree](#generic-tree)
- [LRU](#lru)
- [Set](#set)
- [Small Vector](#small-vector)

## Algorithms included

- Binary search
- Bubble sort
- Insertion sort
- Merge sort
- Quick sort
- Tree traversals

## Getting started

### Requirements

- cmake >= 3.10

- gcc

### Installing

1. Ensure you have the items listed in [Requirements](#requirements)

2. Clone this repo

```
git clone git@github.com:vincer2040/vlib.git
cd vlib
```

3. Build it

```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

4. Optionally run tests

from within the build directory:

```
make test
```

5. Install

the default path is /usr/local/lib

```
make install
```

### usage

`#include "vlib.h"` into your project and compile vlib along with
your binary. The library should be statically linked.

### String

A string implementation that is optimzed for small strings. Max string length
for a small string is 23 bytes. If it is longer than 23 bytes, it will be
allocated on the heap.

#### Available Operations

create a new vstr:

```c
vstr vstr_new(void);
```

create a vstr from a C string. Must be null terminated as strlen is called:

```c
vstr vstr_from(const char* cstr);
```

create a new vstr from a traditional C string of length len

```c
vstr vstr_from_len(const char* cstr, size_t len);
```

get the length of the vstr:

```c
size_t vstr_len(vstr* s);
```

get the data buffer of a vstr:

```c
const char* vstr_data(vstr* s);
```

compare two vstrs:

```c
int vstr_cmp(vstr* a, vstr* b);
```

append a char to a vstr:

```c
int vstr_push_char(vstr* s, char c);
```

append a string to a vstr

```c
int vstr_push_string(vstr* s, const char* str);
```

free a vstr:

```c
void vstr_free(vstr* s);
```

### Vector

A generic vector implementation

#### Available Operations

Allocate a new vector

```c
vec* vec_new(size_t data_size);
```

Allocate a new vector with an initial capacity

```c
vec* vec_new_with_capacity(size_t data_size, size_t capacity);
```

Append data to the vector

```c
int vec_push(vec** vec, void* data);
```

Remove the most recently push element from the vector

```c
int vec_pop(vec* vec, void* out);
```

Retrieve data from at an index

```c
void* vec_get_at(vec* vec, size_t idx);
```

Remove an element at a specific index

```c
int vec_remove_at(vec* vec, size_t idx, void* out);
```

Find an element in the vector using linear search

```c
ssize_t vec_find(vec* vec, void* cmp_data, void* out, CmpFn* fn);
```

Free a vector

```c
void vec_free(vec* vec, FreeFn* fn);
```

Search in a vector using binary search

```c
int vec_binary_search(vec* vec, void* needle, CmpFn* fn);
```

Sort a vector using bubble sort

```c
void vec_bubble_sort(vec* vec, CmpFn* fn);
```

sort a vector using insertion sort

```c
void vec_insertion_sort(vec* vec, CmpFn* fn);
```

sort a vector using merge sort

```c
void vec_merge_sort(vec* vec, CmpFn* fn);
```

Sort a vector using quick sort

```c
void vec_quick_sort(vec* vec, CmpFn* fn);
```

### Queue

A node based queue implementation

#### Available Operations

Create a new queue

```c
queue queue_new(size_t data_size);
```

Get the number of elements in the queue

```c
size_t queue_len(queue* q);
```

Get the element at the front of the queue without dequing

```c
void* queue_peek(queue* q);
```

Enque data

```c
int queue_enque(queue* q, void* data);
```

Deque data

```c
int queue_deque(queue* q, void* out);
```

Free the queue

```c
void queue_free(queue* q, FreeFn* fn);
```

### Doubly Linked List

A doubly linked list based on the Java interface

#### Available Operations

Create a new list

```c
list list_new(size_t data_size);
```

Get the number of items in the list

```c
size_t list_len(list* list);
```

Insert data at a specific index

```c
int list_insert_at(list* list, void* data, size_t idx);
```

Remove a specific item from a list

```c
int list_remove(list* list, void* data, void* out, CmpFn* cmp_fn);
```

Remove an item from a specific index

```c
int list_remove_at(list* list, size_t idx, void* out);
```

Append data to end of the list

```c
int list_append(list* list, void* data);
```

Prepend data to front of the list

```c
int list_prepend(list* list, void* data);
```

Get an item from the list at a specific index

```c
void* list_get(list* list, size_t idx);
```

Free the whole list

```c
void list_free(list* list, FreeFn* fn);
```

### Priority Queue

A priority queue/min-heap implementation

#### Available Operations

Allocate a new pq

```c
pq* pq_new(size_t data_size);
```

Insert data into priority queue

```c
int pq_insert(pq** pq, void* value, CmpFn* fn);
```

Delete/poll from priority queue

```c
int pq_delete(pq* pq, void* out, CmpFn* fn);
```

Free the while priority queue

```c
void pq_free(pq* pq, FreeFn* fn);
```

### Hashtable

A hashtable implementation

- uses siphash 1-2 for hashing

- uses buckets of vectors for handling collisions

#### Available Operations

create a new hashtable

```c
ht ht_new(size_t data_size, CmpFn* cmp_keys);
```

get the number of entries in a table

```c
size_t ht_len(ht* ht);
```

insert a value into the table

```c
int ht_insert(ht* ht, void* key, size_t key_len, void* value, FreeFn* fn);
```

try to insert a value into the table. if key is already in table, don't insert

```c
int ht_try_insert(ht* ht, void* key, size_t key_len, void* value);
```

retrieve an entry from the table

```c
void* ht_get(ht* ht, void* key, size_t key_len);
```

remove an entry from the table

```c
int ht_delete(ht* ht, void* key, size_t key_len, FreeFn* free_key,
              FreeFn* free_val);
```

free the whole table

```c
void ht_free(ht* ht, FreeFn* free_key, FreeFn* free_val);
```

### AVL Tree

an avl tree implementation

#### Available Operations

create a new avl tree

```c
avl_tree avl_tree_new(size_t key_size);
```

insert a key into the tree

```c
int avl_insert(avl_tree* tree, void* key, CmpFn* fn);
```

delete a key from the tree

```c
int avl_delete(avl_tree* tree, void* key, CmpFn* cmp_fn, FreeFn* free_fn);
```

pre order traversal of the tree

```c
vec* avl_pre_order(avl_tree* tree);
```

in order traversal of the tree

```c
vec* avl_in_order(avl_tree* tree);
```

post order traversal of the tree

```c
vec* avl_post_order(avl_tree* tree);
```

free the tree

```c
void avl_tree_free(avl_tree* tree, FreeFn* fn);
```


### Generic Tree

a generic tree implementation

#### Available Operations


create a new tree

```c
tree tree_new(size_t key_size);
```

determine if a key is in the tree using depth first search

```c
bool tree_depth_first_find(tree* tree, void* key, CmpFn* fn);
```

determine if a key is in the tree using breadth first search

```c
bool tree_breadth_first_find(tree* tree, void* key, CmpFn* fn);
```

insert a key into the tree using depth first search

```c
int tree_depth_first_insert(tree* tree, void* key, void* par_key, CmpFn* fn);
```

insert a key into the tree using breadth first search

```c
int tree_breadth_first_insert(tree* tree, void* key, void* par_key, CmpFn* fn);
```

delete a key from the tree using depth first search

```c
int tree_depth_first_delete(tree* tree, void* key, CmpFn* cmp_fn,
                            FreeFn* free_fn);
```

delete a key from the tree using breadth first search

```c
int tree_breadth_first_delete(tree* tree, void* key, CmpFn* cmp_fn,
                              FreeFn* free_fn);
```

free the whole tree

```c
void tree_free(tree* tree, FreeFn* fn);
```

### LRU

an least recently used cache implementation

#### Available Operations

create a new lru

```c
lru lru_new(size_t cap, size_t data_size, CmpFn* cmp_keys);
```

update a key in the lru

```c
int lru_update(lru* l, void* key, size_t key_len, void* value, FreeFn* fn);
```

get a value from the lru

```c
void* lru_get(lru* l, void* key, size_t key_len);
```

free the lru

```c
void lru_free(lru* l, FreeFn* free_key, FreeFn* free_val);
```

### Set

a generic hash set implementation

#### Available Operations

create a new set

```c
set set_new(CmpFn* cmp_key);
```

get the number of elements in the set

```c
size_t set_len(set* set);
```

check if a key is in the set

```c
bool set_has(set* set, void* key, size_t key_len);
```

insert a key in the set

```c
int set_insert(set* set, void* key, size_t key_len);
```

delete a key from the set

```c
int set_delete(set* set, void* key, size_t key_len, FreeFn* free_fn);
```

free the whole set

```c
void set_free(set* set, FreeFn* free_fn);
```

### Small Vector

a small vector implementation. all memory for the vector is allocated
on the stack

#### Available Operations

initialize the vector's type and functions

```c
SMALL_VEC_INIT(type, name, capacity);
```

initialize a vector

```c
small_vec_t(name) vec = small_vec_init(name);
```

get the number of elements in the vector

```c
size_t len = small_vec_len(name, &vec);
```

append an element to the vector

```c
int res = small_vec_push(name, &vec, 5);
```

remove the last element in the vector

```c
int res = small_vec_pop(name, &vec, &out);
```

get element at specific index in the vector

```c
int* res = small_vec_get_at(name, &vec, 0);
```

remove an element at a specific index in the vector

```c
int res = small_vec_remove_at(name, &vec, 0);
```

