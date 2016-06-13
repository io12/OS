AS        = nasm
CFLAGS    = -m32 -ffreestanding -nostdlib -nostdinc
CFLAGS   += -Wall -Wextra -O0 -g
CFLAGS   += -Isrc/libk/include -Isrc/include

C_FILES   = $(shell find src/ -type f -name "*.c")
H_FILES   = $(shell find src/ -type f -name "*.h")
ASM_FILES = $(shell find src/ -type f -name "*.asm")
# ensure kmain is the first function (for debugging)
C_OBJS    = src/kmain.o $(filter-out src/kmain.o,$(C_FILES:.c=.o))
ASM_OBJS  = $(ASM_FILES:.asm=.o)
OBJS      = $(C_OBJS) $(ASM_OBJS)
LINKER    = link.ld

.PHONY: all clean qemu debug

all: OS.iso

OS.iso: kernel.elf
	cp kernel.elf iso/boot/
	cp kernel.elf iso/efi.img
	grub-mkrescue iso/ -o $@

kernel.elf: $(OBJS) $(LINKER)
	ld -T $(LINKER) -melf_i386 $(OBJS) -o $@

%.o: %.asm
	$(AS) $^ -f elf32

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) kernel.elf iso/boot/kernel.elf iso/efi.img OS.iso serial.out

qemu: OS.iso
	qemu-system-i386 -serial file:serial.out -cdrom OS.iso &

debug: kernel.elf
	qemu-system-i386 -serial file:serial.out -kernel kernel.elf -s -S &
