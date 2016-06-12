; irq.asm ;

%macro	IRQ 1
global	_irq%1
_irq%1:
cli
push	byte 0
push	byte 32
jmp	irq_common_stub
%endmacro

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

extern	irq_handler
irq_common_stub:
pusha
push	ds
push	es
push	fs
push	gs
mov	ax, 0x10
mov	ds, ax
mov	es, ax
mov	fs, ax
mov	gs, ax
mov	eax, irq_handler
call	eax
pop	gs
pop	fs
pop	es
pop	ds
popa
add	esp, 8
iret
