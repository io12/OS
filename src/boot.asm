; boot.asm ;

; Define constants for the multiboot header.
; MAGIC_NUMBER + FLAGS + CHECKSUM must equal 0 for the header to be valid.
MAGIC_NUMBER	equ 0x1BADB002
FLAGS 		equ 11b
CHECKSUM	equ -(MAGIC_NUMBER + FLAGS)

; Declare the multiboot header.
; The linker script makes sure this is at the beginning of the executable.
section .multiboot
align 4
dd	MAGIC_NUMBER
dd	FLAGS
dd	CHECKSUM

section .bss
align 4
; Allocate space for a stack (required to run C code)
KSTACK_SIZE	equ 8192
kernel_stack:
resb	KSTACK_SIZE

section .text
; Entry point
global	bmain
bmain:
; Set the stack pointer.
mov	esp, kernel_stack + KSTACK_SIZE
; Disable interrupts.
cli
; No more assembly is needed to run C code.
; Call the C function kmain.
extern	kmain
call	kmain
; kmain is not supposed to return, but all code below acts as a safety net if it does.
; Disable interrupts
cli
; Halt processor until an interrupt
hlt
; If by some miracle the execution continues, jump to current line forever.
; Lots of care is taken to ensure execution doesn't fly off the edge of the RAM.
jmp	$
