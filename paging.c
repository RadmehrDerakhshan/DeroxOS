#include "paging.h"
#include "vga.h"

#define PDE_PRESENT  0x01
#define PDE_RW       0x02
#define PDE_USER     0x04

// آدرس‌های ثابت در حافظه
#define PAGE_DIR_ADDR   0x200000
#define PAGE_TABLES_ADDR 0x201000

static page_directory_t* kernel_directory;
static page_table_t*     identity_tables;

static inline void load_cr3(uint32_t phys) {
    __asm__ volatile ("mov %0, %%cr3" : : "r"(phys));
}

static inline void flush_tlb(uint32_t virt) {
    __asm__ volatile ("invlpg (%0)" : : "r"(virt) : "memory");
}

void paging_init() {
    kernel_directory = (page_directory_t*) PAGE_DIR_ADDR;
    identity_tables  = (page_table_t*)     PAGE_TABLES_ADDR;

    // Page directory رو صفر کن
    for (int i = 0; i < PAGE_ENTRIES; i++) {
        kernel_directory->entries[i].present = 0;
        kernel_directory->entries[i].rw      = 1;
        kernel_directory->entries[i].user    = 0;
    }

    // 4 تا page table بساز
    for (int t = 0; t < 4; t++) {
        // این page table رو صفر کن
        for (int i = 0; i < PAGE_ENTRIES; i++) {
            identity_tables[t].entries[i].present = 0;
        }

        for (int i = 0; i < PAGE_ENTRIES; i++) {
            uint32_t addr = (t * PAGE_ENTRIES + i) * PAGE_SIZE;
            identity_tables[t].entries[i].frame   = addr >> 12;
            identity_tables[t].entries[i].present = 1;
            identity_tables[t].entries[i].rw      = 1;
            identity_tables[t].entries[i].user    = 0;
        }

        kernel_directory->entries[t].frame   = ((uint32_t) &identity_tables[t]) >> 12;
        kernel_directory->entries[t].present = 1;
        kernel_directory->entries[t].rw      = 1;
        kernel_directory->entries[t].user    = 0;
    }

    load_cr3((uint32_t) kernel_directory);

    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0" : : "r"(cr0));

    vga_printf("Paging enabled (16MB identity mapped)\n");
}

void paging_map(uint32_t virt, uint32_t phys, uint32_t flags) {
    uint32_t dir_idx   = virt >> 22;
    uint32_t table_idx = (virt >> 12) & 0x3FF;

    if (!kernel_directory->entries[dir_idx].present) return;

    page_table_t* table = (page_table_t*)(kernel_directory->entries[dir_idx].frame << 12);

    table->entries[table_idx].frame   = phys >> 12;
    table->entries[table_idx].present = 1;
    table->entries[table_idx].rw      = (flags & PDE_RW) ? 1 : 0;
    table->entries[table_idx].user    = (flags & PDE_USER) ? 1 : 0;

    flush_tlb(virt);
}

void paging_unmap(uint32_t virt) {
    uint32_t dir_idx   = virt >> 22;
    uint32_t table_idx = (virt >> 12) & 0x3FF;

    if (!kernel_directory->entries[dir_idx].present) return;

    page_table_t* table = (page_table_t*)(kernel_directory->entries[dir_idx].frame << 12);
    table->entries[table_idx].present = 0;
    flush_tlb(virt);
}

page_directory_t* paging_get_directory() {
    return kernel_directory;
}