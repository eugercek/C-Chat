#include "shared.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void *memzero(void *ptr, size_t size) { return memset(ptr, 0, size); }

void error_exit(const char *function_name, const int error_return) {
  fprintf(stderr, "%s\t%s\n", function_name, strerror(errno));
  exit(error_return);
}

char *hour_minute() {
  time_t now = time(NULL);
  struct tm *info = localtime(&now);
  // TODO Find better size for hour_minute malloc
  char *ret = malloc(sizeof(char) * 10);
  strftime(ret, 10, "%H:%M", info);
  return ret;
}

void remove_newline(char *string) {
  if (string == NULL) {
    fprintf(stderr, "remove_newline: string was NULL");
    return;
  }
  string[strlen(string) - 1] = 0;
}
