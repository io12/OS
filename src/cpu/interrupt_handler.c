/* interrupt_handler.c */

#include <defs.h>
#include <ints.h>
#include <kprintf.h>

#include <ioport.h>

#include <interrupt_handler.h>

#define ISR_COUNT 32

const char* exception_messages[ISR_COUNT] = {
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

IRQroutine irq_routines[16];

// next two functions are called on interrupt

void isr_handler(InterruptSave is) {
	if (is.int_num < ISR_COUNT) {
		kprintf(PL_FRAMEBUFFER, "%s exception\n", exception_messages[is.int_num]);
		for (;;) {}
	}
}

void irq_handler(InterruptSave is) {
	IRQroutine routine = irq_routines[is.int_num - 32];

	kprintf(PL_FRAMEBUFFER, "IRQ interrupt %d\n", is.int_num);
	if (routine != NULL) {
		kprintf(PL_FRAMEBUFFER, "Running routine\n");
		routine(is);
	}
	if (is.int_num >= 40) {
		io_out(0xA0, 0x20);
	}
	io_out(0x20, 0x20);
}

void irq_new_routine(u8 n, IRQroutine routine) {
	irq_routines[n] = routine;
}

void irq_remove_routine(u8 n) {
	irq_routines[n] = NULL;
}
