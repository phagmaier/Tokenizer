#include "Helper_fucs.h"
#include <string.h>

PairArr pairArr_make(size_t num_threads, size_t file_bytes) {
  size_t even = file_bytes / num_threads;
  size_t leftover = file_bytes % num_threads;

  PairArr arr;
  arr.arr = (IndexPair *)malloc(sizeof(IndexPair) * num_threads);
  arr.size = num_threads;

  size_t prev_end = 0;
  for (size_t i = 0; i < num_threads; ++i) {
    arr.arr[i].start = prev_end;
    arr.arr[i].end = prev_end + even + (i < leftover ? 1 : 0);
    prev_end = arr.arr[i].end;
  }
  return arr;
}

void init_threads_and_data_arr(ThreadAndData *arr, size_t num_threads,
                               size_t file_bytes) {
  size_t even = file_bytes / num_threads;
  size_t leftover = file_bytes % num_threads;

  size_t prev_end = 0;
  for (size_t i = 0; i < num_threads; ++i) {
    arr[i].thread arr[i].index_start = prev_end;
    arr[i].index_end = prev_end + even + (i < leftover ? 1 : 0);
    prev_end = arr[i].index_end;
  }
}

void pairArr_free(PairArr *arr) { free(arr->arr); }

void pairArr_append(PairArr *arr, size_t start, size_t end) {
  IndexPair pair;
  pair.start = start;
  pair.end = end;
  arr->arr[(arr->size)++] = pair;
}

size_t get_num_threads() {
  long cores = sysconf(_SC_NPROCESSORS_ONLN);
  long usable_threads =
      (cores > 2) ? cores - 1 : 1; // Leave at least 1 core free
  return usable_threads;
}

char *append_vocab(char **vocab, char *token, CPool *cpool, size_t index) {
  unsigned int s_size = strlen(token) + 1;
  char *new_token = cPool_get(cpool, s_size);
  memcpy(new_token, token, s_size);
  vocab[index] = new_token;
  return new_token;
}

void swap_mempool_ptrs(CPool **one, CPool **two) {
  CPool *tmp = *one;
  *one = *two;
  *two = tmp;
}

void swap_text_ptrs(StrArr **one, StrArr **two) {
  StrArr *tmp = *one;
  *one = *two;
  *two = tmp;
}

void write_vocab(char **arr, int minutes, double seconds, size_t size) {
  FILE *file = fopen("../C_tokens.txt", "w");
  if (file == NULL) {
    perror("Error: Could not open file.\n");
    exit(1);
  }
  fprintf(file, "PROGRAM RAN IN: %d.%.0f minutes\n", minutes, seconds);
  for (size_t i = 0; i < size; i++) {
    fprintf(file, "%zu: %s\n", i, arr[i]);
  }
  fclose(file);
}

int has_txt_extension(const char *filename) {
  const char *ext = strrchr(filename, '.');
  return (ext && strcmp(ext, ".txt") == 0);
}

long findSize(const char *fileName) {
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) {
    perror("ERROR ON YOUR END DIDN'T MAKE FILE PROPERLY\n");
    exit(1);
  }
  fseek(fp, 0L, SEEK_END);

  // calculating the size of the file
  size_t res = ftell(fp);
  fclose(fp);
  return res;
}

// megabytes of ram free
size_t get_ram() {
#ifdef _WIN32
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  if (GlobalMemoryStatusEx(&statex)) {
    return (statex.ullAvailPhys / 1024 / 1024) * .5;
  } else {
    perror("COULD NOT RETRIEVE RAM INFO");
    exit(1);
  }

#elif defined(__linux__)
  struct sysinfo info;
  if (sysinfo(&info) == 0) {
    return (info.totalram / 1024 / 1024) * .5;
  } else {
    perror("COULD NOT RETRIEVE RAM INFO");
    exit(1);
  }

#elif defined(__APPLE__)
  int mib[2] = {CTL_HW, HW_MEMSIZE};
  unsigned long long mem;
  size_t len = sizeof(mem);
  if (sysctl(mib, 2, &mem, &len, NULL, 0) == 0) {
    return (mem / 1024 / 1024) * .5;
  } else {
    perror("COULD NOT RETRIEVE RAM INFO");
    exit(1);
  }

#else
  perror("Unsupported OS");
  exit(1);
#endif
}
