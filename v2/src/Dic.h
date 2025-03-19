#pragma once
#include <stdlib.h>
#include <string.h>
#define DEFAULT_DIC_SIZE 100000

typedef struct Node {
  char *string;
  size_t count;
} Node;

typedef struct Dic {
  Node *nodes;
  size_t size;
  size_t cap;
} Dic;
