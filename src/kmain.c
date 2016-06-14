/* kmain.c */

#include <sanitycheck.h>

#include <kprintf.h>

#include <framebuffer.h>
#include <gdt.h>
#include <idt.h>
#include <serial.h>
#include <timer.h>

// This is called as the first function after being loaded by bmain.
void kmain() {
	//serial_init(COM1);
	fb_init();
	gdt_init();
	idt_init();
	//timer_init();

	__asm__("sti");
	__asm__("int $0x0");

	for (;;) {
		__asm__("hlt");
	}
	// NOTREACHED
}
