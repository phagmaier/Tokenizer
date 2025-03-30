#pragma once
#include "Dics.h"
#include "Token.h"
#include <stdio.h>
#include <unistd.h>
void read_vocab(DicVocab *dic, const char *file);
void tokenize_file(const char *file_name, DicVocab *dic);
void tokenize_make_words(const char *buffer, size_t start, size_t end,
                         Pairs *arr, Mpool *p_text, Ppool *p_pairs,
                         DicVocab *dic);

// probably need a better data structure
void tokenize_split(const char *buffer, ArrToken *text, Mpool *p_text,
                    Ppool *p_pairs, size_t bytesRead, DicVocab *dic);

void text_merge(Pairs *arr, Mpool *p_text, DicVocab *dic);
