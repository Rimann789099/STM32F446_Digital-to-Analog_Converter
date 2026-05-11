/*
 * dac_adc.c
 *
 *  Created on: Nov 21, 2024
 *      Author: luciuss
 */

#include "main.h"
#include "my_headers/my_code.h"



void configure_timer6_for_dac(void){
	// Connect Timer with APB1 clock
	RCC->APB1ENR|=RCC_APB1ENR_TIM6EN;

	//auto-reset of ARR
	TIM6->CR1|= TIM_CR1_ARPE;
	//Pre-scaler is PSC_num + 1
	TIM6->PSC=(uint16_t) 3;
	TIM6->ARR= (uint16_t) 10;
	TIM6->CR1|=TIM_CR1_CEN;

	//Interrupt update
	TIM6->DIER|=TIM_DIER_UIE;

	// Trigger on update
	TIM6->CR2&=~(TIM_CR2_MMS_Msk);
	TIM6->CR2|=TIM_CR2_MMS_1;

}



void configure_gpio_for_dac(int dac_n){
	///
	GPIO_CLOCK_ON();
	if (dac_n == 1){
		analog_mode(GPIOA, 4);
	} else {
		analog_mode(GPIOA, 5);
	}

}

void configure_dac(void){
	// Connect DAC to RCC
	RCC->APB1ENR|=RCC_APB1ENR_DACEN;

	// Turn ON TRGO  (for DAC channel 2)
	// + Configure trigger channel to TIM6
	DAC->CR|=DAC_CR_TEN2;
	DAC->CR &=~(DAC_CR_TSEL2_Msk);
	DAC->CR |= (0b000)<<DAC_CR_TSEL2_Pos;  // TIM6 channel

	//Turn on Buffer: bit 1->0
	DAC->CR&=~(DAC_CR_BOFF2);


}

void start_dac(void){
	configure_gpio_for_dac(2);
	configure_timer6_for_dac();
	configure_dac();

	DAC->CR|=DAC_CR_EN2;
	// 12 bit right HOLDING REGISTER

	DAC->DHR12R2|=(uint16_t) 2048;
}
