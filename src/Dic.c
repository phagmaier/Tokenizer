#include "Dic.h"

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

Node *dic_get_node(Dic *dic, Token *token) {
  size_t index = dic_hash(token->string, dic->cap);
  Node *node = dic->data[index];
  while (node && node->token != token) {
    node = node->next;
  }
  return node;
}

T_node *tnode_make_tnode(Token *token) {
  T_node *node = (T_node *)malloc(sizeof(T_node));
  node->next = NULL;
  return node;
}

TokenDic *Tdic_make() {
  TokenDic *dic = (TokenDic *)malloc(sizeof(TokenDic));
  dic->tokens = (T_node **)calloc(DEFAULT_TDIC_SIZE, sizeof(T_node *));
  dic->size = 0;
  dic->cap = DEFAULT_TDIC_SIZE;
  return dic;
}

void tnode_free_tnode(T_node *node) {
  T_node *tmp;
  while (node) {
    tmp = node->next;
    free(node);
    node = tmp;
  }
}

void tdic_free_tdic(TokenDic *dic) {
  for (unsigned int i = 0; i < dic->cap; ++i) {
    if (dic->tokens[i]) {
      tnode_free_tnode(dic->tokens[i]);
    }
  }
  free(dic->tokens);
  free(dic);
}

void tdic_insert_helper(T_node *newNode, T_node **data,
                        const unsigned int index) {
  T_node *node = data[index];
  if (!node) {
    data[index] = newNode;
    return;
  }
  T_node *parent;
  while (node) {
    parent = node;
    node = node->next;
  }
  parent->next = newNode;
}

void tdic_resize_helper(T_node *node, const unsigned int cap, T_node **data) {
  T_node *tmp;
  while (node) {
    tmp = node->next;
    node->next = NULL;
    size_t index = dic_hash(node->token->string, cap);
    tdic_insert_helper(node, data, cap);
  }
}

void tdic_resize(TokenDic *dic) {
  const unsigned int old_cap = dic->cap;
  unsigned int new_cap = dic->cap * 2;
  dic->cap = new_cap;
  dic->size = 0;
  T_node **new_data = (T_node **)calloc(new_cap, sizeof(T_node *));
  for (unsigned int i = 0; i < old_cap; ++i) {
    if (dic->tokens[i]) {
      tdic_resize_helper(dic->tokens[i], new_cap, new_data);
    }
  }
  dic->tokens = new_data;
}

// I never increment the size
void tdic_insert(TokenDic *dic, Token *token) {
  if (dic->size * 2 >= dic->cap) {
    tdic_resize(dic);
  }
  // T_node *node = tnode_make_tnode(token);
  size_t index = dic_hash(token->string, dic->cap);
  T_node *node = dic->tokens[index];
  T_node *parent = NULL;
  if (!node) {
    (dic->size)++;
    dic->tokens[index] = tnode_make_tnode(token);
    return;
  }
  while (node) {
    if (!strcmp(node->token->string, token->string)) {
      return;
    }
    parent = node;
    node = node->next;
  }
  parent->next = tnode_make_tnode(token);
}

T_node *tdic_search(TokenDic *dic, Token *token) {
  size_t index = dic_hash(token->string, dic->cap);
  T_node *node = dic->tokens[index];
  T_node *parent = NULL;
  while (node) {
    if (node->token == token) {
      return node;
    }
    parent = node;
    node = node->next;
  }
  printf("NO TOKEN FOUND\n");
  return node;
}

void tdic_delete(TokenDic *dic, Token *token) {
  size_t index = dic_hash(token->string, dic->cap);
  T_node *node = dic->tokens[index];
  T_node *parent = NULL;
  if (node->token == token) {
    parent = node->next;
    node->next = NULL;
    free(node);
    dic->tokens[index] = parent;
    return;
  }
  while (node) {
    if (node->token == token) {
      parent->next = node->next;
      free(node);
    }
    parent = node;
    node = node->next;
  }
  printf("NO TOKEN FOUND\n");
}
