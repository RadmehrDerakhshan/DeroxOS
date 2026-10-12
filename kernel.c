#include "vga.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"
#include "timer.h"
#include "kheap.h"

void kernel_main() {
    vga_init();
    vga_printf("=== kheap Test ===\n\n");

    idt_init();
    pic_remap();
    keyboard_init();
    timer_init(100);

    kheap_init();

    __asm__ volatile ("sti");

    vga_printf("\n--- Allocating ---\n");

    int* a = (int*) kmalloc(sizeof(int));
    int* b = (int*) kmalloc(sizeof(int));
    char* s = (char*) kmalloc(100);

    *a = 42;
    *b = 99;

    for (int i = 0; i < 10; i++) s[i] = 'A' + i;
    s[10] = 0;

    vga_printf("a = %d\n", *a);
    vga_printf("b = %d\n", *b);
    vga_printf("s = %s\n", s);

    kheap_dump();

    vga_printf("\n--- Freeing b ---\n");
    kfree(b);
    kheap_dump();

    while (1) { }
}