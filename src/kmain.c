/* kmain.c */

#include <sanitycheck.h>

#include <framebuffer.h>
#include <ints.h>
#include <kprintf.h>
#include <multiboot.h>
#include <system.h>

// This is called as the first function after being loaded by bmain.
void kmain(u32 mboot_magic, MultibootHeader* mboot_header) {
	fb_init();
	if (mboot_magic != MULTIBOOT_EAX_MAGIC) {
		kprintf(PL_FRAMEBUFFER, "Incorrect magic value: 0x%X\nHALTING",
				mboot_magic);
		for (;;) {
			__asm__("hlt");
		}
	}
	kprintf(PL_FRAMEBUFFER, "mem_lower: %u\nmem_upper %u\n",
			mboot_header->mem_lower, mboot_header->mem_upper);
	serial_init(COM1);
	gdt_init();
	idt_init();
	timer_init();
	keyboard_init();

	__asm__("sti");

	for (;;) {
		__asm__("hlt");
	}
	// NOTREACHED
}
