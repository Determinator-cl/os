#include <stdint.h>

#pragma once

extern void io_wait();

extern void outb(uint16_t dest, uint8_t val);
extern void outw(uint16_t dest, uint16_t val);

extern uint8_t inb(uint16_t dest);
extern uint16_t inw(uint16_t dest);
