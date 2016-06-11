/* gdt.c */

#include <ints.h>

#include <gdt.h>

typedef struct {
	u16 limit_low;
	u16 base_low;
	u8  base_middle;
	u8  access;
	u8  limit_high_and_flags;
	u8  base_high;
} __attribute__((packed))
GDTentry;

struct {
	u16 size;
	GDTentry* offset;
	GDTentry entries[5];
} gdt;

static void gdt_mkentry(u32 index, u32 base, u32 limit, u8 access, u8 flag) {
	gdt.entries[index].limit_low             = (limit & 0x0000FFFF);
	gdt.entries[index].base_low              = (base  & 0x0000FFFF);
	gdt.entries[index].base_middle           = (base  & 0x00FF0000) >> 16;
	gdt.entries[index].access                = access;
	gdt.entries[index].limit_high_and_flags  = (limit & 0x000F0000) >> 16;
	gdt.entries[index].limit_high_and_flags	|= (flag & 0xF0);
	gdt.entries[index].base_high             = (base  & 0xFF000000) >> 24;
}

void gdt_init() {
	gdt.size = sizeof(gdt.entries) - 1;
	gdt.offset = gdt.entries;

	gdt_mkentry(0, 0, 0, 0, 0);
	gdt_mkentry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_mkentry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_mkentry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_mkentry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	lgdt((void*) &gdt);
}
