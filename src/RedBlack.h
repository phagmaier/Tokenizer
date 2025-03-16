#pragma once
#include "Token.h"

typedef enum Color { RED, BLACK } Color;
typedef enum Dir { LEFT, RIGHT } Dir;

typedef struct Value {
  size_t count;
  Token *token;
  Color color;
  struct Value *parent;
  struct Value *left;
  struct Value *right;
} Value;

typedef struct RB {
  Value *root;
} RB;

void rb_init(RB *tree);
Value *rb_make_node(Token *token, size_t count);
void rb_rotate_left(RB *tree, Value *node);
void rb_rotate_right(RB *tree, Value *node);
void rb_helper_insert(RB *tree, Value *node);
void rb_uncle_red(Value *grandparent, Value *parent, Value *uncle);
void rb_insert_token(RB *tree, Token *token, size_t count);
void rb_fix_insert(RB *tree, Value *node);
void rb_insert_value(RB *tree, Value *node);
Value *rb_find_min(Value *node);
void rb_replace_node(RB *tree, Value *old_node, Value *new_node);
void rb_fix_delete(RB *tree, Value *node);
void rb_delete(RB *tree, Value *node);
Value *rb_find_max(RB *tree);
Value *rb_find(RB *tree, Token *token, size_t count);
void rb_free_tree_helper(Value *node);
void rb_free_tree(RB *tree);
