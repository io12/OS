%macro ISR_ERRCODE 1
global _isr%1
_isr%1:
cli
push	%1
jmp	isr_common_stub
%endmacro

%macro ISR_NOERRCODE 1
global _isr%1
_isr%1:
cli
push	0
push	%1
jmp	isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; system call interrupt
ISR_NOERRCODE 186

isr_common_stub:
; save edi, esi, ebp, esp, ebx, edx, ecx, eax
pusha
; save segment descriptors
push	ds
push	es
push	fs
push	gs
; set segment descriptors to kernel data
mov	ax, 0x10
mov	ds, ax
mov	es, ax
mov	fs, ax
mov	gs, ax
; call a C function that handles the exception
extern isr_handler
mov	eax, isr_handler
call	eax
; restore segment descriptors
pop	gs
pop	fs
pop	es
pop	ds
; restore other registers
popa
; restore stack
add	esp, 8
; finish
sti
iret
