#ifndef VGA_H
#define VGA_H

#include <stdint.h>

// آدرس‌های VGA
#define VGA_ADDRESS     0xB8000
#define VGA_WIDTH       80
#define VGA_HEIGHT      25

// رنگ‌ها (4 بیت پایین: متن، 4 بیت بالا: پس‌زمینه)
#define VGA_BLACK       0x0
#define VGA_BLUE        0x1
#define VGA_GREEN       0x2
#define VGA_CYAN        0x3
#define VGA_RED         0x4
#define VGA_MAGENTA     0x5
#define VGA_BROWN       0x6
#define VGA_LIGHT_GRAY  0x7
#define VGA_DARK_GRAY   0x8
#define VGA_LIGHT_BLUE  0x9
#define VGA_LIGHT_GREEN 0xA
#define VGA_LIGHT_CYAN  0xB
#define VGA_LIGHT_RED   0xC
#define VGA_PINK        0xD
#define VGA_YELLOW      0xE
#define VGA_WHITE       0xF

// رنگ پیش‌فرض: متن سفید روی پس‌زمینه مشکی
#define VGA_DEFAULT_COLOR ((VGA_BLACK << 4) | VGA_WHITE)

// توابع اصلی
void vga_init();
void vga_clear();
void vga_putchar(char c);
void vga_puts(const char* str);
void vga_set_color(uint8_t color);
void vga_scroll();

// توابع کمکی
void vga_print_int(int n);
void vga_print_hex(uint32_t n);

// دیباگ: موقعیت فعلی کرسر
int vga_get_row();
int vga_get_col();

#endif