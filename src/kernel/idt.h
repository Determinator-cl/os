#include <stdint.h>

#pragma once

#define IWC_1 0x11

#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0

#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1

#define IRQ_0 0x20
#define IRQ_8 0x28

typedef void (*int_handler_t)();  // int_handler_t - указатель на функцию void(void)

// Дескриптор прерывания
typedef struct {
    uint16_t offset_1;  // Первые 16 бит адреса функции
    uint16_t selector;  // Селектор сегмента кода в GDT
    uint8_t null;       // Так вилел создатель
    uint8_t flags;      // Флаги
    uint16_t offset_2;  // Последние 16 бит адреса функции
} __attribute__((packed)) int_descriptor_t;

// IDTR
typedef struct {
    uint16_t limit;  // Размер IDT
    uint32_t base;   // Указатель на начало IDT
} __attribute__((packed)) idtr_t;

void idt_init();
void irq_mask();
void pic_remap();
void pic_send_eoi();
void register_int_handler(uint8_t vector, int_handler_t hand_address);
