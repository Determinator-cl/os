#include <stddef.h>
#include <stdint.h>

#pragma once

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

extern size_t term_row;
extern size_t term_column;
extern size_t cursor_y;
extern size_t cursor_x;
extern uint8_t term_color;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
void term_put_entry_at(char c, uint8_t color, size_t x, size_t y);
void set_cursor_position(uint8_t x, uint8_t y);
void term_set_color(uint8_t color);
void print_char(char c);
void print_str(char* str);
void clear();
