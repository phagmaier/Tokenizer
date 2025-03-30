#include "Dics.h"
// #include "TextToken.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Default parameters
// #define DEFAULT_INPUT_FILE "../data/data.txt"
#define DEFAULT_INPUT_FILE "../data/infinitejest.txt"
#define DEFAULT_OUTPUT_FILE "../data/myTokens.txt"
#define DEFAULT_VOCAB_SIZE 10000
// #define DEFAULT_VOCAB_SIZE 50000
#define DEFAULT_BYTES_PER_THREAD 500000
#define DEFAULT_MAX_NUM_THREADS 15

int main(int argc, char *argv[]) {
  time_t start, end;

  // Initialize defaults
  char *fileName = DEFAULT_INPUT_FILE;
  char *output_file_name = DEFAULT_OUTPUT_FILE;
  size_t vocab_size = DEFAULT_VOCAB_SIZE;
  size_t bytes_per_thread = DEFAULT_BYTES_PER_THREAD;
  int max_num_threads = DEFAULT_MAX_NUM_THREADS;

  // Parse command-line options
  int opt;
  while ((opt = getopt(argc, argv, "i:o:v:b:t:")) != -1) {
    switch (opt) {
    case 'i':
      fileName = optarg;
      break;
    case 'o':
      output_file_name = optarg;
      break;
    case 'v':
      vocab_size = (size_t)atoi(optarg);
      break;
    case 'b':
      bytes_per_thread = (size_t)atol(optarg);
      break;
    case 't':
      max_num_threads = atoi(optarg);
      break;
    default:
      fprintf(stderr,
              "Usage: %s [-i input_file] [-o output_file] [-v vocab_size] [-b "
              "bytes_per_thread] [-t max_threads]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  time(&start);
  tokenizer(fileName, vocab_size, bytes_per_thread, output_file_name,
            max_num_threads);
  time(&end);

  double elapsed = difftime(end, start);
  int hours = elapsed / 3600;
  int minutes = (elapsed - hours * 3600) / 60;
  int seconds = (int)elapsed % 60;
  printf("Execution time for %zu tokens: %d hours, %d minutes, %d seconds\n",
         vocab_size, hours, minutes, seconds);

  /*
   * SHOULD NOT HAVE TO MAKE DIC LIKE YOU DO BELOW HERE USER SHOULD JUST
   * BE ABLE
   * TO CALL A FUNC
   * FUNCTION AND YOU JUST RUN IT HERE
   */

  // DicVocab dic = dicVocab_make_stack(DEFAULT_VOCAB_SIZE);
  // read_vocab(&dic, DEFAULT_OUTPUT_FILE);
  return 0;
}
