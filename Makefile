CC = riscv64-unknown-elf-gcc
LD = riscv64-unknown-elf-ld
OBJCOPY = riscv64-unknown-elf-objcopy
CFLAGS = -march=rv64gc -mabi=lp64d -nostdlib -fno-builtin -Wall -O2

KERNEL_SRCS = kernel/entry.S kernel/uart.c kernel/main.c
KERNEL_OBJS = $(KERNEL_SRCS:.c=.o)
KERNEL_OBJS := $(KERNEL_OBJS:.S=.o)

all: kernel.elf

kernel.elf: $(KERNEL_OBJS) kernel/kernel.ld
	$(CC) $(CFLAGS) -T kernel/kernel.ld -o $@ $(KERNEL_OBJS) -lgcc

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

run: kernel.elf
	qemu-system-riscv64 -machine virt -nographic \
		-bios none -kernel kernel.elf

debug: kernel.elf
	qemu-system-riscv64 -machine virt -nographic \
		-bios none -kernel kernel.elf -s -S

clean:
	rm -f $(KERNEL_OBJS) kernel.elf

.PHONY: all run debug clean