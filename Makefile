NASM=nasm
CC=gcc
LD=ld
CFLAGS=-m32 -ffreestanding -O2 -fno-builtin -fno-stack-protector -Wall -Wextra -nostdinc
LDFLAGS=-m elf_i386

all: Os.iso

boot.o: boot.s
	$(NASM) -f elf32 boot.s -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin boot.o kernel.o

isofiles/boot/grub/grub.cfg: kernel.bin grub.cfg
	mkdir -p isofiles/boot/grub
	cp kernel.bin isofiles/boot/
	cp grub.cfg isofiles/boot/grub/

Os.iso: isofiles/boot/grub/grub.cfg
	grub-mkrescue -o Os.iso isofiles
	qemu-system-x86_64 Os.iso
	make clean
	clear

clean:
	rm -rf *.o kernel.bin isofiles