#pragma once
#include "Dic.h"
#include "StrArr.h"
#include <stdio.h>

StrArr read_text(const char *fileName, CPool *cpool);

char *first_get_max_tokens(StrArr *text, Dic *dic, CPool *cpoolPairs);

char *get_max_token(const StrArr *arr, StrArr *new_text, CPool *cpool_new_text,
                    Dic *dic, char *max_token);

/*
 ***************************************************
                    THREADED VERSIONS
 ***************************************************
 */
void read_file_thread(StrArr *arr, CPool *cpool, size_t start, size_t end);
