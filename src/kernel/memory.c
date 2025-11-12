#include "memory.h"
#include "io.h"

/* Block header */
typedef struct block {
    size_t size;        /* Data size (excluding header) */
    int is_free;        /* 1 = free, 0 = allocated */
    struct block *next; /* Next block in list */
} block_t;

/* Heap metadata */
static block_t *heap_start = NULL;
static char *heap_end = NULL;

/* Initialize heap */
void memory_init(void) {
    printf("Initializing memory allocator...\n");

    /* Heap starts at HEAP_START */
    heap_start = (block_t *)HEAP_START;
    heap_end = (char *)HEAP_START + HEAP_SIZE;

    /* Create initial free block covering entire heap */
    heap_start->size = HEAP_SIZE - sizeof(block_t);
    heap_start->is_free = 1;
    heap_start->next = NULL;

    printf("Heap initialized: 0x%p - 0x%p (%d KB)\n",
           (void *)HEAP_START,
           (void *)(HEAP_START + HEAP_SIZE),
           HEAP_SIZE / 1024);
}

/* Allocate memory */
void *malloc(size_t size) {
    if (size == 0) return NULL;

    block_t *current = heap_start;

    /* Find first free block large enough */
    while (current) {
        if (current->is_free && current->size >= size) {
            /* Split block if it's larger than needed */
            if (current->size > size + sizeof(block_t)) {
                block_t *new_block = (block_t *)((char *)current + sizeof(block_t) + size);
                new_block->size = current->size - size - sizeof(block_t);
                new_block->is_free = 1;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            current->is_free = 0;
            return (void *)((char *)current + sizeof(block_t));
        }
        current = current->next;
    }

    printf("malloc failed: no memory (size=%d)\n", (int)size);
    return NULL;
}

/* Free memory */
void free(void *ptr) {
    if (ptr == NULL) return;

    /* Get block header (located before data) */
    block_t *block = (block_t *)ptr - 1;
    block->is_free = 1;

    /* Simple coalescing: merge with next free block */
    if (block->next && block->next->is_free) {
        block->size += sizeof(block_t) + block->next->size;
        block->next = block->next->next;
    }
}
