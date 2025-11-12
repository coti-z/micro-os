#include "io.h"
#include <stdint.h>

/* Panic - print error message and halt */
void panic(const char *message) {
    printf("\n========== KERNEL PANIC ==========\n");
    printf("%s\n", message);
    printf("===================================\n");

    /* Halt CPU */
    while (1) {
        __asm__("cli");  /* Disable interrupts */
        __asm__("hlt");  /* Halt */
    }
}

/* Assert - panic if condition is false */
void assert_fail(const char *expr, const char *file, int line) {
    printf("\n========== ASSERTION FAILED ==========\n");
    printf("Expression: %s\n", expr);
    printf("File: %s\n", file);
    printf("Line: %d\n", line);
    printf("========================================\n");

    /* Halt CPU */
    while (1) {
        __asm__("cli");
        __asm__("hlt");
    }
}

void assert_fail_msg(const char *expr, const char *file, int line, const char *msg) {
    printf("\n========== ASSERTION FAILED ==========\n");
    printf("Expression: %s\n", expr);
    printf("File: %s\n", file);
    printf("Line: %d\n", line);
    printf("Message: %s\n", msg);
    printf("========================================\n");

    /* Halt CPU */
    while (1) {
        __asm__("cli");
        __asm__("hlt");
    }
}
