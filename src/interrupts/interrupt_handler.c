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

IRQ_Routine irq_routines[16] = {NULL};

// next two functions are called on interrupt

void isr_handler(InterruptSave is) {
	if (is.int_num < ISR_COUNT) {
		kprintf(PL_ALL,
				"%s exception\n"
				"PANIC\n"
				"interrupt number: 0x%X\n"
				"error code: 0x%X\n"
				"\n"
				"eax=0x%X ebx=0x%X ecx=0x%X edx=0x%X\n"
				"esp=0x%X ebp=0x%X esi=0x%X edi=0x%X\n"
				"cs=0x%X ds=0x%X ss=0x%X es=0x%X fs=0x%X gs=0x%X\n"
				"eip=0x%X\n",
				exception_messages[is.int_num],
				is.int_num,
				is.err,
				is.eax, is.ebx, is.ecx, is.edx,
				is.esp, is.ebp, is.esi, is.edi,
				is.cs, is.ds, is.ss, is.es, is.fs, is.gs,
				is.eip);
		permahalt();
		// NOTREACHED
	}
}

void irq_handler(InterruptSave is) {
	IRQ_Routine routine = irq_routines[is.int_num - 32];

	kprintf(PL_SERIAL, "IRQ interrupt %u\n", is.int_num);
	if (routine != NULL) {
		kprintf(PL_SERIAL, "Callback is registered. Running.\n");
		routine(&is);
	}
	out(0x20, 0x20);
	if (is.int_num >= 40) {
		out(0xA0, 0x20);
	}
}

void irq_new_routine(u8 n, IRQ_Routine routine) {
	irq_routines[n] = routine;
}

void irq_remove_routine(u8 n) {
	irq_routines[n] = NULL;
}
