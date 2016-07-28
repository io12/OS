/* framebuffer.c */

#include <ints.h>
#include <klibc.h>
#include <system.h>

#include <framebuffer.h>

#define FB_ROWS      25
#define FB_COLS      80
#define FB_BOTTOMROW (FB_COLS * (FB_ROWS - 1))
#define FB_FIRSTCELL 0
#define FB_LASTCELL  (FB_COLS * FB_ROWS - 1)

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

volatile struct {
	// Pointer to the framebuffer
	u16* p;
	// Current position
	u16  pos;
} fb;

u8 fb_getx() {
	return fb.pos / FB_ROWS;
}

u8 fb_gety() {
	return fb.pos / FB_COLS;
}

void fb_init() {
	u16 i;
	u16 blank = fb_mkchar(FB_BLACK, FB_WHITE, ' ');

	fb.p = (u16*) 0xB8000;
	for (i = FB_FIRSTCELL; i <= FB_LASTCELL; i++) {
		fb.p[i] = blank;
	}

	fb.pos = FB_FIRSTCELL;
}

// Move cursor to a new position
void fb_mov() {
	out(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	out(FB_DATA_PORT, fb.pos >> 8);
	out(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	out(FB_DATA_PORT, fb.pos & 0x00FF);
}

// Creates a framebuffer cell based on two colors and an ascii value
u16 fb_mkchar(FB_Color bg, FB_Color fg, u8 ascii) {
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
	else if (ascii == '\b') {
		if (fb.pos > FB_FIRSTCELL) {
			fb.pos--;
			fb_mov();
			fb.p[fb.pos] = fb_mkchar(FB_BLACK, FB_WHITE, ' ');
		}
	}
	else {
		if (fb.pos == FB_LASTCELL) {
			fb_scroll();
		}
		fb.p[fb.pos] = fb_mkchar(FB_BLACK, FB_WHITE, ascii);
		fb.pos++;
		fb_mov();
	}
}

void fb_puts(const char* str) {
	size_t i;
	size_t length = strlen(str);

	for (i = 0; i < length; i++) {
		fb_putchar(str[i]);
	}
}

void fb_setpos(u8 x, u8 y) {
	fb.pos = x * y;
}

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
