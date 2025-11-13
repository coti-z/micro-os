#include "spinlock.h"
#include "io.h"

/* Initialize a spinlock */
void spinlock_init(spinlock_t *lock, const char *name) {
    lock->locked = 0;

    /* Copy name */
    int i;
    for (i = 0; name[i] && i < 31; i++) {
        lock->name[i] = name[i];
    }
    lock->name[i] = '\0';
}

/* Acquire the lock.
 * Loops (spins) until the lock is acquired.
 * Holding a lock for a long time may cause other CPUs to waste time spinning.
 */
void acquire(spinlock_t *lock) {
    /* Disable interrupts to avoid deadlock */
    __asm__ volatile("cli");

    /* The xchg is atomic.
     * It also serializes, so that reads after acquire are not
     * reordered before it.
     */
    while (__sync_lock_test_and_set(&lock->locked, 1) != 0) {
        /* Spin (busy wait) */
        __asm__ volatile("pause");
    }

    /* Tell the compiler and the processor to not move loads or stores
     * past this point, to ensure that critical section's memory
     * references happen after the lock is acquired.
     */
    __sync_synchronize();
}

/* Release the lock */
void release(spinlock_t *lock) {
    /* Tell the compiler and the processor to not move loads or stores
     * past this point, to ensure that all the stores in the critical
     * section are visible to other cores before the lock is released.
     */
    __sync_synchronize();

    /* Release the lock */
    __sync_lock_release(&lock->locked);

    /* Re-enable interrupts */
    __asm__ volatile("sti");
}

/* Check whether this CPU is holding the lock.
 * Interrupts must be off.
 */
int holding(spinlock_t *lock) {
    return lock->locked;
}
