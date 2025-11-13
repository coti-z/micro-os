#include "scheduler.h"
#include "process.h"
#include "io.h"
#include <stddef.h>

/* Scheduler's own context (separate from any process) */
context_t scheduler_context;

/* Current running process (per-CPU, but we have 1 CPU) */
static process_t *current_process = NULL;

/* Get current running process */
process_t *myproc(void) {
    return current_process;
}

/* A fork child's very first scheduling by scheduler()
 * will swtch here. "Return" to user space by jumping to the
 * process entry point.
 */
void forkret(void) {
    process_t *p = myproc();

    printf("forkret: starting %s (pid=%d)\n", p->name, p->pid);

    /* Enable interrupts for this process */
    __asm__ volatile("sti");

    uint64_t rflags;
    __asm__ volatile("pushf; pop %0" : "=r"(rflags));
    printf("forkret: after sti, RFLAGS=0x%x, IF=%d\n",
           (int)rflags, (int)((rflags >> 9) & 1));

    /* Jump to process entry point */
    p->entry_point();

    /* Process should never return */
    panic("process returned");
}

/* Per-CPU process scheduler.
 * Each CPU calls scheduler() after setting itself up.
 * Scheduler never returns. It loops, doing:
 *  - choose a process to run.
 *  - swtch to start running that process.
 *  - eventually that process transfers control
 *    via swtch back to the scheduler.
 */
void scheduler(void) {
    process_t *p;

    printf("Scheduler: entering main loop\n");

    for (;;) {
        /* Avoid deadlock by ensuring interrupts are on */
        __asm__ volatile("sti");

        int found = 0;

        /* Loop over process table looking for process to run */
        for (int i = 0; i < MAX_PROCESSES; i++) {
            p = process_get_by_index(i);
            if (!p) continue;

            if (p->state == PROCESS_READY) {
                /* Switch to chosen process */
                p->state = PROCESS_RUNNING;
                current_process = p;

                printf("Scheduler: switching to %s (pid=%d), rip=0x%p, rsp=0x%p, stack=0x%p\n",
                       p->name, p->pid, (void*)p->context.rip, (void*)p->context.rsp, p->stack);

                swtch(&scheduler_context, &p->context);

                printf("Scheduler: returned from %s (pid=%d)\n", p->name, p->pid);

                /* Process is done running for now.
                 * It should have changed its p->state before coming back. */
                current_process = NULL;
                found = 1;
            }
        }

        if (found == 0) {
            /* No runnable processes; halt until interrupt */
            __asm__ volatile("hlt");
        }
    }
}

/* Switch to scheduler.
 * Must have changed proc->state.
 */
void sched(void) {
    process_t *p = myproc();

    if (!p) {
        panic("sched: no process");
    }

    if (p->state == PROCESS_RUNNING) {
        panic("sched: process still running");
    }

    printf("sched: %s (pid=%d) yielding, context.rip=0x%p\n",
           p->name, p->pid, (void*)p->context.rip);

    /* Switch to scheduler */
    swtch(&p->context, &scheduler_context);

    printf("sched: %s (pid=%d) resumed, context.rip=0x%p\n",
           p->name, p->pid, (void*)p->context.rip);
}

/* Give up the CPU for one scheduling round. */
void yield(void) {
    process_t *p = myproc();

    if (!p) {
        return;  /* No process running */
    }

    p->state = PROCESS_READY;
    sched();
}
