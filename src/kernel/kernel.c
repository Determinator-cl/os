#include "../libs/print.h"
#include "idt.h"

void kernel_main() {
    idt_init();
    clear();

    term_setcolor(vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
    print_str("Hello world!\n");
    print_str("Hello world!\r");
    print_str("Hallo world!");

    while (1);
}
