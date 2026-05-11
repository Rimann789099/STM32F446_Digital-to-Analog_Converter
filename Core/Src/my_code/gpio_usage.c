/*
 * gpio_usage.c
 *
 *  Created on: Nov 18, 2024
 *      Author: luciuss
 */

#include "main.h"
#include "my_headers/my_code.h"



void GPIO_CLOCK_ON(){
	TURN_AHB1_GPIO_CLOCK_ON();
}




void output_mode(GPIO_TypeDef *gpio , int n){
	gpio->MODER &= ~(0b11<<n*2);
	gpio->MODER |= (1<<n*2);
}

void input_mode(GPIO_TypeDef *gpio , int n){
	gpio->MODER &= ~(0b11<<n*2);
}

void analog_mode(GPIO_TypeDef *gpio , int n){
	gpio->MODER |= (0b11<<n*2);
}

void alternate_mode(GPIO_TypeDef *gpio ,int n, uint8_t alter_func_code){
	gpio->MODER &= ~(0b11<<n*2);
	gpio->MODER |= (0b10<<n*2);
	if (n<=7){
		gpio->AFR[0]&=~(0b1111<<n*4);
		// Choose AFx
		gpio->AFR[0]|=(alter_func_code<<n*4);
	} else {
		// High "x" GPIOx pins
		gpio->AFR[1]=~(0b1111<<(n-8)*4);
		// Choose AFx
		gpio->AFR[1]|=(alter_func_code<<(n-8)*4);
	}
}


void set_output(GPIO_TypeDef *gpio, int n, int state){
	if (state==0){
		gpio->ODR &= ~(1 << n);
	}
	if (state==1){
		gpio->ODR |= 1 << n;
	}
}


int check_output(GPIO_TypeDef *gpio,int n){
	return (gpio -> ODR & (1<<n))>>n;
}

int get_input(GPIO_TypeDef *gpio,int n){
	return (gpio -> IDR & (1<<n))>>n;
}










// =========== ALGORITHMS ============
void gpioa5_on_off_ALGO(void){
	int clicked_flag = 0;
	while (1)
	{
	  if (clicked_flag==0 && get_input(GPIOC, 13) == 0){
		  if (check_output(GPIOA, 5) == 0)
			  set_output(GPIOA, 5, 1);
		  else {
			  set_output(GPIOA,5,0);
		  }
		  clicked_flag = 1;
	  }

	  if (clicked_flag == 1 && get_input(GPIOC, 13)==1){
		  clicked_flag = 0;
	  }
	}
}
