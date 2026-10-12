#include "vga.h"
#include "stdarg.h"

// ============ متغیرهای داخلی ============
static uint8_t* const vga_buffer = (uint8_t*) VGA_ADDRESS;
static int cursor_row = 0;
static int cursor_col = 0;
static uint8_t current_color = VGA_DEFAULT_COLOR;

// ============ محاسبه offset ============
static inline int vga_offset(int row, int col) {
    return (row * VGA_WIDTH + col) * 2;
}

// ============ راه‌اندازی ============
void vga_init() {
    cursor_row = 0;
    cursor_col = 0;
    current_color = VGA_DEFAULT_COLOR;
    vga_clear();
}

// ============ پاک کردن صفحه ============
void vga_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i * 2]     = ' ';
        vga_buffer[i * 2 + 1] = current_color;
    }
    cursor_row = 0;
    cursor_col = 0;
}

// ============ تنظیم رنگ ============
void vga_set_color(uint8_t color) {
    current_color = color;
}

// ============ اسکرول ============
void vga_scroll() {
    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i++) {
        vga_buffer[i] = vga_buffer[i + VGA_WIDTH * 2];
    }
    int last_row_start = (VGA_HEIGHT - 1) * VGA_WIDTH * 2;
    for (int i = 0; i < VGA_WIDTH; i++) {
        vga_buffer[last_row_start + i * 2]     = ' ';
        vga_buffer[last_row_start + i * 2 + 1] = current_color;
    }
    cursor_row = VGA_HEIGHT - 1;
    cursor_col = 0;
}

// ============ چاپ یه کاراکتر ============
void vga_putchar(char c) {
    while (cursor_row >= VGA_HEIGHT) {
        vga_scroll();
    }

    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else if (c == '\r') {
        cursor_col = 0;
    } else if (c == '\t') {
        for (int i = 0; i < 4; i++) vga_putchar(' ');
        return;
    } else if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            int off = vga_offset(cursor_row, cursor_col);
            vga_buffer[off]     = ' ';
            vga_buffer[off + 1] = current_color;
        }
        return;
    } else {
        int off = vga_offset(cursor_row, cursor_col);
        vga_buffer[off]     = c;
        vga_buffer[off + 1] = current_color;
        cursor_col++;
    }

    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }

    while (cursor_row >= VGA_HEIGHT) {
        vga_scroll();
    }
}

// ============ چاپ رشته ============
void vga_puts(const char* str) {
    while (*str) {
        vga_putchar(*str);
        str++;
    }
}

// ============ چاپ عدد اعشاری ============
void vga_print_int(int n) {
    if (n == 0) {
        vga_putchar('0');
        return;
    }
    if (n < 0) {
        vga_putchar('-');
        n = -n;
    }
    char buf[12];
    int i = 0;
    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    while (i > 0) {
        vga_putchar(buf[--i]);
    }
}

// ============ چاپ عدد هگزادسیمال ============
void vga_print_hex(uint32_t n) {
    const char* hex = "0123456789ABCDEF";
    vga_puts("0x");
    for (int i = 28; i >= 0; i -= 4) {
        vga_putchar(hex[(n >> i) & 0xF]);
    }
}

// ============ printf ============
void vga_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
            case 'd': {
                int val = va_arg(args, int);
                vga_print_int(val);
                break;
            }
            case 'x': {
                uint32_t val = va_arg(args, uint32_t);
                vga_print_hex(val);
                break;
            }
            case 'c': {
                char c = (char) va_arg(args, int);
                vga_putchar(c);
                break;
            }
            case 's': {
                const char* s = va_arg(args, const char*);
                vga_puts(s);
                break;
            }
            case '%':
                vga_putchar('%');
                break;
            default:
                vga_putchar('%');
                vga_putchar(*fmt);
            }
        } else {
            vga_putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}

// ============ دیباگ ============
int vga_get_row() { return cursor_row; }
int vga_get_col() { return cursor_col; }