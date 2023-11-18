#include "vlib.h"

static void pre_order_walk(binary_node* node, vec** vec);

vec* bt_pre_order(binary_tree* tree) {
    vec* vec = vec_new_with_capacity(tree->data_size, tree->num_el);
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
