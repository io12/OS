; lidt.asm ;

global	lidt
extern	idt_ptr
lidt:
lidt	[idt_ptr]
sti
ret
