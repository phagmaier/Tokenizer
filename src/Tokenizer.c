#include "Tokenizer.h"
#include "Dics.h"

const char TOKENIZER_DELIMITERS[] =
    " \t\n\r\f\v.,;:!?()[]{}<>/\\|`~@#$%^&*-+=\'\"";

void write_tokens(SafeDic *global_dic, const char *fileName) {
  // printf("GLOBAL DIC SIZE: %zu\n", global_dic->size);
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
  ThreadData *data = create_thread_queue(filename, vocab_tokens,
                                         bytes_per_thread, &num_threads);
  printf("NUM THREADS %zu\n", num_threads);
  for (size_t i = 0; i < DELIMITERS_SIZE; ++i) {
    safeDic_insert_char(data[0].global_dic, TOKENIZER_DELIMITERS[i]);
  }
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
  Pairs *text = data->pairs;
  Mpool *pool_text = data->pool_text;
  Dic *dic = data->local_dic;
  SafeDic *global = data->global_dic;
  size_t start;
  size_t size;
  size_t vocab_goal;
  size_t vocab_count = 0;
  Token max_token;
  Token *max_ptr = &max_token;
  while (get_index(&start, &size, &vocab_goal, data->indexes)) {
    // printf("READING FILE\n");
    tokenizer_read_file(data->fileName, text, pool_text, dic, start, size);
    // printf("DONE READING FILE\n");

    // printf("DIC RESET GET MAX\n");
    dic_reset_get_max(dic, &max_token, pool_text);
    // printf("DONE DIC RESET GET MAX\n");

    // printf("INSERTING SAFE DIC\n");
    vocab_count = safeDic_insert(global, max_ptr);
    // printf("DONE INSERTING SAFE DIC\n");
    while (vocab_count < vocab_goal) {
      // printf("VOCAB_COUNT/vocab_goal %zu/%zu\n", vocab_count, vocab_goal);
      merge_max(text, pool_text, dic, max_ptr);
      dic_reset_get_max(dic, max_ptr, pool_text);
      vocab_count += safeDic_insert(global, max_ptr);
    }
    // printf("DONE WITH AN ITERATION\n");
    vocab_count = 0;
    pairs_reset(text);
    mpool_reset(pool_text);
    dic_reset(dic);
  }
  return NULL;
}

// make sure the only things passed in have a size

void merge_max(Pairs *arr, Mpool *p_text, Dic *dic, Token *max) {
  Pair *pairs = arr->pairs;
  const unsigned short size = arr->size;
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

void tokenizer_read_file(const char *fileName, Pairs *text, Mpool *p_text,
                         Dic *dic, size_t start, size_t size) {
  FILE *file = fopen(fileName, "rb");
  if (file == NULL) {
    perror("Couldn't open file");
    exit(1);
  }

  fseek(file, start, SEEK_SET);
  char *buffer = (char *)malloc(size);
  const size_t bytesRead = fread(buffer, 1, size, file);
  // printf("bytesRead: %zu\n", bytesRead);
  fclose(file);
  for (size_t i = 0; i < bytesRead - 1; ++i) {
    pairs_insert_chars(text, buffer[i], buffer[i + 1], p_text);
    dic_insert(dic, &text->pairs[text->size - 1].full);
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
