#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Token {
  char *string;
  unsigned int val;
  unsigned int size;
} Token;

Token *token_from_bytes(const char one, const char two);

void token_free_token(Token *token);
Token *token_merge_tokens(const Token *left, const Token *right,
                          unsigned int val);

bool token_is_equal(const Token *one, const Token *two);
