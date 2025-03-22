#include "Dic.h"
#include "StrArr.h"
#include <pthread.h>
#include <string.h>

void node_init_node(Node *node, char *string) {
  node->string = string;
  node->count = 1;
}

void dic_init_dic(Dic *dic) {
  dic->nodes = (Node *)calloc(DEFAULT_DIC_SIZE, sizeof(Node));
  dic->size = 0;
  dic->cap = DEFAULT_DIC_SIZE;
  dic->max_token = NULL;
  dic->max_count = 0;
}

Dic dic_make_dic_default() {
  Dic dic;
  dic.nodes = (Node *)calloc(DEFAULT_DIC_SIZE, sizeof(Node));
  dic.size = 0;
  dic.cap = DEFAULT_DIC_SIZE;
  dic.max_token = NULL;
  dic.max_count = 0;
  return dic;
}

Dic dic_make_dic(size_t size) {
  Dic dic;
  dic.nodes = (Node *)calloc(size, sizeof(Node));
  dic.size = 0;
  dic.cap = size;
  dic.max_token = NULL;
  dic.max_count = 0;
  return dic;
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
    if (!dic->max_count) {
      dic->max_count = 1;
      dic->max_token = dic->nodes[index].string;
    }
    return;
  }
  while (dic->nodes[index].string) {
    if (!strcmp(string, dic->nodes[index].string)) {
      ++(dic->nodes[index].count);
      if (dic->nodes[index].count > dic->max_count) {
        dic->max_count = dic->nodes[index].count;
        dic->max_token = dic->nodes[index].string;
      }
      return;
    }
    index = (index + 1) % dic->cap;
  }
  node_init_node(&dic->nodes[index], string);
  if (!dic->max_count) {
    dic->max_count = 1;
    dic->max_token = dic->nodes[index].string;
  }
}

char *dic_reset(Dic *dic) {
  char *token = dic->max_token;
  memset(dic->nodes, 0, sizeof(Node) * dic->cap);
  dic->size = 0;
  dic->max_count = 0;
  dic->max_token = NULL;
  return token;
}

void dic_free(Dic *dic) {
  free(dic->nodes);
  dic->size = 0;
  dic->max_count = 0;
  dic->max_token = NULL;
}

/***************************************************
 *               DIC SAFE STUF
 ****************************************************/
void dicSafe_init_dic(DicSafe *dic, size_t size) {
  pthread_mutex_init(&dic->lock, NULL);
  dic->cpool = cPool_make(size * 5); // 5 characters per word should be plentty
  dic->nodes = (char **)calloc(size, sizeof(char *));
  dic->size = 0;
  dic->cap = size;
}

void dicSafe_resize(DicSafe *dic) {
  size_t old_cap = dic->cap;
  size_t new_cap = dic->cap * 2;
  dic->cap = new_cap;
  char **old_nodes = dic->nodes;
  char **new_nodes = (char **)calloc(new_cap, sizeof(char *));
  for (size_t i = 0; i < old_cap; ++i) {
    if (old_nodes[i]) {
      size_t index = dic_hash(old_nodes[i], new_cap);
      while (new_nodes[index]) {
        index = (index + 1) % new_cap;
      }
      new_nodes[index] = old_nodes[i];
    }
  }
  dic->nodes = new_nodes;
  free(old_nodes);
}

// tue for new insert false for old
bool dicSafe_insert(DicSafe *dic, String string) {
  pthread_mutex_lock(&dic->lock);
  if (dic->size * 2 >= dic->cap) {
    dicSafe_resize(dic);
  }
  size_t index = dic_hash(string.str, dic->cap);
  if (!dic->nodes[index]) {
    // node_init_node(&dic->nodes[index], string);
    dic->nodes[index] = str_deep_copy(string, &dic->cpool).str;
    pthread_mutex_unlock(&dic->lock);
    return true;
  }
  while (dic->nodes[index]) {
    if (!strcmp(string.str, dic->nodes[index])) {
      pthread_mutex_unlock(&dic->lock);
      return false;
    }
    index = (index + 1) % dic->cap;
  }

  pthread_mutex_unlock(&dic->lock);
  dic->nodes[index] = str_deep_copy(string, &dic->cpool).str;
  return true;
}

void dicSafe_free(DicSafe *dic) {
  cPool_free(&dic->cpool);
  free(dic->nodes);
  dic->size = 0;
}
