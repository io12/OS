/* kmain.c */

#include <sanitycheck.h>

#include <ints.h>
#include <kprintf.h>

#include <framebuffer.h>
#include <multiboot.h>
#include <system.h>

// This is called as the first function after being loaded by bmain.
void kmain(u32 mboot_magic/*, MultibootHeader* mboot_header*/) {
	fb_init();
	if (mboot_magic != MULTIBOOT_EAX_MAGIC) {
		kprintf(PL_FRAMEBUFFER, "Incorrect magic value: 0x%X\nHALTING",
				mboot_magic);
		for (;;) {
			__asm__("hlt");
		}
	}
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
