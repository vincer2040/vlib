#include "vlib.h"
#include <memory.h>

typedef struct qnode {
    struct qnode* next;
    unsigned char data[];
} qnode;

static qnode* qnode_new(void* data, size_t data_size);
static void qnode_free(qnode* node, FreeFn* fn);

queue queue_new(size_t data_size) {
    queue q = {0};
    q.head = q.tail = NULL;
    q.data_size = data_size;
    return q;
}

size_t queue_len(queue* q) { return q->len; }

void* queue_peek(queue* q) {
    if (q->len == 0) {
        return NULL;
    }
    return q->head->data;
}

int queue_enque(queue* q, void* data) {
    qnode* node = qnode_new(data, q->data_size);
    if (node == NULL) {
        return -1;
    }
    q->len++;
    if (q->head == NULL) {
        q->head = q->tail = node;
        return 0;
    }
    q->tail->next = node;
    q->tail = node;
    return 0;
}

int queue_deque(queue* q, void* out) {
    qnode* node;
    if (q->len == 0) {
        return -1;
    }
    node = q->head;
    memcpy(out, node->data, q->data_size);
    q->len--;
    if (q->len == 0) {
        qnode_free(node, NULL);
        q->head = q->tail = NULL;
        return 0;
    }
    q->head = node->next;
    qnode_free(node, NULL);
    return 0;
}

void queue_free(queue* q, FreeFn* fn) {
    size_t i, len = q->len;
    qnode* cur = q->head;
    for (i = 0; i < len; ++i) {
        qnode* next = cur->next;
        qnode_free(cur, fn);
        cur = next;
    }
    q->len = 0;
}

static qnode* qnode_new(void* data, size_t data_size) {
    qnode* node;
    size_t needed = (sizeof *node) + data_size;
    node = malloc(needed);
    if (node == NULL) {
        return NULL;
    }
    memcpy(node->data, data, data_size);
    return node;
}

static void qnode_free(qnode* node, FreeFn* fn) {
    if (fn) {
        fn(node->data);
    }
    free(node);
}
