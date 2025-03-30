#include "TextToken.h"

const char DELIMITERS[] = " \t\n\r\f\v.,;:!?()[]{}<>/\\|`~@#$%^&*-+=\'\"";

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

    c = buffer[++i];
    start = i;
  }
  free(buffer);
}

void text_merge(Pairs *arr, Mpool *p_text, DicVocab *dic) {
  Pair *pairs = arr->pairs;
  bool found_pair = true;
  while (found_pair && arr->size > 1) {
    found_pair = false;
    unsigned short i = 0;
    unsigned short write_index = 0;
    const unsigned short size = arr->size;
    while (i < size) {
      if (dicVocab_check(dic, &pairs[i].full)) {

        // Merging: this pair gets removed
        --(arr->size);
        found_pair = true;

        if (write_index > 0) {
          pairs[write_index - 1].right = pairs[i].full;
          token_merge_deep(&pairs[write_index - 1].full,
                           &pairs[write_index - 1].left, &pairs[i].full,
                           p_text);
        }

        if (i + 1 < size - 1) {
          pairs[i + 1].left = pairs[i].full;
          token_merge_deep(&pairs[i + 1].full, &pairs[i].full,
                           &pairs[i + 1].right, p_text);
        }

        ++i;
        // skip write_index increment – we’ve removed a pair
      } else {
        if (write_index != i) {
          pairs[write_index] = pairs[i];
        }
        ++write_index;
        ++i;
      }
    }
  }
}

void tokenize_make_words(const char *buffer, size_t start, size_t end,
                         Pairs *arr, Mpool *p_text, Ppool *p_pairs,
                         DicVocab *dic) {

  size_t size = (end - start);
  if (size == 0) {
    // One character word
    arr->pairs = ppool_get(p_pairs, 1);
    token_deep_copy_char(&arr->pairs[0].full, buffer[start], p_text);
    arr->size = 1;
    return;
  }

  Pair *pairs = ppool_get(p_pairs, size);
  arr->size = size;
  for (unsigned int i = 0; i < size; ++i) {
    token_deep_copy_char(&pairs[i].left, buffer[i + start], p_text);
    token_deep_copy_char(&pairs[i].right, buffer[i + 1 + start], p_text);
    token_merge_deep(&pairs[i].full, &pairs[i].left, &pairs[i].right, p_text);
  }
  arr->pairs = pairs;
  text_merge(arr, p_text, dic);
}

void tokenize_split(const char *buffer, ArrToken *text, Mpool *p_text,
                    Ppool *p_pairs, size_t bytesRead, DicVocab *dic) {
  size_t i = 0;
  size_t word_start = 0;
  // size_t word_count = 0;

  while (i < bytesRead) {
    if (strchr(DELIMITERS, buffer[i])) {
      if (i > word_start) {
        tokenize_make_words(buffer, word_start, i - 1, arrToken_get_pairs(text),
                            p_text, p_pairs, dic);
      }
      tokenize_make_words(buffer, i, i, arrToken_get_pairs(text), p_text,
                          p_pairs, dic);
      word_start = i + 1;
    }
    ++i;
  }
  if (i > word_start) {
    tokenize_make_words(buffer, word_start, i - 1, arrToken_get_pairs(text),
                        p_text, p_pairs, dic);
  }
}

// Take into a file for now
void tokenize_file(const char *filename, DicVocab *dic) {

  // read file
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

  // init data structures
  Mpool p_text = mpool_make_stack(file_size * 5);
  Ppool p_pairs = ppool_make_stack(file_size * 2);
  ArrToken text = arrToken_make_stack(file_size);

  // process the file
  tokenize_split(buffer, &text, &p_text, &p_pairs, bytesRead, dic);
  free(buffer);
  /*
  bool pair_found = true;
  Pairs *pairs = text.pairs;
  const size_t arr_size = text.size;
  while (pair_found) {
    pair_found = false;
    for (int i = 0; i <arr_size;++i){
      if(pairs[i].size>1){

      }
    }
  }
  */
}
