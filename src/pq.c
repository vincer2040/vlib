#include "vlib.h"
#include <memory.h>
#include <stdlib.h>

#define PQ_INITIAL_CAP 2

pq* pq_new(size_t data_size) {
    pq* pq;
    size_t needed = (sizeof *pq) + (data_size * PQ_INITIAL_CAP);
    pq = malloc(needed);
    if (pq == NULL) {
        return NULL;
    }
    memset(pq, 0, needed);
    pq->data_size = data_size;
    pq->cap = PQ_INITIAL_CAP;
    return pq;
}

static inline size_t parent_idx(size_t idx) { return (idx - 1) / 2; }

static inline size_t left_child_idx(size_t idx) { return (idx * 2) + 1; }

static inline size_t right_child_idx(size_t idx) { return (idx * 2) + 2; }

static void heapify_up(pq* pq, size_t idx, CmpFn* fn) {
    size_t par_idx, data_size = pq->data_size;
    void *par_val, *val;
    int cmp;
    if (idx == 0) {
        return;
    }
    for (; idx > 0; idx--) {
        par_idx = parent_idx(idx);
        par_val = pq->data + (par_idx * data_size);
        val = pq->data + (idx * data_size);
        cmp = fn(par_val, val);
        if (cmp > 0) {
            swap(val, par_val, data_size);
        }
    }
}

static void heapify_down(pq* pq, size_t idx, CmpFn* fn) {
    size_t left_idx, right_idx, len = pq->len, data_size = pq->data_size;
    void *left_val, *right_val, *val;
    int cmp_lv_rv, cmp_v_rv, cmp_v_lv;
    if (idx >= len) {
        return;
    }

    while (idx < len) {
        left_idx = left_child_idx(idx);
        if (left_idx >= len) {
            return;
        }
        right_idx = right_child_idx(idx);
        if (right_idx >= len) {
            return;
        }
        left_val = pq->data + (left_idx * data_size);
        right_val = pq->data + (right_idx * data_size);
        val = pq->data + (idx * data_size);
        cmp_lv_rv = fn(right_val, left_val);
        cmp_v_rv = fn(right_val, val);
        cmp_v_lv = fn(left_val, val);
        if ((cmp_lv_rv < 0) && (cmp_v_rv < 0)) {
            swap(val, right_val, data_size);
            idx = right_idx;
        } else if ((cmp_lv_rv > 0) && (cmp_v_lv < 0)) {
            swap(val, left_val, data_size);
            idx = left_idx;
        } else {
            break;
        }
    }
}

static int pq_realloc(pq** pq, size_t cap, size_t len, size_t len_x_size,
                      size_t data_size) {
    void* tmp;
    cap <<= 1;
    tmp = realloc(*pq, sizeof(**pq) + (cap * data_size));
    if (tmp == NULL) {
        return -1;
    }
    *pq = tmp;
    memset((*pq)->data + len_x_size, 0, (cap - len) * data_size);
    (*pq)->cap = cap;
    return 0;
}

int pq_insert(pq** pq, void* value, CmpFn* fn) {
    size_t len = (*pq)->len, cap = (*pq)->cap, data_size = (*pq)->data_size;
    size_t len_x_size = len * data_size;
    if (len == cap) {
        int realloc_res = pq_realloc(pq, cap, len, len_x_size, data_size);
        if (realloc_res == -1) {
            return -1;
        }
    }
    memcpy((*pq)->data + len_x_size, value, data_size);
    heapify_up(*pq, len, fn);
    (*pq)->len++;
    return 0;
}

int pq_delete(pq* pq, void* out, CmpFn* fn) {
    size_t len = pq->len, data_size = pq->data_size;
    size_t len_x_size;
    if (len == 0) {
        return -1;
    }
    memcpy(out, pq->data, data_size);
    if (len == 1) {
        memset(pq->data, 0, data_size);
        pq->len--;
        return 0;
    }
    pq->len--;
    len_x_size = pq->len * data_size;
    swap(pq->data, pq->data + len_x_size, data_size);
    memset(pq->data + len_x_size, 0, data_size);
    heapify_down(pq, 0, fn);
    return 0;
}

void pq_free(pq* pq, FreeFn* fn) {
    if (fn) {
        size_t i, len = pq->len, data_size = pq->data_size;

        for (i = 0; i < len; ++i) {
            void* at = pq->data + (i * data_size);
            fn(at);
        }
    }
    free(pq);
}
