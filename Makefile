# variables
ISO = iso/os.iso
KERNEL = kernel/bin/kernel.bin
OBJS = kernel/obj/boot.o kernel/obj/kernel.o

$(KERNEL): $(OBJS)
	ld -m elf_i386 -T kernel/src/link.ld -o $@ $^

kernel/obj/boot.o: kernel/src/asm/boot.s
	mkdir -p kernel/obj
	nasm -f elf32 $< -o $@

kernel/obj/kernel.o: kernel/src/c/kernel.c
	gcc -m32 -ffreestanding -c $< -o $@

# build
build: $(ISO)
$(ISO): $(KERNEL) boot/grub/grub.cfg
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	cp boot/grub/grub.cfg iso/boot/grub/
	grub-mkrescue -o $@ iso

# run
run: $(ISO)
	qemu-system-i386 -cdrom $<

# clean
clean:
	rm -rf kernel/obj kernel/bin $(ISO)
