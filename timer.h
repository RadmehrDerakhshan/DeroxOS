#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "isr.h"

#define TIMER_FREQ 100

void timer_init(uint32_t freq);
void timer_handler(struct regs* r);
uint32_t timer_get_ticks();

#endif