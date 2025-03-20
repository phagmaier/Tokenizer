#include "StringArr.h"

// eventually you'll do this with mempool
StrArr strArr_make() {
  StrArr arr;
  arr.arr = (char **)malloc(sizeof(char *) * DEFAULT_STRARR_SIZE);
  arr.size = 0;
  arr.cap = DEFAULT_STRARR_SIZE;
  return arr;
}

// eventually you'll do this with mempool
StrArr strArr_make_size(size_t size) {
  StrArr arr;
  arr.arr = (char **)malloc(sizeof(char *) * size);
  arr.size = 0;
  arr.cap = size;
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

// eventually do this mempool
char *str_make_str(char *src) {
  char *dest = (char *)malloc(strlen(src) + 1);
  strcpy(dest, src);
  return dest;
}

char *str_make_str_from_strings(const char *src1, const char *src2) {
  size_t size1 = strlen(src1);
  size_t size2 = strlen(src2);

  char *dest = (char *)malloc(size1 + size2 + 1);
  if (!dest) {
    return NULL; // Handle malloc failure
  }

  strcpy(dest, src1);
  strcpy(dest + size1, src2);

  return dest;
}
