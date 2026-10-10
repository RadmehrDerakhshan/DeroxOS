#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F
#define RED_ON_BLACK 0x0C
#define GREEN_ON_BLACK 0x0A
#define YELLOW_ON_BLACK 0x0E

void clear_screen() {
    unsigned char* vga = (unsigned char*) VGA_ADDRESS;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        vga[i]   = ' ';
        vga[i+1] = WHITE_ON_BLACK;
    }
}

void put_char(char c, int row, int col, unsigned char color) {
    unsigned char* vga = (unsigned char*) VGA_ADDRESS;
    int offset = (row * 80 + col) * 2;
    vga[offset]   = c;
    vga[offset+1] = color;
}

void print_string(const char* str, int row, unsigned char color) {
    int col = 0;
    while (*str) {
        if (*str == '\n') {
            row++;
            col = 0;
        } else {
            put_char(*str, row, col, color);
            col++;
        }
        str++;
    }
}

void kernel_main() {
    clear_screen();
    print_string("Hello from C kernel!", 0, WHITE_ON_BLACK);
    print_string("This is running in protected mode.", 1, GREEN_ON_BLACK);
    print_string("C is working!", 2, YELLOW_ON_BLACK);

    while (1) { }
}