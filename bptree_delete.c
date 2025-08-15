#include "bptree.h"

void bptree_delete(int key) {
    NODE *leaf;

    leaf = find_leaf(g_root, key);
    delete_entry(leaf, key, NULL);
}

void delete_entry(NODE *node, int key, NODE *child_node) {
    NODE *sibling_node, *temp_node;
    int parent_key, borrow_index, i;

    // Delete key and child pointer from the node
    delete_from_node(node, key, child_node);

    // Root shrinking: (key=1, child=2) → delete → (key=0, child=1)
    // Promote the only remaining child to become new root
    if (node->parent == NULL && count_child(node) == 1) {
        g_root = node->child[0];    // After shift(delete_from_node), only child[0] remains
        g_root->parent = NULL;
        free(node);
        return;
    }

    // Check for underflow (leaf: keys < ⌈(N-1)/2⌉, internal: children < ⌈N/2⌉)
    int is_leaf_underflow = (node->is_leaf && node->num_keys < (int)ceil((N - 1) / 2.0));
    int is_internal_underflow = (!node->is_leaf && node->num_keys + 1 < (int)ceil(N / 2.0));
    
    if (node->parent != NULL && (is_leaf_underflow || is_internal_underflow)) {
        sibling_node = find_sibling_node(node->parent, node);
        parent_key = find_parent_key(node->parent, node, sibling_node);

        // Check if merge is possible
        int total_keys = node->num_keys + sibling_node->num_keys;
        int total_children = count_child(node) + count_child(sibling_node);
        
        if (total_keys < N && total_children < N + 1) {
            // Merge with sibling node
            // Ensure sibling_node->node order
            if (check_node_order(node->parent, node, sibling_node) == 1) {
                temp_node = sibling_node;
                sibling_node = node;
                node = temp_node;
            }

            if (node->is_leaf == 0) {
                // Internal node
                sibling_node->key[sibling_node->num_keys] = parent_key;
                sibling_node->num_keys++;
                merge_node_into_sibling_node(node, sibling_node);
            } else {
                // Leaf node
                merge_node_into_sibling_node(node, sibling_node);
                sibling_node->parent = node->parent;
            }
    
            delete_entry(node->parent, parent_key, node);
            free(node);
        } else {
            // Cannot merge, redistribute by borrowing from sibling
            if (check_node_order(node->parent, node, sibling_node) == 0) {
                // Borrow from right sibling
                if (node->is_leaf == 0) {
                    // Internal node: borrow last child and move parent key down
                    borrow_index = sibling_node->num_keys;

                    // Shift existing elements right to make space
                    for (i = node->num_keys; i > 0; i--) {
                        node->key[i] = node->key[i - 1];
                    }
                    for (i = count_child(node); i > 0; i--) {
                        node->child[i] = node->child[i - 1];
                    }

                    // Insert parent key and borrowed child
                    node->key[0] = parent_key;
                    node->child[0] = sibling_node->child[borrow_index];
                    node->child[0]->parent = node;
                    node->num_keys++;

                    // Update parent key with sibling's promoted key
                    for (i = 0; i < node->parent->num_keys; i++) {
                        if (node->parent->key[i] == parent_key) {
                            node->parent->key[i] = sibling_node->key[borrow_index - 1];
                            break;
                        }
                    }

                    // Remove borrowed elements from sibling
                    sibling_node->key[borrow_index - 1] = 0;
                    sibling_node->child[borrow_index] = NULL;
                    sibling_node->num_keys--;
                } else {
                    // Leaf node: borrow last key-data pair
                    borrow_index = sibling_node->num_keys - 1;
                    insert_in_leaf(node, sibling_node->key[borrow_index], (DATA *)sibling_node->child[borrow_index]);
                    
                    // Update parent boundary key
                    for (i = 0; i < node->parent->num_keys; i++) {
                        if (node->parent->key[i] == parent_key) {
                            node->parent->key[i] = sibling_node->key[borrow_index];
                            break;
                        }
                    }

                    // Remove borrowed element from sibling
                    sibling_node->key[borrow_index] = 0;
                    sibling_node->child[borrow_index] = NULL;
                    sibling_node->num_keys--;
                }
            } else {
                // Borrow from left sibling
                if (sibling_node->is_leaf == 0) {
                    // Internal node: move parent key down and borrow first child
                    node->key[node->num_keys] = parent_key;
                    node->child[node->num_keys + 1] = sibling_node->child[0];
                    node->child[node->num_keys + 1]->parent = node;
                    node->num_keys++;

                    // Update parent key with sibling's first key
                    for (i = 0; i < sibling_node->parent->num_keys; i++) {
                        if (sibling_node->parent->key[i] == parent_key) {
                            sibling_node->parent->key[i] = sibling_node->key[0];
                            break;
                        }
                    }

                    delete_from_node(sibling_node, sibling_node->key[0], sibling_node->child[0]);
                } else {
                    // Leaf node: borrow first key-data pair
                    node->key[node->num_keys] = sibling_node->key[0];
                    node->child[node->num_keys] = sibling_node->child[0];
                    node->num_keys++;

                    // Update parent boundary key
                    for (i = 0; i < sibling_node->parent->num_keys; i++) {
                        if (sibling_node->parent->key[i] == parent_key) {
                            sibling_node->parent->key[i] = sibling_node->key[0];
                            break;
                        }
                    }

                    delete_from_node(sibling_node, sibling_node->key[0], sibling_node->child[0]);
                }
            }
        }
    }
}

void delete_from_node(NODE *node, int key, NODE *child_node) {
    int i, j, data_index;

    // Find the key to delete
    for (i = 0; i < node->num_keys; i++) {
        if (node->key[i] == key) {
            break;
        }
    }
    
    // Save key position for data index in leaf node
    data_index = i;

    // Shift keys left
    for (i; i < node->num_keys - 1; i++) {
        node->key[i] = node->key[i + 1];
    }

    if (child_node == NULL) {
        // Leaf node: delete data at same index as key
        for(data_index; data_index < node->num_keys - 1; data_index++) {
            node->child[data_index] = node->child[data_index + 1];
        }
    } else {
        // Internal node: find and delete specific child pointer
        for (j = 0; j < node->num_keys + 1; j++) {
            if (node->child[j] == child_node) {
                break;
            }
        }

        for (j; j < node->num_keys; j++) {
            node->child[j] = node->child[j + 1];
        }
    }

    // Clear last elements (now duplicated or stale after shifting)
    node->key[node->num_keys - 1] = 0;
    node->child[node->num_keys] = NULL;
    node->num_keys--;
}


void merge_node_into_sibling_node(NODE *node, NODE *sibling_node) {
    int i;

    // Copy all keys and children from node to sibling_node
	for(i = 0; i < node->num_keys; i++) {
		sibling_node->key[sibling_node->num_keys + i] = node->key[i];
		sibling_node->child[sibling_node->num_keys + i] = node->child[i];
		if(sibling_node->is_leaf == 0) {
			sibling_node->child[sibling_node->num_keys + i]->parent = sibling_node;	// Update parent pointer
		}
	}

	sibling_node->num_keys += node->num_keys;

    // For internal nodes, copy the last child pointer
	if(node->is_leaf == 0) {
		sibling_node->child[sibling_node->num_keys] = node->child[node->num_keys];
		sibling_node->child[sibling_node->num_keys]->parent = sibling_node;
	}
}

int check_node_order(NODE *parent_node, NODE *child_node, NODE *sibling_node) {
    int i;

    // Find which node appears first in parent's children
    for (i = 0; i < parent_node->num_keys + 1; i++) {
        if (parent_node->child[i] == child_node || 
            parent_node->child[i] == sibling_node) {
            break;
        }
    }

    // Return 1 if child_node comes first
    if (parent_node->child[i] == child_node) {
        return 1;
    } else {
        return 0;
    }
}

int count_child(NODE *node) {
    int i, count = 0;

    for (i = 0; i < node->num_keys + 1; i++) {
        if (node->child[i] != NULL) {
            count++;
        }
    }

    return count;
}