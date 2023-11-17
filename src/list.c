#include "vlib.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

static node* list_get_at(list* list, size_t idx);
static void list_remove_node(list* list, node* node, void* out,
                             size_t data_size);

list list_new(size_t data_size) {
    list l = {0};
    l.data_size = data_size;
    l.head = l.tail = NULL;
    l.len = 0;
    return l;
}

size_t list_len(list* list) { return list->len; }

int list_insert_at(list* list, void* data, size_t idx) {
    node *node, *cur;
    if (idx > list->len) {
        return -1;
    } else if (idx == 0) {
        return list_prepend(list, data);
    } else if (idx == list->len) {
        return list_append(list, data);
    }

    node = node_new(data, list->data_size);
    if (node == NULL) {
        return -1;
    }
    list->len++;
    cur = list_get_at(list, idx);
    node->next = cur;
    node->prev = cur->prev;
    cur->prev = node;
    cur->prev->next = node;
    return 0;
}

int list_remove(list* list, void* data, void* out, CmpFn* cmp_fn) {
    size_t i, len = list->len, data_size = list->data_size;
    node* cur;
    if (len == 0) {
        return -1;
    }

    cur = list->head;

    for (i = 0; i < len; ++i) {
        int cmp = cmp_fn(data, cur->data);
        if (cmp == 0) {
            list_remove_node(list, cur, out, data_size);
            return 0;
        }
        cur = cur->next;
    }
    return -1;
}

int list_remove_at(list* list, size_t idx, void* out) {
    node* cur;
    size_t data_size = list->data_size;
    if (idx > list->len) {
        return -1;
    }
    cur = list_get_at(list, idx);
    list_remove_node(list, cur, out, data_size);
    return 0;
}

int list_append(list* list, void* data) {
    node* node = node_new(data, list->data_size);
    if (node == NULL) {
        return -1;
    }
    list->len++;
    if (!list->tail) {
        list->head = list->tail = node;
        return 0;
    }
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
    return 0;
}

int list_prepend(list* list, void* data) {
    node* node = node_new(data, list->data_size);
    if (node == NULL) {
        return -1;
    }
    list->len++;
    if (!list->head) {
        list->head = list->tail = node;
        return 0;
    }
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
    return 0;
}

void* list_get(list* list, size_t idx) {
    node* cur;
    if (idx > list->len) {
        return NULL;
    }
    cur = list_get_at(list, idx);
    return cur->data;
}

void list_free(list* list, FreeFn* fn) {
    node* cur;
    size_t i, len = list->len;
    cur = list->head;
    for (i = 0; i < len; ++i) {
        node* next = cur->next;
        node_free(cur, fn);
        cur = next;
    }
}

static node* list_get_at(list* list, size_t idx) {
    node* cur;
    size_t i;
    cur = list->head;
    for (i = 0; i < idx; ++i) {
        cur = cur->next;
    }
    return cur;
}

static void list_remove_node(list* list, node* node, void* out,
                             size_t data_size) {
    list->len--;
    memcpy(out, node->data, data_size);
    if (list->len == 0) {
        node_free(node, NULL);
        list->head = list->tail = NULL;
        return;
    }
    if (node == list->head) {
        list->head = node->next;
        node_free(node, NULL);
        list->head->prev = NULL;
        return;
    }
    if (node == list->tail) {
        list->tail = node->prev;
        node_free(node, NULL);
        list->tail->next = NULL;
        return;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;
    node_free(node, NULL);
}

node* node_new(void* data, size_t data_size) {
    node* node;
    size_t needed = (sizeof *node) + data_size;
    node = malloc(needed);
    memset(node, 0, needed);
    node->next = node->prev = NULL;
    memcpy(node->data, data, data_size);
    return node;
}

void node_free(node* node, FreeFn* fn) {
    if (fn) {
        fn(node->data);
    }
    free(node);
}
