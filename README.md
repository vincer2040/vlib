# vlib

A pure C data structure and and algorithms library

## Data structures included

- String
- Vector
- Queue
- Doubly Linked List
- Priotity Queue
- Hashtable
- AVL tree

## Algorithms included

- Binary search
- Bubble sort
- Quick sort
- Binary tree traversals
- Binary tree breadth first search
- Binary tree comparison

### String

A string implementation that is optimzed for small strings. Max string length
for a small string is 23 bytes. If it is longer than 23 bytes, it will be
allocated on the heap.

#### Available Operations

create a new vstr:

```
vstr vstr_new(void);
```

create a vstr from a C string. Must be null terminated as strlen is called:

```
vstr vstr_from(const char* cstr);
```

get the length of the vstr:

```
size_t vstr_len(vstr* s);
```

get the data buffer of a vstr:

```
char* vstr_data(vstr* s);
```

compare two vstrs:

```
int vstr_cmp(vstr* a, vstr* b);
```

append a char to a vstr:

```
int vstr_push_char(vstr* s, char c);
```

free a vstr:

```
void vstr_free(vstr* s);
```

### Vector

A generic vector implementation

#### Available Operations

Allocate a new vector

```
vec* vec_new(size_t data_size);
```

Allocate a new vector with an initial capacity

```
vec* vec_new_with_capacity(size_t data_size, size_t capacity);
```

Append data to the vector

```
int vec_push(vec** vec, void* data);
```

Remove the most recently push element from the vector

```
int vec_pop(vec* vec, void* out);
```

Retrieve data from at an index

```
void* vec_get_at(vec* vec, size_t idx);
```

Remove an element at a specific index

```
int vec_remove_at(vec* vec, size_t idx, void* out);
```

Find an element in the vector using linear search

```
ssize_t vec_find(vec* vec, void* cmp_data, void* out, CmpFn* fn);
```

Free a vector

```
void vec_free(vec* vec, FreeFn* fn);
```

Search in a vector using binary search

```
int vec_binary_search(vec* vec, void* needle, CmpFn* fn);
```

Sort a vector using bubble sort

```
void vec_bubble_sort(vec* vec, CmpFn* fn);
```

Sort a vector using quick sort

```
void vec_quick_sort(vec* vec, CmpFn* fn);
```

### Queue

A node based queue implementation

#### Available Operations

Create a new queue

```
queue queue_new(size_t data_size);
```

Get the number of elements in the queue

```
size_t queue_len(queue* q);
```

Get the element at the front of the queue without dequing

```
void* queue_peek(queue* q);
```

Enque data

```
int queue_enque(queue* q, void* data);
```

Deque data

```
int queue_deque(queue* q, void* out);
```

Free the queue

```
void queue_free(queue* q, FreeFn* fn);
```

### Doubly Linked List

A doubly linked list based on the Java interface

#### Available Operations

Create a new list

```
list list_new(size_t data_size);
```

Get the number of items in the list

```
size_t list_len(list* list);
```

Insert data at a specific index

```
int list_insert_at(list* list, void* data, size_t idx);
```

Remove a specific item from a list

```
int list_remove(list* list, void* data, void* out, CmpFn* cmp_fn);
```

Remove an item from a specific index

```
int list_remove_at(list* list, size_t idx, void* out);
```

Append data to end of the list

```
int list_append(list* list, void* data);
```

Prepend data to front of the list

```
int list_prepend(list* list, void* data);
```

Get an item from the list at a specific index

```
void* list_get(list* list, size_t idx);
```

Free the whole list

```
void list_free(list* list, FreeFn* fn);
```

### Priority Queue

A priority queue/min-heap implementation

#### Available Operations

Allocate a new pq

```
pq* pq_new(size_t data_size);
```

Insert data into priority queue

```
int pq_insert(pq** pq, void* value, CmpFn* fn);
```

Delete/poll from priority queue

```
int pq_delete(pq* pq, void* out, CmpFn* fn);
```

Free the while priority queue

```
void pq_free(pq* pq, FreeFn* fn);
```


