AS        = nasm
ASFLAGS   = -f elf32
CFLAGS    = -m32 -ffreestanding -nostdlib -nostdinc
CFLAGS   += -Wall -Wextra -std=gnu99 -O3 -g
CFLAGS   += -Isrc/libk/include -Isrc/include

ISO       = OS.iso
KERNEL    = kernel.elf
RAMDISK   = cdrom/rootfs.img
C_FILES   = $(shell find src/ -type f -name "*.c")
H_FILES   = $(shell find src/ -type f -name "*.h")
ASM_FILES = $(shell find src/ -type f -name "*.asm")
C_OBJS    = $(C_FILES:.c=.o)
ASM_OBJS  = $(ASM_FILES:.asm=.o)
OBJS      = $(C_OBJS) $(ASM_OBJS)
LINKER    = link.ld

.PHONY: all clean qemu debug

all: $(ISO)

$(ISO): $(KERNEL) $(RAMDISK)
	cp $(KERNEL) cdrom/boot/
	cp $(KERNEL) cdrom/efi.img
	grub-mkrescue --output=$@ cdrom/

$(KERNEL): $(OBJS) $(LINKER)
	ld -T $(LINKER) -melf_i386 $(OBJS) -o $@

$(RAMDISK): rootfs/ rootfs/*
	genext2fs -d rootfs/ -U -b 100 -N 100 $@

%.o: %.asm
	$(AS) $(ASFLAGS) $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) $(KERNEL) \
		cdrom/boot/$(KERNEL) cdrom/efi.img $(ISO) \
		serial.out bochslog.txt bx_enh_dbg.ini \
		$(RAMDISK)

qemu: $(ISO)
	qemu-system-i386 -serial file:serial.out -cdrom $(ISO) &

bochs: $(ISO)
	bochs -f bochsrc.txt -q &

debug: $(ISO)
	qemu-system-i386 -serial file:serial.out -cdrom $(ISO) -s -S &
