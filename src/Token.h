#pragma once
#include "String.h"
#include <stdbool.h>

typedef struct Token {
  bool is_token;
  String string;
  unsigned int token;
} Token;

Token *token_init_from_chars(const char *pair);
void token_copy_token(const Token *src, Token *dest);
bool token_is_equal(const Token *one, const Token *two);
Token *token_merge_tokens(const Token *left, const Token *right,
                          unsigned int val);
void token_free_token(Token *token);
