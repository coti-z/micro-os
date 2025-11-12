#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

extern volatile uint8_t keyboard_buffer[256];
extern volatile uint16_t keyboard_head;
extern volatile uint16_t keyboard_tail;




#endif