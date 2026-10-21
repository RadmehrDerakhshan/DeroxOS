[bits 32]
global switch_task

; void switch_task(uint32_t* old_esp, uint32_t new_esp)
switch_task:
    mov eax, [esp + 4]      ; old_esp
    mov edx, [esp + 8]      ; new_esp

    ; 1. Save current task's context (exactly like irq_common)
    pushfd
    push cs
    push dword [esp + 12]   ; EIP (return address to schedule)
    push dword 0            ; fake int_no
    push dword 0            ; fake err_code
    pusha

    ; 2. Save ESP to *old_esp
    mov [eax], esp

    ; 3. Switch to new ESP
    mov esp, edx

    ; 4. Restore context (exactly like irq_common)
    popa
    add esp, 8              ; skip int_no and err_code
    iret                    ; pop EIP, CS, EFLAGS