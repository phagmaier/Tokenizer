#pragma once
#include "Dics.h"
#include "Helper.h"
#include "Token.h"
#define ASCII_SIZE 128

void tokenizer(char *filename, size_t vocab_tokens, size_t bytes_per_thread,
               char *output_fileName, size_t max_num_threads);

void *thread_starter(void *args);
bool get_index(size_t *start, size_t *size, size_t *vocab_goal,
               Indexes *indexes);
void write_tokens(SafeDic *global_dic, const char *filename);

void tokenizer_read_file(const char *fileName, Pairs *text, Mpool *pool_text,
                         Dic *dic, size_t start, size_t size);

void tokenizer_find_max(Pairs *text, Mpool *pool_text, Dic *dic, Token *max);

void make_word(const char *buffer, size_t start, size_t end, Pairs *arr,
               Mpool *p_text, Dic *dic);

void merge_max(Pairs *arr, Mpool *p_text, Dic *dic, Token *max);
