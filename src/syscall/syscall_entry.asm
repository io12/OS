global syscall_entry
syscall_entry:
cli
pusha
extern syscall_handler
call	syscall_handler
popa
sti
iret
