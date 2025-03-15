#include "Token.h"
#include "String.h"
#include <stdio.h>

Token *token_init_from_chars(const char *pair) {
  Token *token = malloc(sizeof(Token));
  token->is_token = false;
  token->token = 0;
  token->string.chars = (char *)malloc(DEFAULT_STRING_SIZE + 1);
  if (!token->string.chars) {
    perror("ERRROR: Could not init token from chars");
    return NULL;
  }
  token->string.chars[0] = pair[0];
  token->string.chars[1] = pair[1];
  token->string.chars[2] = 0;
  token->string.cap = DEFAULT_STRING_SIZE;
  token->string.size = 2;
  return token;
}

bool token_is_equal(const Token *one, const Token *two) {
  if (one->is_token && two->is_token) {
    return (one->token == two->token);
  }
  if (one->string.size != two->string.size) {
    return false;
  }
  const char *s1 = one->string.chars;
  const char *s2 = two->string.chars;
  size_t i = 0;
  while (s1[i] != 0 && s2[i] != 0) {
    ++i;
  }
  return (s1[i] == s2[i]);
}

void token_copy_token(const Token *src, Token *dest) {
  string_init_copy(&dest->string, &dest->string);
  dest->is_token = src->is_token;
  dest->token = src->token;
}

Token *token_merge_tokens(const Token *left, const Token *right,
                          unsigned int val) {
  size_t size = left->string.size + right->string.size;

  Token *token = (Token *)malloc(sizeof(Token));
  if (!token) {
    perror("ERROR: Could not allocate memory for Token");
    return NULL;
  }

  token->string.chars = (char *)malloc(size + 1);
  if (!token->string.chars) {
    perror("ERROR: Could not allocate memory for Token string");
    free(token);
    return NULL;
  }

  token->is_token = true;
  token->token = val;
  token->string.size = size;
  token->string.cap = size;

  memcpy(token->string.chars, left->string.chars, left->string.size);
  memcpy(token->string.chars + left->string.size, right->string.chars,
         right->string.size);

  token->string.chars[size] = '\0';
  return token;
}

void token_free_token(Token *token) {
  if (token) {
    string_free_string(&token->string);
    free(token);
  }
}
