#include "Dic.h"
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

// have this just because you know the token will be null
// so no need for pointless check
char *first_tokenize(StrArr *text, Dic *dic) {
  StrArr tokens = strArr_make();
  for (size_t i = 0; i < text->size - 1; ++i) {
    char *string = string_append_strings(text->arr[i], text->arr[i + 1]);
    strArr_append(&tokens, string);
    dic_insert_dic(dic, string);
  }
  char *string = dic->max_token;
  dic_reset(dic);
  return string;
}

// all of these strings need to be owned or part of a mempool
char *tokenize(StrArr *text, Dic *dic, char *max_token) {
  StrArr new_text = strArr_make();
  char **tokens = text->arr;
  char *pl = tokens[0];
  char *pr = tokens[1];
  char *curr;
  char *tmp = str_make_str_from_strings(pl, pr);
  unsigned int x = 2;
  // NEED TO HAVE L & R AND THEY CAN'T BE THE TOKEN
  if (!strcmp(tmp, max_token)) {
    pl = max_token;
    pr = tokens[2];
    x = 3;
  }
  for (size_t i = x; i < text->size - 1; ++i) {
    curr = tokens[i];
    tmp = str_make_str_from_strings(pr, curr);
    if (!strcmp(tmp, max_token)) {
      pr = max_token;
    } else {
      strArr_append(&new_text, pl);
      tmp = str_make_str_from_strings(pl, pr);
      dic_insert_dic(dic, tmp);
      pl = pr;
      pr = curr;
    }
  }
  strArr_append(&new_text, pl);
  curr = tokens[text->size - 1];
  tmp = str_make_str_from_strings(pr, curr);
  if (!strcmp(tmp, max_token)) {
    tmp = str_make_str_from_strings(pl, max_token);
    dic_insert_dic(dic, tmp);
    strArr_append(&new_text, tmp);
  } else {
    tmp = str_make_str_from_strings(pl, max_token);
    dic_insert_dic(dic, tmp);
    tmp = str_make_str_from_strings(pr, curr);
    dic_insert_dic(dic, tmp);
    strArr_append(&new_text, pr);
    strArr_append(&new_text, curr);
  }

  char *string = dic->max_token;
  dic_reset(dic);
  return string;
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
  max_token = first_tokenize(&text, &dic);
  printf("MAX TOKEN: %s\n", max_token);
  return 0;
}
