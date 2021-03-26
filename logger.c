#include "logger.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Logger *NewLogger() {
  Logger *object = malloc(sizeof(Logger));

  // Fields
  strcpy(object->file_name, "log.log");
  object->fptr = fopen(object->file_name, "a");

  // Methods
  object->log_line = log_line;
  object->free_self = free_self;
  object->mutex_lock = mutex_lock;
  object->mutex_unlock = mutex_unlock;
  return object;
}

void log_line(void *self, const char *format, ...) {
  va_list args;
  fprintf(((Logger *)self)->fptr, "%s\t%s\t", __DATE__, __TIME__);
  va_start(args, format);
  vfprintf(((Logger *)self)->fptr, format, args);
  va_end(args);
  fprintf(((Logger *)self)->fptr, "\n");
  fflush(((Logger *)self)->fptr);
}

void free_self(void *self) {
  pthread_mutex_destroy(&((Logger *)self)->mutex);
  free(self);
}

void mutex_lock(void *self) { pthread_mutex_lock(&((Logger *)self)->mutex); }

void mutex_unlock(void *self) {
  pthread_mutex_unlock(&((Logger *)self)->mutex);
}
