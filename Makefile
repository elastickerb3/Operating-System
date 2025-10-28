NASM=nasm
CC=gcc
G=g++
LD=ld
GFLAGS=-ffreestanding  -m32
CFLAGS=-m32 -ffreestanding -O2 -fno-builtin -fno-stack-protector -Wall -Wextra -nostdinc
LDFLAGS=-m elf_i386

all: Os.iso

boot.o: boot.s
	$(NASM) -f elf32 boot.s -o boot.o

Link.o:./kernel_assets/Link.cpp
	$(G) $(GFLAGS) -c ./kernel_assets/Link.cpp -o Link.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: boot.o kernel.o linker.ld Link.o
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin boot.o Link.o kernel.o 

isofiles/boot/grub/grub.cfg: kernel.bin grub.cfg
	mkdir -p isofiles/boot/grub
	sudo cp kernel.bin isofiles/boot/
	sudo cp grub.cfg isofiles/boot/grub/

Os.iso: isofiles/boot/grub/grub.cfg
	sudo grub-mkrescue -o ./Os.iso isofiles
	qemu-system-x86_64 Os.iso

make run: Os.iso
	make clean
	clear

clean:
	sudo rm -rf *.o kernel.bin isofiles