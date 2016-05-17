AS = nasm
CFLAGS = -m32 -nostdlib -ffreestanding -Wall -Wextra -Werror -O3

OBJS = src/arch/i386/boot.o src/kmain.o src/framebuffer.o
LINKER_SCRIPT = link.ld

.PHONY = all clean qemu

all: OS.iso

OS.iso: kernel.elf
	cp kernel.elf iso/boot/
	cp kernel.elf iso/efi.img
	grub-mkrescue iso/ -o $@

kernel.elf: $(OBJS) $(LINKER_SCRIPT)
	ld -T $(LINKER_SCRIPT) -melf_i386 $(OBJS) -o $@

src/arch/i386/boot.o: src/arch/i386/boot.asm
	$(AS) $^ -f elf32

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) kernel.elf iso/boot/kernel.elf iso/efi.img OS.iso

qemu: OS.iso
	qemu-system-i386 -cdrom $^
