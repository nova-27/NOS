CC = gcc
CFLAGS =  -Wall -Wextra
CFLAGS += -nostdlib
OBJS = objs/main.o objs/entry.o objs/graphics.o objs/console.o objs/asmfunc.o objs/segmentation.o

all: kernel.elf

run: kernel.elf
	mv $< ../root/
	qemu-system-x86_64 \
		-bios /usr/share/ovmf/OVMF.fd \
		-m 4G \
		-drive format=raw,file=fat:rw:../root \
		-device nec-usb-xhci,id=xhci -device usb-mouse -device usb-kbd

objs/%.o: %.asm
	nasm -f elf64 -o $@ $<

objs/%.o: %.c
	$(CC) $(CFLAGS) -I includes -c -o $@ $<

objs/%.o: %.cpp
	clang++ -I includes -c $< -o $@

kernel.elf: $(OBJS)
	ld.lld --entry KernelEntry -L/lib/x86_64-linux-gnu/ -lstdc++ -lgcc_s -z norelro --image-base 0x100000 --static -o $@ $+
 
install: kernel.elf
	mv $< ../root/
	cp -r ../root/* /mnt/usb/

clean:
	rm -rf objs/*.o