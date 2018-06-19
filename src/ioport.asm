; ioport.asm ;

; This function is just a wrapper around the "out" asm instruction
global io_out
io_out:
; load I/O port address into dx
mov	dx, [esp + 4]
; load data into al
mov	al, [esp + 8]
; send data to I/O port
out	dx, al
ret
