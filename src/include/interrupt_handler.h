/* interrupt_handler.h */

#ifndef _INTERRUPT_HANDLER_H_
#define _INTERRUPT_HANDLER_H_

#include <ints.h>

// the state of the stack frame when an interrupt handler is called
typedef struct {
	// segment registers
	u32 gs;
	u32 fs;
	u32 es;
	u32 ds;
	// pusha registers
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 esp;
	u32 ebx;
	u32 edx;
	u32 ecx;
	u32 eax;
	// interrupt info
	u32 int_num;
	u32 err;
	// pushed automatically
	u32 eip;
	u32 cs;
	u32 eflags;
	u32 useresp;
	u32 ss;
} __attribute__((packed))
InterruptSave;

// a function pointer to an IRQ routine
typedef void (*IRQroutine)(InterruptSave is);

void irq_new_routine(u8 n, IRQroutine routine);
void irq_remove_routine(u8 n);

#endif
