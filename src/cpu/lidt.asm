; lidt.asm ;

global lidt
lidt:
mov	eax, [esp + 4]
lidt	[eax]
ret
