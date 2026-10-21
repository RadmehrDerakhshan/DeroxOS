#include "task.h"
#include "vga.h"
#include "kheap.h"

static task_t  tasks[MAX_TASKS];
static int     task_count = 0;
static task_t* current    = 0;

extern void switch_task(uint32_t* old_esp, uint32_t new_esp);

void tasking_init() {
    task_count = 0;
    current    = 0;
}

task_t* task_create(const char* name, void (*entry)()) {
    if (task_count >= MAX_TASKS) return 0;

    task_t* t = &tasks[task_count];
    t->state = TASK_READY;

    int i = 0;
    while (name[i] && i < 31) {
        t->name[i] = name[i];
        i++;
    }
    t->name[i] = 0;

    uint32_t* stack = (uint32_t*) kmalloc(TASK_STACK_SIZE);
    uint32_t* sp    = (uint32_t*)((uint32_t) stack + TASK_STACK_SIZE);

    // Build stack EXACTLY like iret expects
    *(--sp) = 0x202;                // EFLAGS
    *(--sp) = 0x08;                 // CS
    *(--sp) = (uint32_t) entry;     // EIP

    *(--sp) = 0;                    // fake int_no
    *(--sp) = 0;                    // fake err_code

    // pusha order: EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
    *(--sp) = 0; // EDI
    *(--sp) = 0; // ESI
    *(--sp) = 0; // EBP
    *(--sp) = 0; // ESP
    *(--sp) = 0; // EBX
    *(--sp) = 0; // EDX
    *(--sp) = 0; // ECX
    *(--sp) = 0; // EAX

    t->esp = (uint32_t) sp;
    task_count++;
    vga_printf("Task '%s' created\n", t->name);
    return t;
}

void schedule() {
    if (task_count == 0) return;
    
    task_t* prev = current;
    int next_id = prev ? (prev - tasks + 1) % task_count : 0;
    task_t* next = &tasks[next_id];

    if (prev == next) return;
    
    if (prev) prev->state = TASK_READY;
    next->state = TASK_RUNNING;
    current = next;

    if (prev) {
        switch_task(&prev->esp, next->esp);
    } else {
        __asm__ volatile (
            "mov %0, %%esp\n"
            "popa\n"
            "add $8, %%esp\n"
            "iret\n"
            :
            : "r"(next->esp)
        );
    }
}

task_t* task_current() {
    return current;
}