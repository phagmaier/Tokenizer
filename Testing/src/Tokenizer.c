#include "Tokenizer.h"

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

/*
 ***************************************************
                    THREADED VERSIONS
 ***************************************************
 */

// the file name will always be ../data/data.txt
void read_file_thread(StrArr *arr, CPool *cpool, size_t start, size_t end) {
  FILE *file = fopen("../data/data.txt", "rb");
  if (file == NULL) {
    perror("Couldn't Open file");
    exit(1);
  }
  size_t total_size = end - start;
  // Move to a specific position (index) in the file
  fseek(file, start, SEEK_SET);

  // Read some data at this position
  char *buffer = (char *)malloc(total_size);
  size_t bytesRead = fread(buffer, 1, total_size, file);
  for (size_t i = 0; i < bytesRead; ++i) {
    strArr_append_char(arr, cpool, buffer[i]);
  }
  free(buffer);
}
