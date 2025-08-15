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
// Find
// ====================

/**
 * @brief Find the leaf node where key should be located
 * @param node Current node to start search from
 * @param key Key value to search for
 * @return Pointer to leaf node where key should be inserted/found
 */
NODE *find_leaf(NODE *node, int key);

/**
 * @brief Find a sibling node for merging
 * @param node Parent node containing the child
 * @param child_node Target child node to find sibling for
 * @return Pointer to sibling node (previous or next child)
 * 
 * Strategy: Return right sibling if leftmost child, otherwise return left sibling
 */
NODE *find_sibling_node(NODE *node, NODE *child_node);

NODE *find_leftmost_leaf(NODE *node);

/**
 * @brief Find parent key between child and sibling for merge/redistribution
 * @param parent_node Parent node
 * @param child_node Target child
 * @param sibling_node Sibling node
 * @return Boundary key in parent
 */
int find_parent_key(NODE *parent_node, NODE *child_node, NODE *sibling_node);

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

/**
 * @brief Delete key from B+tree
 * @param key Key to delete from the tree
 */
void bptree_delete(int key);

/**
 * @brief Delete entry and handle tree rebalancing (internal use)
 * @param node Target node for deletion
 * @param key Key to delete
 * @param child_node Child node to delete (NULL for data stored at same index as key in leaf)
 */
void delete_entry(NODE *node, int key, NODE *child_node);

/**
 * @brief Delete key or child from node based on operation type
 * @param node Target node
 * @param key Key to delete
 * @param child_node Child node to delete (NULL for data stored at same index as key in leaf)
 */
void delete_from_node(NODE *node, int key, NODE *child_node);

/**
 * @brief Merge all keys and children from node into sibling_node
 * @param node Node to merge from
 * @param sibling_node Sibling node to merge into
 */
void merge_node_into_sibling_node(NODE *node, NODE *sibling_node);

/**
 * @brief Count the number of non-NULL child pointers in a node
 * @param node Node to count children for
 * @return Number of valid child pointers
 */
int count_child(NODE *node);

/**
 * @brief Check if child node appears before sibling in parent
 * @param parent_node Parent containing both nodes
 * @param child_node Child node to check position
 * @param sibling_node Sibling node to compare with
 * @return 1 if child appears first, 0 if sibling appears first
 */
int check_node_order(NODE *parent_node, NODE *child_node, NODE *sibling_node);

// ====================
// Scan
// ====================

/**
 * @brief Scan and print all keys in ascending order
 */
void bptree_scan_all(void);

/**
 * @brief Scan and print keys within specified range
 * @param start_key Start of range (inclusive)
 * @param end_key End of range (inclusive)
 */
void bptree_scan_range(int start_key, int end_key);

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