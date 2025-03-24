#pragma once
#include "StrArr.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// GLOBAL
typedef struct DicSafe {
  pthread_mutex_t lock;
  CPool cpool;
  char **nodes;
  size_t size;
  size_t cap;
} DicSafe;

size_t dic_hash(const char *s, const size_t cap);
DicSafe *dicSafe_make_dic(size_t size);
void dicSafe_init_dic(DicSafe *dic, size_t size);
void dicSafe_resize(DicSafe *dic);
bool dicSafe_insert(DicSafe *dic, String *ptr);
void dicSafe_free(DicSafe *dic);

typedef struct Node {
  char *string;
  size_t count;
} Node;

typedef struct Dic {
  Node *nodes;
  size_t size;
  size_t cap;
  String max_token;
  size_t max_count;
} Dic;

Dic *dic_make(size_t cap);
void dic_resize(Dic *dic);
void dic_insert(Dic *dic, String str);
void dic_free(Dic *dic);
void dic_reset(Dic *dic);
void dic_reset_copy_max_token(Dic *dic, String *string);
