#include "Leds.h"

//0~9，外加一个小数点
uint16_t sgh_value[11]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x80};

void SN74HC595_Send_Data(unsigned char sn_num,unsigned int sendValue)
{
	uint8_t i = 0;
	if(sn_num == Humi_LED)	//第一个数码管
	{
		for(i = 0;i < 8;i++)
		{
			if(((sendValue << i)&0x80)!=0)	//如果数据第一位是1
			{
				HAL_GPIO_WritePin(LED1_SER_GPIO_Port,LED1_SER_Pin,GPIO_PIN_SET);		//高电平
			}
			else
			{
				HAL_GPIO_WritePin(LED1_SER_GPIO_Port,LED1_SER_Pin,GPIO_PIN_RESET);	//低电平
			}
			HAL_GPIO_WritePin(LED1_SCLK_GPIO_Port,LED1_SCLK_Pin,GPIO_PIN_RESET);	//产生一个SCLK上升沿
			HAL_GPIO_WritePin(LED1_SCLK_GPIO_Port,LED1_SCLK_Pin,GPIO_PIN_SET);
			}
		HAL_GPIO_WritePin(LED1_RCLK_GPIO_Port,LED1_RCLK_Pin,GPIO_PIN_RESET);	//产生一个RCLK上升沿
		HAL_GPIO_WritePin(LED1_RCLK_GPIO_Port,LED1_RCLK_Pin,GPIO_PIN_SET);
	}
	else if(sn_num == Temp_LED)
	{
		for(i = 0;i < 8;i++)
		{
			if(((sendValue << i)&0x80)!=0)
			{
				HAL_GPIO_WritePin(LED2_SER_GPIO_Port,LED2_SER_Pin,GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(LED2_SER_GPIO_Port,LED2_SER_Pin,GPIO_PIN_RESET);
			}
			HAL_GPIO_WritePin(LED2_SCLK_GPIO_Port,LED2_SCLK_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_SCLK_GPIO_Port,LED2_SCLK_Pin,GPIO_PIN_SET);
			}
		HAL_GPIO_WritePin(LED2_RCLK_GPIO_Port,LED2_RCLK_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_RCLK_GPIO_Port,LED2_RCLK_Pin,GPIO_PIN_SET);	
	}
	else if(sn_num == SN_DIG)
	{
		for(i = 0;i < 8;i++)
		{
			if(((sendValue << i)&0x80)!=0)
			{
				HAL_GPIO_WritePin(DIG_SER_GPIO_Port,DIG_SER_Pin,GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(DIG_SER_GPIO_Port,DIG_SER_Pin,GPIO_PIN_RESET);
			}
			HAL_GPIO_WritePin(DIG_SCLK_GPIO_Port,DIG_SCLK_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DIG_SCLK_GPIO_Port,DIG_SCLK_Pin,GPIO_PIN_SET);
			}
		HAL_GPIO_WritePin(DIG_RCLK_GPIO_Port,DIG_RCLK_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_RCLK_GPIO_Port,DIG_RCLK_Pin,GPIO_PIN_SET);	
	}
}

void ShowNum(uint8_t row, uint8_t column, uint8_t value)
{
	if(row == 1)
	{
		switch(column)
		{
			case 1:	//如果是第一排第一个
					SN74HC595_Send_Data(SN_DIG,0xFE);
					SN74HC595_Send_Data(Humi_LED,sgh_value[value]);//显示值对应16进制数
				break;
			case 2:
					SN74HC595_Send_Data(SN_DIG,0xFD);
					SN74HC595_Send_Data(Humi_LED,(sgh_value[value]|0x80));
				break;
			case 3:
					SN74HC595_Send_Data(SN_DIG,0xFB);
					SN74HC595_Send_Data(Humi_LED,sgh_value[value]);
				break;
			default:
				break;
		}
		SN74HC595_Send_Data(Humi_LED,0x00);	//消影，防止错位
	}
	else
	{
		switch(column)
		{
			case 1:
					SN74HC595_Send_Data(SN_DIG,0xF7);
					SN74HC595_Send_Data(Temp_LED,sgh_value[value]);
				break;
			case 2:
					SN74HC595_Send_Data(SN_DIG,0xEF);
					SN74HC595_Send_Data(Temp_LED,(sgh_value[value]|0x80));
				break;
			case 3:
					SN74HC595_Send_Data(SN_DIG,0xDF);
					SN74HC595_Send_Data(Temp_LED,sgh_value[value]);
				break;
			default:
				break;
		}
		SN74HC595_Send_Data(Temp_LED,0x00);
	}
}

void ShowVol(uint8_t row, uint8_t column, uint8_t value)
{
	if(row == 1)
	{
		switch(column)
		{
			case 1:	//如果是第一排第一个
					SN74HC595_Send_Data(SN_DIG,0xFE);
					SN74HC595_Send_Data(1,sgh_value[value]|0x80);//显示值对应16进制数
				break;
			case 2:
					SN74HC595_Send_Data(SN_DIG,0xFD);
					SN74HC595_Send_Data(1,(sgh_value[value]));
				break;
			case 3:
					SN74HC595_Send_Data(SN_DIG,0xFB);
					SN74HC595_Send_Data(1,sgh_value[value]);
				break;
			default:
				break;
		}
		SN74HC595_Send_Data(1,0x00);	//消影，防止错位
	}
	else
	{
		switch(column)
		{
			case 1:
					SN74HC595_Send_Data(SN_DIG,0xF7);
					SN74HC595_Send_Data(2,sgh_value[value]|0x80);
				break;
			case 2:
					SN74HC595_Send_Data(SN_DIG,0xEF);
					SN74HC595_Send_Data(2,(sgh_value[value]));
				break;
			case 3:
					SN74HC595_Send_Data(SN_DIG,0xDF);
					SN74HC595_Send_Data(2,sgh_value[value]);
				break;
			default:
				break;
		}
		SN74HC595_Send_Data(2,0x00);
	}
}





