#include "bptree.h"

NODE *alloc_leaf(NODE *parent) {
    NODE *node;

    if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
    node->is_leaf = 1;
    node->parent = parent;
    node->num_keys = 0;

    return node;
}

TEMP *alloc_temp(NODE *node) {
    TEMP *temp;
    int i;

    if (!(temp = (TEMP *)calloc(1, sizeof(TEMP)))) ERR;
    
    // Copy keys and children from original node
    for (i = 0; i < node->num_keys; i++) {
        temp->key[i] = node->key[i];
        temp->child[i] = node->child[i];
    }
    temp->is_leaf = node->is_leaf;
    temp->num_keys = node->num_keys;

    // Internal nodes have one extra child pointer
    if (node->is_leaf == 0) {
        temp->child[temp->num_keys] = node->child[temp->num_keys];
    }

    return temp;
}

void clear_node(NODE *node) {
    int i;

    // Clear all keys and child pointers
    for (i = 0; i < node->num_keys; i++) {
        node->key[i] = 0;
        node->child[i] = NULL;
    }

    // Internal nodes have one extra child pointer
    if (node->is_leaf == 0) {
        node->child[node->num_keys] = NULL;
    }

    node->num_keys = 0;
}