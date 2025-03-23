#include "Helper_fucs.h"
#include "Dic.h"
#include "StrArr.h"
#include <string.h>

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

void write_vocab(const DicSafe *dic, int minutes, double seconds) {
  size_t count = 0;
  const size_t size = dic->cap;
  FILE *file = fopen("../C_tokens.txt", "w");
  if (file == NULL) {
    perror("Error: Could not open file.\n");
    exit(1);
  }
  fprintf(file, "PROGRAM RAN IN: %d.%.0f minutes\n", minutes, seconds);
  for (size_t i = 0; i < size; i++) {
    if (dic->nodes[i]) {
      fprintf(file, "%zu: %s\n", i, dic->nodes[i]);
      ++count;
    }
  }
  fclose(file);
  printf("SUCESSFULLY WROTE %zu TOKENS", count);
}

int has_txt_extension(const char *filename) {
  const char *ext = strrchr(filename, '.');
  return (ext && strcmp(ext, ".txt") == 0);
}

size_t findSize(const char *fileName) {
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

// eventually take out bytes per thread and determine that in here

ThreadDataList thread_data_make(const char *filename, const size_t vocab_size,
                                const size_t bytes_per_thread) {

  const size_t file_bytes = findSize(filename);

  // LIMITING NUMBER OF THREADS FOR NOW TO 8
  const size_t num_threads = get_num_threads() > 8 ? 8 : get_num_threads();

  // Compute the number of batches (ensure it's at least 1)
  size_t num_batches = (file_bytes + (num_threads * bytes_per_thread) - 1) /
                       (num_threads * bytes_per_thread);

  if (num_batches == 0) {
    num_batches = 1;
  }

  // Compute base batch size (integer division)
  size_t base_batch_size = file_bytes / num_batches;
  size_t leftover_bytes = file_bytes % num_batches;

  // Compute tokens per batch
  size_t tokens_per_batch = vocab_size / num_batches;
  size_t leftover_tokens = vocab_size % num_batches;

  ThreadDataList arr;
  arr.batches = num_batches;
  arr.num_threads = num_threads;
  arr.num_tokens = (size_t *)malloc(num_batches * num_threads * sizeof(size_t));
  arr.data =
      (ThreadData *)malloc(num_batches * num_threads * sizeof(ThreadData));

  size_t total_bytes = 0;
  size_t total_tokens = 0;

  DicSafe *global_dic = dicSafe_make_dic(vocab_size);
  Dic *batch_dic = dic_make_dic(1000000); // hardcoding this

  StrArr **arrs = (StrArr **)malloc(sizeof(StrArr *) * num_threads * 2);
  CPool **pools = (CPool **)malloc(sizeof(CPool *) * num_threads * 2);

  for (size_t i = 0; i < num_threads * 2; ++i) {
    arrs[i] = strArr_make_ptr((base_batch_size + 1) / num_threads + 1);
    pools[i] = cPool_make_ptr(((base_batch_size + 1) / num_threads + 1) * 2);
  }

  size_t start = 0;
  for (size_t i = 0; i < num_batches; ++i) {

    size_t ptr_count = 0;
    size_t batch_size = base_batch_size + (i < leftover_bytes ? 1 : 0);
    size_t batch_tokens = tokens_per_batch + (i < leftover_tokens ? 1 : 0);

    // Distribute batch size among threads
    size_t thread_base_size = batch_size / num_threads;
    size_t thread_extra = batch_size % num_threads;

    for (size_t x = 0; x < num_threads; ++x) {
      arr.num_tokens[i * num_threads + x] = batch_tokens;

      ThreadData data;
      data.filename = filename;
      data.start = start;
      data.bytes = thread_base_size + (x < thread_extra ? 1 : 0);
      data.pool_text = pools[ptr_count];
      data.text = arrs[ptr_count++];
      data.pool_new_text = pools[ptr_count];
      data.new_text = arrs[ptr_count++];
      start += data.bytes;
      data.batch_dic = batch_dic;
      data.global_dic = global_dic;
      arr.data[i * num_threads + x] = data;
    }

    total_bytes += batch_size;
    total_tokens += batch_tokens;
  }
  if (total_bytes == file_bytes) {
    printf("FILE BYTES MATCH GOOD TO GO: \n");
  } else {
    printf("ERROR BYTES DON'T MATCH\n");
  }

  if (total_tokens == vocab_size) {
    printf("VOCAB MATCHes GOOD TO GO: \n");
  } else {
    printf("ERROR TOKENS DON'T MATCH\n");
  }

  free(arrs);
  free(pools);

  return arr;
}

// be careful with this because you free the dics as well
void thread_data_list_free(ThreadDataList *arr) {
  free(arr->num_tokens);
  free(arr->data[0].global_dic);
  free(arr->data[0].batch_dic);
  for (size_t i = 0; i < arr->num_threads; ++i) {
    free(arr->data[i].text);
    free(arr->data[i].new_text);
    free(arr->data[i].pool_text);
    free(arr->data[i].pool_new_text);
  }
  free(arr->data);
}
