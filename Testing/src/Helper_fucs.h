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

typedef struct ThreadData {
  StrArr text;
  StrArr new_text;
  CPool cpool_text;
  CPool cpool_new_text;
  Dic dic;
  DicSafe *global_dic;
  size_t index_start;
  size_t index_end;
  size_t num_tokens;
} ThreadData;

ThreadData *threads_data_init(size_t num_threads, size_t file_bytes,
                              DicSafe *dic, size_t toal_tokens);
void threads_data_free(ThreadData *arr, size_t num_threads);

size_t get_num_threads();

char *append_vocab(char **vocab, char *token, CPool *cpool, size_t index);

void swap_mempool_ptrs(CPool **one, CPool **two);

void swap_text_ptrs(StrArr **one, StrArr **two);

void write_vocab(char **arr, int minutes, double seconds, size_t size);

int has_txt_extension(const char *filename);

long findSize(const char *fileName);

size_t get_ram();
