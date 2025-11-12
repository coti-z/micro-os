#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

/* Port I/O functions */
static inline uint8_t __inb(uint16_t port) {
    uint8_t value;
    __asm__("inb %1, %0" : "=a" (value) : "dN" (port));
    return value;
}

static inline void __outb(uint16_t port, uint8_t value) {
    __asm__("outb %0, %1" : : "a" (value), "dN" (port));
}

/* Serial output functions */
void serial_init(void);
void serial_putchar(char c);
void serial_write(const char *str);

/* Printf for debugging */
int printf(const char *format, ...);

/* Panic and assert */
void panic(const char *message);
void assert_fail(const char *expr, const char *file, int line);
void assert_fail_msg(const char *expr, const char *file, int line, const char *msg);

#define ASSERT(expr) \
    do { \
        if (!(expr)) { \
            assert_fail(#expr, __FILE__, __LINE__); \
        } \
    } while (0)

#define ASSERT_MSG(expr, msg) \
    do { \
        if (!(expr)) { \
            assert_fail_msg(#expr, __FILE__, __LINE__, msg); \
        } \
    } while (0)

#endif /* _IO_H_ */
