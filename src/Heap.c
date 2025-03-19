#include "Heap.h"

H_node Hnode_make(char *string, size_t count) {
  H_node node;
  node.string = string;
  node.count = count;
  return node;
}

Heap heap_make_heap(size_t size) {
  Heap heap;
  heap.arr = (H_node *)calloc(size, sizeof(H_node));
  heap.size = 0;
  heap.cap = size;
  return heap;
}

size_t heap_left(size_t index) { return 2 * index + 1; }
size_t heap_right(size_t index) { return 2 * index + 2; }
size_t heap_parent(size_t index) { return (index - 1) / 2; }

void heap_swap(H_node *heap, size_t parent, size_t child) {
  H_node tmp = heap[parent];
  heap[parent] = heap[child];
  heap[child] = tmp;
}

// Left child index = 2 * i + 1
// Right child index = 2 * i + 2
// Parent index = (i - 1) / 2
void heap_insert(Heap *heap, char *string, size_t count) {
  if (heap->size == heap->cap) {
    printf("ERROR SHOULD NEVER HAVE TO RESIZE HEAP\n");
    heap->cap *= 2;
    heap->arr = (H_node *)realloc(heap->arr, sizeof(H_node) * heap->cap);
  }
  if (!heap->size) {
    heap->arr[0] = Hnode_make(string, count);
    ++(heap->size);
    return;
  }
  size_t child_index = heap->size;
  size_t parent_index = heap_parent(child_index);
  H_node *arr = heap->arr;
  arr[child_index] = Hnode_make(string, count);

  while (child_index > 0 && arr[child_index].count > arr[parent_index].count) {
    heap_swap(arr, parent_index, child_index);
    child_index = parent_index;
    parent_index = heap_parent(parent_index);
  }
  ++(heap->size);
}

size_t heap_max_child_index(H_node *heap, size_t parent) {
  size_t l_index = heap_left(parent);
  size_t r_index = heap_right(parent);
  return heap[l_index].count > heap[r_index].count ? l_index : r_index;
}

// actually don't need to do anything after pop cause
// the heap will be deleted
H_node heap_pop(Heap *heap) {
  heap->size = 0;
  return heap->arr[0];
}
