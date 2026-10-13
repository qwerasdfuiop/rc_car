#ifndef __MAX7219_H
#define __MAX7219_H

void max7219_send(uint8_t addr, uint8_t data);

void max7219_init(void);

void display_voltage_level(uint8_t level);

#endif
