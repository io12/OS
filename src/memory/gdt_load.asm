; gdt_load.asm ;

global gdt_load
gdt_load:
; load the GDT
mov	eax, [esp + 4]
lgdt	[eax]
; set segment registers to 0x10
mov	ax, 0x10
mov	ds, ax
mov	es, ax
mov	fs, ax
mov	gs, ax
mov	ss, ax
; set the cs register to 0x08
jmp	0x08:.reload_cs
.reload_cs:
ret

global tss_load
tss_load:
; load the TSS
mov	ax, 0x2B
ltr	ax
ret
