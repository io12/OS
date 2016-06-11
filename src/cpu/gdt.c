/* gdt.c */

#include <ints.h>

#include <gdt.h>

struct {
	u16 limit_low;
	u16 base_low;
	u8  base_middle;
	u8  access;
	u8  limit_high_and_flags;
	u8  base_high;
} __attribute__((packed))
gdt[5];

struct {
	u16   limit;
	void* base;
} __attribute__((packed))
gdt_desc;

static void gdt_set_gate(u32 index, u32 base, u32 limit, u8 access, u8 flag) {
	gdt[index].limit_low             = (limit & 0x0000FFFF);
	gdt[index].base_low              = (base  & 0x0000FFFF);
	gdt[index].base_middle           = (base  & 0x00FF0000) >> 16;
	gdt[index].access                = access;
	gdt[index].limit_high_and_flags  = (limit & 0x000F0000) >> 16;
	gdt[index].limit_high_and_flags	|= (flag & 0xF0);
	gdt[index].base_high             = (base  & 0xFF000000) >> 24;
}

void gdt_init() {
	gdt_desc.limit = sizeof(gdt) - 1;
	gdt_desc.base  = gdt;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	lgdt((void*) &gdt_desc);
}
