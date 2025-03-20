#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_ARR_SIZE 100
#define DEFAULT_NUM_CHARS 100
#define DEFAULT_NUM_STRS 100

// remeber that size of string is the whole size
typedef struct String {
  char *str;
  unsigned short size;
} String;

typedef struct CharPool {
  char *chars;
  size_t size;
  size_t cap;
  bool full;
  struct CharPool *next;
} CharPool;

typedef struct StringPool {
  String *strings;
  size_t size;
  size_t cap;
  bool full;
  struct StringPool *next;
} StringPool;

typedef struct StrArr {
  String **strings;
  size_t size;
  size_t cap;
  // bool full;
  // struct StrArr *next;
} StrArr;

CharPool charPool_make(size_t size) {
  CharPool pool;
  pool.chars = (char *)malloc(size);
  pool.size = 0;
  pool.cap = size;
  pool.full = false;
  pool.next = NULL;
  return pool;
}

StringPool strPool_make(size_t size) {
  StringPool pool;
  pool.strings = (String *)malloc(sizeof(String) * size);
  pool.size = 0;
  pool.cap = size;
  pool.full = false;
  pool.next = NULL;
  return pool;
}

StrArr StrArr_make(size_t size) {
  StrArr arr;
  arr.strings = (String **)malloc(sizeof(String *) * size);
  arr.size = 0;
  arr.cap = size;
  return arr;
}

void charpool_grow(CharPool *pool) {
  CharPool *tmp = (CharPool *)malloc(sizeof(CharPool));
  tmp->cap = pool->cap;
  tmp->chars = (char *)malloc(tmp->cap);
  tmp->size = 0;
  tmp->full = false;
  tmp->next = NULL;
  pool->next = tmp;
}

void strPool_grow(StringPool *pool) {
  StringPool *tmp = (StringPool *)malloc(sizeof(StringPool));
  tmp->cap = pool->cap;
  tmp->strings = (String *)malloc(tmp->cap * sizeof(String));
  tmp->size = 0;
  tmp->full = false;
  tmp->next = NULL;
  pool->next = tmp;
}

String *strPool_get(StringPool *pool) {
  while (pool->full) {
    pool = pool->next;
  }
  if (pool->size == pool->cap) {
    strPool_grow(pool);
    pool->full = true;
    pool = pool->next;
  }
  String *str = pool->strings + pool->size;
  ++(pool->size);
  return str;
}

char *charPool_get(CharPool *pool, unsigned short size) {
  while (pool->full) {
    pool = pool->next;
  }
  if (pool->size + size >= pool->cap) {
    charpool_grow(pool);
    pool->full = true;
    pool = pool->next;
  }
  char *str = pool->chars + pool->size;
  pool->size += size;
  return str;
}

String *str_deep_copy(const String *string, StringPool *strPool,
                      CharPool *charPool) {
  char *chars = charPool_get(charPool, string->size);
  String *str = strPool_get(strPool);
  strcpy(chars, string->str);
  str->size = string->size;
  str->str = chars;
  return str;
}

String *str_merge(const String *l, const String *r, StringPool *strPool,
                  CharPool *charPool) {
  unsigned short new_size = l->size + r->size - 1;
  char *chars = charPool_get(charPool, new_size);
  String *str = strPool_get(strPool);
  strcpy(chars, l->str);
  strcpy(chars + l->size - 1, r->str);
  str->size = new_size;
  str->str = chars;
  return str;
}

String *str_from_char(char c, StringPool *strPool, CharPool *charPool) {
  char *chars = charPool_get(charPool, 2);
  chars[0] = c;
  chars[1] = 0;
  String *str = strPool_get(strPool);
  str->size = 2;
  str->str = chars;
  return str;
}

StrArr strArr_make(size_t size) {
  StrArr arr;
  arr.strings = (String **)malloc(sizeof(String *) * size);
  arr.size = 0;
  // arr.full = false;
  // arr.next = NULL;
  return arr;
}

/*
voidid strArr_grow(StrArr *arr) {
  StrArr *tmp = (StrArr *)malloc(sizeof(CharPool));
  tmp->cap = arr->cap;
  tmp->strings = (String **)malloc(tmp->cap);
  tmp->size = 0;
  tmp->full = false;
  tmp->next = NULL;
  arr->next = tmp;
}
*/

// consider this a private function
// call append cause string must be unique
void strArr_insert(StrArr *arr, String *string) {
  if (arr->size == arr->cap) {
    arr->cap *= 2;
    arr->strings = (String **)realloc(arr->strings, arr->cap);
  }
  arr->strings[(arr->size)++] = string;
}

void strArr_append_char(StrArr *arr, StringPool *sPool, CharPool *cPool,
                        char c) {
  String *string = str_from_char(c, sPool, cPool);
  strArr_insert(arr, string);
}

void strArr_append_string(StrArr *arr, StringPool *sPool, CharPool *cPool,
                          String *src) {
  String *string = str_deep_copy(src, sPool, cPool);
  strArr_insert(arr, string);
}

void strArr_append_merge(StrArr *arr, StringPool *sPool, CharPool *cPool,
                         String *src1, String *src2) {
  String *string = str_merge(src1, src2, sPool, cPool);
  strArr_insert(arr, string);
}

void StrArr_reset(StrArr *arr) {
  StrArr *tmp = arr->next;
  size_t new_size = arr->cap;
  while (tmp) {
    StrArr *tmp2 = tmp->next;
    new_size += tmp->cap;
    free(tmp->strings);
    free(tmp);
    tmp = tmp2;
  }
  if (new_size > arr->cap) {
    arr->strings =
        (String **)realloc(arr->strings, sizeof(String *) * new_size);
    arr->cap = new_size;
  }
  arr->size = 0;
  arr->next = false;
  arr->full = false;
}

void sPool_reset(StringPool *pool) {
  StringPool *tmp = pool->next;
  size_t new_size = pool->cap;
  while (tmp) {
    StringPool *tmp2 = tmp->next;
    new_size += tmp->cap;
    free(tmp->strings);
    free(tmp);
    tmp = tmp2;
  }
  if (new_size > pool->cap) {
    pool->strings = (String *)realloc(pool->strings, sizeof(String) * new_size);
    pool->cap = new_size;
  }
  pool->size = 0;
  pool->next = false;
  pool->full = false;
}

void cPool_reset(CharPool *pool) {
  CharPool *tmp = pool->next;
  size_t new_size = pool->cap;
  while (tmp) {
    CharPool *tmp2 = tmp->next;
    new_size += tmp->cap;
    free(tmp->chars);
    free(tmp);
    tmp = tmp2;
  }
  if (new_size > pool->cap) {
    pool->chars = (char *)realloc(pool->chars, new_size);
    pool->cap = new_size;
  }
  pool->size = 0;
  pool->next = false;
  pool->full = false;
}

/*
StrArr read_text(const char *fileName) {
  Arr arr = strArr_make();
  FILE *file = fopen(fileName, "rb");
  if (!file) {
    perror("Error opening file");
    return arr;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  // Allocate buffer and read file
  char *buffer = (char *)malloc(file_size);
  if (!buffer) {
    perror("Memory allocation failed");
    fclose(file);
    return arr;
  }
  fread(buffer, 1, file_size, file);
  fclose(file);
  for (size_t i = 0; i < file_size; ++i) {
    strArr_append(&arr, string_make_char(buffer[i]));
  }
  free(buffer);
  return arr;
}
*/

int main() {
  char *fileName = "infiniteJest.txt";
  return 0;
}
