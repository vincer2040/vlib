#include "vlib.h"
#include <stdbool.h>

static bool bfs(binary_node* head, void* needle, CmpFn* fn);

bool bt_bfs(binary_tree* tree, void* needle, CmpFn* fn) {
    return bfs(tree->root, needle, fn);
}

static bool bfs(binary_node* head, void* needle, CmpFn* fn) {
    queue q;
    q = queue_new(sizeof(binary_node*));

    queue_enque(&q, &head);

    while (q.len) {
        binary_node* cur;
        int cmp;
        queue_deque(&q, &cur);
        cmp = fn(cur->data, needle);
        if (cmp == 0) {
            queue_free(&q, NULL);
            return true;
        }

        if (cur->left) {
            queue_enque(&q, &(cur->left));
        }
        if (cur->right) {
            queue_enque(&q, &(cur->right));
        }
    }

    queue_free(&q, NULL);
    return false;
}
