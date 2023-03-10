CC		:= gcc
AS		:= nasm
TMP		:= ./temp
LD		:= ld
GCCFG	:= -Wall -Wno-format -Wno-unused
GCCFG	+= -std=gnu99 -static -m32
#GCCFG	+= -I./libc/include
GCCFG	+= -ffunction-sections -nostdlib -nostdinc -fno-builtin -ffreestanding
GCCFG	+= -fno-pie

asm.bin:
	$(AS) -f elf ./lib/*.asm -o asm.bin -l $(TMP)/asm.lst

c.bin:
	$(CC) $(GCCFG) -c ./lib/*.c -o c.bin
	
com: clean app 
#
app: c.bin asm.bin
	$(CC) $(GCCFG) -c cmd.c -o cmd.bin
	$(LD) -m elf_i386 --oformat binary -o cmd.app -T application.ld cmd.bin c.bin asm.bin
clean:
	rm -rf *.bin
	rm -rf *.app