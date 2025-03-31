#pragma once
#include "Dics.h"
#include "Helper.h"
#include "Token.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Index {
  size_t start;
  size_t size;
  size_t tokens;
} Index;

typedef struct Indexes {
  pthread_mutex_t lock;
  size_t size;
  size_t cap;
  Index *arr;
} Indexes;

// made the pairs so i can constantly clear the temp pairs
typedef struct ThreadData {
  char *fileName;
  Indexes *indexes;
  Dic *local_dic;
  SafeDic *global_dic;
  Pairs *pairs;
  Mpool *pool_text;
} ThreadData;

size_t get_file_size(const char *filename);
size_t get_num_threads();

ThreadData *create_thread_queue(char *filename, size_t vocab_tokens,
                                size_t bytes_per_thread, size_t *thread_count);

void threadDataList_free(ThreadData *data, size_t num_threads);
