; boot.asm ;

MAGIC_NUMBER	equ 0x1BADB002
FLAGS 		equ 0x0
CHECKSUM	equ -MAGIC_NUMBER

section .multiboot
align 4
dd	MAGIC_NUMBER
dd	FLAGS
dd	CHECKSUM

section .bss
align 4
kernel_stack_bottom:
resb	4096
kernel_stack_top:

section .text
global boot_main
boot_main:
mov	eax, 0xB002ED
mov	esp, kernel_stack_top
extern kmain
call	kmain
cli

.loop:
hlt
jmp	.loop
