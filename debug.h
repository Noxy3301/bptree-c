#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#define ERR do { \
    fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
    perror(""); \
    exit(1); \
} while (0)

#endif // DEBUG_H