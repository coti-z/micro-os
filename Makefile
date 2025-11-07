CC = gcc
LD = ld
AS = as

CFLAGS = -std=c99 -ffreestanding -O2 -Wall -Wextra -m64 -nostdlib
ASFLAGS = --64
LDFLAGS = -T linker.ld -m elf_x86_64

OBJ = src/boot.o src/kernel.o
KERNEL = iso/boot/kernel.elf
ISO = micro.iso

.PHONY: all clean qemu

all: $(ISO)

src/%.o: src/%.s
	$(AS) $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

iso/boot/grub/grub.cfg: grub.cfg
	cp grub.cfg iso/boot/grub/grub.cfg

$(ISO): $(KERNEL) iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso/

clean:
	rm -f src/*.o $(KERNEL) $(ISO)
	rm -f iso/boot/grub/grub.cfg

qemu: $(ISO)
	qemu-system-x86_64 -boot d -cdrom $(ISO) 

qemu-debug: $(ISO)
	qemu-system-x86_64 -boot d -cdrom $(ISO) -s -S 
