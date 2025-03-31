#include "Dics.h"

/*UTILITY FUNCTION STUFF*/
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
/*END OF UTILITY FUNCTION STUFF*/

/*REGULAR DIC STUFF*/
Dic *dic_make_heap(size_t cap) {
  if (!cap) {
    perror("DICTIONARY MUST HAVE A SIZE GREATER THAN 0");
    exit(1);
  }
  Dic *dic = (Dic *)malloc(sizeof(Dic));
  if (!dic) {
    perror("Could not allocate dic onto the heap");
    exit(1);
  }
  dic->nodes = (DicNode *)calloc(cap, sizeof(DicNode));
  if (!dic->nodes) {
    perror("COULD NOT ALLOCATE NODES ONTO THE HEAP");
    exit(1);
  }
  dic->size = 0;
  dic->cap = cap;
  dic->max_token.string = NULL;
  dic->max_token.size = 0;
  dic->max_count = 0;
  return dic;
}

void dic_resize(Dic *dic) {
  const size_t old_cap = dic->cap;
  const size_t new_cap = dic->cap * 2;
  dic->cap = new_cap;
  DicNode *old_nodes = dic->nodes;
  DicNode *new_nodes = (DicNode *)calloc(new_cap, sizeof(DicNode));
  if (!new_nodes) {
    perror("COULD NOT RESIZE NEW NODES");
    exit(1);
  }
  for (size_t i = 0; i < old_cap; ++i) {
    if (old_nodes[i].string) {
      size_t index = dic_hash(old_nodes[i].string, new_cap);
      while (new_nodes[index].string) {
        index = (index + 1) % new_cap;
      }
      new_nodes[index] = old_nodes[i];
    }
  }
  free(old_nodes);
  dic->nodes = new_nodes;
}

void dic_insert(Dic *dic, Token *token) {
  if (!token->string) {
    perror("INSERTING A NULL");
    exit(1);
  }
  if (dic->size * 2 >= dic->cap) {
    dic_resize(dic);
  }
  size_t index = dic_hash(token->string, dic->cap);
  while (dic->nodes[index].string) {
    if (!strcmp(dic->nodes[index].string, token->string)) {
      if (++dic->nodes[index].count > dic->max_count) {
        dic->max_count = dic->nodes[index].count;
        dic->max_token = *token;
      }
      return;
    }
    index = (index + 1) % dic->cap;
  }
  dic->nodes[index].string = token->string;
  dic->nodes[index].count = 1;
  ++(dic->size);
  if (!dic->max_count) {
    dic->max_token = *token;
    dic->max_count = 1;
  }
}

void dic_reset_get_max(Dic *dic, Token *token, Mpool *pool) {
  token->string = mpool_get(pool, dic->max_token.size);
  memcpy(token->string, dic->max_token.string, dic->max_token.size);
  token->size = dic->max_token.size;
  //*token = dic->max_token;
  memset(dic->nodes, 0, sizeof(DicNode) * dic->cap);
  dic->size = 0;
  dic->max_count = 0;
  dic->max_token.string = NULL;
  dic->max_token.size = 0;
}

void dic_reset(Dic *dic) {
  memset(dic->nodes, 0, sizeof(DicNode) * dic->cap);
  dic->size = 0;
  dic->max_count = 0;
  dic->max_token.string = NULL;
  dic->max_token.size = 0;
}

void dic_free_heap(Dic *dic) {
  free(dic->nodes);
  free(dic);
}

void dic_free_stack(Dic dic) { free(dic.nodes); }
/*END OF REGULAR DIC STUFF*/

/*SAFE DIC STUFF*/
SafeDic *safeDic_make_heap(size_t cap) {
  if (!cap) {
    perror("SAFE DICTIONARY MUST HAVE A SIZE GREATER THAN 0");
    exit(1);
  }
  SafeDic *dic = malloc(sizeof(SafeDic));
  if (!dic) {
    perror("COULD NOT ALLOCATE SAFE DIC ONTO THE HEAP");
    exit(1);
  }

  int result = pthread_mutex_init(&dic->lock, NULL);
  if (result != 0) {
    perror("Mutex initialization failed");
    exit(1);
  }
  dic->cap = cap;
  dic->size = 0;
  dic->nodes = (char **)calloc(cap, sizeof(char *));
  if (!dic->nodes) {
    perror("COULD NOT ALLOCATE SAFE DIC TOKENS ON THE HEAP");
    exit(1);
  }
  dic->pool = mpool_make_heap(cap * 3);
  if (!dic->pool) {
    perror("pools broken");
    exit(1);
  }
  return dic;
}

void safeDic_resize(SafeDic *dic) {
  // printf("DON'T THINK SAFE DIC SHOULD EVER GROW\n");
  const size_t old_cap = dic->cap;
  const size_t new_cap = dic->cap * 2;
  dic->cap = new_cap;
  char **old_nodes = dic->nodes;
  char **new_nodes = (char **)calloc(new_cap, sizeof(char *));
  if (!new_nodes) {
    perror("COULD NOT RESIZE NEW NODES");
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
  free(old_nodes);
  dic->nodes = new_nodes;
}

// need to use the Tokens pool to
bool safeDic_insert(SafeDic *dic, Token *token) {
  if (!token->string) {
    perror("TRYING TO INSERT NULL STRING INTO SAFE DIC");
    exit(1);
  }
  pthread_mutex_lock(&dic->lock);
  if (dic->size * 2 >= dic->cap) {
    safeDic_resize(dic);
  }
  size_t index = dic_hash(token->string, dic->cap);
  while (dic->nodes[index]) {
    if (!strcmp(dic->nodes[index], token->string)) {
      pthread_mutex_unlock(&dic->lock);
      return false;
    }
    index = (index + 1) % dic->cap;
  }

  dic->nodes[index] = cstr_deep_copy(token, dic->pool);

  ++(dic->size);
  pthread_mutex_unlock(&dic->lock);
  return true;
}

void safeDic_insert_char(SafeDic *dic, char c) {
  // pthread_mutex_lock(&dic->lock);
  if (dic->size * 2 >= dic->cap) {
    safeDic_resize(dic);
  }
  char *tmp = mpool_get(dic->pool, 2);
  tmp[0] = c;
  tmp[1] = 0;
  size_t index = dic_hash(tmp, dic->cap);
  while (dic->nodes[index]) {
    if (!strcmp(dic->nodes[index], tmp)) {
      perror("ALL DELIMETERS SHOULD BE UNIQUE ERROR");
      exit(1);
      // pthread_mutex_unlock(&dic->lock);
    }
    index = (index + 1) % dic->cap;
  }

  dic->nodes[index] = tmp;

  ++(dic->size);
  // pthread_mutex_unlock(&dic->lock);
}

void safeDic_free_heap(SafeDic *dic) {
  pthread_mutex_destroy((&dic->lock));
  free(dic->nodes);
  mpool_free_heap(dic->pool);
  free(dic);
}

/*END OF SAFE DIC STUFF*/

/*START DICVOCAB*/

DicVocab dicVocab_make_stack(size_t cap) {
  if (!cap) {
    perror("Vocab DICTIONARY MUST HAVE A SIZE GREATER THAN 0");
    exit(1);
  }
  DicVocab dic;
  dic.cap = cap;
  dic.size = 0;
  dic.nodes = (VNode *)calloc(cap, sizeof(VNode));
  if (!dic.nodes) {
    perror("COULD NOT ALLOCATE SAFE DIC TOKENS ON THE HEAP");
    exit(1);
  }
  dic.pool = mpool_make_stack(cap * 4);
  return dic;
}
bool dicVocab_check(DicVocab *dic, Token *token) {
  size_t index = dic_hash(token->string, dic->cap);
  while (dic->nodes[index].token.string) {
    if (!strcmp(dic->nodes[index].token.string, token->string)) {
      return true;
    }
    index = (index + 1) % dic->cap;
  }
  return false;
}

void dicVocab_free_stack(DicVocab *dic) {
  mpool_free_stack(&dic->pool);
  free(dic->nodes);
}

void dicVocab_resize(DicVocab *dic) {
  // printf("DIC VOCAB SHOULD ALWAYS BE GIVEN ENOUGH SIZE BUT RESIZING
  // ANYWAY\n");
  const size_t old_cap = dic->cap;
  const size_t new_cap = dic->cap * 2;
  dic->cap = new_cap;
  VNode *old_nodes = dic->nodes;
  VNode *new_nodes = (VNode *)calloc(new_cap, sizeof(VNode));
  if (!new_nodes) {
    perror("COULD NOT RESIZE NEW NODES");
    exit(1);
  }
  for (size_t i = 0; i < old_cap; ++i) {
    if (old_nodes[i].token.string) {
      size_t index = dic_hash(old_nodes[i].token.string, new_cap);
      while (new_nodes[index].token.string) {
        index = (index + 1) % new_cap;
      }
      new_nodes[index] = old_nodes[i];
    }
  }
  free(old_nodes);
  dic->nodes = new_nodes;
}

void dicVocab_insert(DicVocab *dic, Token *string, int value) {
  if (!string->string) {
    perror("TRYING TO INSERT NULL INTO VOCAB DIC");
    exit(1);
  }
  if (dic->size * 2 >= dic->cap) {
    dicVocab_resize(dic);
  }
  size_t index = dic_hash(string->string, dic->cap);
  while (dic->nodes[index].token.string) {
    if (!strcmp(dic->nodes[index].token.string, string->string)) {
      // printf("%s INSERTED MULTIPLE TIMES \n", string->string);
      return;
    }
    index = (index + 1) % dic->cap;
  }

  token_deep_copy(&dic->nodes[index].token, string, &dic->pool);
  dic->nodes[index].val = value;
  ++(dic->size);
}

/*START DICVOCAB*/
