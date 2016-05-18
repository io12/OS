/* kmain.c */

#include "framebuffer.h"

// This is called as the first function after being loaded by the bootloader.
void kmain() {
	// Write "Hello world!\n" to the framebuffer
	fb_puts("Hello world!\n");
}
