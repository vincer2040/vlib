#include "vlib.h"
#include <memory.h>

binary_node* binary_node_new(void* data, size_t data_size) {
    binary_node* node;
    size_t needed = (sizeof *node) + data_size;
    node = malloc(needed);
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, needed);
    memcpy(node->data, data, data_size);
    return node;
}
