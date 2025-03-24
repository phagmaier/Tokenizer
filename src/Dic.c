#include "Dic.h"

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
DicSafe *dicSafe_make_dic(size_t size) {
  DicSafe *dic = malloc(sizeof(DicSafe));
  pthread_mutex_init(&dic->lock, NULL);
  dic->cpool = cPool_make(size * 5); // 5 characters per word should be plentty
  dic->nodes = (char **)calloc(size, sizeof(char *));
  if (!dic->nodes) {
    perror("Couldn't initialize Safe Dic");
    exit(1);
  }
  dic->size = 0;
  dic->cap = size;
  return dic;
}

void dicSafe_resize(DicSafe *dic) {
  size_t old_cap = dic->cap;
  size_t new_cap = dic->cap * 2;
  dic->cap = new_cap;
  char **old_nodes = dic->nodes;
  char **new_nodes = (char **)calloc(new_cap, sizeof(char *));
  if (!new_nodes) {
    perror("Couldn't resize safe Dic");
    exit(1);
  }
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
  pthread_mutex_destroy(&dic->lock);
  free(old_nodes);
}

// tue for new insert false for old
bool dicSafe_insert(DicSafe *dic, String *str) {
  const char *string = str->str;
  pthread_mutex_lock(&dic->lock);
  if (dic->size * 2 >= dic->cap) {
    dicSafe_resize(dic);
  }
  size_t index = dic_hash(string, dic->cap);
  if (!dic->nodes[index]) {
    dic->nodes[index] = str_deep_copy_cstring(str, &dic->cpool);
    ++(dic->size);
    pthread_mutex_unlock(&dic->lock);
    return true;
  }
  while (dic->nodes[index]) {
    if (!strcmp(string, dic->nodes[index])) {
      pthread_mutex_unlock(&dic->lock);
      return false;
    }
    index = (index + 1) % dic->cap;
  }

  dic->nodes[index] = str_deep_copy_cstring(str, &dic->cpool);
  ++(dic->size);
  pthread_mutex_unlock(&dic->lock);
  return true;
}

void dicSafe_free(DicSafe *dic) {
  printf("FREEING DICSAFE\n");
  cPool_free(&dic->cpool);
  free(dic->nodes);
  dic->size = 0;
  free(dic);
  printf("FREED DICSAFE\n");
}

Dic *dic_make(size_t size) {
  Dic *dic = malloc(sizeof(Dic));
  dic->nodes = (Node *)calloc(size, sizeof(Node));
  if (!dic->nodes) {
    perror("Couldn't initialize Dic");
    exit(1);
  }
  dic->size = 0;
  dic->cap = size;
  // dic.max_token = NULL;
  dic->max_count = 0;
  return dic;
}

void dic_resize(Dic *dic) {
  size_t old_cap = dic->cap;
  size_t new_cap = dic->cap * 2;
  dic->cap = new_cap;
  Node *old_nodes = dic->nodes;
  Node *new_nodes = (Node *)calloc(new_cap, sizeof(Node));
  if (!new_nodes) {
    perror("Couldn't resize safe Dic");
    exit(1);
  }
  for (size_t i = 0; i < old_cap; ++i) {
    if (old_nodes[i].count) {
      size_t index = dic_hash(old_nodes[i].string, new_cap);
      while (new_nodes[index].count) {
        index = (index + 1) % new_cap;
      }
      new_nodes[index] = old_nodes[i];
    }
  }
  dic->nodes = new_nodes;
  free(old_nodes);
}

// tue for new insert false for old
void dic_insert(Dic *dic, String str) {
  char *string = str.str;
  if (dic->size * 2 >= dic->cap) {
    dic_resize(dic);
  }
  size_t index = dic_hash(string, dic->cap);
  if (!dic->nodes[index].count) {
    ++(dic->size);
    dic->nodes[index].string = string;
    dic->nodes[index].count = 1;
    if (!dic->max_count) {
      dic->max_count = 1;
      dic->max_token = str;
    }
    return;
  }
  while (dic->nodes[index].count) {
    if (!strcmp(string, dic->nodes[index].string)) {
      ++(dic->nodes[index].count);
      if (dic->nodes[index].count > dic->max_count) {
        ++(dic->max_count);
        dic->max_token = str;
      }
      return;
    }
    index = (index + 1) % dic->cap;
  }

  dic->nodes[index].string = string;
  dic->nodes[index].count = 1;
  ++(dic->size);
}

void dic_reset(Dic *dic) {
  dic->size = 0;
  dic->max_count = 0;
  memset(dic->nodes, 0, sizeof(Node) * dic->cap);
}
void dic_reset_copy_max_token(Dic *dic, String *string) {
  string->size = dic->max_token.size;
  memcpy(string->str, dic->max_token.str, string->size);
  dic_reset(dic);
}

void dic_free(Dic *dic) {
  printf("FEEING DIC\n");
  free(dic->nodes);
  free(dic);
  printf("DIC FREED\n");
}
