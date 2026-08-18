#include <stdint.h>

#include "../libs/asm.h"
#include "idt.h"

int_descriptor_t idt_table[256];
idtr_t idtr;

void int_disable() { asm("cli"); }
void int_enable() { asm("sti"); }

// Ремапит PIC куда надо
void pic_remap() {
    int_disable();

    // Переводит PIC Master и Slave в ожидание 4-х команд инициализации
    outb(PIC_1_CTRL, IWC_1);
    io_wait();
    outb(PIC_2_CTRL, IWC_1);
    io_wait();

    // Master PIC (IRQ 0..7): сопоставляется с векторами IRQ_0 ... IRQ_0 + 7
    // Slave PIC (IRQ 8..15): сопоставляется с векторами IRQ_8 ... IRQ_8 + 7
    outb(PIC_1_DATA, IRQ_0);
    io_wait();
    outb(PIC_2_DATA, IRQ_8);
    io_wait();

    // Указывает Master PIC, что Slave подключен к его линии IRQ2, а Slave PIC сообщает его
    // каскадный номер (2).
    outb(PIC_1_DATA, 0x04);
    io_wait();
    outb(PIC_2_DATA, 0x02);
    io_wait();

    // Переводит оба контроллера в режим работы с x86
    outb(PIC_1_DATA, 0x01);
    io_wait();
    outb(PIC_2_DATA, 0x01);
    io_wait();

    // Все IRQ в Master кроме IRR6 отключены
    // Все IRQ в Slave отключены
    outb(PIC_1_DATA, 0b11111101);
    io_wait();
    outb(PIC_2_DATA, 0xFF);
    io_wait();

    int_enable();
}

void idt_load() {
    uint32_t idt_row_count = sizeof(idt_table) / sizeof(idt_table[0]);
    idtr.base = (uint32_t)(&idt_table[0]);
    idtr.limit = (sizeof(int_descriptor_t) * idt_row_count) - 1;
    __asm__ volatile("lidt %0" : : "m"(idtr));
}

void register_int_handler(uint8_t vector, int_handler_t hand_address) {
    int_disable();

    uint32_t addr = (uint32_t)hand_address;

    idt_table[vector].offset_1 =
        (uint16_t)(addr & 0xFFFF);                // offset_1 получит первые 16 бит адреса функции
    idt_table[vector].selector = (uint16_t)0x08;  // Сегмент кода ядра
    idt_table[vector].null = (uint8_t)0x00;
    idt_table[vector].flags = (uint8_t)(0x80 | 0x0E);
    idt_table[vector].offset_2 =
        (uint16_t)(addr >> 16);  // offset_2 получит последние 16 бит адреса функции

    idt_load();

    int_enable();
}

void idt_init() {
    pic_remap();

    extern void keyboard_int_handler_asm();
    register_int_handler(0x21, &keyboard_int_handler_asm);
}
