#include "bptree.h"

void bptree_insert(int key, DATA *data) {
    NODE *leaf, *new_leaf;
    TEMP *temp;

    // Check if the tree is empty
    if (g_root == NULL) {
        // Tree is empty, create the first leaf node as root
        leaf = alloc_leaf(NULL);
        g_root = leaf;
    } else {
        // Tree exists, find the appropriate leaf node for insertion
        leaf = find_leaf(g_root, key);
    }

    // Check if we can insert without splitting
    if (leaf->num_keys < N - 1) {
        // Space available, insert directly
        insert_in_leaf(leaf, key, data);
    } else {
        // No space, split the leaf node
        // Create temporary structure to hold all keys + new key
        temp = alloc_temp(leaf);
        insert_in_temp(temp, key, (NODE *)data);

        // Create new leaf node
        new_leaf = alloc_leaf(leaf->parent);

        // Clear the original leaf node
        clear_node(leaf);

        // Redistribute keys between original and new leaf nodes
        split_temp_to_nodes(leaf, new_leaf, temp);

        // Promote key to parent level
        insert_in_parent(leaf, new_leaf->key[0], new_leaf);

        // Cleanup
        free(temp);
    }
}

NODE *insert_in_leaf(NODE *leaf, int key, DATA *data) {
    int i, j;
    
    // Find insertion position
    for (i = 0; i < leaf->num_keys; i++) {
        if (key < leaf->key[i]) {
            break;
        }
    }

    // Shift keys and children to make space
    for (j = leaf->num_keys; j > i; j--) {
        leaf->key[j] = leaf->key[j - 1];
        leaf->child[j] = leaf->child[j - 1];
    }

    // Insert new key-data pair
    leaf->key[i] = key;
    leaf->child[i] = (NODE *)data;
    leaf->num_keys++;

    return leaf;
}

TEMP *insert_in_temp(TEMP *temp, int key, NODE *child_node) {
    int i, j;

    // Find insertion position
    for (i = 0; i < temp->num_keys; i++) {
        if (key < temp->key[i]) {
            break;
        }
    }

    if (temp->is_leaf == 1) {
        // Leaf temp: shift and insert at position i
        for (j = temp->num_keys; j > i; j--) {
            temp->key[j] = temp->key[j - 1];
            temp->child[j] = temp->child[j - 1];
        }
        temp->key[i] = key;
        temp->child[i] = child_node;
    } else {
        // Internal temp: children are num_keys+1, so insert at i+1
        for (j = temp->num_keys; j > i; j--) {
            temp->key[j] = temp->key[j - 1];
            temp->child[j + 1] = temp->child[j];
        }
        temp->key[i] = key;
        temp->child[i + 1] = child_node;
    }

    temp->num_keys++;
    return temp;
}

int split_temp_to_nodes(NODE *node, NODE *new_node, TEMP *temp) {
    int i, split_index;
    split_index = (int)ceil(temp->num_keys / 2.0);

    if (temp->is_leaf == 1) {
        // Leaf node split: distribute keys evenly
        // First half goes to original node
        for (i = 0; i < split_index; i++) {
            node->key[i] = temp->key[i];
            node->child[i] = temp->child[i];
            node->num_keys++;
        }
        
        // Second half goes to new node
        for (i = 0; i < temp->num_keys - split_index; i++) {
            new_node->key[i] = temp->key[split_index + i];
            new_node->child[i] = temp->child[split_index + i];
            new_node->num_keys++;
        }
    } else {
        // Internal node split: middle key is promoted to parent
        // First half goes to original node
        for (i = 0; i < split_index; i++) {
            node->key[i] = temp->key[i];
            node->child[i] = temp->child[i];
            node->child[i]->parent = node;  // Update child's parent pointer
            node->num_keys++;
        }
        node->child[i] = temp->child[i];
        node->child[i]->parent = node;
        
        // Second half goes to new node (skip the middle key)
        for (i = 0; i < temp->num_keys - (split_index + 1); i++) {
            new_node->key[i] = temp->key[split_index + 1 + i];
            new_node->child[i] = temp->child[split_index + 1 + i];
            new_node->child[i]->parent = new_node;  // Update child's parent pointer
            new_node->num_keys++;
        }
        new_node->child[i] = temp->child[split_index + 1 + i];
        new_node->child[i]->parent = new_node;

        // Return the middle key to be promoted to parent
        return temp->key[split_index];
    }

    return 0;
}

NODE *insert_in_parent(NODE *node, int key, NODE *new_node) {
    NODE *new_root;

    if (node == g_root) {
        // Create new root when splitting the root node
        new_root = alloc_leaf(NULL);
        new_root->key[0] = key;
        new_root->child[0] = node;
        new_root->child[1] = new_node;
        new_root->num_keys = 1;
        new_root->is_leaf = 0;

        // Update global root pointer
        g_root = new_root;

        // Update parent pointers
        node->parent = g_root;
        new_node->parent = g_root;
        return g_root;

    } else {
        NODE *parent = node->parent;

        if (parent->num_keys < N - 1) {
            // Parent has space, insert directly
            insert_in_node(node, key, new_node);
        } else {
            // Parent is full, need to split
            TEMP *temp;
            NODE *new_internal;

            // Create temporary structure and add new key
            temp = alloc_temp(parent);
            insert_in_temp(temp, key, new_node);

            // Create new internal node
            new_internal = alloc_leaf(parent->parent);
            new_internal->is_leaf = 0;

            clear_node(parent);

            // Split the temporary structure into parent and new node
            int promoted_key = split_temp_to_nodes(parent, new_internal, temp);

            // Recursively promote split up the tree
            insert_in_parent(parent, promoted_key, new_internal);

            // Cleanup
            free(temp);
        }
    }

    return node;
}

NODE *insert_in_node(NODE *node, int key, NODE *child_node) {
	int i, j;
	NODE *parent = node->parent;
	
	// Find position of current node in parent's children
	for(i = 0; i < parent->num_keys + 1; i++) {
		if(parent->child[i] == node) break;
	}
	
	// Shift keys and children to make space
	for(j = parent->num_keys; j > i; j--) {
		parent->key[j] = parent->key[j - 1];
		parent->child[j + 1] = parent->child[j];
	}

	// Insert new key and child
	parent->key[i] = key;
	parent->child[i + 1] = child_node;
	parent->num_keys++;

	return node;
}