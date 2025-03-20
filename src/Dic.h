#pragma once
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

void node_init_node(Node *node, char *string);

void dic_make_dic(Dic *dic);

void dic_reset(Dic *dic);

size_t dic_hash(const char *s, const size_t cap);

void dic_resize(Dic *dic);

void dic_insert_dic(Dic *dic, char *string);

void dic_clear(Dic *dic);
