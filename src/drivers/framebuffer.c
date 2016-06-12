/* framebuffer.c */

#include <ints.h>
#include <string.h>

#include <ioport.h>
#include <serial.h>

#include <framebuffer.h>

volatile Framebuffer fb;

// Initialize the framebuffer
void fb_init() {
	u16 i;
	FBcell blank = fb_mkchar(FB_BLACK, FB_WHITE, ' ');

	fb.p = (FBcell*) 0xB8000;
	for (i = 0; i <= FB_LASTCELL; i++) {
		fb.p[i] = blank;
	}

	fb.pos = 0;
}

// Move cursor to a new position
void fb_mov() {
	io_out(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	io_out(FB_DATA_PORT, fb.pos >> 8);
	io_out(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	io_out(FB_DATA_PORT, fb.pos & 0x00FF);
}

// Creates a framebuffer cell based on two colors and an ascii value
FBcell fb_mkchar(FBcolor bg, FBcolor fg, u8 ascii) {
	return ((u8) bg << 12) | ((u8) fg << 8) | ascii;
}

// Write a character to the framebuffer
void fb_putchar(u8 ascii) {
	//serial_wait();
	// move to the next line if character is a newline
	if (ascii == '\n') {
		// if bottom row
		if (fb.pos >= FB_BOTTOMROW) {
			fb_scroll();
		}
		else {
			fb.pos = ((fb.pos / FB_COLS) + 1) * FB_COLS;
			fb_mov();
		}
	}
	else {
		fb.p[fb.pos] = fb_mkchar(FB_BLACK, FB_WHITE, ascii);
		fb.pos++;
		fb_mov();
	}
}

// Write a string to the framebuffer
void fb_puts(const char* str) {
	size_t i;
	size_t length = strlen(str);

	for (i = 0; i < length; i++) {
		fb_putchar(str[i]);
	}
}

// Scroll the terminal down
void fb_scroll() {
	u16 i;

	// Copy each row up
	for (i = FB_COLS; i <= FB_LASTCELL; i++) {
		serial_wait();
		fb.p[i - FB_COLS] = fb.p[i];
	}
	// Clear the bottom row
	for (i = FB_BOTTOMROW; i <= FB_LASTCELL; i++) {
		serial_wait();
		fb.p[i] = fb_mkchar(FB_BLACK, FB_WHITE, ' ');
	}

	fb.pos = FB_BOTTOMROW;
	fb_mov();
}
