#include <stdint.h>
#include "io.h"
#include "exception.h"
#include "timer.h"
#include "process.h"
#include "scheduler.h"

/* External initializations */
extern void idt_init(void);
extern void pic_init(void);
extern void keyboard_init(void);
extern void memory_init(void);

/* External process tasks */
extern void idle_task_1(void);
extern void idle_task_2(void);

void kernel_main(uint64_t magic, uint64_t addr) {
    /* Initialize serial port for debugging */
    serial_init();

    printf("\n====================================\n");
    printf("  64-bit Kernel Started!\n");
    printf("====================================\n");
    printf("Magic: 0x%x\n", magic);
    printf("Multiboot Info: 0x%p\n", (void *)addr);

    /* Initialize interrupt descriptor table */
    printf("\nSetting up exception handlers...\n");
    idt_init();

    /* Initialize PIC (Programmable Interrupt Controller) */
    printf("Setting up interrupt controller...\n");
    pic_init();

    /* Initialize timer */
    printf("Setting up timer...\n");
    timer_init(11932);  /* ~100 Hz (1193182 / 11932 ≈ 100) */

    /* Initialize keyboard */
    printf("Setting up keyboard...\n");
    keyboard_init();

    /* Initialize memory allocator */
    printf("Setting up memory allocator...\n");
    memory_init();

    /* Initialize process management */
    printf("\nSetting up process management...\n");
    process_init();

    /* Create test processes */
    printf("\nCreating test processes...\n");
    process_t *proc1 = process_create("idle_task_1", idle_task_1);
    if (proc1) {
        printf("✓ Process 1: pid=%d, name=%s\n", proc1->pid, proc1->name);
    }

    process_t *proc2 = process_create("idle_task_2", idle_task_2);
    if (proc2) {
        printf("✓ Process 2: pid=%d, name=%s\n", proc2->pid, proc2->name);
    }

    printf("\n====================================\n");
    printf("  Kernel initialization complete!\n");
    printf("  Starting scheduler...\n");
    printf("====================================\n");

    /* Enable interrupts */
    __asm__("sti");


    while(1) {
        __asm__("hlt");
    }
    /* Should never reach here */
    panic("scheduler returned!");
}
