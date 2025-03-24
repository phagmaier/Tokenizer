#pragma once
#include "Helper_fucs.h"

typedef struct ThreadData {
  char const *filename;
  size_t start;
  size_t bytes;
  StrArr *text;
  StrArr *new_text;
  CPool *pool_text;
  CPool *pool_new_text;
  Dic *thread_dic;
  DicSafe *global_dic;
  String *max_token;
  size_t vocab_size;
  // size_t count;
} ThreadData;

typedef struct ThreadDataList {
  size_t batches;
  size_t num_threads;
  // size_t *num_tokens;
  ThreadData *data;
} ThreadDataList;

// in the future add a way for them to say how much mem to use
// for now we limit to like 500k words a thread
ThreadDataList thread_data_make(const char *filename, const size_t vocab_size);

// be careful with this because you free the dics as well
void thread_data_list_free(ThreadDataList *arr);
