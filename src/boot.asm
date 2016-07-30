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
resb	8192
global kernel_stack_high
kernel_stack_high:

section .text
; Entry point
global bmain
bmain:
; bochs magic breakpoint
xchg	bx, bx
; set the stack pointer
mov	esp, kernel_stack_high
; disable interrupts before interrupt handling is in place
cli
; push multiboot header pointer
push	ebx
; push multiboot header magic
push	eax
; call the C function kmain
extern kmain
call	kmain
; kmain is not supposed to return, but all code below acts as a safety net if it does.
; disable interrupts
cli
; Halt processor until an interrupt
hlt
; If by some miracle the execution continues, jump to current line forever.
; Lots of care is taken to ensure execution doesn't fly off the edge of the RAM.
jmp	$
