.PHONY: all clean run

all: ./build/os.bin

./build/boot.bin: ./src/bootloader/bootloader.asm
	mkdir -p build
	nasm -f bin $< -o $@

./build/load_kernel.o: ./src/kernel/load_kernel.asm
	mkdir -p build
	nasm -f elf32 -g $< -o $@

./build/asm.o: ./src/libs/asm.asm
	mkdir -p build
	nasm -f elf32 -g $< -o $@

./build/keyboard_int_handler_asm.o: ./src/drivers/keyboard/keyboard_handler.asm
	mkdir -p build
	nasm -f elf32 -g $< -o $@

./build/kernel.o: ./src/kernel/kernel.c
	mkdir -p build
	clang -target i386-unknown-none-elf -ffreestanding -nostdlib -mno-red-zone -c $< -o $@

./build/idt.o: ./src/kernel/idt.c
	mkdir -p build
	clang -target i386-unknown-none-elf -ffreestanding -nostdlib -mno-red-zone -c $< -o $@

./build/print.o: ./src/libs/print.c
	mkdir -p build
	clang -target i386-unknown-none-elf -ffreestanding -nostdlib -mno-red-zone -c $< -o $@

./build/keyboard_int_handler.o: ./src/drivers/keyboard/keyboard.c
	mkdir -p build
	clang -target i386-unknown-none-elf -ffreestanding -nostdlib -mno-red-zone -c $< -o $@
		
./build/kernel.bin: ./build/load_kernel.o ./build/kernel.o ./build/asm.o ./build/keyboard_int_handler_asm.o ./build/keyboard_int_handler.o ./build/idt.o ./build/print.o
	ld.lld -m elf_i386 -T ./src/linker.ld $^ -o ./build/kernel.elf
	llvm-objcopy -O binary ./build/kernel.elf $@

./build/os.bin: ./build/boot.bin ./build/kernel.bin
	cat ./build/boot.bin ./build/kernel.bin > $@
	dd if=/dev/zero of=$@ bs=512 seek=15 count=1 conv=notrunc

run: ./build/os.bin
	qemu-system-x86_64 -hda ./build/os.bin

clean:
	rm -rf ./build/*
