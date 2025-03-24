#include "Helper_fucs.h"
#include "Dic.h"
#include "StrArr.h"
#include <string.h>

size_t get_num_threads() {
  long cores = sysconf(_SC_NPROCESSORS_ONLN);
  long usable_threads =
      (cores > 2) ? cores - 1 : 1; // Leave at least 1 core free
  return usable_threads;
}

size_t findSize(const char *fileName) {
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) {
    perror("ERROR ON YOUR END DIDN'T MAKE FILE PROPERLY\n");
    exit(1);
  }
  fseek(fp, 0L, SEEK_END);

  // calculating the size of the file
  size_t res = ftell(fp);
  fclose(fp);
  return res;
}

// megabytes of ram free
size_t get_ram() {
#ifdef _WIN32
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  if (GlobalMemoryStatusEx(&statex)) {
    return (statex.ullAvailPhys / 1024 / 1024) * .5;
  } else {
    perror("COULD NOT RETRIEVE RAM INFO");
    exit(1);
  }

#elif defined(__linux__)
  struct sysinfo info;
  if (sysinfo(&info) == 0) {
    return (info.totalram / 1024 / 1024) * .5;
  } else {
    perror("COULD NOT RETRIEVE RAM INFO");
    exit(1);
  }

#elif defined(__APPLE__)
  int mib[2] = {CTL_HW, HW_MEMSIZE};
  unsigned long long mem;
  size_t len = sizeof(mem);
  if (sysctl(mib, 2, &mem, &len, NULL, 0) == 0) {
    return (mem / 1024 / 1024) * .5;
  } else {
    perror("COULD NOT RETRIEVE RAM INFO");
    exit(1);
  }

#else
  perror("Unsupported OS");
  exit(1);
#endif
}
