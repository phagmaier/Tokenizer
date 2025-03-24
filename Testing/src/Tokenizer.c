#include "Tokenizer.h"

void *tokenizer_read_file(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  // FILE *file = fopen("../data/data.txt", "rb");
  FILE *file = fopen(data->filename, "rb");
  if (file == NULL) {
    perror("Couldn't Open file");
    exit(1);
  }
  fseek(file, data->start, SEEK_SET);
  /***********GET INITIAL STRING AND COUNT PAIRS*********************/
  char *buffer = (char *)malloc(data->bytes);
  size_t bytesRead = fread(buffer, 1, data->bytes, file);
  printf("READ INTO BUFFER\n");
  fclose(file);
  printf("CLOSED FILE\n");
  StrArr *text = data->text;
  CPool *pool_text = data->pool_text;
  CPool *pool_pairs = data->pool_new_text;
  Dic *dic = data->batch_dic;
  for (size_t i = 0; i < bytesRead - 1; ++i) {
    strArr_append_char(text, pool_text, buffer[i]);
    String pair = str_from_chars(buffer[i], buffer[i + 1], pool_pairs);
    dic_insert(dic, pair);
    printf("%s\n", pair.str);
  }
  // get last char
  strArr_append_char(text, pool_text, buffer[bytesRead - 1]);

  /***********GET INITIAL STRING AND COUNT PAIRS*********************/
  free(buffer);
  printf("DONE READING FILE READ FILE\n");
  return NULL;
}

void *tokenizer_find_max(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  CPool *pool = data->pool_new_text;
  String string_max = str_deep_copy(*data->max_token, pool);
  StrArr *new_text = data->new_text;
  const String *old_text = data->text->strings;
  const size_t text_size = data->text->size - 1;
  const char *max_token = data->max_token->str;
  Dic *batch_dic = data->batch_dic;

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
      dic_insert(batch_dic, tmp);
      l = r;
      r = curr;
    }
  }

  // FOR THE LAST COUPLE TOKENS
  strArr_insert(new_text, l);
  curr = str_deep_copy(old_text[text_size], pool);
  tmp = str_merge(r, curr, pool);
  // token found
  if (!strcmp(tmp.str, max_token)) {
    tmp = str_merge(l, string_max, pool);
    dic_insert(batch_dic, tmp);
    strArr_insert(new_text, string_max);
  } else {
    tmp = str_merge(l, r, pool);
    dic_insert(batch_dic, tmp);
    tmp = str_merge(r, curr, pool);
    dic_insert(batch_dic, tmp);
    strArr_insert(new_text, r);
    strArr_insert(new_text, curr);
  }
  return NULL;
}

// this should actually also return bool cause
// calling it on multiple threads
bool tokenizer_get_first_token(const size_t num_threads, pthread_t *threads,
                               ThreadData *data, String *max_token,
                               Dic *batch_dic, DicSafe *global_dic) {

  // literally might be faster to read with just the main process not threads
  for (size_t thread = 0; thread < num_threads; ++thread) {
    pthread_create(&threads[thread], NULL, tokenizer_read_file, &data[thread]);
    // tokenizer_read_file(&data[thread]);
  }

  for (size_t thread = 0; thread < num_threads; ++thread) {
    pthread_join(threads[thread], NULL);
  }

  dic_reset_copy_max_token(batch_dic, max_token);
  for (size_t thread = 0; thread < num_threads; ++thread) {
    cPool_reset(data[thread].pool_new_text);
    data[thread].max_token = max_token;
  }
  return dicSafe_insert(global_dic, max_token);
}

bool tokenizer_get_token(const size_t num_threads, pthread_t *threads,
                         ThreadData *data, String *max_token, Dic *batch_dic,
                         DicSafe *global_dic) {

  for (size_t thread = 0; thread < num_threads; ++thread) {
    pthread_create(&threads[thread], NULL, tokenizer_find_max, &data[thread]);
  }

  for (size_t thread = 0; thread < num_threads; ++thread) {
    pthread_join(threads[thread], NULL);
  }

  dic_reset_copy_max_token(batch_dic, max_token);
  CPool *p_tmp;
  StrArr *a_tmp;
  for (size_t thread = 0; thread < num_threads; ++thread) {

    cPool_reset(data[thread].pool_new_text);
    StrArr_reset(data[thread].text);
    data[thread].max_token = max_token;

    // swaping
    p_tmp = data[thread].pool_text;
    a_tmp = data[thread].text;
    data[thread].pool_text = data[thread].pool_new_text;
    data[thread].pool_new_text = p_tmp;
    data[thread].text = data[thread].new_text;
    data[thread].new_text = a_tmp;
    // swaping
  }
  // swap before return
  return dicSafe_insert(global_dic, max_token);
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

/****************************************************
 eventually you willneed to get rid of
 max bytes per thread and make it
 max bytes per batch
 and it won't be a param you'll calculate dynamically
 In the function where you get thread data
 ****************************************************/
Vocab *tokenize(const char *fileName, const size_t vocab_size,
                const size_t max_bytes_per_thread) {

  printf("IN TOKENIZER\n");
  /******************GET THREAD DATA*********************/
  ThreadDataList data_arr =
      thread_data_make(fileName, vocab_size, max_bytes_per_thread);

  const size_t num_threads = data_arr.num_threads;
  pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
  String *max_token = data_arr.max_token;
  Dic *batch_dic = data_arr.data[0].batch_dic;
  DicSafe *global_dic = data_arr.data[0].global_dic;
  printf("GOT THREAD DATA\n");
  /******************GET THREAD DATA*********************/
  printf("STARTING\n");
  /******************SPAWN AND RUN THREADS*********************/
  for (size_t batch = 0; batch < data_arr.batches; ++batch) {
    printf("*********************");
    printf("STARTING BATCH: %zu / %zu", batch, data_arr.batches);
    printf("*********************\n");

    const size_t batch_vocab_size = data_arr.num_tokens[batch];

    ThreadData *data = &data_arr.data[batch * num_threads];
    size_t i = tokenizer_get_first_token(num_threads, threads, data, max_token,
                                         batch_dic, global_dic);
    while (i < batch_vocab_size) {
      i += tokenizer_get_token(num_threads, threads, data, max_token, batch_dic,
                               global_dic);
    }
    printf("*********************DONE WITH BATCH");
    printf("%zu*********************", batch);
  }
  Vocab *vocab_arr = make_vocab(global_dic, vocab_size);

  /******************CLEAN UP*********************/
  free(threads);
  /******************IMPORTANT*********************/
  // MAKE SURE YOU EITHER COPY OR STORE THE GLOBAL
  // DICTIONARY TOKEN VALUES SOMEWHERE FIRST
  thread_data_list_free(&data_arr);
  /******************IMPORTANT*********************/

  /******************CLEAN UP*********************/
  return vocab_arr;
}
