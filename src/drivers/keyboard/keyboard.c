#include "../../libs/asm.h"
#include "../../libs/print.h"
#include "keyboard.h"

uint8_t keyboard_shift_state = 0;

char buffer[256] = {0};
uint8_t buffer_pos = 0;

const char keyboard_layout[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7',  '8', '9', '0',  '-',  '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',  'p', '[', ']',  '\n', 0,   'a',  's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z',  'x', 'c',  'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,    ' ', 0,   0,    0,    0,   0,    0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8',  '9', '-', '4',  '5',  '6', '+',  '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0};

const char keyboard_layout_shift[128] = {
    0,   27,  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',  '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,   'A',  'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,   '|', 'Z',  'X', 'C',  'V',
    'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,    0,   0,    0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5',  '6', '+',  '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,    0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,    0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,    0};

char scancode_to_char(uint8_t scancode) {
    if (scancode >= 128) return 0;

    if (keyboard_shift_state) {
        return keyboard_layout_shift[scancode];
    } else {
        return keyboard_layout[scancode];
    }
}

void keyboard_int_handler() {
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {                           // Клавиша отпущена
        if (scancode == 0xAA || scancode == 0xB6) {  // Shift отпущен
            keyboard_shift_state = 0;
        }
    } else {                                         // Клавиша нажата
        if (scancode == 0x2A || scancode == 0x36) {  // Shift нажат
            keyboard_shift_state = 1;
        } else if (scancode == 0x0E) {  // Backspace
            if (cursor_x > 0) {
                cursor_x--;
            } else if (cursor_y > 0) {
                cursor_y--;
                cursor_x = 80 - 1;
            } else {
                outb(0x20, 0x20);
                return;
            }

            term_put_entry_at(' ', term_color, cursor_x, cursor_y);
            set_cursor_position(cursor_x, cursor_y);
        } else {
            char ch = scancode_to_char(scancode);

            if (ch != 0) {
                print_char(ch);
            }
        }
    }

    outb(0x20, 0x20);
}
