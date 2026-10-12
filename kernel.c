#include "vga.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"
#include "timer.h"
#include "kheap.h"
#include "paging.h"

void kernel_main() {
    vga_init();
    vga_printf("STEP 1: vga_init done\n");

    idt_init();
    vga_printf("STEP 2: idt_init done\n");

    pic_remap();
    vga_printf("STEP 3: pic_remap done\n");

    keyboard_init();
    vga_printf("STEP 4: keyboard_init done\n");

    timer_init(100);
    vga_printf("STEP 5: timer_init done\n");

    kheap_init();
    vga_printf("STEP 6: kheap_init done\n");

    __asm__ volatile ("sti");
    vga_printf("STEP 7: sti done\n");

    vga_printf("STEP 8: before paging_init\n");
    paging_init();
    vga_printf("STEP 9: after paging_init\n");

    vga_printf("STEP 10: ALL DONE\n");

    while (1) { }
}