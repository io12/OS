/* kmain.c */

#include "framebuffer.h"

void kmain() {
	fb[0] = fb_mkchar(FB_BLACK, FB_WHITE, '?');
}
