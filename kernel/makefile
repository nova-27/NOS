CC = gcc
CFLAGS =  -Wall -Wextra
CFLAGS += -nostdlib
OBJS = objs/main.o objs/entry.o objs/graphics.o objs/console.o objs/asmfunc.o objs/legacy/serial_port.o objs/legacy/kbc.o objs/segmentation.o objs/legacy/interrupt.o

all: kernel.elf

run: kernel.elf
	mv $< ../root/
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,readonly,file=../ovmf/OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=../ovmf/OVMF_VARS.fd \
		-m 4G \
		-drive format=raw,file=fat:rw:../root \
		-device nec-usb-xhci,id=xhci -device usb-mouse \
		-serial stdio

objs/%.o: %.asm
	nasm -f elf64 -o $@ $<

objs/%.o: %.c
	$(CC) $(CFLAGS) -I includes -I legacy/includes -c -o $@ $<

objs/%.o: %.cpp
	g++ $(CFLAGS) -I includes -I legacy/includes -c $< -o $@

kernel.elf: $(OBJS)
	ld.lld --entry kernelEntry -L/usr/lib/x86_64-linux-gnu/ -L/lib/x86_64-linux-gnu/ -L/usr/lib/gcc/x86_64-linux-gnu/8/ -lstdc++ -lgcc_s -lc -z norelro --image-base 0x100000 --static -o $@ $+
 
install: kernel.elf
	mv $< ../root/
	cp -r ../root/* /mnt/usb/

clean:
	rm -rf objs/*.o