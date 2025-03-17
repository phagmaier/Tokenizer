// #include "Map_token.h"
#include "Dic.h"
#include "RedBlack.h"
#include "Token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count_ll(Node *node) {
  size_t count = 0;
  while (node) {
    count += node->count;
    node = node->next;
  }
  return count;
}
size_t dic_count(Dic *dic) {
  size_t count = 0;
  for (size_t i = 0; i < dic->cap; ++i) {
    if (dic->data[i]) {
      count += count_ll(dic->data[i]);
    }
  }
  return count;
}

size_t insert_ll(Node *node, RB *tree) {
  size_t max = 0;
  while (node) {
    if (node->count > max) {
      max = node->count;
    }
    rb_insert_token(tree, node->token, node->count);
    node = node->next;
  }
  return max;
}

void print_node(Value *node) {
  if (node) {
    printf("%s\n", node->token->string);
    print_node(node->left);
    print_node(node->right);
  }
}

void print_tree(Value *node) {
  while (node) {
    print_node(node->left);
    print_node(node->right);
  }
}

void make_rb(Dic *dic) {
  size_t max = 0;
  RB rb;
  RB *rb_ptr = &rb;
  rb_init(rb_ptr);
  for (size_t i = 0; i < dic->cap; ++i) {
    if (dic->data[i]) {
      size_t tmp = insert_ll(dic->data[i], rb_ptr);
      if (tmp > max) {
        max = tmp;
      }
    }
  }
  printf("THE HIGHEST COUNT IS: %zu\n", max);
  Value *val = rb_find_max(rb_ptr);
  printf("THE MOST FREQUENTLY OCCURING TOKEN WAS: %s\n", val->token->string);
  size_t rb_max = val->count;
  if (rb_max == max) {
    printf("YAY RB TREE GAVE THE MAX\n");
  } else {
    printf("BOO THE RB TREE GAVE THE WRONG NUMBER\n");
  }
  // print_tree(rb.root);
  rb_free_tree(rb_ptr);
}

// #define DELIMITERS " \t\n\r.,;:!?\"'()[]{}<>|\\/+=-_*&^%$#@`~"

int is_printable_ascii(char c) { return c >= 32 && c <= 126; }

////////////////////////////////////////////////////
bool make_map(const char *fileName) {
  Dic dic;
  dic_init_dic(&dic);
  Dic *dic_ptr = &dic;
  Token *prev = NULL;
  Token *curr = NULL;
  Token *next = NULL;

  FILE *file = fopen(fileName, "rb");
  if (!file) {
    perror("Error opening file");
    return true;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  // Allocate buffer and read file
  char *buffer = (char *)malloc(file_size);
  if (!buffer) {
    perror("Memory allocation failed");
    fclose(file);
    return true;
  }
  fread(buffer, 1, file_size, file);
  fclose(file);

  // Process character pairs
  size_t total = 0;
  char pair[2] = {buffer[0], 0}; // Initialize first char
  for (size_t i = 1; i < file_size; i++) {
    if (is_printable_ascii(buffer[i])) {
      pair[1] = buffer[i];
      curr = token_from_bytes(pair[0], pair[1]);
      dic_insert(dic_ptr, curr, prev, next);
      pair[0] = pair[1]; // Shift to the next pair
      total++;
    }
  }

  printf("TOTAL NUMBER OF BYTE/CHAR PAIRS IS: %zu\n", total);
  size_t count = dic_count(dic_ptr);
  if (count == total) {
    printf("YAY THE COUNTS WERE EQUAL\n");
  } else {
    printf("COUNTS NOT EQUAL\n");
  }
  printf("TOTAL COUNT DIC: %zu\n", count);

  free(buffer);
  make_rb(dic_ptr);
  dic_free_dic(dic_ptr);
  return false;
}
///////////////////////////////////////////////////

// int is_delimiter(char c) { return strchr(DELIMITERS, c) != NULL; }

// in real version just pass path of the data folder then glob all the .txt
// files
// FOR TESTING NOW ALL THE ADJACENT TOKEN POINTERS ARE NULL
/*
bool make_map(const char *fileName) {
  Dic dic;
  dic_init_dic(&dic);
  Dic *dic_ptr = &dic;
  size_t total = 0;
  Token *prev = NULL;
  Token *curr = NULL;
  Token *next = NULL;
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
      // pair[0] = pair[1];
      curr = token_from_bytes(pair[0], pair[1]);
      dic_insert(dic_ptr, curr, prev, next);
      pair[0] = pair[1];
    }
  }
  printf("TOTAL NUMBER OF BYTE/CHAR PAIRS IS:%zu\n", total);
  size_t count = dic_count(dic_ptr);
  if (count == total) {
    printf("YAY THE COUNTS WERE EQUAL\n");
  } else {
    printf("COUNTS NOT EQUAL\n");
  }
  printf("TOTAL COUNT DIC: %zu\n", count);
  fclose(file);
  make_rb(dic_ptr);
  dic_free_dic(dic_ptr);
  return false;
}
*/

int main() {
  const char *fileName = "../data/infiniteJest.txt";
  make_map(fileName);
  return 0;
}
