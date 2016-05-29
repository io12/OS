/* kmain.c */

#include <fb.h>
#include <serial.h>

// This is called as the first function after being loaded by the bootloader.
void kmain() {
	serial_init(COM1);
	fb_init();
	// Write to the framebuffer
	fb_puts("Hello world!\n");
}
