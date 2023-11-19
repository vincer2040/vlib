#include "vlib.h"

static bool binary_node_compare(binary_node* a, binary_node* b, CmpFn* fn);

bool bt_compare(binary_tree* a, binary_tree* b, CmpFn* fn) {
    if (a->num_el != b->num_el) {
        return false;
    }
    return binary_node_compare(a->root, b->root, fn);
}

static bool binary_node_compare(binary_node* a, binary_node* b, CmpFn* fn) {
    int cmp;
    if ((a == NULL) && (b == NULL)) {
        return true;
    }
    if ((a == NULL) || (b == NULL)) {
        return false;
    }
    cmp = fn(a->data, b->data);
    if (cmp != 0) {
        return false;
    }
    return binary_node_compare(a->left, b->left, fn) &&
           binary_node_compare(a->right, b->right, fn);
}
