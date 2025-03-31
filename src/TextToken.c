#include "TextToken.h"

TArr tarr_make_stack(size_t cap) {
  TArr arr;
  arr.arr = (Token *)malloc(sizeof(Token) * cap);
  arr.size = 0;
  arr.cap = cap;
  return arr;
}
void tarr_insert_char(TArr *arr, const char c, Mpool *pool) {
  if (arr->size == arr->cap) {
    arr->cap *= 2;
    arr->arr = (Token *)realloc(arr->arr, arr->cap);
  }
  token_deep_copy_char(&arr->arr[arr->size], c, pool);
  ++(arr->size);
}
void tarr_free_stack(TArr *arr) { free(arr->arr); }
void tar_reset(TArr *arr) { arr->size = 0; }

// shouuld never have to resize when you use this
void tar_insert_token(TArr *arr, Token *token) {
  arr->arr[arr->size] = *token;
  ++(arr->size);
}

void read_vocab(DicVocab *dic, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Failed to open file");
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  fseek(file, 0, SEEK_SET);
  char *buffer = (char *)malloc(file_size + 1);
  buffer[file_size] = '\n';
  size_t bytesRead = fread(buffer, 1, file_size, file);
  fclose(file);

  size_t i = 0;
  char c = buffer[0];
  size_t start = 0;
  int val;
  char *word = (char *)malloc(100);
  while (i < bytesRead) {
    while (c != ',') {
      c = buffer[++i];
    }
    memcpy(word, buffer + start, i - start);
    word[i - start] = 0; // may not need this
    val = atoi(word);
    c = buffer[++i];
    start = i;
    while (c != '\n') {
      c = buffer[++i];
    }
    memcpy(word, buffer + start, i - start);
    word[i - start] = 0; // may not need this
    Token tmp;
    tmp.string = word;
    tmp.size = (i - start) + 1;
    dicVocab_insert(dic, &tmp, val);
    // printf("INSERTED: %s\n", tmp.string);

    c = buffer[++i];
    start = i;
  }
  free(buffer);
  free(word);
}

void text_merge(TArr *arr, Mpool *pool, DicVocab *dic) {
  Token *tokens = arr->arr;
  bool changed = true;

  while (changed) {
    changed = false;
    size_t write = 0;
    size_t i = 0;
    while (i < arr->size) {
      // Try to merge tokens[i] and tokens[i+1] greedily
      if (i + 1 < arr->size) {
        Token merged;
        token_merge_deep(&merged, &tokens[i], &tokens[i + 1], pool);
        if (dicVocab_check(dic, &merged)) {
          tokens[write] = merged;
          changed = true;
          i += 2;

          // Keep trying to merge the result with the next token
          while (i < arr->size) {
            Token try_more;
            token_merge_deep(&try_more, &tokens[write], &tokens[i], pool);
            if (dicVocab_check(dic, &try_more)) {
              tokens[write] = try_more;
              ++i;
            } else {
              break;
            }
          }

          ++write;
          continue;
        }
      }

      // No merge, just move token
      if (write != i) {
        tokens[write] = tokens[i];
      }
      ++write;
      ++i;
    }

    arr->size = write;
  }
}

void tokenize_file(const char *fileName, const char *vocab_file_name,
                   unsigned int vocab_size) {

  DicVocab dic = dicVocab_make_stack(vocab_size * 3);
  read_vocab(&dic, vocab_file_name);

  FILE *file = fopen(fileName, "r");
  if (!file) {
    perror("Failed to open file");
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  fseek(file, 0, SEEK_SET);
  char *buffer = (char *)malloc(file_size + 1);
  const size_t bytesRead = fread(buffer, 1, file_size, file);
  buffer[bytesRead] = 0;

  // printf("ORIGINAL BUFFER\n");
  // printf("%s\n", buffer);

  fclose(file);

  // init data structures
  Mpool pool = mpool_make_stack(bytesRead * 5);
  Mpool *pool_ptr = &pool;
  TArr tokens = tarr_make_stack(bytesRead + 3);
  TArr *tokens_ptr = &tokens;
  for (size_t i = 0; i < bytesRead; ++i) {
    tarr_insert_char(tokens_ptr, buffer[i], pool_ptr);
  }
  free(buffer);
  text_merge(tokens_ptr, pool_ptr, &dic);

  mpool_free_stack(pool_ptr);
  tarr_free_stack(tokens_ptr);
  dicVocab_free_stack(&dic);
}
