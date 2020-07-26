CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra -nostdinc -nostdlib

all: main.efi

run: main.efi
	mv $< root/EFI/BOOT/BOOTX64.EFI
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive format=raw,file=fat:rw:root

main.efi: main.c
	$(CC) $(CFLAGS)  -Wl,--subsystem,10 -e efi_main -o $@ $<

install: main.efi
	mv $< root/EFI/BOOT/BOOTX64.EFI
	cp -r root/* /mnt/usb/
