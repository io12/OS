; elf_test.asm ;

global _start
_start:
mov	eax, 0xDEADBEEF
int	10
jmp	$
