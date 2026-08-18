global keyboard_int_handler_asm

extern keyboard_int_handler

section .text
keyboard_int_handler_asm:
        pushad
        call keyboard_int_handler
        popad
        iret
