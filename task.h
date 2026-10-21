#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define TASK_STACK_SIZE 4096
#define MAX_TASKS       8

typedef enum {
    TASK_RUNNING,
    TASK_READY,
    TASK_DEAD
} task_state_t;

typedef struct {
    uint32_t esp;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t esi;
    uint32_t edi;
    uint32_t eip;
    uint32_t state;
    char     name[32];
} task_t;

void    tasking_init();
task_t* task_create(const char* name, void (*entry)());
void    schedule();
task_t* task_current();

#endif