#include "isr.h"
#include "idt.h"
#include "vga.h"

// آرایه handlerها (256 تا)
static isr_t interrupt_handlers[256];

// ============ خروجی/ورودی پورت ============
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait() {
    outb(0x80, 0);
}

// ============ PIC ============
#define PIC1        0x20
#define PIC2        0xA0
#define PIC1_CMD    PIC1
#define PIC1_DATA   (PIC1 + 1)
#define PIC2_CMD    PIC2
#define PIC2_DATA   (PIC2 + 1)
#define PIC_EOI     0x20

void pic_remap() {
    // شروع مقداردهی مجدد
    outb(PIC1_CMD, 0x11);
    io_wait();
    outb(PIC2_CMD, 0x11);
    io_wait();

    // offset: IRQ 0-7 → 32-39، IRQ 8-15 → 40-47
    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    // اتصال master به slave (روی IRQ2)
    outb(PIC1_DATA, 0x04);
    io_wait();
    outb(PIC2_DATA, 0x02);
    io_wait();

    // mode: 8086
    outb(PIC1_DATA, 0x01);
    io_wait();
    outb(PIC2_DATA, 0x01);
    io_wait();

    // باز کردن همه IRQها (فعلاً)
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);

    vga_printf("PIC remapped: IRQ0-7 -> 32-39, IRQ8-15 -> 40-47\n");
}

// ============ Exception handler ============
void isr_handler(struct regs* r) {
    if (r->int_no < 32) {
        vga_set_color((VGA_BLACK << 4) | VGA_LIGHT_RED);
        vga_printf("\n\n!!! EXCEPTION !!!\n");
        vga_printf("Interrupt: %d\n", (int) r->int_no);
        vga_printf("Error code: %x\n", r->err_code);
        vga_printf("EIP: %x\n", r->eip);
        vga_set_color(VGA_DEFAULT_COLOR);

        // متوقف شو
        while (1) { __asm__ volatile ("hlt"); }
    }

    // اگه handler ثبت شده بود، صداش بزن
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t h = interrupt_handlers[r->int_no];
        h(r);
    }
}

// ============ IRQ handler ============
void irq_handler(struct regs* r) {
    // اگه IRQ از slave PIC اومده (IRQ 8-15)
    if (r->int_no >= 40) {
        outb(PIC2_CMD, PIC_EOI);
    }
    outb(PIC1_CMD, PIC_EOI);

    // اگه handler ثبت شده بود، صداش بزن
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t h = interrupt_handlers[r->int_no];
        h(r);
    }
}

// ============ ثبت handler ============
void register_interrupt_handler(int n, isr_t handler) {
    interrupt_handlers[n] = handler;
}