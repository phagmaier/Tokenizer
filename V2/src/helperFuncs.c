#include "helperFuncs.h"

/*
size_t ll_count(LL *ll, size_t *ptr) {
  size_t count = 0;
  while (ll) {
    count += ll->node->count;
    ll = ll->next;
    ++(*ptr);
  }
  return count;
}

size_t map_count(Map_token *map) {
  size_t entries = 0;
  size_t non_ll = 0;
  LL **data = map->data;
  size_t count = 0;
  for (size_t i = 0; i < map->cap; ++i) {
    if (data[i]) {
      count += ll_count(data[i], &entries);
      ++non_ll;
    }
  }
  printf("TOTAL NUMBER OF UNIQUE PAIRS %zu\n", entries);
  printf("TOTAL NUMBER ELEMENTS WITHOUT COLISION: %zu\n", non_ll);
  return count;
}
*/
