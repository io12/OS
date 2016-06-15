/* idt.c */

#include <ints.h>
#include <kprintf.h>
#include <string.h>

#include <system.h>

void _isr0();  void _isr1();  void _isr2();  void _isr3();
void _isr4();  void _isr5();  void _isr6();  void _isr7();
void _isr8();  void _isr9();  void _isr10(); void _isr11();
void _isr12(); void _isr13(); void _isr14(); void _isr15();
void _isr16(); void _isr17(); void _isr18(); void _isr19();
void _isr20(); void _isr21(); void _isr22(); void _isr23();
void _isr24(); void _isr25(); void _isr26(); void _isr27();
void _isr28(); void _isr29(); void _isr30(); void _isr31();

void _irq0();  void _irq1();  void _irq2();  void _irq3();
void _irq4();  void _irq5();  void _irq6();  void _irq7();
void _irq8();  void _irq9();  void _irq10(); void _irq11();
void _irq12(); void _irq13(); void _irq14(); void _irq15();

void idt_load(u32 idt_ptr);
void idt_set_gate(u8 n, u32 base, u16 selector, u8 flags);

struct {
	u16 base_low;
	u16 selector;
	u8  zero;
	u8  flags;
	u16 base_high;
} __attribute__((packed))
idt[256];

struct {
	u16 limit;
	u32 base;
} __attribute__((packed))
idt_ptr;

void idt_init() {
	idt_ptr.limit = sizeof(idt) - 1;
	idt_ptr.base  = (u32) &idt;
	memset(idt, 0, sizeof(idt));

	// initialize PIC
	io_out(0x20, 0x11);
	io_out(0xA0, 0x11);
	io_out(0x21, 0x20);
	io_out(0xA1, 0x28);
	io_out(0x21, 0x04);
	io_out(0xA1, 0x02);
	io_out(0x21, 0x01);
	io_out(0xA1, 0x01);
	io_out(0x21, 0x00);
	io_out(0xA1, 0x00);

	// ISRs
	idt_set_gate(0,  (u32) _isr0,  0x08, 0x8E);
	idt_set_gate(1,  (u32) _isr1,  0x08, 0x8E);
	idt_set_gate(2,  (u32) _isr2,  0x08, 0x8E);
	idt_set_gate(3,  (u32) _isr3,  0x08, 0x8E);
	idt_set_gate(4,  (u32) _isr4,  0x08, 0x8E);
	idt_set_gate(5,  (u32) _isr5,  0x08, 0x8E);
	idt_set_gate(6,  (u32) _isr6,  0x08, 0x8E);
	idt_set_gate(7,  (u32) _isr7,  0x08, 0x8E);
	idt_set_gate(8,  (u32) _isr8,  0x08, 0x8E);
	idt_set_gate(9,  (u32) _isr9,  0x08, 0x8E);
	idt_set_gate(10, (u32) _isr10, 0x08, 0x8E);
	idt_set_gate(11, (u32) _isr11, 0x08, 0x8E);
	idt_set_gate(12, (u32) _isr12, 0x08, 0x8E);
	idt_set_gate(13, (u32) _isr13, 0x08, 0x8E);
	idt_set_gate(14, (u32) _isr14, 0x08, 0x8E);
	idt_set_gate(15, (u32) _isr15, 0x08, 0x8E);
	idt_set_gate(16, (u32) _isr16, 0x08, 0x8E);
	idt_set_gate(17, (u32) _isr17, 0x08, 0x8E);
	idt_set_gate(18, (u32) _isr18, 0x08, 0x8E);
	idt_set_gate(19, (u32) _isr19, 0x08, 0x8E);
	idt_set_gate(20, (u32) _isr20, 0x08, 0x8E);
	idt_set_gate(21, (u32) _isr21, 0x08, 0x8E);
	idt_set_gate(22, (u32) _isr22, 0x08, 0x8E);
	idt_set_gate(23, (u32) _isr23, 0x08, 0x8E);
	idt_set_gate(24, (u32) _isr24, 0x08, 0x8E);
	idt_set_gate(25, (u32) _isr25, 0x08, 0x8E);
	idt_set_gate(26, (u32) _isr26, 0x08, 0x8E);
	idt_set_gate(27, (u32) _isr27, 0x08, 0x8E);
	idt_set_gate(28, (u32) _isr28, 0x08, 0x8E);
	idt_set_gate(29, (u32) _isr29, 0x08, 0x8E);
	idt_set_gate(30, (u32) _isr30, 0x08, 0x8E);
	idt_set_gate(31, (u32) _isr31, 0x08, 0x8E);

	// IRQs
	idt_set_gate(32, (u32) _irq0,  0x08, 0x8E);
	idt_set_gate(33, (u32) _irq1,  0x08, 0x8E);
	idt_set_gate(34, (u32) _irq2,  0x08, 0x8E);
	idt_set_gate(35, (u32) _irq3,  0x08, 0x8E);
	idt_set_gate(36, (u32) _irq4,  0x08, 0x8E);
	idt_set_gate(37, (u32) _irq5,  0x08, 0x8E);
	idt_set_gate(38, (u32) _irq6,  0x08, 0x8E);
	idt_set_gate(39, (u32) _irq7,  0x08, 0x8E);
	idt_set_gate(40, (u32) _irq8,  0x08, 0x8E);
	idt_set_gate(41, (u32) _irq9,  0x08, 0x8E);
	idt_set_gate(42, (u32) _irq10, 0x08, 0x8E);
	idt_set_gate(43, (u32) _irq11, 0x08, 0x8E);
	idt_set_gate(44, (u32) _irq12, 0x08, 0x8E);
	idt_set_gate(45, (u32) _irq13, 0x08, 0x8E);
	idt_set_gate(46, (u32) _irq14, 0x08, 0x8E);
	idt_set_gate(47, (u32) _irq15, 0x08, 0x8E);

	idt_load((u32) &idt_ptr);
}

void idt_set_gate(u8 n, u32 base, u16 selector, u8 flags) {
	idt[n].base_low  = (base & 0x0000FFFF);
	idt[n].selector  = selector;
	idt[n].zero      = 0;
	idt[n].flags     = flags | 0x60;
	idt[n].base_high = (base & 0xFFFF0000) >> 16;
}
