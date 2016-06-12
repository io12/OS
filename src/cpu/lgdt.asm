; lgdt.asm ;

global	lgdt
extern	gdt_ptr
lgdt:
; load the GDT
lgdt	[gdt_ptr]
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
