all:
	make build
	make run

build:
	make clean
	mkdir -p kernel/obj kernel/bin iso/boot/grub
	nasm -f elf32 kernel/src/asm/boot.s -o kernel/obj/boot.o
	gcc -m32 -ffreestanding -c kernel/src/c/kernel.c -o kernel/obj/kernel.o
	ld -m elf_i386 -T kernel/src/link.ld -o kernel/bin/kernel.bin kernel/obj/boot.o kernel/obj/kernel.o

	cp kernel/bin/kernel.bin iso/boot
	cp boot/grub/grub.cfg iso/boot/grub/
	grub-mkrescue -o iso/os.iso iso/

clean:
	rm -rf iso/os.iso
	rm -rf kernel/obj/*
	rm -rf kernel/bin/*
run:
	qemu-system-i386 -cdrom iso/os.iso
