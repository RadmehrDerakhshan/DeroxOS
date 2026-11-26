# DeroxOS

A hobby operating system written in C and Assembly — bootloader, protected mode, interrupts, memory management, and more. Under active development.

## Features

### Implemented
- **Bootloader** — custom bootloader in Assembly
- **Protected Mode** — switched from real mode to 32-bit protected mode
- **Kernel in C** — main kernel logic written in C
- **VGA + printf** — text output on VGA buffer with custom printf
- **IDT + ISR + IRQ** — interrupt descriptor table, interrupt service routines, and IRQ handlers
- **PIC Remap** — remapped the Programmable Interrupt Controller
- **Keyboard Driver** — PS/2 keyboard input
- **Timer** — PIT (Programmable Interval Timer)
- **kmalloc / kfree** — kernel heap allocator
- **Paging** — 16 MB identity-mapped paging

### Planned
- **Multitasking** — process/thread scheduling
- **Syscalls** — system call interface
- **Filesystem** — virtual file system (VFS)
- **Userspace** — user-mode processes

## Requirements

- gcc (cross-compiler recommended: i686-elf-gcc)
- nasm (assembler)
- make
- qemu-system-i386 (for running)

## Build & Run

```bash
make
make run
```

## Project Structure

```text
boot/       ← bootloader (Assembly)
kernel/     ← kernel (C)
drivers/    ← keyboard, timer, VGA
include/    ← header files
Makefile
```

## Screenshots

<!-- TODO: add screenshots here -->

## Roadmap

- [ ] Multitasking
- [ ] Syscalls
- [ ] Filesystem
- [ ] Userspace

## License

GPL-v3

## Author

Radmehr Derakhshan
