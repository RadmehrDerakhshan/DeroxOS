ENTRY(_start)

SECTIONS
{
    . = 0x1000;

    .text : {
        *(.text.entry)   /* entry.asm اول */
        *(.text)
    }

    .rodata : { *(.rodata) }
    .data   : { *(.data) }
    .bss    : { *(.bss) }
}