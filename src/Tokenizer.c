#include "Tokenizer.h"
#include "Token.h"
#define DELIMITERS_SIZE

const char TOKENIZER_DELIMITERS[] =
    " \t\n\r\f\v.,;:!?()[]{}<>/\\|`~@#$%^&*-+=\'\"";

void write_tokens(SafeDic *global_dic, const char *fileName) {
  FILE *fptr;

  fptr = fopen(fileName, "w");
  char **arr = global_dic->nodes;
  const size_t size = global_dic->cap;
  size_t token_symbol = 1;
  for (size_t i = 0; i < size; ++i) {
    if (arr[i]) {
      // printf("%s\n", arr[i]);
      fprintf(fptr, "%zu, %s\n", token_symbol, arr[i]);
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

  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
  // printf("NUMBER OF THREADS: %zu\n", num_threads);
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
  Tpool *pool_tokens = data->pool_tokens;
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
      // printf("%zu/%zu\n", vocab_count, vocab_goal);
      tokenizer_find_max(text, pool_text, max_ptr, dic);
      dic_reset_get_max(dic, max_ptr);
      arrToken_reset(text);
      vocab_count += safeDic_insert(global, &max_token);
    }
    vocab_count = 0;
    arrToken_reset(text);
    tpool_reset(pool_tokens);
    mpool_reset(pool_text);
    // dic_reset(dic);
  }
  return NULL;
}

void tokenizer_find_max(ArrToken *text, Mpool *pool, Token *former_max,
                        Dic *dic) {
  const Token *old_text = text->tokens;
  const size_t text_size = text->size - 1;
  const char *max_token = former_max->string;
  // Token max = *former_max;
  Token l1;
  Token r1;
  Token tmp1;
  Token curr1;
  Token *l = &l1;
  Token *r = &r1;
  Token *curr = &curr1;
  Token *tmp = &tmp1;

  token_deep_copy(l, &old_text[0], pool);
  token_deep_copy(r, &old_text[1], pool);
  token_merge_deep(tmp, l, r, pool);
  size_t i = 2;
  if (!(strcmp(tmp1.string, max_token))) {
    // l1 = max;
    token_deep_copy(l, former_max, pool);
    token_deep_copy(r, &old_text[2], pool);
    i = 3;
  }

  // actual main loop
  for (; i < text_size; ++i) {
    token_deep_copy(curr, &old_text[i], pool);
    token_merge_deep(tmp, r, curr, pool);
    // found matching pair
    if (!strcmp(tmp1.string, max_token)) {
      // r1 = max;
      token_deep_copy(r, former_max, pool);
    } else {
      arrToken_append(new_text, l, pool);
      token_merge_deep(tmp, l, r, pool);
      dic_insert(dic, tmp);
      // l1 = r1;
      token_deep_copy(l, r, pool);
      // r1 = curr1;
      token_deep_copy(r, curr, pool);
    }
  }
  arrToken_append(new_text, l, pool);
  token_deep_copy(curr, &old_text[text_size], pool);
  token_merge_deep(tmp, r, curr, pool);
  // token found
  if (!strcmp(tmp1.string, max_token)) {
    token_merge_deep(tmp, l, former_max, pool);
    dic_insert(dic, tmp);
    arrToken_append(new_text, former_max, pool);
  } else {
    token_merge_deep(tmp, l, r, pool);
    dic_insert(dic, tmp);
    token_merge_deep(tmp, r, curr, pool);
    dic_insert(dic, tmp);
    arrToken_append(new_text, r, pool);
    arrToken_append(new_text, curr, pool);
  }
}

void tokenizer_read_file(const char *fileName, ArrToken *text, Mpool *pool_text,
                         Tpool *pool_token, Dic *dic, size_t start,
                         size_t size) {

  FILE *file = fopen(fileName, "rb");
  if (file == NULL) {
    perror("Couldn't Open file");
    exit(1);
  }
  fseek(file, start, SEEK_SET);
  /***********GET INITIAL STRING AND COUNT PAIRS*********************/
  char *buffer = (char *)malloc(size);
  size_t bytesRead = fread(buffer, 1, size, file);
  fclose(file);
  size_t i = 0;
  size_t word_start;
  size_t word_end;
  char c = buffer[i];
  while (i < bytesRead) {
    word_start = i;
    while (!strchr(TOKENIZER_DELIMITERS, c)) {
    }
    // call some make word fuction and pass in the buffer
    // the array and both pools
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
