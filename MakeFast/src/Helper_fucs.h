#pragma once
#include "StrArr.h"
#include <unistd.h>

long get_num_threads();

char *append_vocab(char **vocab, char *token, CPool *cpool, size_t index);

void swap_mempool_ptrs(CPool **one, CPool **two);

void swap_text_ptrs(StrArr **one, StrArr **two);

void write_vocab(char **arr, int minutes, double seconds, size_t size);
