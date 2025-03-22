#include "Dic.h"
#include "Helper_fucs.h"
#include "StrArr.h"
#include "Tokenizer.h"
#include <bits/pthreadtypes.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DEFAULT_VOCAB_SIZE 50000

void *threads_start(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  read_file_thread(&data->text, &data->cpool_text, data->index_start,
                   data->index_end);
  /*
   **************************************************
                        IMPORTANT
                    --------------------
    THE REASON YOU WANT TO SAVE STRINGS IN GLOBAL DIC IS SO YOU Don'T have to do
    a shit ton of strlens on vocab but it cuts down on memory to just use
    chars and initializing and possibly moving all those strings may be slower.
   For now maybe just use strlen in global dic and fix later

    TLDS: MAKE GLOBAL THREAD DIC USE STRINGS LOCAL DIC SHOULD STORE MAX AS
   STRING AND TAKE IN STRING 1.Both dic and global dic should take in strings to
   make it easier They don't have to save them as strings only save the max one
   as a string It will persist in memory in each dic for at least an iteration
   (since it's char existed as well) so it's fine. global even has it's own
   mempool for strings to create a persistent String You only need to actually
   store it as a string in max for local dics But everything needs to be a
   string for global dic cause i'll need those tokens later and don't want to do
   shitty strlen() on 50k tokens to make a word

    2. (NOT SO IMPORTANT) change name to get first_get_max_token (shouldn't be
   tokens with s)
   **************************************************
    */

  // It should also take in store and return a string it's just easier same
  // with the global_dic
  // get first max token (standard don't change)

  return NULL;
}

int main() {
  size_t file_bytes = findSize("../data/data.txt");
  size_t num_threads = get_num_threads();

  DicSafe global_dic;
  DicSafe *global_dic_ptr = &global_dic;
  dicSafe_init_dic(global_dic_ptr, DEFAULT_VOCAB_SIZE);
  // array for the thread data
  ThreadData *thread_data = threads_data_init(
      num_threads, file_bytes, global_dic_ptr, DEFAULT_VOCAB_SIZE);

  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);

  // spawn threads
  for (size_t i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, threads_start, &thread_data[i]);
  }

  for (size_t i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  // cleanup
  threads_data_free(thread_data, num_threads);
  free(threads);

  // free this last as it has all the tokens
  dicSafe_free(global_dic_ptr);
  return 0;
}
