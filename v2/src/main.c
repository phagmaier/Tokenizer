#include "Heap.h"
#include "StringArr.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int is_printable_ascii(char c) { return c >= 32 && c <= 126; }

StrArr read_text(const char *fileName) {
  StrArr arr = strArr_make();
  FILE *file = fopen(fileName, "rb");
  if (!file) {
    perror("Error opening file");
    return arr;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  // Allocate buffer and read file
  char *buffer = (char *)malloc(file_size);
  if (!buffer) {
    perror("Memory allocation failed");
    fclose(file);
    return arr;
  }
  fread(buffer, 1, file_size, file);
  fclose(file);
  for (size_t i = 0; i < file_size; ++i) {
    strArr_append(&arr, string_make_char(buffer[i]));
  }
  free(buffer);
  return arr;
}

StrArr tokenize(StrArr *text) {
  StrArr tokens = strArr_make();
  for (size_t i = 0; i < text->size - 1; ++i) {
    char *string = string_append_strings(text->arr[i], text->arr[i + 1]);
    /*
    strArr_append(&tokens,
                  string_append_strings(text->arr[i], text->arr[i + 1]));
    */
    strArr_append(&tokens, string);
  }
  return tokens;
}

void print_tokens(char **tokens, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    printf("%s", tokens[i]);
  }
}

void make_heap(Heap *heap, StrArr *arr) {
  size_t max = 0;
  size_t tmp;
  for (int i = 0; i < 100; ++i) {
    tmp = rand();
    max = tmp > max ? tmp : max;
    size_t index = rand() % arr->size;
    heap_insert(heap, arr->arr[index], tmp);
  };
  H_node node = heap_pop(heap);
  if (max == node.count) {
    printf("GOT THE MAX NODE\n");
  } else {
    printf("MAX DID NOT MATCH\n");
  }
  printf("MAX: %zu\n", max);
  printf("MY RESULT: %zu\n", node.count);
}

int main() {
  const char *fileName = "../data/infiniteJest.txt";
  StrArr text = read_text(fileName);
  // print_tokens(text.arr, text.size);
  StrArr tokens = tokenize(&text);
  // print_tokens(tokens.arr, tokens.size);
  srand(time(NULL));
  Heap heap = heap_make_heap(text.size);
  for (size_t i = 0; i < 10; ++i) {

    make_heap(&heap, &text);
  }
  return 0;
}
