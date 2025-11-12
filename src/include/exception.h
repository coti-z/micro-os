#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <stdint.h>

/* Exception frame pushed by CPU during interrupt */
typedef struct {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi;
    uint64_t rbp, r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} exception_frame_t;

/* Exception handler */
void exception_handler(int exception_num, exception_frame_t *frame);

#endif /* _EXCEPTION_H_ */
