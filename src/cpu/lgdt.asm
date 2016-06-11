; lgdt.asm ;

global lgdt
lgdt:
; get address of GDT from argument
mov	eax, [esp + 4]
; load the GDT
lgdt	[eax]
; set the cs register to 0x08
jmp	0x08:.reload_cs
.reload_cs:
; set all other segment registers to 0x10
mov	ax, 0x10
mov	ds, ax
mov	es, ax
mov	fs, ax
mov	gs, ax
mov	ss, ax
ret
