/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Leds.h"
#include "SHT40.h"
#include "Voltage.h"
#include "Key.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t buttonPressTime = 0;

struct
{
    uint8_t Key;
	uint8_t TempHumi;
	uint8_t Voltage;
    uint8_t Sleep;
}Status;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */

  //使数码管全部熄灭
    HAL_Delay(100);
	SN74HC595_Send_Data(SN_DIG,0x00);
	SN74HC595_Send_Data(Temp_LED,0x00);
	SN74HC595_Send_Data(Humi_LED,0x00);
    
    Status.TempHumi = 0;
    Status.Voltage = 0;
    Status.Key = 0;
    Status.Sleep = 0;
    
    uint32_t ADC_Value = 0;
	float Data = 0;
	uint16_t Vol_Value = 0;

  	HAL_SuspendTick();	//暂停滴答定时器，防止通过滴答定时器中断唤醒
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); /* 执行WFI指令, 进入睡眠模式 */
    
    HAL_GPIO_WritePin(Test_GPIO_GPIO_Port,Test_GPIO_Pin,1);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(Status.TempHumi == 1 && Status.Voltage == 0)
    {
        if(Status.Key == 1)
        {
            Status.Key = 0;
            SHT40_Get();
        }
        ShowNum(Temp_LED,1,Temp/100);
        ShowNum(Temp_LED,2,Temp/ 10 % 10);
        ShowNum(Temp_LED,3,Temp%10);
        ShowNum(Humi_LED,1,Humi/100);
        ShowNum(Humi_LED,2,Humi/ 10 % 10);
        ShowNum(Humi_LED,3,Humi%10);
    }
    else if(Status.TempHumi == 0 && Status.Voltage == 1)
    {
        if(Status.Key == 1)
        {
            Status.Key = 0;
            HAL_ADC_Start(&hadc1);     //启动ADC转换
            HAL_ADC_PollForConversion(&hadc1, 50);   //等待转换完成，50为最大等待时间，单位为ms
            if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
            {
                ADC_Value = HAL_ADC_GetValue(&hadc1);   //获取AD值
                Data = (ADC_Value*3.3f)/4095.0f;
            }
        }
        Vol_Value = (uint16_t)(Data * 100)*2;
		ShowVol(1,1,Vol_Value / 100);
		ShowVol(1,2,Vol_Value /10 %10);
		ShowVol(1,3,Vol_Value % 10);
        ShowVol(2,1,Vol_Value / 100);
		ShowVol(2,2,Vol_Value /10 %10);
		ShowVol(2,3,Vol_Value % 10);
        
    }
    if(Status.Sleep == 1)
    {
        HAL_SuspendTick();	//暂停滴答定时器，防止通过滴答定时器中断唤醒
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); /* 执行WFI指令, 进入睡眠模式 */
        Status.Sleep = 0;
    }
     
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == WakeUp_Key_Pin)
    {
        HAL_ResumeTick();   //恢复滴答定时器
        if (HAL_GPIO_ReadPin(WakeUp_Key_GPIO_Port, WakeUp_Key_Pin) == GPIO_PIN_RESET) // 按钮按下时
        {
            buttonPressTime = HAL_GetTick(); // 记录按下时间
            Status.Key = 1;
//            HAL_GPIO_TogglePin(Test_GPIO_GPIO_Port, Test_GPIO_Pin); // 切换测试LED状态
        }
    }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == WakeUp_Key_Pin)
    {
        if (HAL_GPIO_ReadPin(WakeUp_Key_GPIO_Port, WakeUp_Key_Pin) == GPIO_PIN_SET) // 按钮松开时
        {
            uint32_t pressDuration = HAL_GetTick() - buttonPressTime;   // 记录按下持续时间
//            HAL_GPIO_TogglePin(Test_GPIO_GPIO_Port, Test_GPIO_Pin); // 例：切换一个LED
            
            if (pressDuration >= LONG_PRESS_THRESHOLD)// 长按操作
            {
//                HAL_GPIO_TogglePin(Test_GPIO_GPIO_Port, Test_GPIO_Pin); // 切换测试LED状态
                HAL_TIM_Base_Start_IT(&htim14);    //开始定时器
                Status.Voltage = 1;
                
            }
            else if (pressDuration >= SHORT_PRESS_THRESHOLD)// 短按操作
            {
//                HAL_GPIO_TogglePin(Test_GPIO_GPIO_Port, Test_GPIO_Pin); // 例：切换另一个LED的状态
                
                Status.TempHumi = 1;
//                SHT40_Get();
                HAL_TIM_Base_Start_IT(&htim14);    //开始定时器
            }
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM14)
	{
        static uint16_t updata_flag = 0;
		updata_flag++;
        if(updata_flag == 3000)
		{
//            HAL_GPIO_TogglePin(Test_GPIO_GPIO_Port,Test_GPIO_Pin);
            updata_flag= 0;
            Status.TempHumi = 0;
            Status.Voltage = 0;
            Status.Sleep = 1;   
            HAL_TIM_Base_Stop_IT(&htim14);  //关闭定时器
            
        }
	}
//    HAL_TIM_Base_Start_IT(&htim14);    
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
