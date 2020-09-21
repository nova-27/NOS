CC = x86_64-w64-mingw32-gcc
CFLAGS =  -Wall -Wextra
CFLAGS += -nostdlib
CFLAGS += -Wl,--subsystem,10
OBJS = objs/main.o objs/efi.o objs/utils.o

all: main.efi

run: main.efi
	mv $< ../root/EFI/BOOT/BOOTX64.EFI
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -m 4G -drive format=raw,file=fat:rw:../root

objs/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.efi: $(OBJS)
	$(CC) $(CFLAGS) -e efiMain -o $@ $+

install: main.efi
	mv $< ../root/EFI/BOOT/BOOTX64.EFI
	cp -r ../root/* /mnt/usb/

clean:
	rm -rf objs/*.o