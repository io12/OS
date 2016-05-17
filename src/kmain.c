/* kmain.c */

#include "framebuffer.h"

// This is called as the first function after being loaded by the bootloader.
void kmain() {
	// Write a qustion mark to the framebuffer
	fb_putchar('?');
}
