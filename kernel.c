#include "vga.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"
#include "timer.h"

void kernel_main() {
    vga_init();
    vga_printf("=== Timer Test ===\n\n");

    idt_init();
    pic_remap();
    keyboard_init();
    timer_init(100);

    __asm__ volatile ("sti");

    vga_printf("\nWaiting for timer ticks...\n\n");

    while (1) { }
}