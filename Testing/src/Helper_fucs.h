#pragma once
#include "Dic.h"
#include "StrArr.h"
#include <pthread.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#ifdef __linux__
#include <sys/sysinfo.h>
#endif
#endif

typedef struct IndexPair {
  size_t start;
  size_t end;
} IndexPair;

typedef struct PairArr {
  IndexPair *arr;
  size_t size;
} PairArr;

typedef struct ThreadData {
  StrArr text;
  StrArr new_text;
  CPool cpool_text;
  CPool cpool_new_text;
  Dic dic;
  size_t index_start;
  size_t index_end;
  // will also need pointer to safe dic
} ThreadAndData;

void init_threads_and_data_arr(ThreadAndData *arr, size_t num_threads,
                               size_t file_bytes);

PairArr pairArr_make(size_t num_threads, size_t file_bytes);
void pairArr_free(PairArr *arr);
void pairArr_append(PairArr *arr, size_t start, size_t end);

size_t get_num_threads();

char *append_vocab(char **vocab, char *token, CPool *cpool, size_t index);

void swap_mempool_ptrs(CPool **one, CPool **two);

void swap_text_ptrs(StrArr **one, StrArr **two);

void write_vocab(char **arr, int minutes, double seconds, size_t size);

int has_txt_extension(const char *filename);

long findSize(const char *fileName);

size_t get_ram();
