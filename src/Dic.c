#include "Dic.h"
#include <string.h>

void node_init_node(Node *node, char *string) {
  node->string = string;
  node->count = 1;
}

void dic_make_dic(Dic *dic) {
  dic->nodes = (Node *)calloc(DEFAULT_DIC_SIZE, sizeof(Node));
  dic->size = 0;
  dic->cap = DEFAULT_DIC_SIZE;
}

void dic_reset(Dic *dic) {
  memset(dic->nodes, 0, sizeof(Node) * dic->cap);
  dic->size = 0;
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

void dic_resize(Dic *dic) {
  size_t old_cap = dic->cap;
  size_t new_cap = dic->cap * 2;
  dic->cap = new_cap;
  Node *old_nodes = dic->nodes;
  Node *new_nodes = (Node *)calloc(new_cap, sizeof(Node));
  for (size_t i = 0; i < old_cap; ++i) {
    if (old_nodes[i].string) {
      size_t index = dic_hash(old_nodes[i].string, new_cap);
      while (new_nodes[index].string) {
        index = (index + 1) % new_cap;
      }
      new_nodes[index] = old_nodes[i];
    }
  }
  dic->nodes = new_nodes;
  free(old_nodes);
}

void dic_insert_dic(Dic *dic, char *string) {
  if (dic->size * 2 >= dic->cap) {
    dic_resize(dic);
  }
  size_t index = dic_hash(string, dic->cap);
  if (!dic->nodes[index].string) {
    node_init_node(&dic->nodes[index], string);
    return;
  }
  while (dic->nodes[index].string) {
    if (!strcmp(string, dic->nodes[index].string)) {
      ++(dic->nodes[index].count);
      return;
    }
    index = (index + 1) % dic->cap;
  }
  node_init_node(&dic->nodes[index], string);
}

void dic_clear(Dic *dic) { memset(dic->nodes, 0, sizeof(Node) * dic->cap); }
