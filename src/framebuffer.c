/* framebuffer.c */

#include <ints.h>
#include <string.h>

#include "ioport.h"

#include "framebuffer.h"

// Initialize values for the framebuffer struct
fb_t fb = {
	// pointer to the framebuffer
	(fb_char_t*) 0xB8000,
	// position
	0
};

// Move cursor to a new position
void fb_mov(fb_pos_t pos) {
	io_out(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	io_out(FB_DATA_PORT, pos >> 8);
	io_out(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	io_out(FB_DATA_PORT, pos & 0x00FF);
}

// Creates a framebuffer cell based on two colors and an ascii value
fb_char_t fb_mkchar(fb_color_t fb_bgcolor, fb_color_t fb_fgcolor, uint8_t ascii) {
	return ((uint8_t) fb_bgcolor << 12) | ((uint8_t) fb_fgcolor << 8) | ascii;
}

// Write a character to the framebuffer
void fb_putchar(uint8_t ascii) {
	fb.p[fb.pos] = fb_mkchar(FB_BLACK, FB_WHITE, ascii);
	fb.pos++;
	fb_mov(fb.pos);
}

// Write a string to the framebuffer
void fb_puts(const char* str) {
	size_t i;
	size_t length = strlen(str);

	for (i = 0; i < length; i++) {
		// move to the next line if newline char is found
		if (str[i] == '\n') {
			fb.pos = ((fb.pos / 80) + 1) * 80;
		}
		else {
			fb_putchar(str[i]);
		}
	}
}
