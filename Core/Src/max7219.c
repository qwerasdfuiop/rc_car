#include "main.h"
#include <math.h>

extern SPI_HandleTypeDef hspi1;

void max7219_send(uint8_t addr, uint8_t data)
{
    uint8_t tx[2] = { addr, data };
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx, 2, 1000);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void max7219_init(void) // FUNC 설정
{
	  max7219_send(0x0F, 0x00);
	  max7219_send(0x0C, 0x01);
	  max7219_send(0x0B, 0x07);
	  max7219_send(0x0A, 0x08);
	  max7219_send(0x09, 0x00);

	  for (uint8_t r=1; r<=8; ++r){
		  max7219_send(r, 0x00);
	  }
}

void display_voltage_level(uint8_t level)
{
	uint8_t data = pow(2,level) - 1;

    for (uint8_t r=4; r<=5; ++r){
	    max7219_send(r, data);
    }
}
