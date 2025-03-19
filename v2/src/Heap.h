#pragma once
#include "StringArr.h"
#include <stdio.h>
typedef struct H_node {
  char *string;
  size_t count;
} H_node;

typedef struct Heap {
  H_node *arr;
  size_t size;
  size_t cap;
} Heap;

H_node Hnode_make(char *string, size_t count);
Heap heap_make_heap(size_t size);

size_t heap_left(size_t index);
size_t heap_right(size_t index);
size_t heap_parent(size_t index);
size_t heap_max_child_index(H_node *heap, size_t parent);
void heap_swap(H_node *heap, size_t parent, size_t child);
void heap_insert(Heap *heap, char *string, size_t count);
H_node heap_pop(Heap *heap);
