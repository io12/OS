/* framebuffer.c */

#include "framebuffer.h"

// Initialize values for the framebuffer struct
fb_t fb = {
	// pointer to the framebuffer
	(fb_char_t*) 0xB8000,
	// position
	0
};
