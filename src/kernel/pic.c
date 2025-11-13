#include "io.h"

/* PIC I/O Ports */
#define PIC1_COMMAND 0x20    /* Master PIC command port */
#define PIC1_DATA 0x21       /* Master PIC data port */
#define PIC2_COMMAND 0xA0    /* Slave PIC command port */
#define PIC2_DATA 0xA1       /* Slave PIC data port */

/* PIC Command Words */
#define ICW1 0x11            /* ICW1: Initialize */
#define ICW4 0x01            /* ICW4: 8086 mode */

/* Initialize PIC (8259A Programmable Interrupt Controller) */
void pic_init(void) {
    printf("Initializing PIC...\n");

    /* ICW1: Initialize both PICs */
    __outb(PIC1_COMMAND, ICW1);
    __outb(PIC2_COMMAND, ICW1);

    /* ICW2: Set interrupt vector offset
     * Master PIC handles IRQ0-7 (mapped to interrupt 32-39)
     * Slave PIC handles IRQ8-15 (mapped to interrupt 40-47)
     */
    __outb(PIC1_DATA, 32);   /* Master offset: 32 */
    __outb(PIC2_DATA, 40);   /* Slave offset: 40 */

    /* ICW3: Configure cascading
     * Master: IRQ2 is connected to Slave
     * Slave: Connected to Master IRQ2
     */
    __outb(PIC1_DATA, 0x04); /* Master: IRQ2 has slave */
    __outb(PIC2_DATA, 0x02); /* Slave: Connected to Master IRQ2 */

    /* ICW4: 8086 mode */
    __outb(PIC1_DATA, ICW4);
    __outb(PIC2_DATA, ICW4);

    /* OCW1: Mask interrupts (allow IRQ0, IRQ1, IRQ2, and IRQ12) */
    __outb(PIC1_DATA, 0xF8); /* 11111000 - allow IRQ0, IRQ1, IRQ2 (cascade), mask others */
    __outb(PIC2_DATA, 0xEF); /* 11101111 - allow IRQ12 (bit 4), mask others */

    printf("PIC initialized\n");
}
