#include "vlib.h"

static void post_order_walk(binary_node* node, vec** vec);

vec* bt_post_order(binary_tree* tree) {
    vec* v = vec_new(tree->data_size);
    if (v == NULL) {
        return NULL;
    }
    post_order_walk(tree->root, &v);
    return v;
}

static void post_order_walk(binary_node* node, vec** vec) {
    if (!node) {
        return;
    }
    post_order_walk(node->left, vec);
    post_order_walk(node->right, vec);
    vec_push(vec, node->data);
}
