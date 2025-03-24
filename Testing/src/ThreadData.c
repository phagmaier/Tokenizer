#include "ThreadData.h"
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
  String *max_token = malloc(sizeof(String));
  max_token->str = malloc(100); // make sure always has enough mem
  max_token->size = 0;
  size_t start = 0;
  arr.max_token = max_token;
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
      data.max_token = max_token;
      arr.data[i * num_threads + x] = data;
    }

    total_bytes += batch_size;
    total_tokens += batch_tokens;
  }
  if (total_bytes != file_bytes) {
    printf("ERROR BYTES DON'T MATCH\n");
  }
  if (total_tokens != vocab_size) {
    printf("ERROR TOKENS DON'T MATCH: \n");
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
  free(arr->max_token->str);
  free(arr->max_token);
  for (size_t i = 0; i < arr->num_threads; ++i) {
    free(arr->data[i].text);
    free(arr->data[i].new_text);
    free(arr->data[i].pool_text);
    free(arr->data[i].pool_new_text);
  }
  free(arr->data);
}
