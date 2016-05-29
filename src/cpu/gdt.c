/* gdt.c */

#include <ints.h>

// GDT descriptor flags
#define GDT_DESCTYPE(X)
#define GDT_PRESENT(X)
#define GDT_SYS_AVAILABLE(X)
#define GDT_LONG_MODE(X)
#define GDT_SIZE(X)
#define GDT_GRANULARITY(X)
#define GDT_PRIVILEGE(X)

// Make a GDT descriptor
void gdt_mkdesc(uint32_t base, uint32_t limit, uint32_t flag) {
	uint64_t descriptor = 0;

	// Create high 32 bit segment
	descriptor |= limit		& 0x000F0000;
	descriptor |= (flag << 8)	& 0x00F0FF00;
	descriptor |= (base >> 16)	& 0x000000FF;
	descriptor |= base		& 0xFF000000;

	descriptor <<= 32;

	// Create low segment
	descriptor |= base << 16;
	descriptor |= limit		& 0x0000FFFF;
}
