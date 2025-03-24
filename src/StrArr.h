#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_NUM_CHARS 10000000
#define DEFAULT_NUM_STRS 1000000

// remeber that size of string is the whole size
typedef struct String {
  char *str;
  unsigned short size;
} String;

typedef struct CPool {
  char *chars;
  size_t size;
  size_t cap;
  bool full;
  struct CPool *next;
} CPool;

typedef struct StrArr {
  String *strings;
  size_t size;
  size_t cap;
} StrArr;

CPool cPool_make(size_t size);
CPool *cPool_make_ptr(size_t size);
CPool cPool_make_default();
void cPool_grow(CPool *pool);
char *cPool_get(CPool *pool, unsigned short size);
String str_deep_copy(const String string, CPool *cpool);
char *str_deep_copy_cstring(const String *str, CPool *cpool);
String char_deep_copy_cstring(const char *str, CPool *cpool);
String str_merge(const String l, const String r, CPool *cpool);
String str_from_char(const char c, CPool *cpool);
String str_from_chars(const char l, const char r, CPool *cpool);
StrArr strArr_make(size_t size);
StrArr strArr_make_default();
StrArr *strArr_make_ptr(size_t size);

// consider this a private function
// call append cause string must be unique
// actually i need to use this function YOLO
void strArr_insert(StrArr *arr, const String string);
void strArr_append_char(StrArr *arr, CPool *cPool, const char c);

void strArr_append_chars(StrArr *arr, CPool *cPool, const char l, const char r);
void strArr_append_string(StrArr *arr, CPool *cPool, const String src);

void strArr_append_merge(StrArr *arr, CPool *cPool, const String src1,
                         const String src2);

void StrArr_reset(StrArr *arr);
void StrArr_free(StrArr *arr);

void cPool_reset(CPool *cpool);
void cPool_free(CPool *cpool);

void print_arr(const StrArr *arr);
