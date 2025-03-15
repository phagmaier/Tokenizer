//  BY THE WAY NEED A START AND END OF file or like stream thing if only so it
//  knows where to begin and where to end i think

#include "Map_token.h"
#include "String.h"
#include "Token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t ll_count(LL *ll) {
  size_t count = 0;
  while (ll) {
    count += ll->node->count;
    ll = ll->next;
  }
  return count;
}

size_t map_count(Map_token *map) {
  LL **data = map->data;
  size_t count = 0;
  for (size_t i = 0; i < map->cap; ++i) {
    if (data[i]) {
      count += ll_count(data[i]);
    }
  }
  return count;
}

// #define DELIMITERS " \t\n\r.,;:!?\"'()[]{}<>|\\/+=-_*&^%$#@`~"

int is_printable_ascii(char c) { return c >= 32 && c <= 126; }

// int is_delimiter(char c) { return strchr(DELIMITERS, c) != NULL; }

// in real version just pass path of the data folder then glob all the .txt
// files
bool make_map(Map_token *dic, const char *fileName) {
  size_t total = 0;
  FILE *file = fopen("yourfile.txt", "rb");
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
      Token *tmp = token_init_from_chars(pair);
      map_add_map(Map_token * map, Token * token)
    }
  }

  fclose(file);
  printf("THESE COUNTS SHOULD MATCH\n");
  printf("TOTAL COUNT: %zu\n", total);
  total = map_count(dic);
  printf("TOTAL COUNT FROM DIC: %zu\n", total);
  return false;
}

int main() {
  const char *fileName = "../data/infiniteJest.txt";
  Map_token dic;
  map_init_map(&dic);
  if (make_map(&dic, fileName)) {
    perror("ERROR COULDN'T MAKE DICTIONARY");
    return 1;
  }
  return 0;
}
