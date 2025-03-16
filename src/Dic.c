#include "Dic.h"
#include "Token.h"
#include <stdio.h>

Node *node_make_node(Token *token, const size_t count) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (!node) {
    perror("FAILED TO ALLOCATE NODE IN CONSTRUCTOR\n");
  }
  node->pairs = (Pair *)malloc(sizeof(Pair) * DEFAULT_PAIR_SIZE);
  if (!node->pairs) {
    perror("FAILED TO MAKE PAIRS FOR NODE IN CONSTRUCTOR\n");
    return NULL;
  }
  node->token = token;
  node->count = count;
  node->next = NULL;
  node->p_cap = DEFAULT_PAIR_SIZE;
  node->p_size = 0;
  return node;
}

void node_append_pair(Node *node, Token *left, Token *right) {
  if (node->p_cap == node->p_size) {
    node->p_cap *= 2;
    node->pairs = (Pair *)realloc(node->pairs, sizeof(Pair) * node->p_cap);
    if (!node->pairs) {
      perror("FAILED TO REALLOCATE MEMORY FOR PAIRS IN NODE\n");
      return;
    }
  }
  node->pairs[node->p_size].left = left;
  node->pairs[node->p_size].right = right;
  ++(node->p_size);
}

void node_free_node(Node *node) {
  if (!node) {
    return;
  }
  token_free_token(node->token);
  free(node->pairs);
  free(node);
}

void dic_init_dic(Dic *dic) {
  dic->data = (Node **)calloc(DEFAULT_DIC_SIZE, sizeof(Node *));
  if (!dic->data) {
    perror("ERROR (C)ALLOCATING MEMORY FOR DIC IN CONSTRUCTOR");
    return;
  }
  dic->taken = 0;
  dic->cap = DEFAULT_DIC_SIZE;
}

size_t dic_hash(const char *s, const size_t cap) {
  size_t hash = 0;
  for (; *s; ++s) {
    hash += *s;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash % cap;
}

size_t ll_resize(Node *node, Node **data, const size_t new_size) {
  size_t count = 0;
  while (node) {
    Node *next = node->next;
    node->next = NULL;
    size_t index = dic_hash(node->token->string, new_size);
    Node *new_node = data[index];
    if (new_node) {
      while (new_node->next) {
        new_node = new_node->next;
      }
      new_node->next = node;
    } else {
      ++count;
      data[index] = node;
    }
    node = next;
  }
  return count;
}

void dic_resize(Dic *dic) {
  const size_t old_cap = dic->cap;
  Node **old_data = dic->data;
  dic->cap = old_cap * 2;
  dic->taken = 0;
  Node **new_data = (Node **)calloc(dic->cap, sizeof(Node *));
  if (!new_data) {
    perror("ERROR (C)ALLOCATING MEMORY FOR DIC DURING RESIZING");
    return;
  }
  for (size_t i = 0; i < old_cap; ++i) {
    if (old_data[i]) {
      dic->taken += ll_resize(old_data[i], new_data, dic->cap);
    }
  }
  dic->data = new_data;
  free(old_data);
}

void dic_insert(Dic *dic, Token *token, Token *left, Token *right) {
  size_t hash = dic_hash(token->string, dic->cap);
  if (dic->data[hash]) {
    Node *node = dic->data[hash];
    // need to make sure equal
    while (node->next && !token_is_equal(node->token, token)) {
      node = node->next;
    }
    if (token_is_equal(node->token, token)) {
      ++(node->count);
      node_append_pair(node, left, right);
      token_free_token(token);
    } else {
      Node *tmp = node_make_node(token, 1);
      node_append_pair(tmp, left, right);
      node->next = tmp;
    }
  } else {
    ++dic->taken;
    Node *tmp = node_make_node(token, 1);
    node_append_pair(tmp, left, right);
    dic->data[hash] = tmp;
  }
}

void dic_free_dic(Dic *dic) {
  for (size_t i = 0; i < dic->cap; ++i) {
    Node *current = dic->data[i];
    while (current) {
      Node *next = current->next;
      node_free_node(current);
      current = next;
    }
  }
  free(dic->data);
}
