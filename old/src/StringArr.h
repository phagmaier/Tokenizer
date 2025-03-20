#pragma once
#include <stdlib.h>
#include <string.h>
#define DEFAULT_STRARR_SIZE 1000000

typedef struct StrArr {
  char **arr;
  size_t size;
  size_t cap;
} StrArr;

StrArr strArr_make();
StrArr strArr_make_size(size_t size);

void strArr_append(StrArr *arr, char *str);

char *string_append_strings(char *left, char *right);

char *string_make_char(char c);

char *sr_make_str(char *src);

char *str_make_str_from_strings(const char *src1, const char *src2);
