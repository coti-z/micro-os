.code64

/* Macro to create ISR stub without error code */
.macro ISR_NO_ERROR num
.global isr\num
isr\num:
    /* CPU doesn't push error code for this exception */
    /* Push dummy error code to align stack */
    push $0
    /* Push exception number */
    push $\num
    jmp common_handler
.endm

/* Macro to create ISR stub with error code */
.macro ISR_WITH_ERROR num
.global isr\num
isr\num:
    /* CPU already pushed error code */
    /* Push exception number (error code is already on stack) */
    push $\num
    jmp common_handler
.endm

/* Exceptions without error codes */
ISR_NO_ERROR 0
ISR_NO_ERROR 1
ISR_NO_ERROR 2
ISR_NO_ERROR 3
ISR_NO_ERROR 4
ISR_NO_ERROR 5
ISR_NO_ERROR 6
ISR_NO_ERROR 7
ISR_WITH_ERROR 8
ISR_NO_ERROR 9
ISR_WITH_ERROR 10
ISR_WITH_ERROR 11
ISR_WITH_ERROR 12
ISR_WITH_ERROR 13
ISR_WITH_ERROR 14
ISR_NO_ERROR 15
ISR_NO_ERROR 16
ISR_WITH_ERROR 17
ISR_NO_ERROR 18
ISR_NO_ERROR 19

/* Common handler */
common_handler:
    /* Save registers */
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rbp
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx
    push %rax

    /* Stack layout at this point:
     *  0(%rsp) = rax
     *  8(%rsp) = rbx
     * 16(%rsp) = rcx
     * 24(%rsp) = rdx
     * 32(%rsp) = rsi
     * 40(%rsp) = rdi
     * 48(%rsp) = rbp
     * 56(%rsp) = r8
     * 64(%rsp) = r9
     * 72(%rsp) = r10
     * 80(%rsp) = r11
     * 88(%rsp) = r12
     * 96(%rsp) = r13
     * 104(%rsp) = r14
     * 112(%rsp) = r15
     * 120(%rsp) = error code (from ISR macro)
     * 128(%rsp) = exception number (from ISR macro)
     */

    /* Prepare arguments for C handler */
    mov 128(%rsp), %rdi     /* exception number (arg 1) */
    mov %rsp, %rsi          /* frame pointer (arg 2) */

    /* Call C exception handler */
    call exception_handler

    /* Should not return, but cleanup just in case */
    add $16, %rsp
    iretq
