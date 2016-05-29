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
	uint64_t descriptor;
}
