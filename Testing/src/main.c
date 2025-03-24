// In the future instead of putting an arbitary limit
// use like 60% of system avaliable ram
// You already have a way to determine this
// so just do that and use max threads and
//  decide how much ram they can get based on that

#include "Tokenizer.h"
#include <pthread.h>
#include <time.h>
void write_vocab(const Vocab *vocab, size_t vocab_size, int hours, int minutes,
                 int seconds) {

  FILE *file = fopen("../C_tokens.txt", "w");
  if (file == NULL) {
    perror("Error: Could not open file.\n");
    exit(1);
  }

  printf("Execution time: %d hours, %d minutes, %d seconds\n", hours, minutes,
         seconds);

  fprintf(file, "Execution time: %d hours, %d minutes, %d seconds\n", hours,
          minutes, seconds);

  for (size_t i = 0; i < vocab_size; i++) {
    fprintf(file, "%u : %s\n", vocab[i].val, vocab[i].vocab);
  }
  fclose(file);
  printf("SUCESSFULLY WROTE %zu TOKENS", vocab_size);
}

/***************************************
            NOTE
HAVE HARD CODED THREAD LIMIT OF 8
IN THE FUNCTION THAT SPAWNS THE DATA ARR
***************************************/

int main() {
  time_t start, end;
  /******************PROGRAM CONSTANTS*********************/
  const char *fileName = "../data/infiniteJest.txt";
  // const size_t VOCAB_SIZE = 50000;
  const size_t VOCAB_SIZE = 1000;
  /******************PROGRAM CONSTANTS*********************/
  const size_t MAX_BYTES_PER_THREAD = 1000000; // Maximum bytes per batch
  time(&start);
  Vocab *vocab_arr = tokenize(fileName, VOCAB_SIZE, MAX_BYTES_PER_THREAD);
  time(&end);
  double elapsed = difftime(end, start);
  int hours = elapsed / 3600;
  int minutes = (elapsed - hours * 3600) / 60;
  int seconds = (int)elapsed % 60;
  write_vocab(vocab_arr, VOCAB_SIZE, hours, minutes, seconds);
  return 0;
}
