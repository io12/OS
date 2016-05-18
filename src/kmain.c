/* kmain.c */

#include "framebuffer.h"

// This is called as the first function after being loaded by the bootloader.
void kmain() {
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
