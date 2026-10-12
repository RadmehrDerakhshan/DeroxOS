#include "timer.h"
#include "vga.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43

static volatile uint32_t ticks = 0;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void timer_init(uint32_t freq) {
    uint32_t divisor = 1193180 / freq;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));

    register_interrupt_handler(32, timer_handler);
    vga_printf("Timer initialized at %d Hz\n", (int) freq);
}

void timer_handler(struct regs* r) {
    (void) r;
    ticks++;

    if (ticks % TIMER_FREQ == 0) {
        vga_printf("[%d sec] ", (int)(ticks / TIMER_FREQ));
    }
}

uint32_t timer_get_ticks() {
    return ticks;
}