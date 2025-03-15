#pragma once
#include "Token.h"
#define DEFAULT_DIC_SIZE 100000
#define DEFAULT_PAIR_SIZE 5

typedef struct Pair {
  Token *left;
  Token *right;
} Pair;

typedef struct Node {
  Token *token;
  Pair *pairs;
  struct Node *next;
  size_t count;
  size_t p_size;
  size_t p_cap;
} Node;

typedef struct Dic {
  Node **data;
  size_t taken;
  size_t cap;
} Dic;

Node *node_make_node(Token *token, const size_t count);
void node_append_pair(Node *node, const Token *left, const Token *right);
void node_free_node(Node *node);
void dic_init_dic(Dic *dic);
size_t dic_hash(const char *str, const size_t cap);
size_t ll_resize(Node *node, Node **data, const size_t new_size);
void dic_resize(Dic *dic);
void dic_insert(Dic *dic, Token *token, Token *left, Token *right);
void free_dic(Dic *dic);
