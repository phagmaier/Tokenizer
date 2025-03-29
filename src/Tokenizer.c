#include "Tokenizer.h"
#include "Dics.h"
#include "Token.h"
#include <string.h>
#define DELIMITERS_SIZE

const char TOKENIZER_DELIMITERS[] =
    " \t\n\r\f\v.,;:!?()[]{}<>/\\|`~@#$%^&*-+=\'\"";

void write_tokens(SafeDic *global_dic, const char *fileName) {
  printf("GLOBAL DIC SIZE: %zu\n", global_dic->size);
  FILE *fptr;

  fptr = fopen(fileName, "w");
  char **arr = global_dic->nodes;
  const size_t size = global_dic->cap;
  size_t token_symbol = 1;
  for (size_t i = 0; i < size; ++i) {
    if (arr[i]) {
      fprintf(fptr, "%zu,%s\n", token_symbol, arr[i]);
      ++token_symbol;
    }
  }
  fclose(fptr);
}

void tokenizer(char *filename, size_t vocab_tokens, size_t bytes_per_thread,
               char *output_fileName, size_t max_num_threads) {
  size_t num_threads = max_num_threads;
  printf("IN TOKENIZER\n");
  ThreadData *data = create_thread_queue(filename, vocab_tokens,
                                         bytes_per_thread, &num_threads);
  printf("NUM THREADS: %zu\n", num_threads);
  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
  for (size_t thread = 0; thread < num_threads; ++thread) {
    pthread_create(&threads[thread], NULL, thread_starter, &data[thread]);
  }
  for (size_t thread = 0; thread < num_threads; ++thread) {
    pthread_join(threads[thread], NULL);
  }
  write_tokens(data[0].global_dic, output_fileName);
  //  freeing
  free(threads);
  threadDataList_free(data, num_threads);
}

void *thread_starter(void *args) {
  ThreadData *data = (ThreadData *)args;
  ArrToken *text = data->text;
  Mpool *pool_text = data->pool_text;
  Ppool *pool_tokens = data->pool_pairs;
  Dic *dic = data->local_dic;
  SafeDic *global = data->global_dic;
  size_t start;
  size_t size;
  size_t vocab_goal;
  size_t vocab_count = 0;
  Token max_token;
  Token *max_ptr = &max_token;
  while (get_index(&start, &size, &vocab_goal, data->indexes)) {
    tokenizer_read_file(data->fileName, text, pool_text, pool_tokens, dic,
                        start, size);

    dic_reset_get_max(dic, &max_token);
    vocab_count = safeDic_insert(global, &max_token);
    while (vocab_count < vocab_goal) {
      // printf("VOCAB_COUNT/vocab_goal %zu/%zu\n", vocab_count, vocab_goal);
      tokenizer_find_max(text, pool_text, dic, max_ptr);
      dic_reset_get_max(dic, max_ptr);
      vocab_count += safeDic_insert(global, &max_token);
    }
    // printf("DONE WITH AN ITERATION\n");
    vocab_count = 0;
    arrToken_reset(text);
    ppool_reset(pool_tokens);
    mpool_reset(pool_text);
    // dic_reset(dic);
  }
  return NULL;
}

// make sure the only things passed in have a size

void merge_max(Pairs *arr, Mpool *p_text, Dic *dic, Token *max) {
  Pair *pairs = arr->pairs;
  const unsigned short size = arr->size;
  if (size == 1) {
    if (!strcmp(pairs[0].full.string, max->string)) {
      arr->size = 0;
    } else {
      dic_insert(dic, &pairs[0].full);
    }
    return;
  }

  unsigned short i = 0;
  unsigned short write_index = 0;

  while (i < size) {
    if (!strcmp(pairs[i].full.string, max->string)) {
      // Merging: this pair gets removed
      --(arr->size);

      if (write_index > 0) {
        pairs[write_index - 1].right = *max;
        token_merge_deep(&pairs[write_index - 1].full,
                         &pairs[write_index - 1].left, max, p_text);
      }

      if (i + 1 < size - 1) {
        pairs[i + 1].left = *max;
        token_merge_deep(&pairs[i + 1].full, max, &pairs[i + 1].right, p_text);
      }

      ++i;
      // skip write_index increment – we’ve removed a pair
    } else {
      if (write_index != i) {
        pairs[write_index] = pairs[i];
      }
      dic_insert(dic, &pairs[write_index].full);
      ++write_index;
      ++i;
    }
  }
}

void tokenizer_find_max(ArrToken *text, Mpool *p_text, Dic *dic, Token *max) {
  const size_t text_size = text->size;
  Pairs *pairs = text->pairs;
  for (size_t i = 0; i < text_size; ++i) {
    if (pairs[i].size) {
      merge_max(&pairs[i], p_text, dic, max);
    }
  }
}

// pass size which is (end-start) +1
void make_word(const char *buffer, size_t start, size_t end, Pairs *arr,
               Mpool *p_text, Ppool *p_pairs, Dic *dic) {

  size_t size = (end - start);
  if (size == 0) {
    // One character word
    arr->pairs = ppool_get(p_pairs, 1);
    token_deep_copy_char(&arr->pairs[0].full, buffer[start], p_text);
    dic_insert(dic, &arr->pairs[0].full);
    arr->size = 1;
    return;
  }

  Pair *pairs = ppool_get(p_pairs, size);
  arr->size = size;
  for (unsigned int i = 0; i < size; ++i) {
    token_deep_copy_char(&pairs[i].left, buffer[i + start], p_text);
    token_deep_copy_char(&pairs[i].right, buffer[i + 1 + start], p_text);
    token_merge_deep(&pairs[i].full, &pairs[i].left, &pairs[i].right, p_text);
    dic_insert(dic, &pairs[i].full);
  }
  arr->pairs = pairs;
  // token pairs is temporary so we don't worry about it
}

void tokenizer_read_file(const char *fileName, ArrToken *text, Mpool *p_text,
                         Ppool *p_pairs, Dic *dic, size_t start, size_t size) {
  FILE *file = fopen(fileName, "rb");
  if (file == NULL) {
    perror("Couldn't open file");
    exit(1);
  }

  fseek(file, start, SEEK_SET);
  char *buffer = (char *)malloc(size);
  size_t bytesRead = fread(buffer, 1, size, file);
  fclose(file);
  size_t i = 0;
  size_t word_start = 0;
  // size_t word_count = 0;

  while (i < bytesRead) {
    if (strchr(TOKENIZER_DELIMITERS, buffer[i])) {
      if (i > word_start) {
        make_word(buffer, word_start, i - 1, arrToken_get_pairs(text), p_text,
                  p_pairs, dic);
      }

      make_word(buffer, i, i, arrToken_get_pairs(text), p_text, p_pairs, dic);

      word_start = i + 1;
    }
    ++i;
  }
  if (i > word_start) {
    make_word(buffer, word_start, i - 1, arrToken_get_pairs(text), p_text,
              p_pairs, dic);
  }
  free(buffer);
}

// false means couldn't get index
bool get_index(size_t *start, size_t *size, size_t *vocab_goal,
               Indexes *indexes) {
  pthread_mutex_lock(&indexes->lock);
  if (indexes->size == indexes->cap) {
    pthread_mutex_unlock(&indexes->lock);
    return false;
  }
  *start = indexes->arr[indexes->size].start;
  *size = indexes->arr[indexes->size].size;
  *vocab_goal = indexes->arr[indexes->size++].tokens;
  pthread_mutex_unlock(&indexes->lock);
  return true;
}
