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
void kmain(u32 mboot_magic, MultibootInfo* mboot_header) {
	MultibootMemoryMap* mmap = (void*) mboot_header->mmap_address;
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

	serial_init(COM1);
	gdt_init();
	idt_init();
	timer_init();
	keyboard_init();

	mmap_init(mboot_header->mem_lower + mboot_header->mem_upper);
	while ((u32) mmap < mboot_header->mmap_address + mboot_header->mmap_length) {
		if (mmap->type == MULTIBOOT_MEMORY_RESERVED) {
			for (i = 0; i < mmap->length; i += 0x1000) {
				if (mmap->base_address + i > 0xFFFFFFFF) {
					break;
				}
				mmap_set_used((mmap->base_address + i) & 0xFFFFF000);
			}
		}
		else if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			for (i = 0; i < mmap->length; i += 0x1000) {
				if (mmap->base_address + i > 0xFFFFFFFF) {
					break;
				}
				mmap_set_free((mmap->base_address + i) & 0xFFFFF000);
			}
		}
		else {
			kprintf(PL_FRAMEBUFFER,
					"Invalid memory map containing type %u\n"
					"HALTING", mmap->type);
			HALT();
		}
		mmap = (MultibootMemoryMap*) ((u32) mmap + mmap->size + sizeof(u32));
	}

	paging_init();

	__asm__("sti");

	for (;;) {
		__asm__("hlt");
	}
	// NOTREACHED
}
