#include "timer.h"
#include "io.h"

/* Global timer tick counter */
volatile uint64_t timer_ticks = 0;

/* PIT I/O Ports */
#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_CONTROL 0x43

/* PIT Control Word Format:
 * Bits 7-6: Channel Select (00 = Channel 0)
 * Bits 5-4: Access Mode (11 = low byte then high byte)
 * Bits 3-1: Operating Mode (010 = Rate Generator)
 * Bit 0: Binary/BCD (0 = binary)
 */
#define PIT_CMD_CHANNEL0 0x34  /* Channel 0, both bytes, Mode 2 */

/* Initialize PIT for timer interrupts */
void timer_init(uint16_t divisor) {
    printf("Initializing PIT timer (divisor: %d)...\n", divisor);

    /* Send control command to PIT */
    __outb(PIT_CONTROL, PIT_CMD_CHANNEL0);

    /* Send divisor (low byte first, then high byte) */
    __outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));        /* Low byte */
    __outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF)); /* High byte */

    printf("PIT initialized (frequency: ~%d Hz)\n", 1193182 / divisor);
}

/* Timer interrupt handler */
void timer_handler(void) {
    timer_ticks++;

    /* Print every 1000 ticks (roughly every second at ~1000 Hz) */
    if (timer_ticks % 1000 == 0) {
        printf("Timer: %d seconds elapsed\n", timer_ticks / 1000);
    }
}
