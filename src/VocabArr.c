#include "VocabArr.h"

void init_vocab(VocabArr *vocab) {
  vocab->arr = (char **)malloc(sizeof(char *) * DEFAULT_VOCAB_SIZE);
  vocab->size = 0;
  vocab->cap = DEFAULT_VOCAB_SIZE;
}

void free_vocab(VocabArr *vocab) { free(vocab->arr); }

void vocab_append_token(VocabArr *vocab, Token *token) {
  char *new_word = (char *)malloc(token->size + 1);
  strcpy(new_word, token->string);
  vocab->arr[vocab->size++] = new_word;
}
