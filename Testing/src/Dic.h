#pragma once
#include "StrArr.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#define DEFAULT_DIC_SIZE 100000

typedef struct Node {
  char *string;
  size_t count;
} Node;

// before you reset and delete all strings in whatever file
// you need to make sure you allocate enough space for max_token
// either that or just use a huge array of chars to make sure that the
// token will always fit and just copy it in
// but you should do that later after returning the max
typedef struct Dic {
  Node *nodes;
  size_t size;
  size_t cap;
  char *max_token;
  size_t max_count;
} Dic;

// take in strings only save max as string
void node_init_node(Node *node, char *string);
void dic_init_dic(Dic *dic);
Dic dic_make_dic_default();
Dic dic_make_dic(size_t size);
size_t dic_hash(const char *s, const size_t cap);
void dic_resize(Dic *dic);
void dic_insert_dic(Dic *dic, char *string);
char *dic_reset(Dic *dic);
void dic_free(Dic *dic);

/***************************************************
 *                  SAFE DIC STUF
 ****************************************************/

// all your nodes should be of type String not char*
typedef struct DicSafe {
  pthread_mutex_t lock;
  CPool cpool;
  char **nodes;
  size_t size;
  size_t cap;
} DicSafe;

// recomended to not return a object
void dicSafe_init_dic(DicSafe *dic, size_t size);
void dicSafe_resize(DicSafe *dic);
// tue for new insert false for old
bool dicSafe_insert(DicSafe *dic, String string);

// when you call free it also calls free on the mempool and you lose all tokens
// Might need to change since you'll want to use those tokens/strings later
void dicSafe_free(DicSafe *dic);
