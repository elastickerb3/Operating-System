CXX = x86_64-w64-mingw32-g++

INCLUDES = -isystem /usr/include/efi -isystem /usr/include/efi/x86_64

# Wichtig: Wir fügen -fno-stack-protector hinzu, damit der __stack_chk_fail Fehler verschwindet
CXXFLAGS = -ffreestanding -fno-rtti -fno-exceptions -std=c++20 -Wall -Wextra -fshort-wchar -fno-stack-protector $(INCLUDES)
LDFLAGS = -nostdlib -shared -Wl,--subsystem,10 -e kernel_main

TARGET = BOOTX64.EFI
SRC = ./kernel/kernel.cpp # Ihr bereitgestellter Kernel-Code!
IMAGE = disk.img

OVMF_CODE = /usr/share/edk2/x64/OVMF_CODE.4m.fd
OVMF_VARS = /usr/share/edk2/x64/OVMF_VARS.4m.fd

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(IMAGE): $(TARGET)
	@echo "Erstelle virtuelles FAT32 Disk-Image..."
	dd if=/dev/zero of=$(IMAGE) bs=1M count=48 status=none
	mkfs.vfat -F 32 $(IMAGE) > /dev/null
	mmd -i $(IMAGE) ::/EFI
	mmd -i $(IMAGE) ::/EFI/BOOT
	mcopy -i $(IMAGE) $(TARGET) ::/EFI/BOOT/BOOTX64.EFI

run: $(IMAGE)
	@echo "Starte QEMU mit UEFI..."
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE) \
		-drive if=pflash,format=raw,readonly=on,file=$(OVMF_VARS) \
		-drive format=raw,file=$(IMAGE) \
		-net none \
		-vga std 
	make clean

clean:
	rm -f $(TARGET) $(IMAGE)

USB: $(IMAGE)
	sudo mount /dev/sda1 /home/elastickerb3/USB/
	sudo rm -rf /home/elastickerb3/USB/*
	sudo cp ./disk.img /home/elastickerb3/USB/
	sudo systemctl reboot --firmware-setup
