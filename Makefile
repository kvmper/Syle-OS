# i know the comments are bad, i will improve them later

# variables
ISO = build/os.iso # for example this is used in "build: $(ISO)", basically a shortcut
KERNEL = build/bin/kernel.bin
OBJS = build/obj/boot.o build/obj/kernel.o build/obj/ports.o build/obj/keyboard.o build/obj/print.o build/obj/video.o
CFLAGS = -Iinclude -ffreestanding


$(KERNEL): $(OBJS)
	ld -m elf_i386 -T kernel/src/link.ld -o $@ $^ # linker script, necessary for the OS to work

build/obj/boot.o: kernel/src/asm/boot.s
	mkdir -p build/obj
	nasm -f elf32 $< -o $@

# this compiles kernel.c file into a kernel.o file 
build/obj/kernel.o: kernel/src/c/kernel.c
	gcc -m32 $(CFLAGS) -fno-stack-protector -c $< -o $@

build/obj/ports.o: kernel/src/drivers/c/ports.c
	gcc -m32 $(CFLAGS) -fno-stack-protector -c $< -o $@

build/obj/keyboard.o: kernel/src/drivers/c/keyboard.c
	gcc -m32 $(CFLAGS) -fno-stack-protector -c $< -o $@

build/obj/print.o: kernel/src/c/print.c
	gcc -m32 $(CFLAGS) -fno-stack-protector -c $< -o $@

build/obj/video.o: kernel/src/c/video.c
	gcc -m32 $(CFLAGS) -fno-stack-protector -c $< -o $@

# build
build: $(ISO) # the "$(ISO) points to build/os.iso
$(ISO): $(KERNEL) #build/boot/grub/grub.cfg
	mkdir -p build/bin
	grub-mkrescue -o $@ build

run: $(ISO)
	qemu-system-i386 -cdrom $<

clean:
	rm -rf build/obj/* build/bin/* build/boot/kernel.bin $(ISO) # cleans files as the name says
