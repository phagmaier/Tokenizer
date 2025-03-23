#include "Helper_fucs.h"
// In the future instead of putting an arbitary limit
// use like 60% of system avaliable ram
// You already have a way to determine this
// so just do that and use max threads and
//  decide how much ram they can get based on that
int main() {
  const char *fileName = "../data/data.txt";
  const size_t VOCAB_SIZE = 50000;

  const size_t MAX_BYTES_PER_THREAD = 1000000; // Maximum bytes per batch
  const size_t thread_limit = 8;

  ThreadDataList data_arr =
      thread_data_make(fileName, VOCAB_SIZE, MAX_BYTES_PER_THREAD);

  /******************IMPORTANT*********************/
  // MAKE SURE YOU EITHER COPY OR STORE THE GLOBAL
  // DICTIONARY TOKEN VALUES SOMEWHERE FIRST
  thread_data_list_free(&data_arr);
  return 0;
}
