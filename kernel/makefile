CC = gcc
CFLAGS =  -Wall -Wextra
CFLAGS += -nostdinc -nostdlib
OBJS = objs/main.o objs/entry.o

all: kernel.elf

run: kernel.elf
	mv $< ../root/
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -m 4G -drive format=raw,file=fat:rw:../root

objs/%.o: %.asm
	nasm -f elf64 -o $@ $<

objs/%.o: %.c
	$(CC) $(CFLAGS) -I includes -c -o $@ $<

objs/%.o: %.cpp
	g++ $(CFLAGS) -I includes -c -o $@ $<

kernel.elf: $(OBJS)
	ld.lld --entry KernelEntry -z norelro --image-base 0x100000 --static -o $@ $+

install: kernel.elf
	mv $< ../root/
	cp -r ../root/* /mnt/usb/

clean:
	rm -rf objs/*.o