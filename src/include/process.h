#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESSES 64
#define PROCESS_STACK_SIZE 4096  /* 4KB stack per process */

/* Process states */
typedef enum {
    PROCESS_UNUSED,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED
} process_state_t;

/* CPU context for context switching (will be used in later stages) */
typedef struct {
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rsi, rdi, rbp, rsp;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rip;
    uint64_t rflags;
} context_t;

/* Process control block */
typedef struct {
    int pid;
    char name[32];
    process_state_t state;
    void *stack;                  /* Allocated stack */
    context_t context;            /* For future context switching */
    void (*entry_point)(void);    /* Function pointer to process entry */
} process_t;

/* Process management functions */
void process_init(void);
process_t *process_create(const char *name, void (*entry)(void));
process_t *process_get(int pid);

#endif
