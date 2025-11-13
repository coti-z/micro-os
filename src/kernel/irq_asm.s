.code64

/* IRQ 0 (Timer) handler */
.global irq0
irq0:
    /* Save all registers */
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    /* Pass stack pointer to timer_handler (System V ABI: first arg in RDI) */
    mov %rsp, %rdi

    /* Call C handler */
    call timer_handler

    /* Restore registers */
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax

    /* Send EOI (End Of Interrupt) to PIC */
    mov $0x20, %al
    out %al, $0x20

    /* Return from interrupt */
    iretq



  .global irq1
  irq1:
      push %rax
      push %rbx
      push %rcx
      push %rdx
      push %rsi
      push %rdi
      push %rbp
      push %r8
      push %r9
      push %r10
      push %r11
      push %r12
      push %r13
      push %r14
      push %r15

      call keyboard_handler

      pop %r15
      pop %r14
      pop %r13
      pop %r12
      pop %r11
      pop %r10
      pop %r9
      pop %r8
      pop %rbp
      pop %rdi
      pop %rsi
      pop %rdx
      pop %rcx
      pop %rbx
      pop %rax

      mov $0x20, %al
      out %al, $0x20

      iretq

/* IRQ 12 (Mouse) handler */
.global irq12
irq12:
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    call mouse_handler

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax

    /* Send EOI to both PICs (IRQ 12 is on slave PIC) */
    mov $0x20, %al
    out %al, $0xA0      /* EOI to slave PIC */
    out %al, $0x20      /* EOI to master PIC */

    iretq