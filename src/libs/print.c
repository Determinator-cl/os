#include <stddef.h>
#include <stdint.h>

#include "print.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t term_row;
size_t term_column;
uint8_t term_color;
uint16_t* term_buffer = (uint16_t*)VGA_MEMORY;

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) { return fg | bg << 4; }
uint16_t vga_entry(unsigned char uc, uint8_t color) { return (uint16_t)uc | (uint16_t)color << 8; }

void clear() {
    term_row = 0;
    term_column = 0;
    term_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            size_t idx = y * VGA_WIDTH + x;
            term_buffer[idx] = vga_entry(' ', term_color);
        }
    }
}

void term_setcolor(uint8_t color) { term_color = color; }

void term_put_entry_at(char c, uint8_t color, size_t x, size_t y) {
    const size_t idx = y * VGA_WIDTH + x;
    term_buffer[idx] = vga_entry(c, color);
}

void print_char(char c) {
    term_put_entry_at(c, term_color, term_column, term_row);
    if (++term_column == VGA_WIDTH) {
        term_column = 0;
        if (++term_row == VGA_HEIGHT) term_row = 0;
    }
}

void print_str(char* str) {
    size_t size = strlen(str);
    for (size_t i = 0; i < size; i++) print_char(str[i]);
}
