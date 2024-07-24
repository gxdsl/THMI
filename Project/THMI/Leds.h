#ifndef __LEDS_H__
#define __LEDS_H__

#include "gpio.h"
#include <stdint.h>
#include <main.h>

#define Humi_LED		0x01
#define Temp_LED		0x02
#define SN_DIG		0x03

void SN74HC595_Send_Data(unsigned char sn_num,unsigned int sendValue);
void ShowNum(uint8_t row, uint8_t column, uint8_t value);
void ShowVol(uint8_t row, uint8_t column, uint8_t value);

#endif
