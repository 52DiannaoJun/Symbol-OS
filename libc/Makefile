CFLAGS += -Wall -Wno-format -Wno-unused
CFLAGS += -std=gnu99 -static -m32
CFLAGS += -I./include -Wno-unused-parameter
CFLAGS += -ffunction-sections -nostdlib -nostdinc -fno-builtin -ffreestanding
CFLAGS += -fno-pie
CC	:= $(GCCPREFIX)gcc
com:
	rm -rf */*.bin
	$(CC) $(CFLAGS) -c stdio/stdio.c -o stdio/stdio.bin
	$(CC) $(CFLAGS) -c string/string.c -o string/string.bin
	