#include "Tokenizer.h"
#include "Helper_fucs.h"
#include "StrArr.h"

/*

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


char *first_get_max_token(const StrArr *text, Dic *dic, CPool *new_text) {
  const size_t size = text->size - 1;
  const String *arr = text->strings;
  for (size_t i = 0; i < size; ++i) {
    // printf("STARTING %zu", i);
    // printf("MAKING PAIR\n");
    String pair = str_merge(arr[i], arr[i + 1], new_text);
    // printf("PAIR MADE\n");
    // printf("INSERTING INTO DIC\n");
    dic_insert_dic(dic, pair.str);
    // printf("INSERTED INTO DIC\n");
  }
  printf("DONE JUST RETURN AND RESET DIC\n");
  // String max_token = str_deep_copy(dic->max_token, new_text);
  char *max_token = str_deep_copy_cstring(dic->max_token, new_text);
  dic_reset(dic);
  return max_token;
}

// just pass in the pointer to the strings
char *get_max_token(const StrArr *arr, StrArr *new_text, CPool *cpool_new_text,
                    Dic *dic, const char *max_token) {

  // initilization
  String string_max = char_deep_copy_cstring(max_token, cpool_new_text);
  const String *old_text = arr->strings;
  const size_t text_size = arr->size - 1;

  // make sure the first two tokens don't == max_token
  String l = str_deep_copy(old_text[0], cpool_new_text);
  String r = str_deep_copy(old_text[1], cpool_new_text);
  String tmp = str_merge(l, r, cpool_new_text);
  String curr;
  size_t i = 2;
  if (!(strcmp(tmp.str, max_token))) {
    l = string_max;
    r = str_deep_copy(old_text[2], cpool_new_text);
    i = 3;
  }

  // actual main loop
  for (; i < text_size; ++i) {
    curr = str_deep_copy(old_text[i], cpool_new_text);
    tmp = str_merge(r, curr, cpool_new_text);
    // found matching pair
    if (!strcmp(tmp.str, max_token)) {
      r = string_max;
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
    tmp = str_merge(l, string_max, cpool_new_text);
    dic_insert_dic(dic, tmp.str);
    strArr_insert(new_text, string_max);
  } else {
    tmp = str_merge(l, r, cpool_new_text);
    dic_insert_dic(dic, tmp.str);
    tmp = str_merge(r, curr, cpool_new_text);
    dic_insert_dic(dic, tmp.str);
    strArr_insert(new_text, r);
    strArr_insert(new_text, curr);
  }
  char *new_max = str_deep_copy_cstring(dic->max_token, cpool_new_text);
  dic_reset(dic);
  return new_max;
}



// the file name will always be ../data/data.txt
void read_file_thread(const char *fileName, StrArr *arr, CPool *cpool,
                      size_t start, size_t size) {
  // FILE *file = fopen("../data/data.txt", "rb");
  FILE *file = fopen(fileName, "rb");
  if (file == NULL) {
    perror("Couldn't Open file");
    exit(1);
  }
  // Move to a specific position (index) in the file
  fseek(file, start, SEEK_SET);

  // Read some data at this position
  char *buffer = (char *)malloc(size);
  size_t bytesRead = fread(buffer, 1, size, file);
  for (size_t i = 0; i < bytesRead; ++i) {
    strArr_append_char(arr, cpool, buffer[i]);
  }
  free(buffer);
}

*/
void *tokenizer_read_file(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  printf("READING FILE starting at: %zu\n", data->start);
  FILE *file = fopen("../data/data.txt", "rb");
  if (file == NULL) {
    perror("Couldn't Open file");
    exit(1);
  }
  // Move to a specific position (index) in the file
  fseek(file, data->start, SEEK_SET);

  // Read some data at this position
  char *buffer = (char *)malloc(data->bytes);
  size_t bytesRead = fread(buffer, 1, data->bytes, file);
  for (size_t i = 0; i < bytesRead; ++i) {
    strArr_append_char(data->text, data->pool_text, buffer[i]);
  }
  free(buffer);
  printf("DONE READING FILE\n");
  return NULL;
}
