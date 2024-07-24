#include "SHT40.h"

uint8_t writeData[1] = {0xFD};
uint8_t readData[6] = {0};

double Temperature = 0;
double Humidity = 0;

uint16_t Temp = 0;
uint16_t Humi = 0;

void SHT40_Get(void)
{
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)SHT40_Write, (uint8_t *)writeData, 1, HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(&hi2c1, (uint16_t)SHT40_Read, (uint8_t *)readData, 6, HAL_MAX_DELAY);
	
	Temperature = (1.0 * 175 * (readData[0] * 256 + readData[1])) / 65535.0 - 45;
	Humidity = (1.0 * 125 * (readData[3] * 256 + readData[4])) / 65535.0 - 6.0;
	
	Temp = (uint16_t)(Temperature * 10);
	Humi = (uint16_t)(Humidity * 10);

}

