/* system.h */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <ints.h>

// memory/gdt.c
void gdt_init();

// interrupts/idt.c
void idt_init();

// asm_misc.asm
void int_wait_forever();
void permahalt();
void out(u16 port, u8 data);
u8 in(u16 port);

// drivers/keyboard.c
void keyboard_init();

// drivers/serial.c
#define COM1 0x3F8

void serial_init(u16 com);
int serial_fifo_isempty(u16 com);
void serial_wait();
void serial_putchar(u8 data);
void serial_puts(const char* str);

// memory/mmap.c
void* kalloc_frame();
void* kalloc_frames(u32 num);
u8 mmap_check(u32 n);
void mmap_init(u32 size);
void mmap_init_finalize();
void mmap_address_set_free(u32 address);
void mmap_address_set_used(u32 address);
u8 mmap_index_check(u32 n);
void mmap_index_set_free(u32 n);
void mmap_index_set_used(u32 n);

// memory/control_registers.asm
u32 cr0_read();
void cr0_write(u32 cr0);
u32 cr3_read();
void cr3_write(u32 cr3);

// memory/paging.c
void paging_init();

// drivers/ext2.c
void* ext2_block_to_ptr(u32 block);
void ext2_init(u32 ramdisk_address);
u32 ext2_path_to_inode_num(u32 inode_num, char* fpath);

// scheduler.c
void scheduler_init();

#endif
