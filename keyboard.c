#include "keyboard.h"
#include "vga.h"
#include "isr.h"

// ============ ورودی از پورت ============
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// ============ scancode به ASCII ============
// جدول ساده: فقط کلیدهای اصلی
static const char scancode_to_ascii[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\','z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*',
    0,   ' '
};

// ============ handler کیبورد ============
void keyboard_handler(struct regs* r) {
    (void) r;   // استفاده نمی‌کنیم

    // scancode رو از پورت 0x60 بخون
    uint8_t scancode = inb(0x60);

    // اگه بیت 7 روشن باشه، یعنی کلید رها شده
    if (scancode & 0x80) {
        return;   // ignore
    }

    // تبدیل به ASCII
    if (scancode < 128) {
        char c = scancode_to_ascii[scancode];
        if (c != 0) {
            vga_putchar(c);
        }
    }
}

// ============ راه‌اندازی ============
void keyboard_init() {
    register_interrupt_handler(33, keyboard_handler);
    vga_printf("Keyboard initialized (IRQ1)\n");
}