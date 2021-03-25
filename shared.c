#include "shared.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void *memzero(void *ptr, size_t size) { return memset(ptr, 0, size); }

void error_exit(const char *function_name, const int error_return) {
  fprintf(stderr, "%s\n", function_name);
  exit(error_return);
}

char *hour_minute() {
  time_t now = time(NULL);
  struct tm *time = localtime(&now);
  // TODO Find better size for hour_minute malloc
  char *ret = malloc(sizeof(char) * 10);
  snprintf(ret, 10, "%d:%d", time->tm_hour, time->tm_min);
  return ret;
}
