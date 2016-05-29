AS = nasm
CFLAGS = -m32 -nostdlib -nostdinc -ffreestanding -Wall -Wextra -O2 -Isrc/libk -Isrc/include -g

C_FILES = $(shell find src/ -type f -name "*.c")
H_FILES = $(shell find src/ -type f -name "*.h")
ASM_FILES = $(shell find src/ -type f -name "*.asm")
C_OBJS = $(C_FILES:.c=.o)
ASM_OBJS = $(ASM_FILES:.asm=.o)
OBJS = $(C_OBJS) $(ASM_OBJS)
LINKER_SCRIPT = link.ld

.PHONY = all clean qemu

all: OS.iso

OS.iso: kernel.elf $(H_FILES)
	cp kernel.elf iso/boot/
	cp kernel.elf iso/efi.img
	grub-mkrescue iso/ -o $@

kernel.elf: $(OBJS) $(LINKER_SCRIPT)
	ld -T $(LINKER_SCRIPT) -melf_i386 $(OBJS) -o $@

%.o: %.asm
	$(AS) $^ -f elf32

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) kernel.elf iso/boot/kernel.elf iso/efi.img OS.iso serial.out

qemu: kernel.elf
	qemu-system-i386 -serial file:serial.out -kernel kernel.elf -s &
