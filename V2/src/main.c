// #include "Map_token.h"
#include "Token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DELIMITERS " \t\n\r.,;:!?\"'()[]{}<>|\\/+=-_*&^%$#@`~"

int is_printable_ascii(char c) { return c >= 32 && c <= 126; }

// int is_delimiter(char c) { return strchr(DELIMITERS, c) != NULL; }

// in real version just pass path of the data folder then glob all the .txt
// files
bool make_map(const char *fileName) {
  size_t total = 0;
  FILE *file = fopen(fileName, "rb");
  if (!file) {
    perror("Error opening file");
    return true;
  }
  char c;
  char pair[2];
  pair[0] = fgetc(file);
  while ((c = fgetc(file)) != EOF) {
    if (is_printable_ascii(c)) {
      ++total;
      pair[1] = c;
      pair[0] = pair[1];
    }
  }
  printf("TOTAL NUMBER OF BYTE/CHAR PAIRS IS:%zu\n", total);
  fclose(file);
  return false;
}

int main() {
  const char *fileName = "../data/infiniteJest.txt";
  // make_map(fileName);
  char a = 'P';
  char b = 'a';
  char c = 'r';
  char d = 'k';

  Token *one = token_from_bytes(a, b);
  Token *two = token_from_bytes(c, d);
  Token *three = token_merge_tokens(one, two, 1);
  printf("%s\n", one->string);
  printf("%s\n", two->string);
  printf("%s\n", three->string);
  return 0;
}
