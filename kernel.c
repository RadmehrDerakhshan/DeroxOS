#include "vga.h"

void kernel_main() {
    vga_init();

    // ============ تست ۱: چاپ ساده ============
    vga_puts("=== VGA Test ===\n\n");

    // ============ تست ۲: چاپ عدد ============
    vga_puts("Number: ");
    vga_print_int(42);
    vga_putchar('\n');

    vga_puts("Negative: ");
    vga_print_int(-100);
    vga_putchar('\n');

    vga_puts("Hex: ");
    vga_print_hex(0xDEADBEEF);
    vga_putchar('\n');

    // ============ تست ۳: رنگ ============
    vga_set_color((VGA_BLACK << 4) | VGA_LIGHT_RED);
    vga_puts("This is RED!\n");

    vga_set_color((VGA_BLACK << 4) | VGA_LIGHT_GREEN);
    vga_puts("This is GREEN!\n");

    vga_set_color(VGA_DEFAULT_COLOR);
    vga_puts("Back to white.\n\n");

    // ============ تست ۴: اسکرول ============
    vga_puts("Testing scroll...\n");

    for (int i = 0; i < 30; i++) {
        vga_puts("Line ");
        vga_print_int(i);
        vga_putchar('\n');
    }

    vga_puts("\nIf you see this, scroll works!\n");

    // ============ تست ۵: موقعیت کرسر ============
    vga_puts("Cursor at row=");
    vga_print_int(vga_get_row());
    vga_puts(", col=");
    vga_print_int(vga_get_col());
    vga_putchar('\n');

    while (1) { }
}