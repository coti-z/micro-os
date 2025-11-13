#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdint.h>

/* Spinlock for mutual exclusion */
typedef struct {
    volatile uint32_t locked;  /* Is the lock held? */
    char name[32];             /* Name of lock (for debugging) */
} spinlock_t;

/* Initialize a spinlock */
void spinlock_init(spinlock_t *lock, const char *name);

/* Acquire the lock */
void acquire(spinlock_t *lock);

/* Release the lock */
void release(spinlock_t *lock);

/* Check if this CPU is holding the lock */
int holding(spinlock_t *lock);

#endif
