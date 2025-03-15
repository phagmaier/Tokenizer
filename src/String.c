#include "String.h"
#include <string.h>

void string_init_default(String *string) {
  string->chars = (char *)malloc(DEFAULT_STRING_SIZE + 1);
  string->size = 0;
  string->cap = DEFAULT_STRING_SIZE;
}

void string_init_copy(String *dest, String *src) {
  size_t size = src->size;
  dest->chars = (char *)malloc(size + 1);
  dest->size = size;
  dest->cap = size;
  memcpy(dest, src, size + 1);
}

void string_append_char(String *string, char c) {
  if (string->cap == string->size) {
    string->cap *= 2;
    string->chars = (char *)realloc(string->chars, string->cap + 1);
    if (!string->chars) {
      perror("COULD NOT REALLOCATE AND GROW STRING");
      return;
    }
  }
  string->chars[string->size++] = c;
  string->chars[string->size] = '\0';
}

void string_concat(String *dest, String *source) {
  size_t total_size = dest->size + source->size;
  if (dest->cap < total_size) {
    dest->chars = (char *)realloc(dest->chars, dest->cap + 1);
    if (!dest->chars) {
      perror("COULD NOT REALLOCATE AND GROW STRING FOR CONCAT");
      return;
    }
  }
  memcpy(dest->chars + dest->size, source->chars, source->size);
}

void string_make_from_pair(String *string, const char *pair) {
  string->chars = (char *)malloc(DEFAULT_STRING_SIZE + 1);
  string->size = 2;
  string->cap = DEFAULT_STRING_SIZE;
  string->chars[0] = pair[0];
  string->chars[1] = pair[1];
  string->chars[2] = '\0';
}

void string_free_string(String *string) {
  if (string) {
    free(string->chars);
  }
}
