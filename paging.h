#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_SIZE    4096
#define PAGE_ENTRIES 1024

typedef struct {
    uint32_t present  : 1;
    uint32_t rw       : 1;
    uint32_t user     : 1;
    uint32_t accessed : 1;
    uint32_t dirty    : 1;
    uint32_t unused   : 7;
    uint32_t frame    : 20;
} __attribute__((packed)) page_entry_t;

typedef struct {
    page_entry_t entries[PAGE_ENTRIES];
} __attribute__((packed)) page_table_t;

typedef struct {
    page_entry_t entries[PAGE_ENTRIES];
} __attribute__((packed)) page_directory_t;

void paging_init();
void paging_map(uint32_t virt, uint32_t phys, uint32_t flags);
void paging_unmap(uint32_t virt);
page_directory_t* paging_get_directory();

#endif