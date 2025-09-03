# i know the comments are bad, i will improve them later

# variables
ISO = iso/os.iso # for example this is used in "build: $(ISO)", basically a shortcut 
KERNEL = kernel/bin/kernel.bin
OBJS = kernel/obj/boot.o kernel/obj/kernel.o

$(KERNEL): $(OBJS)
	ld -m elf_i386 -T kernel/src/link.ld -o $@ $^ # linker script, necessary for the OS to work

kernel/obj/boot.o: kernel/src/asm/boot.s
	mkdir -p kernel/obj
	nasm -f elf32 $< -o $@

# this compiles kernel.c file into a kernel.o file 
kernel/obj/kernel.o: kernel/src/c/kernel.c
	gcc -m32 -ffreestanding -fno-stack-protector -c $< -o $@ 

# build
build: $(ISO) # the "$(ISO) points to iso/os.iso
$(ISO): $(KERNEL) boot/grub/grub.cfg
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	cp boot/grub/grub.cfg iso/boot/grub/
	grub-mkrescue -o $@ iso

run: $(ISO)
	qemu-system-i386 -cdrom $<

clean:
	rm -rf kernel/obj kernel/bin $(ISO) # cleans files as the name says
