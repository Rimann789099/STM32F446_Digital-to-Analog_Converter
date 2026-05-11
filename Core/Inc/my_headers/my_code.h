/*
 * my_code.h
 *
 *  Created on: Nov 17, 2024
 *      Author: luciuss
 */



#ifndef INC_MY_HEADERS_MY_CODE_H_
#define INC_MY_HEADERS_MY_CODE_H_

//====== BINARY OPERATIONS =======
//It is hard to do them without MACROSES
//void clear_bits(int number)

#include "stm32f446xx.h"


//======= CLOCK CONFIG ===========
void SWITCH_CLOCKS(int clock_num);
void configure_PLL(int hsi_hse_base, uint8_t plln_scaler);

void configure_APB1_prescaler(int div);
void configure_APB2_prescaler(int div);

void SET_APB1_PRESCALER(int freq_MHz);

// Configuration of Clock Signal (Taktirovanie) for AHB1 peripherals
void TURN_AHB1_GPIO_CLOCK_ON(void);


// Timers
TIM1_enable();




// SysTick_Interrupt
void Enable_SysTick_Interrupt(void);

void SysTick_reset_value(int Scaler);


// ======= GPIO CONFIG ===========

void GPIO_CLOCK_ON();

void output_mode(GPIO_TypeDef *gpio, int n);
void input_mode(GPIO_TypeDef *gpio, int n);
void analog_mode(GPIO_TypeDef *gpio , int n);

void set_output(GPIO_TypeDef *gpio, int n, int state);
int check_output(GPIO_TypeDef *gpio, int n);
int get_input(GPIO_TypeDef *gpio, int n);


// ========== ADC DAC =============
void START_DAC(void);


#endif /* INC_MY_HEADERS_MY_CODE_H_ */
