#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Token {
  char *string;
  unsigned short size;
} Token;

typedef struct Word {
  Token *tokens;
  size_t size;
  size_t cap;
} Word;

typedef struct ArrToken {
  Word *words;
  size_t size;
  size_t cap;
} ArrToken;

typedef struct Mpool {
  char *mem;
  size_t size;
  size_t cap;
  struct Mpool *next;
} Mpool;

typedef struct Tpool {
  Token *mem;
  size_t size;
  size_t cap;
  struct Tpool *next;
} Tpool;

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

/*MEMPOOL STUFF */
Tpool *tpool_make_heap(const size_t cap);
Token *tpool_get(Tpool *pool, const size_t size);
void tpool_reset(Tpool *pool);
void tpool_free_stack(Tpool pool);
void tpool_free_heap(Tpool *pool);
size_t tpool_free_heap_count(Tpool *pool);
/*MEMPOOL STUFF DONE */

/*ARR STUFF*/
ArrToken *arrToken_make_heap(const size_t cap);
void arrToken_append_char(ArrToken *arr, const char c, Mpool *pool);
void arrToken_reset(ArrToken *arr);
void arrToken_free_heap(ArrToken *arr);
/*ARR STUFF DONE*/
