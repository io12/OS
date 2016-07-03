; asm_misc.asm ;

; next 3 are interrupt instruction wrappers

global cli
cli:
cli
ret

global sti
sti:
sti
ret

global hlt
hlt:
hlt
ret

; unconditionally halt forever
global permahalt
permahalt:
cli
.loop:
hlt
jmp	.loop
ret

; wrapper around the "out" asm instruction
global out
out:
; load I/O port address into dx
mov	dx, [esp + 4]
; load data into al
mov	al, [esp + 8]
; send data to I/O port
out	dx, al
ret

; similar to above, but for "in"
global in
in:
mov	dx, [esp + 4]
in	al, dx
ret
