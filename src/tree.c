#include "vlib.h"
#include <memory.h>

#define tree_padding(size)                                                     \
    ((sizeof(void*) - ((size + 24) % sizeof(void*))) & (sizeof(void*) - 1))

#define tree_offset(key_size) key_size + tree_padding(key_size)

/**
 * @brief node in the tree
 *
 * node is a leaf if length == 0
 *
 * the key is padded to ensure that the first child ptr is on 8 byte alligned
 * address
 *
 * memory layout:
 *
 * Leaf node:
 *  -----------------------------------------
 * | len | capacity | parent | key + padding |
 *  -----------------------------------------
 *
 * Node with children:
 *  -------------------------------------------------------------------------
 * | len | capacity | parent | key + padding | child 1 | child 2 | nth child |
 *  -------------------------------------------------------------------------
 */
typedef struct tree_node {
    size_t len;
    size_t cap;
    struct tree_node* parent;
    unsigned char data[];
} tree_node;

static tree_node** depth_first_find_node(tree_node** root, void* key,
                                         size_t key_size, size_t* idx,
                                         CmpFn* fn);
static void tree_node_delete(tree_node** parent, tree_node** node,
                             size_t key_size, size_t idx, FreeFn* fn);
static int tree_node_insert(tree_node** parent, tree_node** node,
                            size_t key_size);
static int tree_node_realloc(tree_node** node, size_t key_size);
static tree_node* tree_node_new(void* key, size_t key_size);
static void tree_node_free(tree_node* node, size_t key_size, FreeFn* fn);

tree tree_new(size_t key_size) {
    tree t = {0};
    t.key_size = key_size;
    t.num_el = 0;
    t.root = NULL;
    return t;
}

int tree_depth_first_insert(tree* tree, void* key, void* par_key, CmpFn* fn) {
    tree_node* node;
    tree_node** parent;
    int insert_res;
    size_t idx;
    size_t key_size = tree->key_size;
    if (par_key == NULL) {
        if (tree->root != NULL) {
            return -1;
        }
        node = tree_node_new(key, key_size);
        if (node == NULL) {
            return -1;
        }
        node->parent = NULL;
        tree->root = node;
        return 0;
    }
    parent = depth_first_find_node(&(tree->root), par_key, key_size, &idx, fn);
    if (!parent) {
        return -1;
    }
    node = tree_node_new(key, key_size);
    if (node == NULL) {
        return -1;
    }
    insert_res = tree_node_insert(parent, &node, key_size);
    if (insert_res == 0) {
        tree->num_el++;
    }
    return insert_res;
}

bool tree_depth_first_find(tree* tree, void* key, CmpFn* fn) {
    tree_node** node;
    size_t idx;
    if (tree->root == NULL) {
        return false;
    }
    node = depth_first_find_node(&(tree->root), key, tree->key_size, &idx, fn);
    if (!node) {
        return false;
    }
    return true;
}

bool tree_breadth_first_find(tree* tree, void* key, CmpFn* fn) {
    queue q;
    size_t offset;
    if (tree->root == NULL) {
        return false;
    }
    q = queue_new(sizeof(tree_node*));
    queue_enque(&q, &(tree->root));
    offset = tree_offset(tree->key_size);
    while (q.len) {
        tree_node* cur;
        int cmp;
        size_t i, len;
        queue_deque(&q, &cur);
        cmp = fn(key, cur->data);
        if (cmp == 0) {
            queue_free(&q, NULL);
            return true;
        }
        len = cur->len;
        for (i = 0; i < len; ++i) {
            tree_node** child =
                (tree_node**)(cur->data + offset + (sizeof(tree_node*) * i));
            queue_enque(&q, child);
        }
    }
    queue_free(&q, NULL);
    return false;
}

int tree_depth_first_delete(tree* tree, void* key, CmpFn* cmp_fn,
                            FreeFn* free_fn) {
    tree_node** node;
    size_t idx, key_size = tree->key_size;
    if (tree->root == NULL) {
        return -1;
    }
    node = depth_first_find_node(&(tree->root), key, key_size, &idx, cmp_fn);
    if (!node) {
        return -1;
    }
    if ((*node)->parent) {
        tree_node_delete(&((*node)->parent), node, key_size, idx, free_fn);
    } else {
        tree_node_delete(NULL, node, key_size, idx, free_fn);
    }
    tree->num_el--;
    return 0;
}

void tree_free(tree* t, FreeFn* fn) {
    tree_node_free(t->root, t->key_size, fn);
}

static tree_node** depth_first_find_node(tree_node** root, void* key,
                                         size_t key_size, size_t* idx,
                                         CmpFn* fn) {
    int cmp;
    size_t i, len, offset;
    cmp = fn((*root)->data, key);
    if (cmp == 0) {
        return root;
    }
    len = (*root)->len;
    offset = tree_offset(key_size);
    for (i = 0; i < len; ++i) {
        tree_node** cur =
            (tree_node**)((*root)->data + offset + (sizeof(tree_node*) * i));
        *idx = i;
        tree_node** t = depth_first_find_node(cur, key, key_size, idx, fn);
        if (t) {
            return t;
        }
    }
    return NULL;
}

static void tree_node_delete(tree_node** parent, tree_node** node,
                             size_t key_size, size_t idx, FreeFn* fn) {
    size_t len = (*parent)->len - 1;
    size_t offset = tree_offset(key_size);
    if (parent) {
        tree_node* n = *node;
        if (idx == len) {
            memset((*parent)->data + offset + (idx * sizeof(tree_node*)), 0,
                   sizeof(tree_node*));
            (*parent)->len--;
            tree_node_free(n, key_size, fn);
            return;
        }
        memmove((*parent)->data + offset + (idx * sizeof(tree_node*)),
                (*parent)->data + offset + ((idx + 1) * sizeof(tree_node*)),
                (len - idx) * sizeof(tree_node*));
        (*parent)->len--;
        tree_node_free(n, key_size, fn);
    } else {
        tree_node_free(*node, key_size, fn);
    }
}

static int tree_node_insert(tree_node** parent, tree_node** node,
                            size_t key_size) {
    size_t len = (*parent)->len, cap = (*parent)->cap,
           offset = tree_offset(key_size);
    if (len == cap) {
        int realloc_res = tree_node_realloc(parent, key_size);
        if (realloc_res == -1) {
            tree_node_free(*node, key_size, NULL);
            return -1;
        }
    }
    memcpy((*parent)->data + offset + (len * sizeof(tree_node*)), node,
           sizeof(tree_node*));
    (*parent)->len++;
    (*node)->parent = *parent;
    return 0;
}

static int tree_node_realloc(tree_node** node, size_t key_size) {
    void* tmp;
    size_t len = (*node)->len, cap = (*node)->cap, needed, offset;
    if (cap == 0) {
        cap = 2;
    } else {
        cap <<= 1;
    }
    offset = tree_offset(key_size);
    needed = sizeof(tree_node) + offset + (cap * sizeof(tree_node*));
    tmp = realloc(*node, needed);
    if (tmp == NULL) {
        return -1;
    }
    *node = tmp;
    memset((*node)->data + offset + (len * sizeof(tree_node*)), 0,
           (cap - len) * sizeof(tree_node*));
    (*node)->cap = cap;
    return 0;
}

static tree_node* tree_node_new(void* key, size_t key_size) {
    tree_node* node;
    size_t offset = tree_offset(key_size);
    size_t needed = (sizeof *node) + offset;
    node = malloc(needed);
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, needed);
    memcpy(node->data, key, key_size);
    node->len = node->cap = 0;
    return node;
}

static void tree_node_free(tree_node* node, size_t key_size, FreeFn* fn) {
    size_t i, len = node->len;
    size_t offset = tree_offset(key_size);
    for (i = 0; i < len; ++i) {
        tree_node** cur =
            (tree_node**)(node->data + offset + (sizeof(tree_node*) * i));
        tree_node_free(*cur, key_size, fn);
    }
    if (fn) {
        fn(node->data);
    }
    free(node);
}
