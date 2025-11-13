#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

/* Mouse state */
typedef struct {
    int32_t x;           /* X position */
    int32_t y;           /* Y position */
    uint8_t buttons;     /* Button state (bit 0=left, 1=right, 2=middle) */
} mouse_state_t;

/* Initialize PS/2 mouse */
void mouse_init(void);

/* Mouse interrupt handler (IRQ 12) */
void mouse_handler(void);

/* Get current mouse state */
mouse_state_t mouse_get_state(void);

#endif /* MOUSE_H */
