# .gdbinit #

# add "set auto-load safe-path /" to ~/.gdbinit for this to run

file kernel.elf
target remote :1234
wh
b kmain
c
