#include "Map_token.h"
#include "String.h"
#include "Token.h"
#include <stdio.h>
#include <string.h>

void map_init_map(Map_token *map) {
  map->data = (LL **)calloc(DEFAULT_MAP_SIZE, sizeof(LL *));
  if (!map->data) {
    perror("ERROR COULDN'T INTIALIZE MAP");
    return;
  }
  map->size = 0;
  map->cap = DEFAULT_MAP_SIZE;
}

size_t map_hash_token(Token *token, size_t cap) {
  size_t hash = 14695981039346656037ULL;
  size_t prime = 1099511628211ULL;
  const char *str = token->string.chars;
  while (*str) {
    hash ^= (unsigned char)(*str);
    hash *= prime;
  }
  return hash % cap;
}

Node *node_make_node_token(Token *token) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (!node) {
    perror("ERROR: COULD NOT MAKE NEW NODE");
    return NULL;
  }
  node->token = token;
  node->count = 1;
  node->left = NULL;
  node->right = NULL;
  return node;
}

LL *ll_make_new_ll_token(Token *token) {
  LL *new_ll = (LL *)malloc(sizeof(LL));
  if (!new_ll) {
    perror("ERROR: COULD NOT MAKE NEW NODE");
    return NULL;
  }
  new_ll->node = node_make_node_token(token);
  new_ll->next = NULL;
  return new_ll;
}

void map_add_to_ll(LL *ll, Token *token, size_t *count) {
  Token *ll_token = ll->node->token;
  while (!token_is_equal(token, ll_token) && ll->next) {
    ll = ll->next;
    ll_token = ll->node->token;
  }
  if (token_is_equal(token, ll->node->token)) {
    ++(ll->node->count);
    return;
  }
  (*count)++;
  ll->next = ll_make_new_ll_token(token);
}

// reutn the new size
// The problem here is that LL
// can still point to new things so need a tmp variable
size_t helper_resize(LL **new_data, LL *ll, size_t cap) {
  size_t count = 0;
  while (ll) {
    size_t hash = map_hash_token(ll->node->token, cap);
    if (new_data[hash]) {
      LL *new_ll = new_data[hash];
      while (new_ll->next) {
        new_ll = new_ll->next;
      }
      LL *old_tmp = ll->next;
      ll->next = NULL;
      new_ll->next = ll;
      ll = old_tmp;
    }
    // there is no ll so make the first node
    else {
      LL *old_tmp = ll->next;
      ll->next = NULL;
      new_data[hash] = ll;
      ll = old_tmp;
    }

    ++count;
  }
  return count;
}

void map_resize(Map_token *map) {
  size_t new_size = 0;
  size_t new_cap = map->cap * 2;
  LL **old_data = map->data;
  LL **data = (LL **)calloc(new_cap, sizeof(LL *));
  for (size_t i = 0; i < map->cap; ++i) {
    if (old_data[i]) {
      new_size += helper_resize(data, old_data[i], new_cap);
    }
  }
  map_free_after_resize(old_data);
  map->data = data;
  map->size = new_size;
  map->cap = new_cap;
}

void node_free_node(Node *node) {
  if (!node) {
    return;
  }
  token_free_token(node->token);
  free(node);
}

void ll_free_ll(LL *ll) {
  while (ll) {
    LL *tmp = ll->next;
    node_free_node(ll->node);
    free(ll);
    ll = tmp;
  }
}

void map_free_data(LL **data, size_t cap) {
  if (!data) {
    return;
  }

  for (size_t i = 0; i < cap; ++i) {
    ll_free_ll(data[i]);
  }
  free(data);
}

void map_free_after_resize(LL **old) { free(old); }

// need to check for resize
void map_add_map(Map_token *map, Token *token) {
  if (map->size * 2 >= map->cap) {
    map_resize(map);
  }
  size_t hash = map_hash_token(token, map->cap);
  LL *ll = map->data[hash];
  if (ll) {
    map_add_to_ll(ll, token, &map->size);
    return;
  }
  map->data[hash] = ll_make_new_ll_token(token);
  ++map->size;
}
