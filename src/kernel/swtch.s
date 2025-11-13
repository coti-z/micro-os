.code64

# Context switch
#
#   void swtch(context_t *old, context_t *new);
#
# Save current registers in old. Load from new.
# x86-64 calling convention: RDI = first arg, RSI = second arg

.global swtch
swtch:
    # DEBUG: Print return address on stack
    push %rax
    push %rdi
    mov 16(%rsp), %rax    # Get return address (skip 2 pushes)
    mov %rax, %rdi
    # Can't easily call printf from asm, skip for now
    pop %rdi
    pop %rax

    # Save old callee-saved registers
    # We only save callee-saved because caller-saved are already on stack

    mov %rsp, 56(%rdi)    # context.rsp (offset 56)
    mov %rbp, 48(%rdi)    # context.rbp (offset 48)
    mov %rbx, 8(%rdi)     # context.rbx (offset 8)
    mov %r12, 96(%rdi)    # context.r12 (offset 96)
    mov %r13, 104(%rdi)   # context.r13 (offset 104)
    mov %r14, 112(%rdi)   # context.r14 (offset 112)
    mov %r15, 120(%rdi)   # context.r15 (offset 120)

    # Save return address as RIP
    mov (%rsp), %rax
    mov %rax, 128(%rdi)   # context.rip (offset 128)

    # Load new callee-saved registers
    mov 56(%rsi), %rsp    # context.rsp
    mov 48(%rsi), %rbp    # context.rbp
    mov 8(%rsi), %rbx     # context.rbx
    mov 96(%rsi), %r12    # context.r12
    mov 104(%rsi), %r13   # context.r13
    mov 112(%rsi), %r14   # context.r14
    mov 120(%rsi), %r15   # context.r15

    # Jump to new RIP (don't use push+ret, just jmp to maintain stack)
    jmp *128(%rsi)

