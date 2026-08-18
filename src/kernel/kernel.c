#include "../libs/print.h"
#include "idt.h"

void kernel_main() {
    idt_init();
    clear();
    term_set_color(vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
}
