#include "idt.h"
#include "vga.h"

#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr   idtp;

// تابع اسمبلی برای لود کردن IDT (توی isr.asm)
extern void idt_load(uint32_t);

// ============ تنظیم یه entry ============
void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

// ============ راه‌اندازی IDT ============
void idt_init() {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base  = (uint32_t) &idt;

    // پاک کردن IDT
    unsigned char* ptr = (unsigned char*) &idt;
    for (unsigned int i = 0; i < sizeof(struct idt_entry) * IDT_ENTRIES; i++) {
        ptr[i] = 0;
    }

    // حالا IDT رو به CPU معرفی کن
    idt_load((uint32_t) &idtp);

    vga_printf("IDT loaded at %x (limit=%d)\n", (uint32_t) &idt, idtp.limit);
}