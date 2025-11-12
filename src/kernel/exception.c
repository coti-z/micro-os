#include "exception.h"
#include "io.h"

/* Exception names */
static const char *exception_names[] = {
    [0] = "Divide by Zero",
    [1] = "Debug",
    [2] = "NMI Interrupt",
    [3] = "Breakpoint",
    [4] = "Overflow",
    [5] = "Bound Range Exceeded",
    [6] = "Invalid Opcode",
    [7] = "Device Not Available",
    [8] = "Double Fault",
    [9] = "Coprocessor Segment Overrun",
    [10] = "Invalid TSS",
    [11] = "Segment Not Present",
    [12] = "Stack-Segment Fault",
    [13] = "General Protection Fault",
    [14] = "Page Fault",
    [15] = "Reserved",
    [16] = "x87 Floating-Point Exception",
    [17] = "Alignment Check",
    [18] = "Machine Check",
    [19] = "SIMD Floating-Point Exception",
};

/* Get CR2 (page fault address) */
static uint64_t get_cr2(void) {
    uint64_t cr2;
    __asm__("mov %%cr2, %0" : "=r" (cr2));
    return cr2;
}

/* Exception handler */
void exception_handler(int exception_num, exception_frame_t *frame) {
    printf("\n");
    printf("========================================\n");
    printf("            EXCEPTION OCCURRED\n");
    printf("========================================\n");

    /* Print exception name */
    if (exception_num >= 0 && exception_num < 20) {
        printf("Exception: %s (0x%x)\n", exception_names[exception_num], exception_num);
    } else {
        printf("Exception: Unknown (0x%x)\n", exception_num);
    }

    /* Print error code if applicable */
    if (exception_num == 8 || exception_num == 10 || exception_num == 11 ||
        exception_num == 12 || exception_num == 13 || exception_num == 14 || exception_num == 17) {
        printf("Error Code: 0x%x\n", frame->error_code);
    }

    /* Print page fault address */
    if (exception_num == 14) {
        printf("Faulting Address: 0x%p\n", (void *)get_cr2());
    }

    /* Print registers */
    printf("\nRegisters:\n");
    printf("  RAX: 0x%p  RBX: 0x%p\n", (void *)frame->rax, (void *)frame->rbx);
    printf("  RCX: 0x%p  RDX: 0x%p\n", (void *)frame->rcx, (void *)frame->rdx);
    printf("  RSI: 0x%p  RDI: 0x%p\n", (void *)frame->rsi, (void *)frame->rdi);
    printf("  R8:  0x%p  R9:  0x%p\n", (void *)frame->r8, (void *)frame->r9);
    printf("  R10: 0x%p  R11: 0x%p\n", (void *)frame->r10, (void *)frame->r11);
    printf("  R12: 0x%p  R13: 0x%p\n", (void *)frame->r12, (void *)frame->r13);
    printf("  R14: 0x%p  R15: 0x%p\n", (void *)frame->r14, (void *)frame->r15);
    printf("\nStack/Frame:\n");
    printf("  RBP: 0x%p  RSP: 0x%p\n", (void *)frame->rbp, (void *)frame->rsp);
    printf("  RIP: 0x%p  CS:  0x%x\n", (void *)frame->rip, (uint32_t)frame->cs);
    printf("  RFLAGS: 0x%p\n", (void *)frame->rflags);

    printf("\n========================================\n");
    printf("         SYSTEM HALTED\n");
    printf("========================================\n");

    /* Halt */
    while (1) {
        __asm__("cli");
        __asm__("hlt");
    }
}
