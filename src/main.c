#include "Dic.h"
#include "StrArr.h"
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

char *first_get_max_tokens(StrArr *text, Dic *dic, CPool *cpool) {

  // might be -2? but i don't think so we don't want the last element
  const size_t size = text->size - 1;
  const String *arr = text->strings;

  for (size_t i = 0; i < size; ++i) {
    String pair = str_merge(arr[i], arr[i + 1], cpool);
    dic_insert_dic(dic, pair.str);
    // strArr_insert(new_text, pair);
  }
  char *token = dic_reset(dic);
  return token;
}

// do it this way to ensure that the string survives and we make a deep copy
void append_vocab(char **vocab, char *token, CPool *cpool, size_t index) {
  unsigned int s_size = strlen(token) + 1;
  char *new_token = cPool_get(cpool, strlen(token));
  memcpy(new_token, token, s_size);
  vocab[index++] = new_token;
}

int main() {
  // assuming vocab size the size of tokens
  CPool cpool_tokens = cPool_make(DEFAULT_VOCAB_SIZE * 5);
  CPool *cpool_tokens_ptr = &cpool_tokens;
  char *vocab[DEFAULT_VOCAB_SIZE];
  char *fileName = "../data/infiniteJest.txt";
  CPool cpool1 = cPool_make_default();
  CPool *pool_ptr1 = &cpool1;
  CPool cpool2 = cPool_make_default();
  CPool *pool_ptr2 = &cpool2;

  StrArr text = read_text(fileName, pool_ptr1);
  StrArr *text_ptr = &text;
  StrArr new_text = strArr_make(text.size + 1);
  StrArr *new_text_ptr = &new_text;
  Dic dic = dic_make_dic(text.size * 2);
  Dic *dic_ptr = &dic;

  // start of the actual logic
  char *new_vocab;

  // passing poolptr2 because must reset 1 when done
  // this is to ensure survival of strings and so we don't have a billion
  // strings on the heap
  new_vocab = first_get_max_tokens(text_ptr, dic_ptr, pool_ptr2);
  append_vocab(vocab, new_vocab, cpool_tokens_ptr, 0);
  for (size_t i = 1; i < DEFAULT_VOCAB_SIZE; ++i) {
    break;

    // will have to swap the pointers of text and new text
    // always send one and then just clear the other
    // so new_text just completley replaces text
    // you then clear text and go again and text becomes new_text
    // so just constantly swap
    // remeber to also swap their pool pointers then as well
    // new_vocab = get_max_tokens(text_ptr, new_text_ptr, dic_ptr, pool_ptr2);
    append_vocab(vocab, new_vocab, cpool_tokens_ptr, i);
  }

  // cleanup
  StrArr_free(text_ptr);
  StrArr_free(new_text_ptr);
  cPool_free(cpool_tokens_ptr);
  cPool_free(pool_ptr1);
  cPool_free(pool_ptr2);
  dic_free(dic_ptr);
  return 0;
}
