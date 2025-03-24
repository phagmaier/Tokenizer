#include "Dic.h"
#include "StrArr.h"
#include <pthread.h>

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

/***************************************************
 *               GLOBAL DIC FOR ALL TOKENS
 ****************************************************/
void dicSafe_init_dic(DicSafe *dic, size_t size) {
  // pthread_mutex_init(&dic->lock, NULL);
  dic->cpool = cPool_make(size * 5); // 5 characters per word should be plentty
  dic->nodes = (char **)calloc(size, sizeof(char *));
  if (!dic->nodes) {
    perror("Couldn't initialize Safe Dic");
    exit(1);
  }
  dic->size = 0;
  dic->cap = size;
}

DicSafe *dicSafe_make_dic(size_t size) {
  DicSafe *dic = malloc(sizeof(DicSafe));
  // pthread_mutex_init(&dic->lock, NULL);
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
  free(old_nodes);
}

// tue for new insert false for old
bool dicSafe_insert(DicSafe *dic, String *ptr) {
  const char *string = ptr->str;
  // pthread_mutex_lock(&dic->lock);
  if (dic->size * 2 >= dic->cap) {
    dicSafe_resize(dic);
  }
  size_t index = dic_hash(string, dic->cap);
  if (!dic->nodes[index]) {
    dic->nodes[index] = str_deep_copy_cstring(ptr, &dic->cpool);
    ++(dic->size);
    // pthread_mutex_unlock(&dic->lock);
    return true;
  }
  while (dic->nodes[index]) {
    if (!strcmp(string, dic->nodes[index])) {
      // pthread_mutex_unlock(&dic->lock);
      return false;
    }
    index = (index + 1) % dic->cap;
  }

  dic->nodes[index] = str_deep_copy_cstring(ptr, &dic->cpool);
  ++(dic->size);
  // pthread_mutex_unlock(&dic->lock);
  return true;
}

void dicSafe_free(DicSafe *dic) {
  cPool_free(&dic->cpool);
  free(dic->nodes);
  dic->size = 0;
  free(dic);
}

Bucket *make_bucket(size_t size) {
  Bucket *bucket = (Bucket *)malloc(sizeof(Bucket));
  pthread_mutex_init(&bucket->lock, NULL);
  bucket->size = 0;
  bucket->cap = size;
  bucket->nodes = (Node *)malloc(sizeof(Node) * size);
  memset(bucket->nodes, 0, sizeof(Node) * size);
  return bucket;
}

// pthread_mutex_init(&dic->lock, NULL);
Dic *dic_make_dic(size_t num_buckets, size_t bucket_size) {
  Dic *dic = (Dic *)malloc(sizeof(Dic));
  pthread_mutex_init(&dic->lock, NULL);
  dic->num_buckets = num_buckets;
  dic->buckets = (Bucket **)malloc(sizeof(Bucket *) * num_buckets);
  for (size_t i = 0; i < num_buckets; ++i) {
    dic->buckets[i] = make_bucket(bucket_size);
  }
  dic->max_count = 0;
  return dic;
}
void bucket_resize(Bucket *bucket) {
  const size_t old_cap = bucket->cap;
  const size_t new_cap = bucket->cap * 2;
  bucket->cap = new_cap;
  Node *old_nodes = bucket->nodes;
  Node *new_nodes = (Node *)calloc(new_cap, sizeof(Node));
  if (!new_nodes) {
    perror("COULDN'T RESIZE BUCKET");
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
  bucket->nodes = new_nodes;
  free(old_nodes);
}

void update_max(Dic *dic, String string) {
  ++dic->max_count;
  dic->max_token = string;
}

void dic_insert(Dic *dic, String string) {
  char *str = string.str;
  size_t bucket_index = dic_hash(str, dic->num_buckets);
  Bucket *bucket = dic->buckets[bucket_index];
  size_t index = dic_hash(str, bucket->cap);
  pthread_mutex_lock(&bucket->lock);
  if (bucket->size * 2 >= bucket->cap) {
    bucket_resize(bucket);
  }
  if (!bucket->nodes[index].count) {
    bucket->nodes[index].string = str;
    bucket->nodes[index].count = 1;
    ++(bucket->size);
    pthread_mutex_unlock(&bucket->lock);
    if (!dic->max_count) {
      pthread_mutex_lock(&dic->lock);
      dic->max_count = 1;
      dic->max_token = string;
      pthread_mutex_unlock(&dic->lock);
    }
    return;
  }
  while (bucket->nodes[index].count) {
    if (!strcmp(str, bucket->nodes[index].string)) {
      ++(bucket->nodes[index].count);
      pthread_mutex_unlock(&bucket->lock);

      pthread_mutex_lock(&dic->lock);
      if (bucket->nodes[index].count > dic->max_count) {
        ++(dic->max_count);
        dic->max_token = string;
      }
      pthread_mutex_unlock(&bucket->lock);
      return;
    }
    index = (index + 1) % bucket->cap;
  }
  bucket->nodes[index].string = str;
  bucket->nodes[index].count = 1;
  ++(bucket->size);
  pthread_mutex_unlock(&bucket->lock);
  return;
}

void dic_reset(Dic *dic) {
  for (size_t i = 0; i < dic->num_buckets; ++i) {
    dic->buckets[i]->size = 0;
    memset(dic->buckets[i]->nodes, 0, sizeof(Node) * dic->buckets[0]->cap);
  }
  dic->max_count = 0;
}

void dic_reset_copy_max_token(Dic *dic, String *string) {
  string->str = memcpy(string->str, dic->max_token.str, dic->max_token.size);
  dic_reset(dic);
}

void dic_free(Dic *dic) {
  for (size_t i = 0; i < dic->num_buckets; ++i) {
    free(dic->buckets[i]->nodes);
    free(dic->buckets[i]);
  }
  free(dic);
}
