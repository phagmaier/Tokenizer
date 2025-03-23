// In the future instead of putting an arbitary limit
// use like 60% of system avaliable ram
// You already have a way to determine this
// so just do that and use max threads and
//  decide how much ram they can get based on that

#include "Helper_fucs.h"
#include "StrArr.h"
#include "Tokenizer.h"
#include <pthread.h>

/***************************************
            NOTE
HAVE HARD CODED THREAD LIMIT OF 8
IN THE FUNCTION THAT SPAWNS THE DATA ARR
***************************************/

int main() {
  /******************PROGRAM CONSTANTS*********************/
  const char *fileName = "../data/data.txt";
  const size_t VOCAB_SIZE = 50000;

  const size_t MAX_BYTES_PER_THREAD = 1000000; // Maximum bytes per batch

  /******************GET THREAD DATA*********************/
  ThreadDataList data_arr =
      thread_data_make(fileName, VOCAB_SIZE, MAX_BYTES_PER_THREAD);

  /******************SPAWN AND RUN THREADS*********************/
  pthread_t *threads = malloc(sizeof(pthread_t) * data_arr.num_threads);
  for (size_t batch = 0; batch < data_arr.batches; ++batch) {

    /******************FIRST GET FILE AND INITIALIZE DATA*********************/
    for (size_t thread = 0; thread < data_arr.num_threads; ++thread) {
      pthread_create(&threads[thread], NULL, tokenizer_read_file,
                     &data_arr.data[batch * data_arr.num_threads + thread]);
    }
    for (size_t thread = 0; thread < data_arr.num_threads; ++thread) {
      pthread_join(threads[thread], NULL);
    }
    /******************FIRST GET FILE AND INITIALIZE DATA*********************/

    /******************GET FIRST TOKEN*********************/
    for (size_t thread = 0; thread < data_arr.num_threads; ++thread) {
      pthread_create(&threads[thread], NULL, tokenizer_read_file,
                     &data_arr.data[batch * data_arr.num_threads + thread]);
    }
    for (size_t thread = 0; thread < data_arr.num_threads; ++thread) {
      pthread_join(threads[thread], NULL);
    }
    /******************GET FIRST TOKEN*********************/

    /***************** ***********************
     *CALL RESET FIRST
     *
     * WHERE WHILE LOOP SHOULD STAT LOOP TILL NUM TOKENS
     * ACHIEVED
     * **********************************************/
    for (size_t thread = 0; thread < data_arr.num_threads; ++thread) {
      pthread_create(&threads[thread], NULL, tokenizer_read_file,
                     &data_arr.data[batch * data_arr.num_threads + thread]);
    }
    for (size_t thread = 0; thread < data_arr.num_threads; ++thread) {
      pthread_join(threads[thread], NULL);
    }
    break;
    /***************** ***********************
     *CALL RESET FIRST
     *
     * WHERE WHILE LOOP SHOULD STAT LOOP TILL NUM TOKENS
     * ACHIEVED
     * **********************************************/
  }

  /******************CLEAN UP*********************/
  free(threads);
  printf("HERE\n");

  /******************IMPORTANT*********************/
  // MAKE SURE YOU EITHER COPY OR STORE THE GLOBAL
  // DICTIONARY TOKEN VALUES SOMEWHERE FIRST
  thread_data_list_free(&data_arr);
  return 0;
}
