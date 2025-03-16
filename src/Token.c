#include "Token.h"

Token *token_from_bytes(const char one, const char two) {
  Token *token = (Token *)malloc(sizeof(Token));
  char *string = (char *)malloc(3);
  token->val = 0;
  token->size = 2;
  string[0] = one;
  string[1] = two;
  string[2] = 0;
  token->string = string;
  return token;
}

void token_free_token(Token *token) {
  if (token) {
    free(token->string);
  }
  free(token);
  token = NULL;
}

Token *token_merge_tokens(const Token *left, const Token *right,
                          unsigned int val) {
  Token *token = (Token *)malloc(sizeof(Token));
  token->val = val;
  token->size = left->size + right->size;
  char *str = (char *)malloc(sizeof(token->size + 1));
  memcpy(str, left->string, left->size);
  memcpy(str + left->size, right->string, right->size);
  str[token->size] = 0;
  token->string = str;
  return token;
}

bool token_is_equal(const Token *one, const Token *two) {
  char *c1 = one->string;
  char *c2 = two->string;
  while (*(c1) && (*c2) && ((*c1) == (*c2))) {
    ++c1;
    ++c2;
  }
  if ((*c1) == (*c2)) {
    return true;
  }
  return false;
}
