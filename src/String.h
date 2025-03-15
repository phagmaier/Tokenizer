#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_STRING_SIZE 5

typedef struct String {
  char *chars;
  size_t size;
  size_t cap;
} String;

void string_append_char(String *string, char c);

void string_init_copy(String *dest, String *src);

void string_append_char(String *string, char c);
void string_concat(String *dest, String *source);
void string_make_from_pair(String *string, const char *pair);
void string_free_string(String *string);
