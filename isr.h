#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// وضعیت CPU وقتی وقفه میاد
struct regs {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

typedef void (*isr_t)(struct regs*);

void isr_handler(struct regs* r);
void irq_handler(struct regs* r);
void register_interrupt_handler(int n, isr_t handler);

// ← این خط رو اضافه کن
void pic_remap();

#endif