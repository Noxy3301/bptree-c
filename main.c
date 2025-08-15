#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "bptree.h"

void show_usage(void) {
    printf("Usage: add <key> | del <key> | exit\n");
}

int main(int argc, char *argv[]) {
    (argc);
    (argv);
    
    char line[100];
    char cmd[10];
    int key;

    bptree_init();
    show_usage();

    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        
        if (sscanf(line, "%s", cmd) != 1) {
            show_usage();
            continue;
        }
        
        if (strcmp(cmd, "exit") == 0) break;
        
        if (sscanf(line, "%s %d", cmd, &key) != 2) {
            show_usage();
            continue;
        }
        
        if (strcmp(cmd, "add") == 0) {
            bptree_insert(key, NULL);
            bptree_print(g_root);
        } else if (strcmp(cmd, "del") == 0) {
            bptree_delete(key);
            bptree_print(g_root);
        } else {
            show_usage();
            continue;
        }
        
        printf("--------------------------------------\n");
    }

    return 0;
}