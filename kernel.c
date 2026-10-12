#include "vga.h"
#include "idt.h"

void kernel_main() {
    vga_init();

    vga_printf("=== IDT Test ===\n\n");

    vga_printf("Initializing IDT...\n");
    idt_init();

    vga_printf("IDT initialized successfully!\n");

    while (1) { }
}