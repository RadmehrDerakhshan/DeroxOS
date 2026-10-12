#include "kheap.h"
#include "vga.h"

#define HEAP_START  0x100000
#define HEAP_SIZE   0x300000
#define MAX_BLOCKS  1024

typedef struct {
    uint32_t start;
    uint32_t size;
    uint8_t  used;
} block_t;

static block_t blocks[MAX_BLOCKS];
static int     block_count = 0;
static uint32_t heap_end;

void kheap_init() {
    block_count = 0;
    heap_end = HEAP_START;

    blocks[0].start = HEAP_START;
    blocks[0].size  = HEAP_SIZE;
    blocks[0].used  = 0;
    block_count = 1;

    vga_printf("Heap: %x - %x (%d KB)\n",
               HEAP_START,
               HEAP_START + HEAP_SIZE,
               HEAP_SIZE / 1024);
}

static void split_block(int idx, uint32_t size) {
    if (blocks[idx].size <= size) return;
    if (block_count >= MAX_BLOCKS) return;

    for (int i = block_count; i > idx + 1; i--) {
        blocks[i] = blocks[i - 1];
    }

    blocks[idx + 1].start = blocks[idx].start + size;
    blocks[idx + 1].size  = blocks[idx].size - size;
    blocks[idx + 1].used  = 0;
    blocks[idx].size      = size;

    block_count++;
}

void* kmalloc(uint32_t size) {
    if (size == 0) return 0;

    size = (size + 7) & ~7;

    for (int i = 0; i < block_count; i++) {
        if (blocks[i].used) continue;
        if (blocks[i].size < size) continue;

        split_block(i, size);

        blocks[i].used = 1;
        return (void*) blocks[i].start;
    }

    vga_printf("kmalloc: out of memory (need %d bytes)\n", (int) size);
    return 0;
}

void kfree(void* ptr) {
    if (!ptr) return;

    uint32_t addr = (uint32_t) ptr;

    for (int i = 0; i < block_count; i++) {
        if (blocks[i].start == addr && blocks[i].used) {
            blocks[i].used = 0;
            return;
        }
    }

    vga_printf("kfree: invalid pointer %x\n", addr);
}

void kheap_dump() {
    vga_printf("Heap blocks:\n");
    for (int i = 0; i < block_count; i++) {
        vga_printf("  [%d] %x - %x, size=%d, %s\n",
                   i,
                   blocks[i].start,
                   blocks[i].start + blocks[i].size,
                   (int) blocks[i].size,
                   blocks[i].used ? "USED" : "free");
    }
}