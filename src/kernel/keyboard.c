#include <keyboard.h>

#include <io.h>
#include <stdint.h>

#define KEYBOARD_PORT 0x60
#define KEYBOARD_STATUS 0x64

volatile uint8_t keyboard_buffer[256];
volatile uint16_t keyboard_head = 0;
volatile uint16_t keyboard_tail = 0;

void keyboard_handler(void) {
    uint8_t scancode = __inb(KEYBOARD_PORT);

    keyboard_buffer[keyboard_head] = scancode;
    keyboard_head = (keyboard_head + 1) % 256;

    printf("key pressed 0x%x\n", scancode);
}


void keyboard_init(void) {
      printf("Initializing keyboard...\n");
}