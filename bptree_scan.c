#include "bptree.h"

void bptree_scan_all(void) {
    NODE *current_leaf;
    int i;

    if (g_root == NULL) {
        printf("[]\n");
        return;
    }

    // Start from leftmost leaf
    current_leaf = find_leftmost_leaf(g_root);

    printf("[ ");
    
    // Traverse all leaf nodes using next leaf pointers
    while (current_leaf != NULL) {
        // Print all keys in current leaf
        for (i = 0; i < current_leaf->num_keys; i++) {
            printf("%d ", current_leaf->key[i]);
        }
        
        // Move to next leaf via child[N-1]
        current_leaf = current_leaf->child[N - 1];
    }

    printf("]\n");
}

void bptree_scan_range(int start_key, int end_key) {
    NODE *current_leaf;
    int i;
    int found_start = 0;

    if (g_root == NULL) {
        printf("[]\n");
        return;
    }

    // Start from leftmost leaf
    current_leaf = find_leftmost_leaf(g_root);

    printf("[ ", start_key, end_key);
    
    // Traverse leaf nodes using next leaf pointers
    while (current_leaf != NULL) {
        // Check all keys in current leaf
        for (i = 0; i < current_leaf->num_keys; i++) {
            int key = current_leaf->key[i];
            
            // Check if key is in range
            if (key >= start_key && key <= end_key) {
                printf("%d ", key);
                found_start = 1;
            } else if (found_start && key > end_key) {
                // Past the end of range, stop scanning
                printf("]\n");
                return;
            }
        }
        
        // If we've passed the end range, stop
        if (found_start && current_leaf->num_keys > 0 && 
            current_leaf->key[current_leaf->num_keys - 1] > end_key) {
            break;
        }
        
        // Move to next leaf via child[N-1]
        current_leaf = current_leaf->child[N - 1];
    }
    
    printf("]\n");
}
