#pragma once
#include "Token.h"
#define DEFAULT_MAP_SIZE 100000

typedef struct Node {
  Token *token;
  size_t count;
  struct Node *left;
  struct Node *right;

} Node;

typedef struct LL {
  Node *node;
  struct LL *next;
} LL;

typedef struct Map_token {
  LL **data;
  size_t size;
  size_t cap;
} Map_token;

void map_init_map(Map_token *map);
void map_add_to_ll(LL *ll, Token *token, size_t *count);
size_t map_hash_token(Token *token, size_t cap);
size_t helper_resize(LL **new_data, LL *ll, size_t cap);
void map_resize(Map_token *map);
void map_add_map(Map_token *map, Token *token);
Node *node_make_node_token(Token *token);
LL *ll_make_new_ll_token(Token *token);
void node_free_node(Node *node);
void ll_free_ll(LL *ll);
void map_free_data(LL **data, size_t cap);
void map_free_after_resize(LL **old);
