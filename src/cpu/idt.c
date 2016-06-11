/* idt.c */

#include <ints.h>
#include <string.h>

#include <idt.h>

#define ISR_NAME(X) _isr##X
#define ISRS        32

typedef void (*IDTbase)();

void _isr0();
void _isr1();
void _isr2();
void _isr3();
void _isr4();
void _isr5();
void _isr6();
void _isr7();
void _isr8();
void _isr9();
void _isr10();
void _isr11();
void _isr12();
void _isr13();
void _isr14();
void _isr15();
void _isr16();
void _isr17();
void _isr18();
void _isr19();
void _isr20();
void _isr21();
void _isr22();
void _isr23();
void _isr24();
void _isr25();
void _isr26();
void _isr27();
void _isr28();
void _isr29();
void _isr30();
void _isr31();

struct {
	u16 base_low;
	u16 selector;
	u8  zero;
	u8  flags;
	u16 base_high;
} __attribute__((packed))
idt[256];

struct {
	u16   limit;
	void* base;
} __attribute__((packed))
idt_desc;

const char* exception_messages[ISRS] = {
	"Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

static void idt_set_gate(u8 num, u32 base, u16 selector, u8 flags) {
	idt[num].base_low = (base & 0x0000FFFF);
	idt[num].selector = selector;
	idt[num].zero     = 0;
	idt[num].flags    = flags;
	idt[num].base_low = (base & 0xFFFF0000) >> 16;
}

void idt_init() {
	idt_desc.limit = sizeof(idt) - 1;
	idt_desc.base  = idt;
	memset(idt, 0, sizeof(idt));

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

	lidt((void*) &idt_desc);
}
