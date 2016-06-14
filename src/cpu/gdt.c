/* gdt.c */

#include <ints.h>

#include <gdt.h>

void gdt_load(u32 gdt_ptr);

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
	u16 limit;
	u32 base;
} __attribute__((packed))
gdt_ptr;

static void gdt_set_gate(u32 n, u32 base, u32 limit, u8 access, u8 flag) {
	gdt[n].limit_low             = (limit & 0x0000FFFF);
	gdt[n].base_low              = (base  & 0x0000FFFF);
	gdt[n].base_middle           = (base  & 0x00FF0000) >> 16;
	gdt[n].access                = access;
	gdt[n].limit_high_and_flags  = (limit & 0x000F0000) >> 16;
	gdt[n].limit_high_and_flags |= (flag & 0xF0);
	gdt[n].base_high             = (base  & 0xFF000000) >> 24;
}

void gdt_init() {
	gdt_ptr.limit = sizeof(gdt) - 1;
	gdt_ptr.base  = (u32) &gdt;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	/*for (int i = 0; i < 5; i++) {
		kprintf(PL_SERIAL, "gdt[%u]:\n limit_low: %u\n base_low: %u\n base_middle: %u\n access: %u\n limit_high_and_flags: %u\n base_high: %u\n", i, gdt[i].limit_low, gdt[i].base_low, gdt[i].base_middle, gdt[i].access, gdt[i].limit_high_and_flags, gdt[i].base_high);
	}*/

	gdt_load((u32) &gdt_ptr);
}
