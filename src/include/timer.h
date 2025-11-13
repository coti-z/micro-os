#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

/* Timer interrupt counter */
extern volatile uint64_t timer_ticks;

/* Initialize PIT (Programmable Interval Timer) */
void timer_init(uint16_t divisor);

/* Timer interrupt handler */
void timer_handler(uint64_t *stack_ptr);

#endif /* _TIMER_H_ */
