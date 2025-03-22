#include "Dic.h"
#include "Helper_fucs.h"
#include "StrArr.h"
#include "Tokenizer.h"
#include <dirent.h>
#include <stdio.h>
#include <time.h>
#define DEFAULT_VOCAB_SIZE 5000

int main() {
  const char *fileName = "../data/data.txt";
  size_t file_bytes = findSize(fileName);
  size_t num_threads = get_num_threads();
  ThreadAndData *threads_and_data =
      (ThreadAndData *)malloc(sizeof(ThreadAndData) * num_threads);

  return 0;
}
