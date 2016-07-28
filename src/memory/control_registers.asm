; control_registers.asm ;

global cr0_read
cr0_read:
mov	eax, cr0
ret

global cr0_write
cr0_write:
mov	eax, [esp + 4]
mov	cr0, eax
ret

global cr3_read
cr3_read:
mov	eax, cr3
ret

global cr3_write
cr3_write:
mov	eax, [esp + 4]
mov	cr3, eax
ret

global invlpg
invlpg:
invlpg	[esp + 4]
ret
