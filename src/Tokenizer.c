#include "Tokenizer.h"

void tokenizer_read_file(const char *fileName, StrArr *text, CPool *text_pool,
                         CPool *pairs_pool, Dic *dic, size_t start,
                         size_t bytes) {

  FILE *file = fopen(fileName, "rb");
  if (file == NULL) {
    perror("Couldn't Open file");
    exit(1);
  }
  fseek(file, start, SEEK_SET);
  /***********GET INITIAL STRING AND COUNT PAIRS*********************/
  char *buffer = (char *)malloc(bytes);
  size_t bytesRead = fread(buffer, 1, bytes, file);
  fclose(file);
  for (size_t i = 0; i < bytesRead - 1; ++i) {
    strArr_append_char(text, text_pool, buffer[i]);
    String pair = str_from_chars(buffer[i], buffer[i + 1], pairs_pool);
    dic_insert(dic, pair);
  }
  strArr_append_char(text, text_pool, buffer[bytesRead - 1]);
  free(buffer);
}

void tokenizer_find_max(const StrArr *text, StrArr *new_text, CPool *pool,
                        String former_max, Dic *dic) {
  String string_max = str_deep_copy(former_max, pool);
  const String *old_text = text->strings;
  const size_t text_size = text->size - 1;
  const char *max_token = string_max.str;

  // make sure the first two tokens don't == max_token
  String l = str_deep_copy(old_text[0], pool);
  String r = str_deep_copy(old_text[1], pool);
  String tmp = str_merge(l, r, pool);
  String curr;
  size_t i = 2;
  if (!(strcmp(tmp.str, max_token))) {
    l = string_max;
    r = str_deep_copy(old_text[2], pool);
    i = 3;
  }

  // actual main loop
  for (; i < text_size; ++i) {
    curr = str_deep_copy(old_text[i], pool);
    tmp = str_merge(r, curr, pool);
    // found matching pair
    if (!strcmp(tmp.str, max_token)) {
      r = string_max;
    } else {
      strArr_insert(new_text, l);
      tmp = str_merge(l, r, pool);
      dic_insert(dic, tmp);
      l = r;
      r = curr;
    }
  }

  strArr_insert(new_text, l);
  curr = str_deep_copy(old_text[text_size], pool);
  tmp = str_merge(r, curr, pool);
  // token found
  if (!strcmp(tmp.str, max_token)) {
    tmp = str_merge(l, string_max, pool);
    dic_insert(dic, tmp);
    strArr_insert(new_text, string_max);
  } else {
    tmp = str_merge(l, r, pool);
    dic_insert(dic, tmp);
    tmp = str_merge(r, curr, pool);
    dic_insert(dic, tmp);
    strArr_insert(new_text, r);
    strArr_insert(new_text, curr);
  }
}

void vocab_free(size_t num_tokens, Vocab *vocab) {
  for (size_t i = 0; i < num_tokens; ++i) {
    free(vocab[i].vocab);
  }
  free(vocab);
}

Vocab *make_vocab(DicSafe *global_dic, size_t num_tokens) {
  Vocab *vocab = (Vocab *)malloc(sizeof(Vocab) * num_tokens);
  size_t count = 0;
  for (size_t i = 0; i < global_dic->cap; ++i) {
    if (global_dic->nodes[i]) {
      char *str = (char *)malloc(strlen(global_dic->nodes[i]) + 1);
      strcpy(str, global_dic->nodes[i]);
      vocab[count].vocab = str;
      ++count;
      vocab[count].val = count;
    }
  }
  return vocab;
}

void *thread_loop(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  size_t count = 0;
  const size_t limit = data->vocab_size;

  tokenizer_read_file(data->filename, data->text, data->pool_text,
                      data->pool_new_text, data->thread_dic, data->start,
                      data->bytes);

  dic_reset_copy_max_token(data->thread_dic, data->max_token);

  cPool_reset(data->pool_new_text);

  count += dicSafe_insert(data->global_dic, data->max_token);

  while (count < limit) {

    tokenizer_find_max(data->text, data->new_text, data->pool_new_text,
                       *data->max_token, data->thread_dic);

    dic_reset_copy_max_token(data->thread_dic, data->max_token);

    StrArr_reset(data->text);

    cPool_reset(data->pool_text);

    swap_arrs(&data->text, &data->new_text);

    swap_pools(&data->pool_text, &data->pool_new_text);

    count += dicSafe_insert(data->global_dic, data->max_token);
  }
  dic_reset(data->thread_dic);

  StrArr_reset(data->text);

  StrArr_reset(data->new_text);

  cPool_reset(data->pool_new_text);

  cPool_reset(data->pool_text);
  return NULL;
}

void thread_starter(pthread_t *threads, ThreadData *data, size_t num_threads) {
  for (size_t thread = 0; thread < num_threads; ++thread) {
    pthread_create(&threads[thread], NULL, thread_loop, &data[thread]);
  }
  for (size_t thread = 0; thread < num_threads; ++thread) {
    pthread_join(threads[thread], NULL);
  }
}

Vocab *tokenize(const char *fileName, const size_t vocab_size) {
  ThreadDataList data_arr = thread_data_make(fileName, vocab_size);

  const size_t num_threads = data_arr.num_threads;
  pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
  DicSafe *global_dic = data_arr.data[0].global_dic;

  for (size_t batch = 0; batch < data_arr.batches; ++batch) {
    printf("*********************");
    printf("STARTING BATCH: %zu / %zu", batch, data_arr.batches);
    printf("*********************\n\n");

    ThreadData *data = &data_arr.data[batch * num_threads];

    thread_starter(threads, data, num_threads);

    printf("*********************DONE WITH BATCH");
    printf(" %zu*********************\n\n\n", batch);
  }
  Vocab *vocab_arr = make_vocab(global_dic, vocab_size);

  free(threads);
  thread_data_list_free(&data_arr);
  return vocab_arr;
}
