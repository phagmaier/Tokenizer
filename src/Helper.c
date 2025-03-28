#include "Helper.h"

size_t get_file_size(const char *fileName) {
  FILE *file = fopen(fileName, "rb"); // Open in binary mode
  if (!file) {
    perror("Failed to open file");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fclose(file);
  return size;
}

size_t get_num_threads() {
  long cores = sysconf(_SC_NPROCESSORS_ONLN);
  long usable_threads =
      (cores > 2) ? cores - 1 : 1; // Leave at least 1 core free
  return usable_threads;
}

ThreadData *create_thread_queue(char *filename, size_t vocab_tokens,
                                size_t bytes_per_thread, size_t *thread_count) {
  size_t file_bytes = get_file_size(filename);
  if (file_bytes < bytes_per_thread || vocab_tokens >= file_bytes / 2) {
    perror("GET A BIGGER FILE ADD SUPPORT LATER");
    exit(1);
  }

  size_t num_threads = get_num_threads();
  num_threads = num_threads > (*thread_count) ? (*thread_count) : num_threads;

  // Calculate total number of iterations (chunks) needed
  size_t iterations = file_bytes / bytes_per_thread;
  if (file_bytes % bytes_per_thread != 0) {
    iterations += 1; // Add one for leftover chunk
  }

  // Don't spawn more threads than iterations
  if (iterations < num_threads) {
    num_threads = iterations;
  }

  *thread_count = num_threads;

  // Spread file bytes evenly over iterations
  size_t base_chunk = file_bytes / iterations;
  size_t leftover_bytes = file_bytes % iterations;

  Index *i_arr = (Index *)malloc(sizeof(Index) * iterations);
  if (!i_arr) {
    perror("Failed to allocate i_arr");
    exit(1);
  }

  size_t tokens_per_iter = vocab_tokens / iterations;
  size_t tokens_leftover = vocab_tokens % iterations;

  size_t count = 0;
  size_t offset = 0;
  for (size_t i = 0; i < iterations; ++i) {
    size_t extra = (i < leftover_bytes) ? 1 : 0;

    i_arr[i].start = offset;
    i_arr[i].size = base_chunk + extra;
    i_arr[i].tokens = tokens_per_iter + (i < tokens_leftover);

    offset += i_arr[i].size;
    count += i_arr[i].size;
  }

  if (count != file_bytes) {
    fprintf(stderr, "ERROR: Count mismatch! count = %zu, file_bytes = %zu\n",
            count, file_bytes);
    exit(1);
  }
  if (offset != file_bytes) {
    fprintf(stderr, "Chunk offset sum (%zu) != file size (%zu)\n", offset,
            file_bytes);
    exit(1);
  }

  size_t max_tokens = i_arr[iterations - 1].size + 3;

  Indexes *indexes = malloc(sizeof(Indexes));
  if (!indexes) {
    perror("Failed to allocate Indexes");
    exit(1);
  }

  indexes->arr = i_arr;
  indexes->cap = iterations;
  indexes->size = 0;
  pthread_mutex_init(&indexes->lock, NULL);

  ThreadData *list = (ThreadData *)malloc(sizeof(ThreadData) * num_threads);
  if (!list) {
    perror("Failed to allocate ThreadData array");
    exit(1);
  }

  SafeDic *global = safeDic_make_heap(vocab_tokens * 2);

  for (size_t i = 0; i < num_threads; ++i) {
    list[i].fileName = filename;
    list[i].indexes = indexes;
    list[i].local_dic = dic_make_heap(max_tokens / 3);
    list[i].global_dic = global;
    list[i].text = arrToken_make_heap(max_tokens);
    list[i].pool_text = mpool_make_heap(max_tokens * 8);
    list[i].pool_tokens = tpool_make_heap(max_tokens * 5);
  }

  return list;
}

void threadDataList_free(ThreadData *data, size_t num_threads) {
  for (size_t i = 0; i < num_threads; ++i) {
    dic_free_heap(data[i].local_dic);
    arrToken_free_heap(data[i].text);
    mpool_free_heap(data[i].pool_text);
    tpool_free_heap(data[i].pool_tokens);
  }
  safeDic_free_heap(data[0].global_dic);
  free(data[0].indexes->arr);
  free(data[0].indexes);
  free(data);
}
