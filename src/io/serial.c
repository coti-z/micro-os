#include <stdint.h>
#include "io.h"

/* Serial port I/O */
#define SERIAL_PORT 0x3F8

/* Serial port registers */
#define SERIAL_DATA 0
#define SERIAL_IER 1
#define SERIAL_FCR 2
#define SERIAL_LCR 3
#define SERIAL_MCR 4
#define SERIAL_LSR 5
#define SERIAL_MSR 6

/* Initialize serial port for debugging */
void serial_init(void) {
    /* Disable interrupts */
    __outb(SERIAL_PORT + SERIAL_IER, 0x00);

    /* Enable DLAB (Divisor Latch Access Bit) */
    __outb(SERIAL_PORT + SERIAL_LCR, 0x80);

    /* Set baud rate to 115200 (divisor = 1) */
    __outb(SERIAL_PORT + 0, 0x01);  /* DLL */
    __outb(SERIAL_PORT + 1, 0x00);  /* DLM */

    /* 8 bits, 1 stop bit, no parity */
    __outb(SERIAL_PORT + SERIAL_LCR, 0x03);

    /* Enable FIFO */
    __outb(SERIAL_PORT + SERIAL_FCR, 0xC7);

    /* Set modem control signals */
    __outb(SERIAL_PORT + SERIAL_MCR, 0x0B);
}

/* Check if serial port is ready to transmit */
static int serial_is_ready(void) {
    uint8_t status = __inb(SERIAL_PORT + SERIAL_LSR);
    return (status & 0x20) != 0;
}

/* Write single character to serial port */
void serial_putchar(char c) {
    /* Wait for transmitter to be ready */
    while (!serial_is_ready());

    /* Send character */
    __outb(SERIAL_PORT + SERIAL_DATA, (uint8_t)c);
}

/* Write string to serial port */
void serial_write(const char *str) {
    while (*str) {
        if (*str == '\n') {
            serial_putchar('\r');
        }
        serial_putchar(*str);
        str++;
    }
}
