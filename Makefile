CC = gcc
AS = nasm
OBJCOPY = objcopy

CFLAGS = -m32 -ffreestanding -fno-pie -nostdlib -Wall -Wextra -c
LDFLAGS = -m32 -ffreestanding -fno-pie -nostdlib -T linker.ld

OBJS = entry.o kernel.o vga.o idt.o isr_asm.o

all: os-image.bin

boot.bin: boot.asm
	$(AS) -f bin boot.asm -o boot.bin

entry.o: entry.asm
	$(AS) -f elf32 entry.asm -o entry.o

isr_asm.o: isr.asm
	$(AS) -f elf32 isr.asm -o isr_asm.o

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

kernel.elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o kernel.elf -lgcc

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

os-image.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > os-image.bin
	dd if=/dev/zero bs=512 count=15 >> os-image.bin 2>/dev/null

run: os-image.bin
	qemu-system-x86_64 -drive format=raw,file=os-image.bin

clean:
	rm -f *.bin *.o *.elf

.PHONY: all run clean