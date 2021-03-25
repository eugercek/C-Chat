#ifndef __SHARED_H_
#define __SHARED_H_

#include <stddef.h> // size_t

// Void pointer is for return value for chaining
void *memzero(void *ptr, const size_t size);

void error_exit(const char *function_name, const int error_return);

char *hour_minute();

void remove_newline(char *string);
#endif // __SHARED_H_
