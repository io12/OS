/* kmain.c */

#include "framebuffer.h"
#include "serial.h"

// This is called as the first function after being loaded by the bootloader.
void kmain() {
	// Test serial port
	serial_init(COM1);
	serial_puts(COM1, "Hello, serial!\n");
	// Write to the framebuffer
	fb_puts(
			"Hello world!\n"
			"This\n"
			"text\n"
			"can\n"
			"span\n"
			"multiple\n"
			"lines.\n");
}
