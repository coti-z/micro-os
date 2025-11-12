#include "io.h"
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

/* Simple printf implementation for debugging */

static void print_number(uint64_t num, int base, int width, char pad) {
    char buffer[32];
    int i = 0;

    if (num == 0) {
        buffer[i++] = '0';
    } else {
        while (num > 0) {
            int digit = num % base;
            if (digit < 10) {
                buffer[i++] = '0' + digit;
            } else {
                buffer[i++] = 'A' + (digit - 10);
            }
            num /= base;
        }
    }

    /* Pad with characters if needed */
    while (i < width) {
        buffer[i++] = pad;
    }

    /* Reverse and print */
    while (i > 0) {
        serial_putchar(buffer[--i]);
    }
}

static void print_signed(int64_t num, int width) {
    if (num < 0) {
        serial_putchar('-');
        width--;
        num = -num;
    }
    print_number((uint64_t)num, 10, width, ' ');
}

int printf(const char *format, ...) {
    va_list ap;
    int count = 0;

    va_start(ap, format);

    while (*format) {
        if (*format == '%') {
            format++;

            /* Parse width and padding */
            int width = 0;
            char pad = ' ';

            if (*format == '0') {
                pad = '0';
                format++;
            }

            while (*format >= '0' && *format <= '9') {
                width = width * 10 + (*format - '0');
                format++;
            }

            /* Handle format specifier */
            switch (*format) {
                case 'd':
                case 'i': {
                    int64_t val = va_arg(ap, int64_t);
                    print_signed(val, width);
                    break;
                }
                case 'u': {
                    uint64_t val = va_arg(ap, uint64_t);
                    print_number(val, 10, width, pad);
                    break;
                }
                case 'x': {
                    uint64_t val = va_arg(ap, uint64_t);
                    print_number(val, 16, width, pad);
                    break;
                }
                case 'X': {
                    uint64_t val = va_arg(ap, uint64_t);
                    print_number(val, 16, width, pad);
                    break;
                }
                case 'p': {
                    uint64_t val = va_arg(ap, uint64_t);
                    serial_write("0x");
                    print_number(val, 16, 0, ' ');
                    break;
                }
                case 's': {
                    const char *str = va_arg(ap, const char *);
                    if (str == NULL) str = "(null)";
                    serial_write(str);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(ap, int);
                    serial_putchar(c);
                    break;
                }
                case '%':
                    serial_putchar('%');
                    break;
                default:
                    serial_putchar('?');
                    break;
            }
            format++;
        } else {
            if (*format == '\n') {
                serial_putchar('\r');
            }
            serial_putchar(*format);
            format++;
        }
        count++;
    }

    va_end(ap);
    return count;
}
