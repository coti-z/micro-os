#include <io.h>
#include "scheduler.h"
#include "timer.h"

static inline uint64_t read_rflags(void) {
    uint64_t rflags;
    __asm__ volatile("pushf; pop %0" : "=r"(rflags));
    return rflags;
}

/* Process 2 - print B */
void idle_task_2(void) {
    int counter = 0;
    while (1) {
        counter++;
        if (counter % 10000000 == 0) {
            uint64_t rflags = read_rflags();
            printf("Task 2 running %d, RFLAGS=0x%x, IF=%d, timer_ticks=%d\n",
                   counter, (int)rflags, (int)((rflags >> 9) & 1), (int)timer_ticks);
            yield();  /* Cooperative scheduling */
        }
    }
}
