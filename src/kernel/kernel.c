#include "idt.h"

volatile unsigned short* vga_buffer = (volatile unsigned short*)0xB8000;

void print_char(int x, int y, char c, unsigned char color) {
    int index = y * 80 + x;
    vga_buffer[index] = ((unsigned short)color << 8) | c;
}

void kernel_main() {
    idt_init();

    print_char(0, 0, 'O', 0x0A);
    print_char(1, 0, 'K', 0x0A);
}
