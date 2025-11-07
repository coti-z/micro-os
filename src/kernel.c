#include <stdint.h>

void kernel_main(uint64_t magic, uint64_t addr) {
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

    /* 무한 루프 */
    while (1) {
        __asm__("hlt");
    }
}
