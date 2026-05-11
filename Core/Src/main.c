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


int main(void)
{
	configure_PLL(0,160);
	SWITCH_CLOCKS(1);


	TURN_AHB1_GPIO_CLOCK_ON();
	input_mode(GPIOC, 13);
	//output_mode(GPIOA, 5);
	output_mode(GPIOC, 0);
	output_mode(GPIOC,3);

	//Enable_SysTick_Interrupt();

	START_DAC();

	int Scaler_1024n =2048;
	int clicked_flag = 0;
	while (1)
	{
	  if (clicked_flag==0 && get_input(GPIOC, 13) == 0){
		  DAC->DHR12R2=(uint16_t) Scaler_1024n;

		  Scaler_1024n+=256;
		  if (Scaler_1024n > 4096){
			  Scaler_1024n = 2048;
		  }

		  clicked_flag = 1;
	  }

	  if (clicked_flag == 1 && get_input(GPIOC, 13)==1){
		  clicked_flag = 0;
	  }
	}

}



