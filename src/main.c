#include "Dic.h"
#include "Heap.h"
#include "StringArr.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>

typedef struct MemPool_char {
  size_t free_count;
  size_t in_use_count;
  struct MemPool_char *next;
} MemPool_char;

typedef struct Max_and_tokens {
  char *max_token;
  StrArr *arr;
} Max_and_tokens;

size_t dic_count(Dic *dic) {
  size_t count = 0;
  for (size_t i = 0; i < dic->cap; ++i) {
    if (dic->nodes[i].string) {
      count += dic->nodes[i].count;
    }
  }
  return count;
}

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

char *get_max_token(Dic *dic, Heap *heap) {
  for (size_t i = 0; i < dic->cap; ++i) {
    if (dic->nodes[i].string) {
      heap_insert(heap, dic->nodes[i].string, dic->nodes[i].count);
    }
  }
  char *max_token = heap_pop(heap).string;
  dic_reset(dic);
  return max_token;
}

char *first_tokenize(StrArr *text, Dic *dic, Heap *heap) {
  StrArr tokens = strArr_make();
  for (size_t i = 0; i < text->size - 1; ++i) {
    char *string = string_append_strings(text->arr[i], text->arr[i + 1]);
    strArr_append(&tokens, string);
    dic_insert_dic(dic, string);
  }
  return get_max_token(dic, heap);
}

char *tokenize(StrArr *text, Dic *dic, Heap *heap, char *max_token) {
  StrArr tokens = strArr_make();
  for (size_t i = 0; i < text->size - 1; ++i) {
    char *string = string_append_strings(text->arr[i], text->arr[i + 1]);
    strArr_append(&tokens, string);
    dic_insert_dic(dic, string);
  }
  return get_max_token(dic, heap);
}

void print_tokens(char **tokens, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    printf("%s", tokens[i]);
  }
}

int main() {
  const char *fileName = "../data/infiniteJest.txt";
  StrArr text = read_text(fileName);
  char *max_token = NULL;
  Dic dic;
  dic_make_dic(&dic);
  Heap heap = heap_make_heap(text.size);
  max_token = first_tokenize(&text, &dic, &heap);
  printf("MAX TOKEN: %s\n", max_token);
  return 0;
}
