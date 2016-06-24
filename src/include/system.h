/* system.h */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <ints.h>

// memory/gdt.c
void gdt_init();

// interrupts/idt.c
void idt_init();

// drivers/ioport.asm
void io_out(u16 port, u8 data);
u8 io_in(u16 port);

// drivers/keyboard.c
void keyboard_init();

// drivers/serial.c
#define COM1 0x3F8

void serial_init(u16 com);
int serial_fifo_isempty(u16 com);
void serial_wait();
void serial_putchar(u8 data);
void serial_puts(const char* str);

// drivers/timer.c
void timer_init();

// memory/mmap.c
void* kalloc(u8 aligned, u32 size);
void* kalloc_frame();
void mmap_init(u32 size);
void mmap_set_free(u32 address);
void mmap_set_used(u32 address);

// memory/paging.c
void paging_init();

// drivers/ext2.c
void ext2_init(u32 ramdisk_address);

#endif
