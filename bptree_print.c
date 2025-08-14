#include "bptree.h"

void bptree_print_core(NODE *node) {
	int i;
	
	printf("["); 
	for (i = 0; i < node->num_keys; i++) {
		// If internal node, recursively print child subtree first
		if (node->is_leaf == 0) {
			bptree_print_core(node->child[i]);
		}
		printf("%d", node->key[i]);
		// Add space between keys in leaf nodes only
		if (i != node->num_keys - 1 && node->is_leaf == 1) {
			putchar(' ');
		}
	}
	// Print rightmost child for internal nodes
	if (node->is_leaf == 0) {
		bptree_print_core(node->child[node->num_keys]);
	}
	printf("]");
}

void bptree_print(NODE *node) {
    if (node == NULL) {
        printf("[]\n");
        return;
    }

	bptree_print_core(node);
	printf("\n");
	fflush(stdout);
}