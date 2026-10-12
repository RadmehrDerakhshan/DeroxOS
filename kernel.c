#include "vga.h"

void kernel_main() {
    vga_init();

    vga_printf("=== printf Test ===\n\n");

    // ============ تست‌های ساده ============
    vga_printf("Hello, world!\n");
    vga_printf("Number: %d\n", 42);
    vga_printf("Negative: %d\n", -100);
    vga_printf("Hex: %x\n", 0xDEADBEEF);
    vga_printf("Char: %c\n", 'A');
    vga_printf("String: %s\n", "This is a string");
    vga_printf("Percent: %%\n");

    // ============ تست چند آرگومان ============
    vga_printf("\nMultiple: %d + %d = %d\n", 3, 4, 3 + 4);

    // ============ تست رنگ + printf ============
    vga_set_color((VGA_BLACK << 4) | VGA_LIGHT_RED);
    vga_printf("Red: %d\n", 123);

    vga_set_color((VGA_BLACK << 4) | VGA_LIGHT_GREEN);
    vga_printf("Green: %s\n", "success");

    vga_set_color(VGA_DEFAULT_COLOR);
    vga_printf("Normal again.\n");

    // ============ تست اسکرول با printf ============
    vga_printf("\nTesting scroll with printf...\n");
    for (int i = 0; i < 30; i++) {
        vga_printf("Line %d\n", i);
    }

    vga_printf("\nDone! Cursor at row=%d, col=%d\n",
               vga_get_row(), vga_get_col());

    while (1) { }
}