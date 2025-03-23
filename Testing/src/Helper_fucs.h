#pragma once
#include "Dic.h"
#include "StrArr.h"
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#ifdef __linux__
#include <sys/sysinfo.h>
#endif
#endif

typedef struct ThreadData {
  char const *filename;
  size_t start;
  size_t bytes;
  StrArr *text;
  StrArr *new_text;
  CPool *pool_text;
  CPool *pool_new_text;
  Dic *batch_dic;
  DicSafe *global_dic;
} ThreadData;

typedef struct ThreadDataList {
  size_t batches;
  size_t num_threads;
  size_t *num_tokens;
  ThreadData *data;
} ThreadDataList;

size_t get_num_threads();

void swap_mempool_ptrs(CPool **one, CPool **two);

void swap_text_ptrs(StrArr **one, StrArr **two);

// void write_vocab(char **arr, int minutes, double seconds, size_t size);
void write_vocab(const DicSafe *dic, int minutes, double seconds);

int has_txt_extension(const char *filename);

size_t findSize(const char *fileName);

size_t get_ram();

ThreadDataList thread_data_make(const char *filename, const size_t vocab_size,
                                const size_t bytes_per_thread);

// be careful with this because you free the dics as well
void thread_data_list_free(ThreadDataList *arr);
