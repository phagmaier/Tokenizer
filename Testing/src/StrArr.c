#include "StrArr.h"
#include <stdio.h>

CPool cPool_make(size_t size) {
  CPool pool;
  pool.chars = (char *)malloc(size);
  pool.size = 0;
  pool.cap = size;
  pool.full = false;
  pool.next = NULL;
  return pool;
}

CPool *cPool_make_ptr(size_t size) {
  CPool *pool = (CPool *)malloc(sizeof(CPool));
  pool->chars = (char *)malloc(size);
  pool->size = 0;
  pool->cap = size;
  pool->full = false;
  pool->next = NULL;
  return pool;
}

CPool cPool_make_default() {
  CPool pool;
  pool.chars = (char *)malloc(DEFAULT_NUM_CHARS);
  pool.size = 0;
  pool.cap = DEFAULT_NUM_CHARS;
  pool.full = false;
  pool.next = NULL;
  return pool;
}

void cPool_grow(CPool *pool) {
  printf("cPool growing\n");
  CPool *tmp = (CPool *)malloc(sizeof(CPool));
  tmp->cap = pool->cap;
  tmp->chars = (char *)malloc(tmp->cap);
  if (!tmp->chars) {
    perror("Couldn't grow Cpool");
    exit(1);
  }
  tmp->size = 0;
  tmp->full = false;
  tmp->next = NULL;
  pool->next = tmp;
}

char *cPool_get(CPool *pool, unsigned short size) {
  while (pool->full) {
    pool = pool->next;
  }
  if (pool->size + size >= pool->cap) {
    cPool_grow(pool);
    pool->full = true;
    pool = pool->next;
  }
  char *str = pool->chars + pool->size;
  pool->size += size;
  return str;
}

char *str_deep_copy_cstring(const char *str, CPool *cpool) {
  size_t size = strlen(str) + 1;
  char *chars = cPool_get(cpool, size);
  // String *str = strPool_get(strPool);
  memcpy(chars, str, size);
  return chars;
}

String char_deep_copy_cstring(const char *str, CPool *cpool) {
  size_t size = strlen(str) + 1;
  char *chars = cPool_get(cpool, size);
  String new_string;
  memcpy(chars, str, size);
  new_string.size = size;
  new_string.str = chars;
  return new_string;
}

String str_deep_copy(const String string, CPool *cpool) {
  char *chars = cPool_get(cpool, string.size);
  String new_string;
  // String *str = strPool_get(strPool);
  memcpy(chars, string.str, string.size);
  new_string.size = string.size;
  new_string.str = chars;
  return new_string;
}

String str_merge(const String l, const String r, CPool *cpool) {
  unsigned short new_size = l.size + r.size - 1;
  char *chars = cPool_get(cpool, new_size);
  String str;
  memcpy(chars, l.str, l.size - 1);
  memcpy(chars + l.size - 1, r.str, r.size);
  str.size = new_size;
  str.str = chars;
  return str;
}

String str_from_char(const char c, CPool *cpool) {
  char *chars = cPool_get(cpool, 2);
  chars[0] = c;
  chars[1] = 0;
  String str;
  str.size = 2;
  str.str = chars;
  return str;
}

String str_from_chars(const char l, const char r, CPool *cpool) {
  char *chars = cPool_get(cpool, 3);
  chars[0] = l;
  chars[1] = r;
  chars[2] = r;
  String str;
  str.size = 3;
  str.str = chars;
  return str;
}

StrArr strArr_make(size_t size) {
  StrArr arr;
  arr.strings = (String *)malloc(sizeof(String) * size);
  arr.size = 0;
  arr.cap = size;
  return arr;
}

StrArr *strArr_make_ptr(size_t size) {
  StrArr *arr = (StrArr *)malloc(sizeof(StrArr));
  arr->strings = (String *)malloc(sizeof(String) * size);
  arr->size = 0;
  arr->cap = size;
  return arr;
}

StrArr strArr_make_default() {
  StrArr arr;
  arr.strings = (String *)malloc(sizeof(String) * DEFAULT_NUM_STRS);
  arr.size = 0;
  arr.cap = DEFAULT_NUM_STRS;
  return arr;
}

void strArr_insert(StrArr *arr, const String string) {
  if (arr->size == arr->cap) {
    printf("String array growing\n");
    arr->cap *= 2;
    arr->strings = (String *)realloc(arr->strings, arr->cap * sizeof(String));
    if (!arr->strings) {
      perror("COULDN'T REALLOC ENOUGH MEM FOR STRARRAY");
      exit(1);
    }
  }
  arr->strings[(arr->size)++] = string;
}

void strArr_append_char(StrArr *arr, CPool *cPool, const char c) {
  String new_string = str_from_char(c, cPool);
  strArr_insert(arr, new_string);
}

void strArr_append_chars(StrArr *arr, CPool *cPool, const char l,
                         const char r) {
  String new_string = str_from_chars(l, r, cPool);
  strArr_insert(arr, new_string);
}

void strArr_append_string(StrArr *arr, CPool *cPool, const String src) {
  String new_string = str_deep_copy(src, cPool);
  strArr_insert(arr, new_string);
}

void strArr_append_merge(StrArr *arr, CPool *cPool, const String src1,
                         const String src2) {
  String new_string = str_merge(src1, src2, cPool);
  strArr_insert(arr, new_string);
}

void StrArr_reset(StrArr *arr) { arr->size = 0; }
void StrArr_free(StrArr *arr) { free(arr->strings); }

void cPool_reset(CPool *cpool) {
  CPool *tmp = cpool->next;
  size_t new_size = cpool->cap;
  while (tmp) {
    CPool *tmp2 = tmp->next;
    new_size += tmp->cap;
    free(tmp->chars);
    free(tmp);
    tmp = tmp2;
  }
  if (new_size > cpool->cap) {
    cpool->chars = (char *)realloc(cpool->chars, new_size);
    cpool->cap = new_size;
    if (!cpool->chars) {
      perror("COULDN'T REALLOC ENOUGH MEM FOR CPOOL CHARS");
      exit(1);
    }
  }
  cpool->size = 0;
  cpool->next = NULL;
  cpool->full = false;
}

void cPool_free(CPool *cpool) {
  CPool *tmp = cpool->next;
  CPool *tmp2;
  while (tmp) {
    tmp2 = tmp->next;
    free(tmp->chars);
    free(tmp);
    tmp = tmp2;
  }
  free(cpool->chars);
  cpool->size = 0;
  cpool->cap = 0;
  cpool->next = NULL;
}

void print_arr(const StrArr *arr) {
  const String *strings = arr->strings;
  for (size_t i = 0; i < arr->size; ++i) {
    printf("%s", strings[i].str);
  }
}
