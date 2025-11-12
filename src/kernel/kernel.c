#include <stdint.h>
#include "io.h"
#include "exception.h"
#include "timer.h"

/* External initializations */
extern void idt_init(void);
extern void pic_init(void);
extern void keyboard_init(void);

void kernel_main(uint64_t magic, uint64_t addr) {
    /* Initialize serial port for debugging */
    serial_init();

    printf("\n====================================\n");
    printf("  64-bit Kernel Started!\n");
    printf("====================================\n");
    printf("Magic: 0x%x\n", magic);
    printf("Multiboot Info: 0x%p\n", (void *)addr);

    /* Initialize interrupt descriptor table */
    printf("\nSetting up exception handlers...\n");
    idt_init();

    /* Initialize PIC (Programmable Interrupt Controller) */
    printf("Setting up interrupt controller...\n");
    pic_init();

    /* Initialize timer */
    printf("Setting up timer...\n");
    timer_init(11932);  /* ~100 Hz (1193182 / 11932 ≈ 100) */

    /* Initialize keyboard */
    printf("Setting up keyboard...\n");
    keyboard_init();

    /* VGA 메모리에 직접 접근 */
    uint16_t *vga = (uint16_t *)0xB8000;

    /* 스크린 클리어 */
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = (0x0F << 8) | ' ';
    }

    /* 문자 출력 */
    const char *str = "Hello from 64-bit kernel!";
    for (int i = 0; str[i]; i++) {
        vga[i] = (0x0F << 8) | str[i];
    }

    printf("\nKernel is ready.\n");
    printf("Enabling interrupts...\n");

    /* Enable interrupts */
    __asm__("sti");

    printf("Waiting for timer interrupts...\n");

    /* 무한 루프 */
    while (1) {
        __asm__("hlt");
    }
}
