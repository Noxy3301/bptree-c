CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm

TARGET = bptree
SOURCES = main.c \
		  bptree.c \
		  bptree_memory.c \
		  bptree_util.c \
		  bptree_insert.c \
		  bptree_delete.c \
		  bptree_scan.c \
		  bptree_print.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files
%.o: %.c bptree.h debug.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
