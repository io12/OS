/* kmain.c */

#include <sanitycheck.h>

#include <framebuffer.h>
#include <ints.h>
#include <kprintf.h>
#include <multiboot.h>
#include <system.h>

#define HALT() \
	do { \
		__asm__("cli"); \
		for (;;) { \
			__asm__("hlt"); \
		} \
	} while (0)

// This is called as the first function after being loaded by bmain.
void kmain(u32 mboot_magic, MultibootInfo* mboot_info) {
	MultibootMemoryMap* mmap  = (void*) mboot_info->mmap_address;
	MultibootModules* modules = (void*) mboot_info->mods_address;
	u32 i;

	fb_init();

	if (mboot_magic != MULTIBOOT_EAX_MAGIC) {
		kprintf(PL_FRAMEBUFFER,
				"Incorrect magic value: 0x%X\n"
				"Not booted with a multiboot compatible bootloader\n"
				"HALTING",
				mboot_magic);
		HALT();
	}

	kprintf(PL_SERIAL, "Initializing serial ports\n");
	serial_init(COM1);

	kprintf(PL_SERIAL, "Initializing the GDT\n");
	gdt_init();

	kprintf(PL_SERIAL, "Initializing the IDT\n");
	idt_init();

	kprintf(PL_SERIAL, "Initializing the keyboard driver\n");
	keyboard_init();

	kprintf(PL_SERIAL, "Initializing the memory map\n");
	mmap_init(mboot_info->mem_lower + mboot_info->mem_upper);
	// copy bootloader's memory map to a bit array
	while ((u32) mmap < mboot_info->mmap_address + mboot_info->mmap_length) {
		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			for (i = 0; i < mmap->length; i += 0x1000) {
				if (mmap->base_address + i > 0xFFFFFFFF) {
					break;
				}
				mmap_set_free((mmap->base_address + i) & 0xFFFFF000);
			}
		}
		else {
			for (i = 0; i < mmap->length; i += 0x1000) {
				if (mmap->base_address + i > 0xFFFFFFFF) {
					break;
				}
				mmap_set_used((mmap->base_address + i) & 0xFFFFF000);
			}
		}
		mmap = (MultibootMemoryMap*) ((u32) mmap + mmap->size + sizeof(u32));
	}
	kprintf(PL_SERIAL, "Finalizing the memory map\n");
	mmap_init_finalize();

	kprintf(PL_SERIAL, "Initializing paging\n");
	paging_init();

	kprintf(PL_SERIAL, "Initializing ext2 driver\n");
	ext2_init(modules->mod_start);

	kprintf(PL_SERIAL, "Initializing the process scheduler\n");
	scheduler_init();

	__asm__("sti");

	for (;;) {
		__asm__("hlt");
	}
	// NOTREACHED
}
