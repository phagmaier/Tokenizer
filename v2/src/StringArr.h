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

void strArr_append(StrArr *arr, char *str);

char *string_append_strings(char *left, char *right);

char *string_make_char(char c);
