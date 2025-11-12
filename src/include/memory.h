#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

/* Memory allocator */
void memory_init(void);
void *malloc(size_t size);
void free(void *ptr);

/* Heap bounds */
#define HEAP_START 0x100000
#define HEAP_SIZE  0x100000  /* 1 MB */

#endif
