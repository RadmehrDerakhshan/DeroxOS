#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// یه entry از IDT
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

// IDT register (برای lidt)
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// API
void idt_init();
void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);

#endif