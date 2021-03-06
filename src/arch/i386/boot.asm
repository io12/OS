; boot.asm ;

; Define constants for the multiboot header.
; MAGIC_NUMBER + FLAGS + CHECKSUM must equal 0 for the header to be valid.
MAGIC_NUMBER	equ 0x1BADB002
FLAGS 		equ 0x0
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
kernel_stack_bottom:
resb	4096
kernel_stack_top:

section .text
; Entry point
global boot_main
boot_main:
; Set the stack pointer.
mov	esp, kernel_stack_top
; No more assembly is needed to run C code.
; Call the C function kmain.
extern kmain
call	kmain
; kmain is not supposed to return, but all code below acts as a safety net if it does.
; Disable interrupts
cli

.loop:
; Halt processor until an interrupt
; Interrupts were disabled before, so this should halt forever.
hlt
; If by some miracle the execution continues, jump back to halt again.
; Otherwise execution would fly off the edge of the RAM.
jmp	.loop
