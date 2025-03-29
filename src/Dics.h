#pragma once
#include "Token.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct DicNode {
  char *string;
  size_t count;
} DicNode;

typedef struct Dic {
  size_t cap;
  size_t size;
  DicNode *nodes;
  Token max_token;
  size_t max_count;
} Dic;

typedef struct SafeDic {
  pthread_mutex_t lock;
  size_t cap;
  size_t size;
  char **nodes;
  Mpool *pool;
} SafeDic;

// UTILITY
size_t dic_hash(const char *s, const size_t cap);

// REGULAR DIC
Dic *dic_make_heap(size_t cap);
void dic_resize(Dic *dic);
void dic_insert(Dic *dic, Token *token);
void dic_reset_get_max(Dic *dic, Token *token, Mpool *pool);
void dic_reset(Dic *dic);
void dic_free_heap(Dic *dic);
void dic_free_stack(Dic dic);

// THREAD SAFE DIC
SafeDic *safeDic_make_heap(size_t cap);
void safeDic_resize(SafeDic *dic);
bool safeDic_insert(SafeDic *dic, Token *token);
void safeDic_free_heap(SafeDic *dic);
