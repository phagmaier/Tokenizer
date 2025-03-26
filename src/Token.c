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

void dumb_swap_thread(Token *dest, Mpool *pool) {
  char *str = mpool_get(pool, dest->size);
  memcpy(str, dest->string, dest->size);
  dest->string = str;
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
void mpool_free_stack(Mpool pool) {
  mpool_free_heap(pool.next);
  free(pool.mem);
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
/*MEMPOOL STUFF DONE*/

/*ARR TOKEN STUFF*/
ArrToken *arrToken_make_heap(const size_t cap) {
  if (!cap) {
    perror("ARR SIZE and MUST BE AT LEAST 1");
    exit(1);
  }

  ArrToken *arr = (ArrToken *)malloc(sizeof(ArrToken));
  if (!arr) {
    perror("COULD NOT MAKE ARR ON THE HEAP");
    exit(1);
  }
  arr->tokens = (Token *)malloc(sizeof(Token) * cap);
  if (!arr->tokens) {
    perror("COULD NOT ALLOCATE TOKENS");
    exit(1);
  }
  arr->size = 0;
  arr->cap = cap;
  return arr;
}

void arrToken_append(ArrToken *arr, const Token *token, Mpool *pool) {
  if (arr->size >= arr->cap) {
    perror("ARR NOT RESIZABLE");
    exit(1);
  }
  token_deep_copy(&arr->tokens[arr->size], token, pool);
  ++arr->size;
}

void arrToken_append_char(ArrToken *arr, const char c, Mpool *pool) {
  if (arr->size >= arr->cap) {
    perror("ARR NOT RESIZABLE");
    exit(1);
  }
  token_deep_copy_char(&arr->tokens[arr->size], c, pool);
  ++arr->size;
}

void arrToken_append_merge(ArrToken *arr, const Token *l, const Token *r,
                           Mpool *pool) {
  if (arr->size >= arr->cap) {
    perror("ARR NOT RESIZABLE");
    exit(1);
  }
  token_merge_deep(&arr->tokens[arr->size], l, r, pool);
  ++arr->size;
}

void arrToken_free_heap(ArrToken *arr) {
  free(arr->tokens);
  free(arr);
}

void arrToken_free_stack(ArrToken arr) { free(arr.tokens); }

void arrToken_reset(ArrToken *arr) { arr->size = 0; }
/*ARR TOKEN STUFF DONE*/
