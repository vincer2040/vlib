#include "vlib.h"
#include <assert.h>
#include <memory.h>

static void lru_prepend(lru* l, node* node);
static void lru_detach(lru* l, node* node);
static void lru_trim_cache(lru* l, FreeFn* fn);
static void node_free_from_lookup(void* ptr);
static void vstr_free_from_reverse(void* ptr);

lru lru_new(size_t cap, size_t data_size, CmpFn* cmp_keys) {
    lru l = {0};
    l.len = 0;
    l.cap = cap;
    l.data_size = data_size;
    l.lookup = ht_new(sizeof(node*), cmp_keys);
    l.reverse_lookup = ht_new(sizeof(vstr), NULL);
    return l;
}

int lru_update(lru* l, void* key, size_t key_len, void* value, FreeFn* fn) {
    node** node_ptr = ht_get(&(l->lookup), key, key_len);
    size_t data_size = l->data_size;
    if (node_ptr == NULL) {
        node* new_node = node_new(value, data_size);
        int insert_node_res;
        int insert_key_res;
        vstr key_vstr = vstr_from_len(key, key_len);
        if (new_node == NULL) {
            return -1;
        }

        insert_node_res =
            ht_insert(&(l->lookup), key, key_len, &new_node, NULL);
        if (insert_node_res == -1) {
            node_free(new_node, NULL);
            vstr_free(&key_vstr);
            return -1;
        }
        insert_key_res = ht_insert(&(l->reverse_lookup), &new_node,
                                   sizeof(node*), &key_vstr, NULL);
        if (insert_key_res == -1) {
            vstr_free(&key_vstr);
            node_free(new_node, NULL);
            return -1;
        }
        l->len++;
        lru_prepend(l, new_node);
        lru_trim_cache(l, fn);
        return 0;
    }
    if (fn) {
        fn((*node_ptr)->data);
    }
    memcpy((*node_ptr)->data, value, data_size);
    lru_detach(l, *node_ptr);
    lru_prepend(l, *node_ptr);
    return 0;
}

void* lru_get(lru* l, void* key, size_t key_len) {
    node** node_ptr = ht_get(&(l->lookup), key, key_len);
    if (node_ptr == NULL) {
        return NULL;
    }
    lru_detach(l, *node_ptr);
    lru_prepend(l, *node_ptr);
    return (*node_ptr)->data;
}

void lru_free(lru* l, FreeFn* free_key, FreeFn* free_val) {
    size_t i, len = l->len;
    if (free_val) {
        /* TODO - this is not efficient at all */
        node* cur = l->head;
        for (i = 0; i < len; ++i) {
            free_val(cur->data);
            cur = cur->next;
        }
    }
    ht_free(&(l->lookup), free_key, node_free_from_lookup);
    ht_free(&(l->reverse_lookup), NULL, vstr_free_from_reverse);
}

static void lru_detach(lru* l, node* node) {
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }

    if (node == l->head) {
        l->head = node->next;
    }

    if (node == l->tail) {
        l->tail = node->prev;
    }

    node->prev = node->next = NULL;
}

static void lru_prepend(lru* l, node* node) {
    if (l->head == NULL) {
        l->head = l->tail = node;
        return;
    }

    node->next = l->head;
    l->head->prev = node;
    l->head = node;
}

static void lru_trim_cache(lru* l, FreeFn* fn) {
    node* tail;
    vstr* key_vstr;
    void* key;
    size_t key_len;
    if (l->len <= l->cap) {
        return;
    }
    tail = l->tail;
    lru_detach(l, tail);
    key_vstr = ht_get(&(l->reverse_lookup), &tail, sizeof(node*));
    assert(key_vstr != NULL);
    key = vstr_data(key_vstr);
    key_len = vstr_len(key_vstr);
    ht_delete(&(l->lookup), key, key_len, NULL, NULL);
    ht_delete(&(l->reverse_lookup), &tail, sizeof(node*), NULL, NULL);
    node_free(tail, fn);
}

static void node_free_from_lookup(void* ptr) {
    node* n = *((node**)ptr);
    node_free(n, NULL);
}

static void vstr_free_from_reverse(void* ptr) {
    vstr* s = ptr;
    vstr_free(s);
}
