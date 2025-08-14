#include "bptree.h"

NODE *find_leaf(NODE *node, int key) {
    int kid;

    // If current node is leaf, return it
    if (node->is_leaf == 1) {
        return node;
    }

    // Find appropriate child to traverse
    for (kid = 0; kid < node->num_keys; kid++) {
        if (key < node->key[kid]) {
            break;
        }
    }

    // Recursively search in child node
    return find_leaf(node->child[kid], key);
}