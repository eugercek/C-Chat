#include "shared.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *memzero(void *ptr, size_t size) { return memset(ptr, 0, size); }

void error_exit(const char *function_name, const int error_return) {
  // TODO Really need file name ?
  fprintf(stderr, "%s\t%s\n", __FILE__, function_name);
  exit(error_return);
}
