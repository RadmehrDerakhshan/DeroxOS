#include "vga.h"
#include "idt.h"
#include "isr.h"

void kernel_main() {
    vga_init();
    vga_printf("=== Interrupt Test ===\n\n");

    idt_init();
    pic_remap();

    vga_printf("Enabling interrupts (sti)...\n");
    __asm__ volatile ("sti");

    vga_printf("Interrupts enabled!\n");
    vga_printf("If you see this, and nothing crashes, we're good.\n");

    while (1) { }
}