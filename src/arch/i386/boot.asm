; boot.asm ;

MAGIC_NUMBER	equ 0x1BADB002
FLAGS 		equ 0x0
CHECKSUM	equ -MAGIC_NUMBER

KERNEL_STACK_SIZE equ 4096

section .bss
align 4
kernel_stack:
resb	KERNEL_STACK_SIZE

section .text
align 4
dd	MAGIC_NUMBER
dd	FLAGS
dd	CHECKSUM

global boot_main
boot_main:
mov	eax, 0xB002ED
mov	esp, kernel_stack + KERNEL_STACK_SIZE
extern kmain
call	kmain

.loop:
jmp	.loop
