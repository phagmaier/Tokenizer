#include "RedBlack.h"
#include <stdio.h>

void rb_init(RB *tree) { tree->root = NULL; }

Value *rb_make_node(Token *token, size_t count) {
  Value *node = (Value *)malloc(sizeof(Value));
  node->count = count;
  node->token = token;
  node->color = RED;
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void rb_rotate_left(RB *tree, Value *node) {
  Value *new_parent = node->right;
  if (!new_parent)
    return;

  node->right = new_parent->left;
  if (new_parent->left)
    new_parent->left->parent = node;

  new_parent->parent = node->parent;
  if (!node->parent) {
    tree->root = new_parent;
  } else if (node == node->parent->left) {
    node->parent->left = new_parent;
  } else {
    node->parent->right = new_parent;
  }

  new_parent->left = node;
  node->parent = new_parent;
}

void rb_rotate_right(RB *tree, Value *node) {
  Value *new_parent = node->left;
  if (!new_parent)
    return;

  node->left = new_parent->right;
  if (new_parent->right)
    new_parent->right->parent = node;

  new_parent->parent = node->parent;
  if (!node->parent) {
    tree->root = new_parent;
  } else if (node == node->parent->left) {
    node->parent->left = new_parent;
  } else {
    node->parent->right = new_parent;
  }

  new_parent->right = node;
  node->parent = new_parent;
}

void rb_helper_insert(RB *tree, Value *node) {
  if (!tree->root) {
    node->color = BLACK;
    tree->root = node;
    return;
  }
  Dir dir;
  const size_t count = node->count;
  Value *curr = tree->root;
  Value *prev = NULL;
  while (curr) {
    if (count > curr->count) {
      dir = RIGHT;
      prev = curr;
      curr = curr->right;
    } else {
      dir = LEFT;
      prev = curr;
      curr = curr->left;
    }
  }
  if (dir == RIGHT) {
    prev->right = node;
  } else {
    prev->left = node;
  }
  node->parent = prev;
}

void rb_uncle_red(Value *grandparent, Value *parent, Value *uncle) {
  grandparent->color = RED;
  parent->color = BLACK;
  uncle->color = BLACK;
}

void rb_fix_insert(RB *tree, Value *node) {
  while (node->parent && node->parent->color == RED) {
    Value *parent = node->parent;
    Value *grandparent = parent->parent;
    if (!grandparent)
      break; // Root reached

    Value *uncle =
        (parent == grandparent->left) ? grandparent->right : grandparent->left;

    if (uncle && uncle->color == RED) {
      // Case 1: Red Uncle -> Recolor and move up
      grandparent->color = RED;
      parent->color = BLACK;
      uncle->color = BLACK;
      node = grandparent;
    } else {
      if (parent == grandparent->left) {
        if (node == parent->right) {
          // Case 2: Left-Right (Triangle) -> Rotate Left
          rb_rotate_left(tree, parent);
          node = parent;
          parent = node->parent;
        }
        // Case 3: Left-Left (Line) -> Rotate Right
        rb_rotate_right(tree, grandparent);
      } else {
        if (node == parent->left) {
          // Case 4: Right-Left (Triangle) -> Rotate Right
          rb_rotate_right(tree, parent);
          node = parent;
          parent = node->parent;
        }
        // Case 5: Right-Right (Line) -> Rotate Left
        rb_rotate_left(tree, grandparent);
      }

      parent->color = BLACK;
      grandparent->color = RED;
    }
  }
  tree->root->color = BLACK; // Ensure root is always black
}

void rb_insert_value(RB *tree, Value *node) {
  rb_helper_insert(tree, node);
  rb_fix_insert(tree, node);
}

Value *rb_find_min(Value *node) {
  while (node->left) {
    node = node->left;
  }
  return node;
}

void rb_insert_token(RB *tree, Token *token, size_t count) {
  Value *node = rb_make_node(token, count);
  rb_insert_value(tree, node);
}

void rb_replace_node(RB *tree, Value *old_node, Value *new_node) {
  if (!old_node->parent) {
    tree->root = new_node;
  } else if (old_node == old_node->parent->left) {
    old_node->parent->left = new_node;
  } else {
    old_node->parent->right = new_node;
  }

  if (new_node) {
    new_node->parent = old_node->parent;
  }
}

void rb_fix_delete(RB *tree, Value *node) {
  while (node != tree->root && (!node || node->color == BLACK)) {
    Value *parent = node->parent;
    Value *sibling;

    if (node == parent->left) {
      sibling = parent->right;
      if (sibling->color == RED) {
        sibling->color = BLACK;
        parent->color = RED;
        rb_rotate_left(tree, parent);
        sibling = parent->right;
      }

      if ((!sibling->left || sibling->left->color == BLACK) &&
          (!sibling->right || sibling->right->color == BLACK)) {
        sibling->color = RED;
        node = parent;
      } else {
        if (!sibling->right || sibling->right->color == BLACK) {
          if (sibling->left)
            sibling->left->color = BLACK;
          sibling->color = RED;
          rb_rotate_right(tree, sibling);
          sibling = parent->right;
        }

        sibling->color = parent->color;
        parent->color = BLACK;
        if (sibling->right)
          sibling->right->color = BLACK;
        rb_rotate_left(tree, parent);
        node = tree->root;
      }
    } else { // Mirror case for right subtree
      sibling = parent->left;
      if (sibling->color == RED) {
        sibling->color = BLACK;
        parent->color = RED;
        rb_rotate_right(tree, parent);
        sibling = parent->left;
      }

      if ((!sibling->right || sibling->right->color == BLACK) &&
          (!sibling->left || sibling->left->color == BLACK)) {
        sibling->color = RED;
        node = parent;
      } else {
        if (!sibling->left || sibling->left->color == BLACK) {
          if (sibling->right)
            sibling->right->color = BLACK;
          sibling->color = RED;
          rb_rotate_left(tree, sibling);
          sibling = parent->left;
        }

        sibling->color = parent->color;
        parent->color = BLACK;
        if (sibling->left)
          sibling->left->color = BLACK;
        rb_rotate_right(tree, parent);
        node = tree->root;
      }
    }
  }
  if (node)
    node->color = BLACK;
}

void rb_delete(RB *tree, Value *node) {
  if (!node)
    return;

  Value *replacement;
  Value *child;
  int original_color = node->color;

  if (!node->left) {
    child = node->right;
    rb_replace_node(tree, node, node->right);
  } else if (!node->right) {
    child = node->left;
    rb_replace_node(tree, node, node->left);
  } else {
    // Find the in-order successor
    replacement = rb_find_min(node->right);
    original_color = replacement->color;
    child = replacement->right;

    if (replacement->parent == node) {
      if (child)
        child->parent = replacement;
    } else {
      rb_replace_node(tree, replacement, replacement->right);
      replacement->right = node->right;
      replacement->right->parent = replacement;
    }

    rb_replace_node(tree, node, replacement);
    replacement->left = node->left;
    replacement->left->parent = replacement;
    replacement->color = node->color;
  }

  free(node);

  if (original_color == BLACK) {
    rb_fix_delete(tree, child);
  }
}

Value *rb_find_max(RB *tree) {
  Value *cur = tree->root;
  if (!cur) {
    return NULL;
  }
  while (cur->right) {
    cur = cur->right;
  }
  return cur;
}

Value *rb_find(RB *tree, Token *token, size_t count) {
  Value *curr = tree->root;
  while (curr) {
    if (count > curr->count) {
      curr = curr->right;
    } else {
      if (curr->count == count && curr->token == token) {
        return curr;
      }
      curr = curr->left;
    }
  }
  return NULL;
}

void rb_free_tree_helper(Value *node) {
  if (node) {
    rb_free_tree_helper(node->left);
    rb_free_tree_helper(node->right);
  }
  free(node);
}

void rb_free_tree(RB *tree) {
  if (tree->root) {
    rb_free_tree_helper(tree->root->right);
    rb_free_tree_helper(tree->root->left);
    free(tree->root);
  }
}
