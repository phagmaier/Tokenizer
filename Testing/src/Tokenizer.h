#pragma once
#include "Dic.h"
#include "StrArr.h"
#include <stdio.h>

StrArr read_text(const char *fileName, CPool *cpool);

char *first_get_max_token(const StrArr *text, Dic *dic, CPool *cpool_new_text);

char *get_max_token(const StrArr *arr, StrArr *new_text, CPool *cpool_new_text,
                    Dic *dic, const char *max_token);

/*
 ***************************************************
                    THREADED VERSIONS
 ***************************************************
 */

void *tokenizer_read_file(void *varg);
