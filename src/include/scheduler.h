#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

/* Scheduler's own context (separate from any process) */
extern context_t scheduler_context;

/* Start scheduler loop (never returns) */
void scheduler(void) __attribute__((noreturn));

/* Switch to scheduler (called by yield/sleep) */
void sched(void);

/* Give up the CPU for one scheduling round */
void yield(void);

/* Get current running process */
process_t *myproc(void);

/* First-time process entry wrapper (releases scheduler lock) */
void forkret(void);

/* Context switch (defined in swtch.s) */
void swtch(context_t *old, context_t *new);

#endif
