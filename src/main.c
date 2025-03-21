#include "Dic.h"
#include "StrArr.h"
#include <stdio.h>
#include <time.h>
#define DEFAULT_VOCAB_SIZE 1000

StrArr read_text(const char *fileName, CPool *cpool) {
  // StrArr arr = strArr_make(DEFAULT_NUM_STRS);
  FILE *file = fopen(fileName, "rb");
  if (!file) {
    perror("Error opening file");
    exit(1);
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
    exit(1);
  }
  fread(buffer, 1, file_size, file);
  fclose(file);
  StrArr arr = strArr_make(file_size + 1);
  for (size_t i = 0; i < file_size; ++i) {
    strArr_append_char(&arr, cpool, buffer[i]);
  }
  free(buffer);
  return arr;
}

char *first_get_max_tokens(StrArr *text, Dic *dic, CPool *cpoolPairs) {
  // might be -2? but i don't think so we don't want the last element
  const size_t size = text->size - 1;
  const String *arr = text->strings;

  for (size_t i = 0; i < size; ++i) {
    String pair = str_merge(arr[i], arr[i + 1], cpoolPairs);
    dic_insert_dic(dic, pair.str);
  }
  return dic_reset(dic);
}

// just pass in the pointer to the strings
char *get_max_token(const StrArr *arr, StrArr *new_text, CPool *cpool_new_text,
                    Dic *dic, char *max_token) {

  // initilization
  const String *old_text = arr->strings;
  const size_t text_size = arr->size - 1;
  String max_token_str;
  max_token_str.size = strlen(max_token) + 1;
  max_token_str.str = max_token;
  max_token_str = str_deep_copy(max_token_str, cpool_new_text);

  // make sure the first two tokens don't == max_token
  String l = str_deep_copy(old_text[0], cpool_new_text);
  String r = str_deep_copy(old_text[1], cpool_new_text);
  String tmp = str_merge(l, r, cpool_new_text);
  String curr;
  size_t i = 2;
  if (!(strcmp(tmp.str, max_token))) {
    l = max_token_str;
    r = str_deep_copy(old_text[2], cpool_new_text);
    i = 3;
  }

  // actual main loop
  for (; i < text_size; ++i) {
    curr = str_deep_copy(old_text[i], cpool_new_text);
    tmp = str_merge(r, curr, cpool_new_text);
    // found matching pair
    if (!strcmp(tmp.str, max_token)) {
      r = max_token_str;
    } else {
      strArr_insert(new_text, l);
      tmp = str_merge(l, r, cpool_new_text);
      dic_insert_dic(dic, tmp.str);
      l = r;
      r = curr;
    }
  }

  // FOR THE LAST COUPLE TOKENS
  strArr_insert(new_text, l);
  curr = str_deep_copy(old_text[text_size], cpool_new_text);
  tmp = str_merge(r, curr, cpool_new_text);
  // token found
  if (!strcmp(tmp.str, max_token)) {
    tmp = str_merge(l, max_token_str, cpool_new_text);
    dic_insert_dic(dic, tmp.str);
    strArr_insert(new_text, max_token_str);
  } else {
    tmp = str_merge(l, r, cpool_new_text);
    dic_insert_dic(dic, tmp.str);
    tmp = str_merge(r, curr, cpool_new_text);
    dic_insert_dic(dic, tmp.str);
    strArr_insert(new_text, r);
    strArr_insert(new_text, curr);
  }
  return dic_reset(dic);
}

char *append_vocab(char **vocab, char *token, CPool *cpool, size_t index) {
  unsigned int s_size = strlen(token) + 1;
  char *new_token = cPool_get(cpool, strlen(token));
  memcpy(new_token, token, s_size);
  vocab[index++] = new_token;
  return new_token;
}

void swap_mempool_ptrs(CPool **one, CPool **two) {
  CPool *tmp = *one;
  *one = *two;
  *two = tmp;
}

void swap_text_ptrs(StrArr **one, StrArr **two) {
  StrArr *tmp = *one;
  *one = *two;
  *two = tmp;
}

void write_vocab(char **arr, int minutes, double seconds) {
  FILE *file = fopen("../C_version.txt", "w");
  if (file == NULL) {
    perror("Error: Could not open file.\n");
    exit(1);
  }
  fprintf(file, "PROGRAM RAN IN: %d.%.0f minutes\n", minutes, seconds);
  for (size_t i = 0; i < DEFAULT_VOCAB_SIZE; i++) {
    fprintf(file, "%zu: %s\n", i, arr[i]);
  }

  fclose(file);
}

int main() {
  clock_t start = clock();
  // VOCAB INIT
  CPool cpool_tokens = cPool_make(DEFAULT_VOCAB_SIZE * 5);
  CPool *cpool_tokens_ptr = &cpool_tokens;
  char *new_vocab;
  char *vocab[DEFAULT_VOCAB_SIZE];

  char *fileName = "../data/infiniteJest.txt";

  // Init Cpools and also text and new_text
  CPool cpool_text = cPool_make_default();
  CPool *cpool_text_ptr = &cpool_text;
  StrArr text = read_text(fileName, cpool_text_ptr);
  StrArr *text_ptr = &text;
  CPool cpool_new_text = cPool_make(text.size + 5);
  CPool *cpool_new_text_ptr = &cpool_new_text;
  StrArr new_text = strArr_make(text.size + 1);
  StrArr *new_text_ptr = &new_text;
  Dic dic = dic_make_dic(text.size * 2);
  Dic *dic_ptr = &dic;

  // FIRST PASS IS SPECIAL
  new_vocab = first_get_max_tokens(text_ptr, dic_ptr, cpool_new_text_ptr);
  new_vocab = append_vocab(vocab, new_vocab, cpool_tokens_ptr, 0);
  // printf("NEW VOCAB WORD ITERATION 0: %s\n", new_vocab);

  cPool_reset(cpool_new_text_ptr);

  // MAIN LOOP
  for (size_t i = 1; i < DEFAULT_VOCAB_SIZE; ++i) {
    new_vocab = get_max_token(text_ptr, new_text_ptr, cpool_new_text_ptr,
                              dic_ptr, new_vocab);
    new_vocab = append_vocab(vocab, new_vocab, cpool_tokens_ptr, 0);
    // printf("NEW VOCAB WORD ITERATION  %zu: %s\n", i, new_vocab);
    StrArr_reset(text_ptr);
    cPool_reset(cpool_text_ptr);
    swap_mempool_ptrs(&cpool_text_ptr, &cpool_new_text_ptr);
    swap_text_ptrs(&text_ptr, &new_text_ptr);
  }
  clock_t end = clock();
  double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
  int minutes = (int)(elapsed_time / 60); // Get the whole number of minutes
  double seconds = elapsed_time - (minutes * 60); // Get the remaining seconds
  write_vocab(vocab, minutes, seconds);
  // cleanup
  StrArr_free(text_ptr);
  StrArr_free(new_text_ptr);
  cPool_free(cpool_tokens_ptr);
  cPool_free(cpool_text_ptr);
  cPool_free(cpool_new_text_ptr);
  dic_free(dic_ptr);
  return 0;
}
