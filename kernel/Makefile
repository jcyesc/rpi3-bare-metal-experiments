SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -mcpu=cortex-a53
CROSS_COMPILER = /opt/arm/linaro/gcc/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-elf/bin/aarch64-elf

all: kernel8.img

start.o: start.s
	$(CROSS_COMPILER)-gcc $(CFLAGS) -c start.s -o start.o

interrupts.o: interrupts.s
	$(CROSS_COMPILER)-gcc $(CFLAGS) -c interrupts.s -o interrupts.o

init_registers.o: init_registers.s
	$(CROSS_COMPILER)-gcc $(CFLAGS) -c init_registers.s -o init_registers.o

%.o: %.c
	$(CROSS_COMPILER)-gcc $(CFLAGS) -c $< -o $@

kernel8.img: start.o interrupts.o init_registers.o $(OBJS)
	$(CROSS_COMPILER)-ld -nostdlib -nostartfiles start.o interrupts.o init_registers.o $(OBJS) -T kernel.ld -o kernel8.elf
	$(CROSS_COMPILER)-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.img kernel8.elf *.o


