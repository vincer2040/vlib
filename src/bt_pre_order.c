#include "vlib.h"

static void pre_order_walk(binary_node* node, vec** vec);

vec* bt_pre_order(binary_tree* tree) {
    vec* vec = vec_new(tree->data_size);
    if (vec == NULL) {
        return NULL;
    }
    pre_order_walk(tree->root, &vec);
    return vec;
}

static void pre_order_walk(binary_node* node, vec** vec) {
    if (!node) {
        return;
    }
    vec_push(vec, node->data);
    pre_order_walk(node->left, vec);
    pre_order_walk(node->right, vec);
}
