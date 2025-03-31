#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Token {
  char *string;
  unsigned short size;
} Token;

typedef struct Pair {
  Token left;
  Token right;
  Token full;
} Pair;

typedef struct Pairs {
  Pair *pairs;
  size_t size;
  size_t cap;
} Pairs;

typedef struct Mpool {
  char *mem;
  size_t size;
  size_t cap;
  struct Mpool *next;
} Mpool;

/*TOKEN STUFF*/
void token_deep_copy(Token *dest, const Token *src, Mpool *pool);
void token_merge_deep(Token *dest, const Token *l, const Token *r, Mpool *pool);
void token_deep_copy_char(Token *dest, const char c, Mpool *pool);
void thread_deep_copy_token(Token *dest, Token *src);
Token token_make_merge(const Token *l, const Token *r, Mpool *pool);
Token token_make_chars(const char l, const char r, Mpool *pool);
char *cstr_deep_copy(const Token *src, Mpool *pool);
/*TOKEN STUFF DONE*/

/*MEMPOOL STUFF */
Mpool *mpool_make_heap(const size_t cap);
Mpool mpool_make_stack(const size_t cap);
char *mpool_get(Mpool *pool, const size_t size);
void mpool_reset(Mpool *pool);
void mpool_free_stack(Mpool *pool);
void mpool_free_heap(Mpool *pool);
size_t mpool_free_heap_count(Mpool *pool);
/*MEMPOOL STUFF DONE */

Pairs pairs_make_stack(size_t cap);
Pairs *pairs_make_heap(size_t cap);
void pairs_free_stack(Pairs *pairs);
void pairs_free_heap(Pairs *pairs);
void pairs_reset(Pairs *pairs);
void pairs_insert_chars(Pairs *pairs, const char l, const char r, Mpool *pool);
