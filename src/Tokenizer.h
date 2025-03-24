#pragma once
#include "Dic.h"
#include "Helper_fucs.h"
#include "StrArr.h"
#include "ThreadData.h"
#include <pthread.h>
#include <stdio.h>

typedef struct Vocab {
  char *vocab;
  unsigned int val;
} Vocab;

Vocab *make_vocab(DicSafe *global_dic, size_t num_tokens);

void tokenizer_read_file(const char *fileName, StrArr *text, CPool *text_pool,
                         CPool *new_pool, Dic *dic, size_t start, size_t bytes);

void tokenizer_find_max(const StrArr *text, StrArr *new_text, CPool *pool,
                        String former_max, Dic *dic);

void thread_starter(pthread_t *threads, ThreadData *data, size_t num_threads);

Vocab *tokenize(const char *fileName, const size_t vocab_size);

void vocab_free(size_t num_tokens, Vocab *vocab);
