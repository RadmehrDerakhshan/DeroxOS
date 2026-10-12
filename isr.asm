[bits 32]

global idt_load

; void idt_load(uint32_t idt_ptr_address)
idt_load:
    mov eax, [esp + 4]      ; اولین آرگومان
    lidt [eax]              ; IDT رو لود کن
    ret