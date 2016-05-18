AS = nasm
CFLAGS = -m32 -nostdlib -nostdinc -ffreestanding -Wall -Wextra -Werror -O3 -Isrc/libk

C_FILES = $(shell find src/ -type f -name "*.c")
H_FILES = $(shell find src/ -type f -name "*.h")
ASM_FILES = $(shell find src/ -type f -name "*.asm")
C_OBJS = $(C_FILES:.c=.o)
ASM_OBJS = $(ASM_FILES:.asm=.o)
OBJS = $(C_OBJS) $(ASM_OBJS)
LINKER_SCRIPT = link.ld

.PHONY = all clean qemu

all: OS.iso

OS.iso: kernel.elf
	cp kernel.elf iso/boot/
	cp kernel.elf iso/efi.img
	grub-mkrescue iso/ -o $@

kernel.elf: $(OBJS) $(LINKER_SCRIPT)
	ld -T $(LINKER_SCRIPT) -melf_i386 $(OBJS) -o $@

%.o: %.asm
	$(AS) $^ -f elf32

%.o: %.c | $(H_FILES)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) kernel.elf iso/boot/kernel.elf iso/efi.img OS.iso

qemu: OS.iso
	qemu-system-i386 -cdrom $^
