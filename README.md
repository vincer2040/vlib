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

A string implementation that is optimzed for small strings

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


