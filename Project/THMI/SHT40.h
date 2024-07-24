#ifndef __SHT40_H__
#define __SHT40_H__

#include "i2c.h"

#define SHT40_Write (0x44<<1)   	//д���ַ
#define SHT40_Read  ((0x44<<1)+1)   //������ַ

extern uint16_t Temp,Humi;

void SHT40_Get(void);

#endif
