#include "shared.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void *memzero(void *ptr, size_t size) { return memset(ptr, 0, size); }
