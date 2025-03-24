#pragma once
#include "StrArr.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// GLOBAL
typedef struct DicSafe {
  // pthread_mutex_t lock;
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

/*
typedef struct Node {
  char *string;
  size_t count;
} Node;

typedef struct Dic {
  pthread_mutex_t lock;
  Node *nodes;
  size_t size;
  size_t cap;
  String max_token;
  size_t max_count;
} Dic;
Dic *dic_make_dic(size_t size);
void dic_resize(Dic *dic);
void dic_insert(Dic *dic, String string);
void dic_free(Dic *dic);
void dic_reset(Dic *dic);
void dic_reset_copy_max_token(Dic *dic, String *string);
*/

/*NEW DIC*/

typedef struct Node {
  char *string;
  size_t count;
} Node;

typedef struct Bucket {
  pthread_mutex_t lock;
  Node *nodes;
  size_t size;
  size_t cap;
} Bucket;

typedef struct Dic {
  pthread_mutex_t lock;
  Bucket **buckets;
  size_t num_buckets;
  String max_token;
  size_t max_count;
} Dic;

Bucket *make_bucket(size_t size);
Dic *dic_make_dic(size_t bucket_size, size_t num_buckets);
void update_max(Dic *dic, String string);
void bucket_resize(Bucket *bucket);
void dic_insert(Dic *dic, String string);
void dic_free(Dic *dic);
void dic_reset(Dic *dic);
void dic_reset_copy_max_token(Dic *dic, String *string);
