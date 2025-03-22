#include "Helper_fucs.h"

long get_num_threads() {
  long cores = sysconf(_SC_NPROCESSORS_ONLN);
  long usable_threads =
      (cores > 2) ? cores - 1 : 1; // Leave at least 1 core free
  return usable_threads;
}

char *append_vocab(char **vocab, char *token, CPool *cpool, size_t index) {
  unsigned int s_size = strlen(token) + 1;
  char *new_token = cPool_get(cpool, s_size);
  memcpy(new_token, token, s_size);
  vocab[index] = new_token;
  return new_token;
}

void swap_mempool_ptrs(CPool **one, CPool **two) {
  CPool *tmp = *one;
  *one = *two;
  *two = tmp;
}

void swap_text_ptrs(StrArr **one, StrArr **two) {
  StrArr *tmp = *one;
  *one = *two;
  *two = tmp;
}

void write_vocab(char **arr, int minutes, double seconds, size_t size) {
  FILE *file = fopen("../C_tokens.txt", "w");
  if (file == NULL) {
    perror("Error: Could not open file.\n");
    exit(1);
  }
  fprintf(file, "PROGRAM RAN IN: %d.%.0f minutes\n", minutes, seconds);
  for (size_t i = 0; i < size; i++) {
    fprintf(file, "%zu: %s\n", i, arr[i]);
  }
  fclose(file);
}
