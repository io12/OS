/* gdt.c */

#include <ints.h>

#include <system.h>

void gdt_load(u32 gdt_ptr);
void tss_load();
void gdt_set_gate(u32 n, u32 base, u32 limit, u8 access, u8 flag);

struct {
	u16 limit_low;
	u16 base_low;
	u8  base_middle;
	u8  access;
	u8  limit_high_and_flags;
	u8  base_high;
} __attribute__((packed))
gdt[6];

struct {
	u16 limit;
	u32 base;
} __attribute__((packed))
gdt_ptr;

struct {
	u32 prev_tss;
	u32 esp0;
	u32 ss0;
	u32 esp1;
	u32 ss1;
	u32 esp2;
	u32 ss2;
	u32 cr3;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	u32 es;
	u32 cs;
	u32 ss;
	u32 ds;
	u32 fs;
	u32 gs;
	u32 ldt;
	u16 trap;
	u16 iomap_base;
} __attribute__((packed))
tss;

void gdt_init() {
	gdt_ptr.limit = sizeof(gdt) - 1;
	gdt_ptr.base  = (u32) &gdt;

	// null entry
	gdt_set_gate(0, 0, 0, 0, 0);
	// kernel code
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	// kernel data
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	// user code
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	// user data
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	// TSS
	memset(&tss, 0, sizeof(tss));
	tss.ss0  = 0x10;
	tss.esp0 = 0x0;
	gdt_set_gate(5, (u32) &tss, (u32) &tss + sizeof(tss), 0xE9, 0x00);

	gdt_load((u32) &gdt_ptr);

	tss_load();
}

void gdt_set_gate(u32 n, u32 base, u32 limit, u8 access, u8 flag) {
	gdt[n].limit_low             = (limit & 0x0000FFFF);
	gdt[n].base_low              = (base  & 0x0000FFFF);
	gdt[n].base_middle           = (base  & 0x00FF0000) >> 16;
	gdt[n].access                = access;
	gdt[n].limit_high_and_flags  = (limit & 0x000F0000) >> 16;
	gdt[n].limit_high_and_flags |= (flag & 0xF0);
	gdt[n].base_high             = (base  & 0xFF000000) >> 24;
}
