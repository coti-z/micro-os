.set ALIGN,    1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1BADB002
.set CHECKSUM,  -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* 64비트 GDT */
.align 16
gdt64:
    .quad 0x0000000000000000                /* 널 세그먼트 */
    .quad 0x00209a0000000000                /* 64비트 코드 세그먼트 */
    .quad 0x0000920000000000                /* 64비트 데이터 세그먼트 */
gdt64_end:

gdt64_descriptor:
    .word gdt64_end - gdt64 - 1
    .quad gdt64

/* 페이지 테이블 - 2MB 페이지 사용 (간단함) */
.align 4096
pml4_table:
    .quad pdp_table + 0x3                   /* Present, Writable */
    .fill 511, 8, 0

.align 4096
pdp_table:
    .quad pd_table + 0x3                    /* Present, Writable */
    .fill 511, 8, 0

.align 4096
pd_table:
    .quad 0x0000000000000183                /* 0MB - 2MB, Present, Writable, Large */
    .quad 0x0000000000200183                /* 2MB - 4MB, Present, Writable, Large */
    .quad 0x0000000000400183                /* 4MB - 6MB, Present, Writable, Large */
    .quad 0x0000000000600183                /* 6MB - 8MB, Present, Writable, Large */
    .fill 508, 8, 0

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.code32
.global _start
.type _start, @function

_start:
    /* 32비트 스택 설정 */
    mov $stack_top, %esp
    mov $0, %ebp

    /* 멀티부트 정보를 스택에 저장 */
    push %ebx                               /* mbi */
    push %eax                               /* magic */

    /* 페이징 비활성화 */
    mov %cr0, %eax
    and $0x7FFFFFFF, %eax
    mov %eax, %cr0

    /* TLB 플러시 */
    xor %eax, %eax
    mov %eax, %cr3

    /* PML4 테이블 주소를 CR3에 로드 */
    lea pml4_table, %eax
    mov %eax, %cr3

    /* CR4 설정 (PAE + PSE 활성화) */
    mov %cr4, %eax
    or $0x20, %eax                          /* PAE (bit 5) */
    or $0x10, %eax                          /* PSE (bit 4) - 2MB 페이지 */
    mov %eax, %cr4

    /* IA32_EFER MSR 설정 (LME 활성화) */
    mov $0xC0000080, %ecx
    rdmsr
    or $0x100, %eax
    wrmsr

    /* 페이징 활성화 */
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0

    /* 64비트 GDT 로드 */
    lgdt gdt64_descriptor

    /* 64비트 코드로 점프 */
    ljmpl $0x8, $start64

.code64
.section .text
start64:
    /* 64비트 데이터 세그먼트 설정 */
    mov $0x10, %eax
    mov %eax, %ds
    mov %eax, %es
    mov %eax, %fs
    mov %eax, %gs
    mov %eax, %ss

    /* 64비트 스택 재설정 */
    lea stack_top, %rsp
    mov $0, %rbp

    /* 스택에서 멀티부트 정보 꺼내기 */
    pop %rdi                                /* magic -> rdi (첫 번째 인자) */
    pop %rsi                                /* mbi -> rsi (두 번째 인자) */

    /* kernel_main 호출 */
    call kernel_main

    /* kernel_main이 반환되면 무한 루프 */
    cli
1:
    hlt
    jmp 1b

.size _start, . - _start
