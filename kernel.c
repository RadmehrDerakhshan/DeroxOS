#include "vga.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"
#include "timer.h"
#include "kheap.h"
#include "paging.h"
#include "task.h"

void task_a() {
    while (1) {
        vga_printf("A");
    }
}

void task_b() {
    while (1) {
        vga_printf("B");
    }
}

void kernel_main() {
    vga_init();
    vga_printf("=== Multitasking Test ===\n\n");

    idt_init();
    pic_remap();
    keyboard_init();
    timer_init(100);
    kheap_init();
    paging_init();
    tasking_init();

    task_create("A", task_a);
    task_create("B", task_b);

    __asm__ volatile ("sti");

    schedule();

    while (1) { }
}