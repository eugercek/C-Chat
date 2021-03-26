#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <pthread.h>
#include <stdio.h>

typedef struct {
  FILE *fptr;
  char file_name[32]; // No need for choose 32
  pthread_mutex_t mutex;

  void (*log_line)(void *self, const char *str);
  void (*free_self)(void *self);
  void (*mutex_lock)(void *self);
  void (*mutex_unlock)(void *self);
} Logger;

// Constructor
Logger *NewLogger();

// Destructor
void free_self(void *self);

// For function pointers
void log_line(void *self, const char *str);

void mutex_lock(void *self);

void mutex_unlock(void *self);

#endif // __LOGGER_H_
