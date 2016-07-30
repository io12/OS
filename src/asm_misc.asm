global sti
sti:
sti
ret

; this is called after all initialization
global int_wait_forever
int_wait_forever:
sti
.loop:
hlt
jmp	.loop

; unconditionally halt forever
global permahalt
permahalt:
cli
hlt

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
