#pragma once
#include "Dic.h"
#include "StrArr.h"
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#ifdef __linux__
#include <sys/sysinfo.h>
#endif
#endif

size_t get_num_threads();

size_t findSize(const char *fileName);

size_t get_ram();
