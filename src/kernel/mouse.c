#include "mouse.h"
#include "io.h"

#define MOUSE_PORT 0x60

/* Mouse state */
static mouse_state_t mouse_state = {0, 0, 0};

/* Initialize PS/2 mouse */
void mouse_init(void) {
    printf("Initializing PS/2 mouse...\n");

    /* Enable mouse on PS/2 controller */
    __outb(0x64, 0xA8);  /* Enable auxiliary device command */

    /* Enable mouse interrupts in controller config */
    __outb(0x64, 0x20);  /* Read controller config */
    uint8_t status = __inb(0x60);
    status |= 0x02;      /* Enable IRQ12 */
    __outb(0x64, 0x60);  /* Write controller config */
    __outb(0x60, status);

    /* Send "Enable Data Reporting" command to mouse */
    __outb(0x64, 0xD4);  /* Write to mouse */
    __outb(0x60, 0xF4);  /* Enable data reporting command */

    printf("PS/2 mouse enabled\n");
}

/* Mouse interrupt handler (IRQ 12) */
void mouse_handler(void) {
    uint8_t data = __inb(MOUSE_PORT);
    // printf("Mouse IRQ! data=0x%x\n", data);
}

/* Get current mouse state */
mouse_state_t mouse_get_state(void) {
    return mouse_state;
}
