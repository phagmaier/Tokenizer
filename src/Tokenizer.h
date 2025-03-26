#pragma once
#include "Dics.h"
#include "Helper.h"
#include "Token.h"

void tokenizer(char *filename, size_t vocab_tokens, size_t bytes_per_thread,
               char *output_fileName);

void *thread_starter(void *args);
bool get_index(size_t *start, size_t *size, size_t *vocab_goal,
               Indexes *indexes);
void write_tokens(SafeDic *global_dic, const char *filename);
void tokenizer_read_file(const char *fileName, ArrToken *text, Mpool *pool_text,
                         Mpool *pairs_pool, Dic *dic, size_t start,
                         size_t size);

void tokenizer_find_max(const ArrToken *text, ArrToken *new_text, Mpool *pool,
                        Token *former_max, Dic *dic);
