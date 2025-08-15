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


NODE *find_sibling_node(NODE *node, NODE *child_node) {
    int i;

    // Find the position of child_node in parent's children array
    for (i = 0; i < node->num_keys + 1; i++) {
        if (node->child[i] == child_node) {
            break;
        }
    }

    // If leftmost child, return right sibling; otherwise return left sibling
    if (i == 0) {
        return node->child[i + 1];
    } else {
        return node->child[i - 1];
    }
}

int find_parent_key(NODE *parent_node, NODE *child_node, NODE *sibling_node) {
    int i;
    for (i = 0; i < parent_node->num_keys; i++) {
        if (parent_node->child[i] == child_node || 
            parent_node->child[i] == sibling_node) {
            break;
        }
    }

    return parent_node->key[i];
}

NODE *find_leftmost_leaf(NODE *node) {
    while (node && !node->is_leaf) {
        node = node->child[0];
    }
    
    return node;
}