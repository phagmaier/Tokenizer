#include "StringArr.h"

StrArr strArr_make() {
  StrArr arr;
  arr.arr = (char **)malloc(sizeof(char *) * DEFAULT_STRARR_SIZE);
  arr.size = 0;
  arr.cap = DEFAULT_STRARR_SIZE;
  return arr;
}

void strArr_append(StrArr *arr, char *str) {
  if (arr->size == arr->cap) {
    arr->cap *= 2;
    arr->arr = (char **)realloc(arr->arr, arr->cap * sizeof(char *));
  }
  arr->arr[arr->size++] = str;
}

char *string_append_strings(char *left, char *right) {
  unsigned int l_size = strlen(left);
  unsigned int r_size = strlen(right);

  char *str = (char *)malloc(l_size + r_size + 1);

  strcpy(str, left);
  strcpy(str + l_size, right);

  return str;
}

char *string_make_char(char c) {
  char *str = (char *)malloc(2);
  str[0] = c;
  str[1] = 0;
  return str;
}
