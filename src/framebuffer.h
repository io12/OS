/* framebuffer.h */

#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <ints.h>
#include <defs.h>

#include "ioport.h"

#define FB_ROWS 25
#define FB_COLS 80
#define FB_BOTTOMROW (FB_COLS * (FB_ROWS - 1))
#define FB_LASTCELL (FB_COLS * FB_ROWS - 1)

// I/O ports
#define FB_COMMAND_PORT			0x3D4
#define FB_DATA_PORT			0x3D5

// I/O commands
#define FB_HIGH_BYTE_COMMAND		14
#define FB_LOW_BYTE_COMMAND		15

// Characters on the framebuffer are just 16 bit integers
typedef uint16_t fb_char_t;

// Framebuffer position type
typedef uint16_t fb_pos_t;

// Struct to store information about the framebuffer
typedef struct {
	// Pointer to the framebuffer
	fb_char_t* p;
	// Current position
	uint16_t pos;
} fb_t;

// All the possible framebuffer color values
typedef enum {
	FB_BLACK    = 0,
	FB_BLUE     = 1,
	FB_GREEN    = 2,
	FB_CYAN     = 3,
	FB_RED      = 4,
	FB_MAGENTA  = 5,
	FB_BROWN    = 6,
	FB_LGREY    = 7,
	FB_DGREY    = 8,
	FB_LBLUE    = 9,
	FB_LGREEN   = 10,
	FB_LCYAN    = 11,
	FB_LRED     = 12,
	FB_LMAGENTA = 13,
	FB_LBROWN   = 14,
	FB_WHITE    = 15
} fb_color_t;

void fb_init();
void fb_mov();
fb_char_t fb_mkchar(fb_color_t fb_bgcolor, fb_color_t fb_fgcolor, uint8_t ascii);
void fb_putchar(uint8_t ascii);
void fb_puts(const char* str);
void fb_scroll();

#endif
