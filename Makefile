AS        = nasm
ASFLAGS   = -f elf32
CFLAGS    = -m32 -ffreestanding -nostdlib -nostdinc
CFLAGS   += -Wall -Wextra -O0 -g
CFLAGS   += -Isrc/libk/include -Isrc/include

ISO       = OS.iso
KERNEL    = kernel.elf
C_FILES   = $(shell find src/ -type f -name "*.c")
H_FILES   = $(shell find src/ -type f -name "*.h")
ASM_FILES = $(shell find src/ -type f -name "*.asm")
C_OBJS    = $(C_FILES:.c=.o)
ASM_OBJS  = $(ASM_FILES:.asm=.o)
OBJS      = $(C_OBJS) $(ASM_OBJS)
LINKER    = link.ld

.PHONY: all clean qemu debug

all: $(ISO)

$(ISO): $(KERNEL)
	cp $(KERNEL) iso/boot/
	cp $(KERNEL) iso/efi.img
	grub-mkrescue iso/ -o $@

$(KERNEL): $(OBJS) $(LINKER)
	ld -T $(LINKER) -melf_i386 $(OBJS) -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) $(KERNEL) iso/boot/$(KERNEL) iso/efi.img $(ISO) serial.out

qemu: $(ISO)
	qemu-system-i386 -serial file:serial.out -cdrom $(ISO) &

bochs: $(ISO)
	bochs -f bochsrc.txt -q

debug: $(KERNEL)
	qemu-system-i386 -serial file:serial.out -kernel $(KERNEL) -s -S &
