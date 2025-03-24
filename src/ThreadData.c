
#include "ThreadData.h"

ThreadDataList thread_data_make(const char *filename, const size_t vocab_size) {
  const size_t file_bytes = findSize(filename);
  const size_t num_threads = get_num_threads();

  // Define a hard limit of 500,000 characters per batch
  const size_t max_chars_per_batch = 500000;

  // Compute the number of batches with an upper limit on batch size
  size_t num_batches =
      (file_bytes + max_chars_per_batch - 1) / max_chars_per_batch;
  num_batches = (num_batches < 1) ? 1 : num_batches;

  // Compute batch size with the enforced limit
  size_t base_batch_size = file_bytes / num_batches;
  size_t leftover_bytes = file_bytes % num_batches;

  // Compute tokens per batch
  size_t tokens_per_batch = vocab_size / num_batches;
  size_t leftover_tokens = vocab_size % num_batches;

  ThreadDataList arr;
  arr.batches = num_batches;
  arr.num_threads = num_threads;
  arr.data =
      (ThreadData *)malloc(num_batches * num_threads * sizeof(ThreadData));

  size_t total_bytes = 0;
  size_t total_tokens = 0;
  DicSafe *global_dic = dicSafe_make_dic(vocab_size);

  // Allocate arrays with proper sizes
  StrArr **arrs = (StrArr **)malloc(sizeof(StrArr *) * num_threads * 2);
  CPool **pools = (CPool **)malloc(sizeof(CPool *) * num_threads * 2);
  Dic **dics = (Dic **)malloc(sizeof(Dic *) * num_threads * 2);
  String **max_tokens = (String **)malloc(sizeof(String *) * num_threads);

  for (size_t i = 0; i < num_threads; ++i) {
    for (size_t j = 0; j < 2; ++j) {
      size_t idx = i * 2 + j;
      dics[idx] = dic_make(base_batch_size);
      arrs[idx] = strArr_make_ptr((base_batch_size / num_threads) + 2);
      pools[idx] = cPool_make_ptr(((base_batch_size / num_threads) + 2) * 2);
    }
    max_tokens[i] = (String *)malloc(sizeof(String));
    max_tokens[i]->str = (char *)malloc(100);
    max_tokens[i]->size = 0;
  }

  size_t start = 0;
  for (size_t i = 0; i < num_batches; ++i) {
    size_t batch_size = base_batch_size + (i < leftover_bytes ? 1 : 0);
    size_t batch_tokens = tokens_per_batch + (i < leftover_tokens ? 1 : 0);

    if (batch_size > max_chars_per_batch) {
      batch_size = max_chars_per_batch;
    }

    size_t thread_base_size = batch_size / num_threads;
    size_t thread_extra = batch_size % num_threads;

    for (size_t x = 0; x < num_threads; ++x) {
      size_t thread_idx = i * num_threads + x;
      ThreadData *data = &arr.data[thread_idx];

      data->filename = filename;
      data->start = start;
      data->bytes = thread_base_size + (x < thread_extra ? 1 : 0);

      size_t ptr_idx = x * 2;
      data->pool_text = pools[ptr_idx];
      data->text = arrs[ptr_idx];
      data->pool_new_text = pools[ptr_idx + 1];
      data->new_text = arrs[ptr_idx + 1];

      start += data->bytes;
      data->thread_dic = dics[x * 2];
      data->global_dic = global_dic;
      data->max_token = max_tokens[x];
      data->vocab_size = batch_tokens;

      printf("DATA START: %zu DATA END: %zu\n", data->start,
             data->start + data->bytes);
    }

    total_bytes += batch_size;
    total_tokens += batch_tokens;
  }

  if (total_bytes != file_bytes) {
    printf("ERROR BYTES DON'T MATCH: %zu != %zu\n", total_bytes, file_bytes);
  }

  if (total_tokens != vocab_size) {
    printf("ERROR TOKENS DON'T MATCH: %zu != %zu\n", total_tokens, vocab_size);
  }

  free(arrs);
  free(pools);
  free(dics);

  return arr;
}

void thread_data_list_free(ThreadDataList *arr) {
  dicSafe_free(arr->data[0].global_dic);
}
