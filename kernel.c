#include "vga.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"

void kernel_main() {
    vga_init();
    vga_printf("=== Keyboard Test ===\n\n");

    idt_init();
    pic_remap();
    keyboard_init();

    vga_printf("Enabling interrupts...\n");
    __asm__ volatile ("sti");

    vga_printf("\nType something: ");
    vga_set_color((VGA_BLACK << 4) | VGA_LIGHT_GREEN);

    while (1) { }
}