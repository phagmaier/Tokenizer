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
  Dic *batch_dic;
  DicSafe *global_dic;
  String *max_token;
} ThreadData;

typedef struct ThreadDataList {
  size_t batches;
  size_t num_threads;
  size_t *num_tokens;
  ThreadData *data;
  String *max_token;
} ThreadDataList;

ThreadDataList thread_data_make(const char *filename, const size_t vocab_size,
                                const size_t bytes_per_thread);

// be careful with this because you free the dics as well
void thread_data_list_free(ThreadDataList *arr);
