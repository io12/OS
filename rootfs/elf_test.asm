global _start
_start:
xchg	bx, bx
mov	eax, 0xDEADBEEF
push	eax
pop	ebx
jmp	$
