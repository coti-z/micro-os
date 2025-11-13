#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

/* Type definitions */
typedef unsigned int uint;

/* Memory allocator */
void memory_init(void);
void *malloc(uint nbytes);
void free(void *ap);

/* Memory utilities */
void *memset(void *s, int c, size_t n);

/* Heap bounds */
#define HEAP_START 0x100000
#define HEAP_SIZE  0x100000  /* 1 MB */

#endif
