/* framebuffer.h */

#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include "libk/ints.h"

// Characters on the framebuffer are just 16 bit integers
typedef uint16_t fb_char_t;

// Struct to store information about the framebuffer
typedef struct {
	// Pointer to the framebuffer
	fb_char_t* p; // TODO: see if this can be const
	// Current position
	uint16_t pos;
} fb_t;
extern fb_t fb;

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

// Creates a framebuffer cell based on two colors and an ascii value
inline fb_char_t fb_mkchar(fb_color_t fb_bgcolor, fb_color_t fb_fgcolor, uint8_t ascii) {
	return ((uint8_t) fb_bgcolor << 12) | ((uint8_t) fb_fgcolor << 8) | ascii;
}

// Write a character to the framebuffer
inline void fb_putchar(uint8_t ascii) {
	fb.p[fb.pos] = fb_mkchar(FB_BLACK, FB_WHITE, ascii);
	fb.pos++;
}

// Write a string to the framebuffer
/*inline void fb_puts(const char* str) {
	
}*/

#endif
