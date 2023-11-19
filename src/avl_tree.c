#include "vlib.h"
#include <memory.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct avl_node {
    ssize_t height;
    struct avl_node* left;
    struct avl_node* right;
    unsigned char key[];
} avl_node;

static avl_node* insert(avl_node* root, void* key, size_t key_size, CmpFn* fn,
                        int* success);
static avl_node* right_rotate(avl_node* y);
static avl_node* left_rotate(avl_node* y);
static avl_node* delete_node(avl_node* root, void* key, size_t key_size,
                             CmpFn* cmp_fn, FreeFn* free_fn, int* success);
static avl_node* min_value_node(avl_node* node);
static ssize_t height(avl_node* node);
static ssize_t get_balance(avl_node* node);
static void free_walk(avl_node* node, FreeFn* fn);
static void avl_pre_walk(avl_node* node, vec** vec);
static void avl_in_walk(avl_node* node, vec** vec);
static void avl_post_walk(avl_node* node, vec** vec);
static avl_node* avl_node_new(void* data, size_t data_size);
static void avl_node_free(avl_node* node, FreeFn* fn);

avl_tree avl_tree_new(size_t key_size) {
    avl_tree tree = {0};
    tree.key_size = key_size;
    tree.root = NULL;
    tree.num_el = 0;
    return tree;
}

int avl_insert(avl_tree* tree, void* key, CmpFn* fn) {
    int success = -1;
    void* root = insert(tree->root, key, tree->key_size, fn, &success);
    if (success == -1) {
        return -1;
    }
    tree->root = root;
    tree->num_el++;
    return 0;
}

int avl_delete(avl_tree* tree, void* key, CmpFn* cmp_fn, FreeFn* free_fn) {
    int success = -1;
    tree->root =
        delete_node(tree->root, key, tree->key_size, cmp_fn, free_fn, &success);
    if (success == 0) {
        tree->num_el--;
    }
    return success;
}

vec* avl_pre_order(avl_tree* tree) {
    vec* vec = vec_new_with_capacity(tree->key_size, tree->num_el);
    if (vec == NULL) {
        return NULL;
    }
    avl_pre_walk(tree->root, &vec);
    return vec;
}

vec* avl_in_order(avl_tree* tree) {
    vec* vec = vec_new_with_capacity(tree->key_size, tree->num_el);
    if (vec == NULL) {
        return NULL;
    }
    avl_in_walk(tree->root, &vec);
    return vec;
}

vec* avl_post_order(avl_tree* tree) {
    vec* vec = vec_new_with_capacity(tree->key_size, tree->num_el);
    if (vec == NULL) {
        return NULL;
    }
    avl_post_walk(tree->root, &vec);
    return vec;
}

void avl_tree_free(avl_tree* tree, FreeFn* fn) {
    free_walk(tree->root, fn);
    tree->num_el = 0;
}

static avl_node* insert(avl_node* root, void* key, size_t key_size, CmpFn* fn,
                        int* success) {
    int cmp;
    ssize_t balance;
    if (root == NULL) {
        avl_node* tmp = avl_node_new(key, key_size);
        if (tmp == NULL) {
            *success = -1;
        } else {
            *success = 0;
        }
        return tmp;
    }
    cmp = fn(key, root->key);
    if (cmp < 0) {
        root->left = insert(root->left, key, key_size, fn, success);
    } else if (cmp > 0) {
        root->right = insert(root->right, key, key_size, fn, success);
    } else {
        return root;
    }

    root->height = max(height(root->left), height(root->right)) + 1;
    balance = get_balance(root);

    if ((balance > 1) && (fn(key, root->left->key) < 0)) {
        return right_rotate(root);
    }

    if ((balance < -1) && (fn(key, root->right->key) > 0)) {
        return left_rotate(root);
    }

    if ((balance > 1) && (fn(key, root->left->key) > 0)) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if ((balance < -1) && (fn(key, root->right->key) < 0)) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

static avl_node* delete_node(avl_node* root, void* key, size_t key_size,
                             CmpFn* cmp_fn, FreeFn* free_fn, int* success) {
    int cmp;
    ssize_t balance;
    if (root == NULL) {
        return root;
    }
    cmp = cmp_fn(key, root->key);
    if (cmp < 0) {
        root->left =
            delete_node(root->left, key, key_size, cmp_fn, free_fn, success);
    } else if (cmp > 0) {
        root->right =
            delete_node(root->right, key, key_size, cmp_fn, free_fn, success);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            avl_node* tmp = root->left ? root->left : root->right;
            if (tmp == NULL) {
                tmp = root;
                root = NULL;
            } else {
                size_t needed = sizeof(avl_node) + key_size;
                memcpy(root, tmp, needed);
                avl_node_free(tmp, free_fn);
                *success = 0;
            }
        } else {
            avl_node* tmp = min_value_node(root->right);
            memcpy(root->key, tmp->key, key_size);
            root->right = delete_node(root->right, tmp->key, key_size, cmp_fn,
                                      free_fn, success);
        }
    }

    if (root == NULL) {
        return root;
    }
    root->height = max(height(root->left), height(root->right)) + 1;
    balance = get_balance(root);
    if ((balance > 1) && (get_balance(root->left) >= 0)) {
        return right_rotate(root);
    }
    if ((balance > 1) && (get_balance(root->left) < 0)) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if ((balance < -1) && (get_balance(root->right) <= 0)) {
        return left_rotate(root);
    }

    if ((balance < -1) && (get_balance(root->right) > 0)) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }
    return root;
}

static avl_node* min_value_node(avl_node* node) {
    avl_node* cur = node;
    while (cur->left != NULL) {
        cur = cur->left;
    }
    return cur;
}

static avl_node* right_rotate(avl_node* y) {
    avl_node* x = y->left;
    avl_node* t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

static avl_node* left_rotate(avl_node* x) {
    avl_node* y = x->right;
    avl_node* t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

static ssize_t height(avl_node* node) {
    if (!node) {
        return 0;
    }
    return node->height;
}

static ssize_t get_balance(avl_node* node) {
    if (!node) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

static void free_walk(avl_node* node, FreeFn* fn) {
    if (!node) {
        return;
    }

    free_walk(node->left, fn);
    free_walk(node->right, fn);
    avl_node_free(node, fn);
}

static void avl_pre_walk(avl_node* node, vec** vec) {
    if (!node) {
        return;
    }
    vec_push(vec, node->key);
    avl_pre_walk(node->left, vec);
    avl_pre_walk(node->right, vec);
}

static void avl_in_walk(avl_node* node, vec** vec) {
    if (!node) {
        return;
    }
    avl_in_walk(node->left, vec);
    vec_push(vec, node->key);
    avl_in_walk(node->right, vec);
}

static void avl_post_walk(avl_node* node, vec** vec) {
    if (!node) {
        return;
    }
    avl_post_walk(node->left, vec);
    avl_post_walk(node->right, vec);
    vec_push(vec, node->key);
}

static avl_node* avl_node_new(void* key, size_t key_size) {
    avl_node* node;
    size_t needed = (sizeof *node) + key_size;
    node = malloc(needed);
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, needed);
    memcpy(node->key, key, key_size);
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void avl_node_free(avl_node* node, FreeFn* fn) {
    if (fn) {
        fn(node->key);
    }
    free(node);
}
