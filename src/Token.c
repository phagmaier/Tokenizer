#include "Token.h"

/*TOKEN STUFF*/
void token_deep_copy(Token *dest, const Token *src, Mpool *pool) {
  dest->size = src->size;
  dest->string = mpool_get(pool, src->size);

  memcpy(dest->string, src->string, src->size);
}

void token_merge_deep(Token *dest, const Token *l, const Token *r,
                      Mpool *pool) {
  dest->size = l->size + r->size - 1;
  dest->string = mpool_get(pool, dest->size);
  memcpy(dest->string, l->string, l->size - 1);
  memcpy(dest->string + l->size - 1, r->string, r->size);
}

void token_deep_copy_char(Token *dest, const char c, Mpool *pool) {
  dest->string = mpool_get(pool, 2);
  dest->size = 2;
  dest->string[0] = c;
  dest->string[1] = 0;
}

void token_deep_copy_chars(Token *dest, const char l, const char r,
                           Mpool *pool) {
  dest->string = mpool_get(pool, 3);
  dest->string[0] = l;
  dest->string[1] = r;
  dest->string[2] = 0;
  dest->size = 3;
}

Token token_make_merge(const Token *l, const Token *r, Mpool *pool) {
  Token token;
  token_merge_deep(&token, l, r, pool);
  return token;
}
Token token_make_chars(const char l, const char r, Mpool *pool) {
  Token token;
  token_deep_copy_chars(&token, l, r, pool);
  return token;
}

char *cstr_deep_copy(const Token *src, Mpool *pool) {
  char *str = mpool_get(pool, src->size);
  memcpy(str, src->string, src->size);
  return str;
}

/* TOKEN STUFF DONE */

/*MEMPOOL STUFF*/
Mpool *mpool_make_heap(const size_t cap) {
  Mpool *pool = (Mpool *)malloc(sizeof(Mpool));
  if (!pool || !cap) {
    perror("COULD NOT MAKE MEMPOOL ON HEAP");
    exit(1);
  }
  pool->mem = (char *)malloc(cap);
  if (!pool->mem) {
    perror("COULD NOT ALLOCATE CHARS FOR MEMPOOL");
    exit(1);
  }
  pool->cap = cap;
  pool->size = 0;
  pool->next = NULL;
  return pool;
}

Mpool mpool_make_stack(const size_t cap) {
  Mpool pool;
  if (!cap) {
    perror("MUST GIVE MEMPOOL A SIZE >0");
    exit(1);
  }
  pool.mem = (char *)malloc(cap);
  if (!pool.mem) {
    perror("COULD NOT ALLOCATE CHARS FOR MEMPOOL");
    exit(1);
  }
  pool.cap = cap;
  pool.size = 0;
  pool.next = NULL;
  return pool;
}

char *mpool_get(Mpool *pool, const size_t size) {
  if (size > pool->cap) {
    perror("ASKING FOR TOO MUCH MEM");
    exit(1);
  }
  while (pool->size + size > pool->cap) {
    if (!pool->next) {
      pool->next = mpool_make_heap(pool->cap);
    }
    pool = pool->next;
  }
  char *chars = pool->mem + pool->size;
  pool->size += size;
  return chars;
}

void mpool_free_heap(Mpool *pool) {
  if (pool) {
    mpool_free_heap(pool->next);
    free(pool->mem);
    free(pool);
  }
}

size_t mpool_free_heap_count(Mpool *pool) {
  if (pool) {
    size_t count = pool->cap + mpool_free_heap_count(pool->next);
    free(pool->mem);
    free(pool);
    return count;
  }
  return 0;
}
void mpool_free_stack(Mpool *pool) {
  mpool_free_heap(pool->next);
  free(pool->mem);
}

void mpool_reset(Mpool *pool) {
  size_t new_max = pool->cap + mpool_free_heap_count(pool->next);
  pool->next = NULL;
  if (new_max > pool->cap) {
    pool->cap = new_max;
    pool->mem = (char *)realloc(pool->mem, new_max);
  }
  pool->size = 0;
}

Pairs pairs_make_stack(size_t cap) {
  Pairs pairs;
  pairs.pairs = (Pair *)calloc(cap, sizeof(Pair));
  if (!pairs.pairs) {
    perror("COULD NOT MAKE PAIR ARR FOR PAIRS");
    exit(1);
  }
  pairs.cap = cap;
  pairs.size = 0;
  return pairs;
}
Pairs *pairs_make_heap(size_t cap) {
  Pairs *pairs = (Pairs *)malloc(sizeof(Pairs));
  pairs->pairs = (Pair *)calloc(cap, sizeof(Pair));
  if (!pairs->pairs) {
    perror("COULD NOT MAKE PAIR ARR FOR PAIRS");
    exit(1);
  }
  pairs->cap = cap;
  pairs->size = 0;
  return pairs;
}
void pairs_free_stack(Pairs *pairs) { free(pairs->pairs); }

void pairs_free_heap(Pairs *pairs) {
  free(pairs->pairs);
  free(pairs);
}

void pairs_reset(Pairs *pairs) { pairs->size = 0; }

void pairs_insert_chars(Pairs *pairs, const char l, const char r, Mpool *pool) {
  if (pairs->size == pairs->cap) {
    pairs->cap *= 2;
    pairs->pairs = (Pair *)realloc(pairs->pairs, pairs->cap);
    if (!pairs->pairs) {
      perror("FAILED TO REALLOC PAIRS ARRAY");
      exit(1);
    }
  }
  token_deep_copy_char(&pairs->pairs[pairs->size].left, l, pool);
  token_deep_copy_char(&pairs->pairs[pairs->size].right, r, pool);
  token_deep_copy_chars(&pairs->pairs[pairs->size].full, l, r, pool);
  ++(pairs->size);
}

/*MEMPOOL STUFF DONE*/
