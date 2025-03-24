#pragma once
#include "Dic.h"
#include "Helper_fucs.h"
#include "StrArr.h"
#include "ThreadData.h"
#include <stdio.h>

typedef struct Vocab {
  char *vocab;
  unsigned int val;
} Vocab;

void *tokenizer_read_file(void *arg);
void *tokenizer_find_max(void *arg);
bool tokenizer_get_first_token(const size_t num_threads, pthread_t *threads,
                               ThreadData *data, String *max_token,
                               Dic *batch_dic, DicSafe *global_dic);

Vocab *make_vocab(DicSafe *global_dic, size_t num_tokens);
Vocab *tokenize(const char *fileName, const size_t vocab_size,
                const size_t max_bytes_per_thread);

void vocab_free(size_t num_tokens, Vocab *vocab);
