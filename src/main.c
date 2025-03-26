#include "Tokenizer.h"
int main() {
  time_t start, end;
  char *fileName = "../data/infiniteJest.txt";
  char *output_file_name = "../data/myTokens.txt";
  const size_t vocab_size = 10000;
  const size_t bytes_per_thread = 500000;
  time(&start);
  tokenizer(fileName, vocab_size, bytes_per_thread, output_file_name);
  time(&end);
  double elapsed = difftime(end, start);
  int hours = elapsed / 3600;
  int minutes = (elapsed - hours * 3600) / 60;
  int seconds = (int)elapsed % 60;
  printf("Execution time for %zu tokens: %d hours, %d minutes, %d seconds\n",
         vocab_size, hours, minutes, seconds);

  return 0;
}
