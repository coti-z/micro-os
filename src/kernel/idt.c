#include <stdint.h>
#include "io.h"
#include "exception.h"

/* IDT Entry */
typedef struct {
    uint16_t offset_low;      /* Offset bits 0-15 */
    uint16_t selector;        /* Code segment selector */
    uint8_t ist;              /* IST index (0 for now) */
    uint8_t attributes;       /* Type and attributes */
    uint16_t offset_mid;      /* Offset bits 16-31 */
    uint32_t offset_high;     /* Offset bits 32-63 */
    uint32_t reserved;        /* Reserved */
} idt_entry_t;

/* IDT Descriptor */
typedef struct {
    uint16_t limit;           /* Size of IDT - 1 */
    uint64_t base;            /* Base address of IDT */
} __attribute__((packed)) idt_descriptor_t;

/* IDT (256 entries) */
static idt_entry_t idt[256];

/* Forward declarations for exception handlers (defined in exception_asm.s) */
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);

/* Forward declarations for IRQ handlers (defined in irq_asm.s) */
extern void irq0(void);  /* Timer */
extern void irq1(void);

/* Set IDT entry */
static void idt_set_entry(int index, uint64_t handler, uint8_t type) {
    idt[index].offset_low = handler & 0xFFFF;
    idt[index].offset_mid = (handler >> 16) & 0xFFFF;
    idt[index].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[index].selector = 0x08;  /* Kernel code segment */
    idt[index].ist = 0;
    idt[index].attributes = type;  /* Type and attributes */
    idt[index].reserved = 0;
}

/* Initialize IDT */
void idt_init(void) {
    printf("Initializing IDT...\n");

    /* Set exception handlers */
    idt_set_entry(0, (uint64_t)isr0, 0x8E);   /* Divide by Zero */
    idt_set_entry(1, (uint64_t)isr1, 0x8E);   /* Debug */
    idt_set_entry(2, (uint64_t)isr2, 0x8E);   /* NMI */
    idt_set_entry(3, (uint64_t)isr3, 0x8E);   /* Breakpoint */
    idt_set_entry(4, (uint64_t)isr4, 0x8E);   /* Overflow */
    idt_set_entry(5, (uint64_t)isr5, 0x8E);   /* Bound Range */
    idt_set_entry(6, (uint64_t)isr6, 0x8E);   /* Invalid Opcode */
    idt_set_entry(7, (uint64_t)isr7, 0x8E);   /* Device Not Available */
    idt_set_entry(8, (uint64_t)isr8, 0x8E);   /* Double Fault */
    idt_set_entry(9, (uint64_t)isr9, 0x8E);   /* Coprocessor Segment */
    idt_set_entry(10, (uint64_t)isr10, 0x8E); /* Invalid TSS */
    idt_set_entry(11, (uint64_t)isr11, 0x8E); /* Segment Not Present */
    idt_set_entry(12, (uint64_t)isr12, 0x8E); /* Stack Fault */
    idt_set_entry(13, (uint64_t)isr13, 0x8E); /* General Protection */
    idt_set_entry(14, (uint64_t)isr14, 0x8E); /* Page Fault */
    idt_set_entry(15, (uint64_t)isr15, 0x8E); /* Reserved */
    idt_set_entry(16, (uint64_t)isr16, 0x8E); /* Floating Point */
    idt_set_entry(17, (uint64_t)isr17, 0x8E); /* Alignment Check */
    idt_set_entry(18, (uint64_t)isr18, 0x8E); /* Machine Check */
    idt_set_entry(19, (uint64_t)isr19, 0x8E); /* SIMD Float */

    /* Set IRQ handlers (starting at interrupt 32) */
    idt_set_entry(32, (uint64_t)irq0, 0x8E);  /* Timer (IRQ 0) */
    idt_set_entry(33, (uint64_t)irq1, 0x8E);  /* Keyboard */ 

    /* Load IDT */
    idt_descriptor_t idt_desc;
    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base = (uint64_t)&idt;

    __asm__("lidt %0" : : "m" (idt_desc));

    printf("IDT initialized (256 entries, including IRQ handlers)\n");
}
