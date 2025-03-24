#pragma once
#include "Dic.h"
#include "StrArr.h"
#include <unistd.h>

size_t get_num_threads();

size_t findSize(const char *fileName);

void swap_arrs(StrArr **l, StrArr **r);
void swap_pools(CPool **l, CPool **r);
