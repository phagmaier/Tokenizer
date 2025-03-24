#include "Helper_fucs.h"

size_t get_num_threads() {
  long cores = sysconf(_SC_NPROCESSORS_ONLN);
  long usable_threads =
      (cores > 2) ? cores - 1 : 1; // Leave at least 1 core free
  return usable_threads;
}

size_t findSize(const char *fileName) {
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) {
    perror("ERROR ON YOUR END DIDN'T MAKE FILE PROPERLY\n");
    exit(1);
  }
  fseek(fp, 0L, SEEK_END);

  // calculating the size of the file
  size_t res = ftell(fp);
  fclose(fp);
  return res;
}

void swap_arrs(StrArr **l, StrArr **r) {
  StrArr *tmp = *l;
  *l = *r;
  *r = tmp;
}
void swap_pools(CPool **l, CPool **r) {
  CPool *tmp = *l;
  *l = *r;
  *r = tmp;
}
