#include "vlib.h"

static void in_order_walk(binary_node* node, vec** vec);

vec* bt_in_order(binary_tree* tree) {
    vec* v = vec_new(tree->data_size);
    if (v == NULL) {
        return NULL;
    }
    in_order_walk(tree->root, &v);
    return v;
}

static void in_order_walk(binary_node* node, vec** vec) {
    if (!node) {
        return;
    }
    in_order_walk(node->left, vec);
    vec_push(vec, node->data);
    in_order_walk(node->right, vec);
}
