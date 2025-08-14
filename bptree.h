#ifndef BPTREE_H
#define BPTREE_H

#include <stddef.h>
#include <math.h>

#include "debug.h"

#define N 4 // Maximum number of children per node

// Data structure to hold the actual data
typedef struct data {
    int value;
} DATA;

// B+tree node structure
typedef struct node {
    int num_keys;
    int key[N - 1];
    struct node *child[N];
    struct node *parent;
    int is_leaf; // 1 if leaf, 0 if internal node
} NODE;

// Temporary structure for node splitting
typedef struct temp {
    int num_keys;
    int key[N];
    struct node *child[N + 1];
    int is_leaf; // 1 if leaf, 0 if internal node
} TEMP;

// Global variables
extern NODE *g_root;

// ====================
// Initialization
// ====================

/**
 * @brief Initialize the B+tree structure
 */
void bptree_init(void);

// ====================
// Memory management
// ====================

/**
 * @brief Allocate and initialize a new node
 * @param parent Pointer to parent node (NULL for root)
 * @return Pointer to newly allocated node
 */
NODE *alloc_leaf(NODE *parent);

/**
 * @brief Create temporary structure
 * @param node Source node to copy data from
 * @return Pointer to temporary structure
 */
TEMP *alloc_temp(NODE *node);

/**
 * @brief Clear all keys and children from node for reuse
 * @param node Node to clear (not freed, just reset)
 */
void clear_node(NODE *node);

// ====================
// Search
// ====================

/**
 * @brief Find the leaf node where key should be located
 * @param node Current node to start search from
 * @param key Key value to search for
 * @return Pointer to leaf node where key should be inserted/found
 */
NODE *find_leaf(NODE *node, int key);

// ====================
// Insert
// ====================

/**
 * @brief Insert key-data pair into B+ tree with automatic splitting
 * @param key Key to insert
 * @param data Associated data
 */
void bptree_insert(int key, DATA *data);

/**
 * @brief Insert key-data into leaf node (space must be available)
 * @param leaf Target leaf node
 * @param key Key to insert
 * @param data Associated data
 * @return Modified leaf node
 */
NODE *insert_in_leaf(NODE *leaf, int key, DATA *data);

/**
 * @brief Insert key-child into temporary structure during node splitting
 * @param temp Temporary structure to insert into
 * @param key Key to insert
 * @param child_node Child node pointer to insert
 * @return Modified temporary structure
 */
TEMP *insert_in_temp(TEMP *temp, int key, NODE *child_node);

/**
 * @brief Split overflowing temporary structure into two nodes
 * @param node Original node to receive first half
 * @param new_node New node to receive second half
 * @param temp Temporary structure containing all keys
 * @return Key to be promoted to parent node
 */
int split_temp_to_nodes(NODE *node, NODE *new_node, TEMP *temp);

/**
 * @brief Handle parent insertion after node split
 * @param node The node that was split
 * @param key Key to be promoted to parent
 * @param new_node New node created from split
 * @return Original node pointer
 */
NODE *insert_in_parent(NODE *node, int key, NODE *new_node);

/**
 * @brief Insert key-child pair into parent's internal node
 * @param node Child node to locate insertion position
 * @param key Key to insert in parent
 * @param child_node New child node to insert
 * @return Original node pointer
 */
NODE *insert_in_node(NODE *node, int key, NODE *child_node);

// ====================
// Delete
// ====================


// ====================
// Debug
// ====================

/**
 * @brief Print entire B+tree structure
 * @param node Root node of the tree
 */
void bptree_print(NODE *node);

/**
 * @brief Recursively print tree structure (internal use)
 * @param node Current node to print
 */
void bptree_print_core(NODE *node);

#endif // BPTREE_H