/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "assignment.h"
#include "i2c.h"
#include "gpio.h"
#include "lis3mdltr.h"
#include "lsm6ds0.h"
#include "lps22hb.h"
#include "tim.h"
#include "display.h"
#include "hts221.h"
#include <stdio.h>
#include <string.h>

uint8_t mode = 0;

void SystemClock_Config(void);


float press, alt, temp, humidity;

int main(void)
{
  char str[4][11];
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);


  SystemClock_Config();


  MX_GPIO_Init();

  setSegments();
  setDigits();

  LL_mDelay(2000);

  resetDigits();
  resetSegments();

  MX_I2C1_Init();


  lsm6ds0_init();
  lps22hb_init();
  hts221_init();

  for(int i = 0; i<4; i++){
	  changeMode(str, i);
  }
  changeDisplayString(str[mode],strlen(str[mode]));

  MX_TIM2_Init();



  while (1)
  {
	  //os			   x      y        z
//	  lsm6ds0_get_acc(acc, (acc+1), (acc+2));

//	humidity = hts221_get_humidity();
//	press = lps22hb_get_press();
//  temp = lps22hb_get_temp();
//	alt = lps22hb_get_altitude();
	changeMode(str, mode);
	changeDisplayString(str[mode],strlen(str[mode]));
	LL_mDelay(500);

  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}


uint8_t checkButtonState(GPIO_TypeDef* PORT, uint8_t PIN, uint8_t edge, uint8_t samples_window, uint8_t samples_required)
{
	int counter = 0;

	for(int i = 0; i < samples_window; i++){
		if(LL_GPIO_IsInputPinSet(PORT, GPIO_BSRR_BS_3) == edge){
			counter += 1;
		}else{
			counter = 0;
		}

		if(counter >= samples_required){
			return 1;
		}

		LL_mDelay(1);
	}

	return 0;
}

void changeMode(char str[4][11], uint8_t mode)
{
	float tmp;

	switch(mode)
	{
		case 0: // teplota
			tmp = lps22hb_get_temp();
			if(tmp >= 100){
				tmp = 99.9;
			}
			if(tmp <= -100){
				tmp = -99.9;
			}
			if(tmp >= 0){
				sprintf(str[0], "teplota [°C]: TEMP_%04.1f",tmp);
			}else{
				sprintf(str[0], "teplota [°C]: TEMP_%05.1f",tmp);

			}
			break;
		case 1: // rel. vlhkost
			sprintf(str[1], "rel. vlhkosť [%]: HUM_%02d",(int)(hts221_get_humidity()*100));
			break;
		case 2: // tlak
			sprintf(str[2], "tlak vzduchu [hPa]: BAR_%06.1f",lps22hb_get_press());
			break;
		case 3: // nadmorska vyska
			sprintf(str[3], "relatívna výška od zeme [m]: ALT_%06.1f",lps22hb_get_altitude());
			break;
	}
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

void EXTI3_IRQHandler(void)
{
	if(checkButtonState(GPIO_PORT_BUTTON,
						GPIO_PIN_BUTTON,
						BUTTON_EXTI_TRIGGER,
						BUTTON_EXTI_SAMPLES_WINDOW,
						BUTTON_EXTI_SAMPLES_REQUIRED))
	{
		mode++;
		if(mode > 3) mode = 0;
		startNewStr();
	}
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
	  {
	    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
	  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
