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
} Pairs;

typedef struct ArrToken {
  Pairs *pairs;
  size_t size;
  size_t cap;
} ArrToken;

typedef struct Mpool {
  char *mem;
  size_t size;
  size_t cap;
  struct Mpool *next;
} Mpool;

typedef struct Ppool {
  Pair *mem;
  size_t size;
  size_t cap;
  struct Ppool *next;
} Ppool;

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
char *mpool_get(Mpool *pool, const size_t size);
void mpool_reset(Mpool *pool);
void mpool_free_stack(Mpool pool);
void mpool_free_heap(Mpool *pool);
size_t mpool_free_heap_count(Mpool *pool);
/*MEMPOOL STUFF DONE */

/*Ppool STUFF */
Ppool *ppool_make_heap(const size_t cap);
Pair *ppool_get(Ppool *pool, const size_t size);
void ppool_reset(Ppool *pool);
void ppool_free_stack(Ppool pool);
void ppool_free_heap(Ppool *pool);
size_t ppool_free_heap_count(Ppool *pool);
/*Ppool STUFF DONE */

/*ARR STUFF*/
ArrToken *arrToken_make_heap(const size_t cap);
void arrToken_append_char(ArrToken *arr, const char c, Mpool *pool);
void arrToken_reset(ArrToken *arr);
void arrToken_free_heap(ArrToken *arr);
void *arrToken_get_pairs(ArrToken *arr);
/*ARR STUFF DONE*/
