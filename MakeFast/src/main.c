#include "Dic.h"
#include "Helper_fucs.h"
#include "StrArr.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <time.h>
#define DEFAULT_VOCAB_SIZE 5000

int main() {
  clock_t start = clock();
  // VOCAB INIT
  CPool cpool_tokens = cPool_make(DEFAULT_VOCAB_SIZE * 5);
  CPool *cpool_tokens_ptr = &cpool_tokens;
  char *new_vocab;
  char *vocab[DEFAULT_VOCAB_SIZE];

  char *fileName = "../data/infiniteJest.txt";

  // Init Cpools and also text and new_text
  CPool cpool_text = cPool_make_default();
  CPool *cpool_text_ptr = &cpool_text;
  StrArr text = read_text(fileName, cpool_text_ptr);
  StrArr *text_ptr = &text;
  CPool cpool_new_text = cPool_make(text.size + 5);
  CPool *cpool_new_text_ptr = &cpool_new_text;
  StrArr new_text = strArr_make(text.size + 1);
  StrArr *new_text_ptr = &new_text;
  Dic dic = dic_make_dic(text.size * 2);
  Dic *dic_ptr = &dic;

  // FIRST PASS IS SPECIAL
  new_vocab = first_get_max_tokens(text_ptr, dic_ptr, cpool_new_text_ptr);
  new_vocab = append_vocab(vocab, new_vocab, cpool_tokens_ptr, 0);
  // printf("NEW VOCAB WORD ITERATION 0: %s\n", new_vocab);

  cPool_reset(cpool_new_text_ptr);

  // MAIN LOOP
  for (size_t i = 1; i < DEFAULT_VOCAB_SIZE; ++i) {
    new_vocab = get_max_token(text_ptr, new_text_ptr, cpool_new_text_ptr,
                              dic_ptr, new_vocab);
    new_vocab = append_vocab(vocab, new_vocab, cpool_tokens_ptr, i);
    // printf("NEW VOCAB WORD ITERATION  %zu: %s\n", i, new_vocab);
    StrArr_reset(text_ptr);
    cPool_reset(cpool_text_ptr);
    swap_mempool_ptrs(&cpool_text_ptr, &cpool_new_text_ptr);
    swap_text_ptrs(&text_ptr, &new_text_ptr);
  }
  clock_t end = clock();
  double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
  int minutes = (int)(elapsed_time / 60); // Get the whole number of minutes
  double seconds = elapsed_time - (minutes * 60); // Get the remaining seconds
  write_vocab(vocab, minutes, seconds, DEFAULT_VOCAB_SIZE);
  // cleanup
  StrArr_free(text_ptr);
  StrArr_free(new_text_ptr);
  cPool_free(cpool_tokens_ptr);
  cPool_free(cpool_text_ptr);
  cPool_free(cpool_new_text_ptr);
  dic_free(dic_ptr);
  return 0;
}
