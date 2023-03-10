# path
TEMP	:= ./temp
ifndef GCCPATH
GCCPATH :=
endif

# compiler
AS	:= nasm
CC	:= $(GCCPATH)gcc
LD	:= $(GCCPATH)ld
MAKE:= make

# args
CFLAGS := -Wall -Wno-format -Wno-unused  -static -m32 -I./libc/include -Wno-unused-parameter -ffunction-sections -nostdlib -nostdinc -fno-builtin -ffreestanding -fno-pie
ifdef DBG
CFLAGS += -g
endif

# C Library Objects
L_OBJS  := libc/*/*.bin
# Kernel Objects
K_OBJS	:= kernel.bin func.bin font.bin int.bin
OBJS    := $(L_OBJS) $(K_OBJS)

# file
SYS	:= OS.sys
IMG	:= OS.img


default: compile

com: compile
compile: clean all
all: ${OBJS} OS.sys image

clean:
	rm -rf *.bin
	rm -rf *.sys
	rm -rf *.obj
	rm -rf *.lst
	rm -rf $(IMG)
.PHONY:
	all

ipl.bin:
	$(AS) -f bin ipl.asm -o ipl.bin

loader.bin:
	$(AS) -f bin loader.asm -o loader.bin

int.bin:
	$(AS) -f elf ./library/int.asm -o int.bin

font:
	$(AS) -f elf font.asm -o font.bin

font.bin:
	$(MAKE) font


%.bin: %.asm
	$(AS) -f elf $< -o $@

%.bin: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.sys: ${K_OBJS} ${L_OBJS}
	$(LD) -m elf_i386 --oformat binary -o kernel.sys -T kernel.ld $^

OS.sys: loader.bin kernel.sys
	cat loader.bin > OS.sys
	cat kernel.sys >> OS.sys

image: ipl.bin OS.sys
	dd if=/dev/zero of=$(IMG) bs=512 count=2880
	dd if=ipl.bin of=$(IMG) seek=0 bs=512 count=1 conv=notrunc
	dd if=./libc/fat.bin of=$(IMG) seek=1 bs=512 conv=notrunc
	dd if=./libc/root.bin of=$(IMG) seek=24 bs=512 conv=notrunc
	dd if=OS.sys of=$(IMG) seek=38 bs=512 conv=notrunc
