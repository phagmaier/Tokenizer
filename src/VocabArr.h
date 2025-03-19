#pragma once
#include "Token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// this is crazy small but just doing this now
#define DEFAULT_VOCAB_SIZE 100

typedef struct VocabArr {
  char **arr;
  unsigned int size;
  unsigned int cap;

} VocabArr;

void vocab_free_vocab(VocabArr *vocab);
void vocab_append_token(VocabArr *vocab, Token *token);
