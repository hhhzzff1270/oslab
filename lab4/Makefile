CC = riscv64-unknown-elf-gcc
CFLAGS = -march=rv64gc -mabi=lp64d -nostdlib -fno-builtin -Wall -O2 -I. -I./include -mcmodel=medany
LDFLAGS = -T kernel/kernel.ld -nostdlib -lgcc

KERNEL_SRCS = \
    kernel/entry.S \
    kernel/uart.c \
    kernel/printf.c \
    kernel/main.c \
    kernel/console.c \
    kernel/pmm.c \
    kernel/vmm.c \
    kernel/trap.c \
    kernel/timer.c \
    kernel/kernelvec.S

KERNEL_OBJS = $(KERNEL_SRCS:.c=.o)
KERNEL_OBJS := $(KERNEL_OBJS:.S=.o)

.PHONY: all run debug clean

all: kernel.elf

kernel.elf: $(KERNEL_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(KERNEL_OBJS)
	@echo "Build successful!"
	@riscv64-unknown-elf-size kernel.elf

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) -march=rv64gc -mabi=lp64d -c -o $@ $<

run: kernel.elf
	@echo "Starting QEMU..."
	qemu-system-riscv64 -machine virt -nographic -bios none -kernel kernel.elf

debug: kernel.elf
	@echo "Starting QEMU in debug mode..."
	qemu-system-riscv64 -machine virt -nographic -bios none -kernel kernel.elf -s -S

clean:
	rm -f $(KERNEL_OBJS) kernel.elf