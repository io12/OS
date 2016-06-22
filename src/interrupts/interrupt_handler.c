/* interrupt_handler.c */

#include <ints.h>
#include <klibc.h>
#include <kprintf.h>
#include <system.h>

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

IRQroutine irq_routines[16] = {NULL};

// next two functions are called on interrupt

void isr_handler(InterruptSave is) {
	if (is.int_num < ISR_COUNT) {
		kprintf(PL_FRAMEBUFFER,
				"%s exception\n"
				"PANIC\n"
				"interrupt number: %X\n"
				"error code: %X\n"
				"\n"
				"eax=%X ebx=%X ecx=%X edx=%X\n"
				"esp=%X ebp=%X esi=%X edi=%X\n"
				"cs=%X ds=%X ss=%X es=%X fs=%X gs=%X\n"
				"eip=%X\n",
				exception_messages[is.int_num],
				is.int_num,
				is.err,
				is.eax, is.ebx, is.ecx, is.edx,
				is.esp, is.ebp, is.esi, is.edi,
				is.cs, is.ds, is.ss, is.es, is.fs, is.gs,
				is.eip);
		__asm__("cli");
		for (;;) {
			__asm__("hlt");
		}
	}
}

void irq_handler(InterruptSave is) {
	IRQroutine routine = irq_routines[is.int_num - 32];

	kprintf(PL_SERIAL, "IRQ interrupt %u\n", is.int_num);
	if (routine != NULL) {
		kprintf(PL_SERIAL, "Callback is registered. Running.\n");
		routine(is);
	}
	io_out(0x20, 0x20);
	if (is.int_num >= 40) {
		io_out(0xA0, 0x20);
	}
}

void irq_new_routine(u8 n, IRQroutine routine) {
	irq_routines[n] = routine;
}

void irq_remove_routine(u8 n) {
	irq_routines[n] = NULL;
}
