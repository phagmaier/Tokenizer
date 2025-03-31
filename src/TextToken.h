#pragma once
#include "Dics.h"
#include "Token.h"
#include <stdio.h>
#include <unistd.h>

typedef struct TArr {
  Token *arr;
  size_t cap;
  size_t size;
} TArr;

void read_vocab(DicVocab *dic, const char *filename);

void tokenize_file(const char *fileName, const char *vocab_file_name,
                   unsigned int vocab_size);

void text_merge(TArr *arr, Mpool *pool, DicVocab *dic);

TArr tarr_make_stack(size_t cap);
void tarr_insert_char(TArr *arr, const char c, Mpool *pool);
void tarr_free_stack(TArr *arr);
void tar_reset(TArr *arr);
void tar_insert_token(TArr *arr, Token *token);
