#include "Dic.h"
#include "Helper_fucs.h"
#include "StrArr.h"
#include "Tokenizer.h"
#include <dirent.h>
#include <stdio.h>
#include <time.h>
#define DEFAULT_VOCAB_SIZE 5000

int has_txt_extension(const char *filename) {
  const char *ext = strrchr(filename, '.');
  return (ext && strcmp(ext, ".txt") == 0);
}

long findSize(const char *fileName) {
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) {
    perror("ERROR ON YOUR END DIDN'T MAKE FILE PROPERLY\n");
    exit(1);
  }
  fseek(fp, 0L, SEEK_END);

  // calculating the size of the file
  size_t res = ftell(fp);
  printf("SIZE OF THE FILE: %s is: %zu\n", fileName, res);
  fclose(fp);
  return res;
}

int main() {
  const char *fileName = "../data/data.txt";
  findSize(fileName);
  return 0;
}
