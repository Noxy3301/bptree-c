#include <stdio.h>
#include <stddef.h>

#include "bptree.h"

int get_user_input(void) {
    int key;
    printf("Enter a key to insert : ");
    scanf("%d", &key);
    return key;
}

int main(int argc, char *argv[]) {
    (argc);
    (argv);

    // Initialize the B+tree
    bptree_init();

    // Interactive loop for inserting keys
    while (1) {
        int key = get_user_input();
        bptree_insert(key, NULL);
        bptree_print(g_root);
        printf("--------------------------------------\n");
    }

    return 0;
}