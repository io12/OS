#!/bin/sh

# Run this after "make qemu"

gdb kernel.elf -ex "target remote :1234"

exit 0
