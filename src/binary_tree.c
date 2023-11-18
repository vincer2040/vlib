#include "vlib.h"

static void free_walk(binary_node* node, FreeFn* fn);

void binary_tree_free(binary_tree* tree, FreeFn* fn) {
    free_walk(tree->root, fn);
    tree->num_el = 0;
}

static void free_walk(binary_node* node, FreeFn* fn) {
    if (!node) {
        return;
    }
    free_walk(node->left, fn);
    free_walk(node->right, fn);
    if (fn) {
        fn(node->data);
    }
    free(node);
}
