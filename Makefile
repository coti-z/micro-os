CC = gcc
LD = ld
AS = as

CFLAGS = -std=c99 -ffreestanding -O2 -Wall -Wextra -m64 -nostdlib -fno-stack-protector -I/home/coti/git/micro/src/include
ASFLAGS = --64
LDFLAGS = -T linker.ld -m elf_x86_64

BUILD_DIR = build

OBJ = \
	$(BUILD_DIR)/boot.o \
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/idt.o \
	$(BUILD_DIR)/exception.o \
	$(BUILD_DIR)/exception_asm.o \
	$(BUILD_DIR)/timer.o \
	$(BUILD_DIR)/keyboard.o \
	$(BUILD_DIR)/pic.o \
	$(BUILD_DIR)/irq_asm.o \
	$(BUILD_DIR)/memory.o \
	$(BUILD_DIR)/spinlock.o \
	$(BUILD_DIR)/process.o \
	$(BUILD_DIR)/scheduler.o \
	$(BUILD_DIR)/swtch.o \
	$(BUILD_DIR)/process_1.o \
	$(BUILD_DIR)/process_2.o \
	$(BUILD_DIR)/serial.o \
	$(BUILD_DIR)/printf.o \
	$(BUILD_DIR)/panic.o

KERNEL = iso/boot/kernel.elf
ISO = micro.iso

.PHONY: all clean qemu qemu-debug

all: $(ISO)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/boot/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: src/kernel/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: src/kernel/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: src/io/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

iso/boot/grub/grub.cfg: grub.cfg
	cp grub.cfg iso/boot/grub/grub.cfg

$(ISO): $(KERNEL) iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso/

clean:
	rm -rf $(BUILD_DIR) $(KERNEL) $(ISO)
	rm -f iso/boot/grub/grub.cfg

qemu: $(ISO)
	qemu-system-x86_64 -boot d -cdrom $(ISO) -serial stdio

qemu-debug: $(ISO)
	qemu-system-x86_64 -boot d -cdrom $(ISO) -serial stdio -s -S
