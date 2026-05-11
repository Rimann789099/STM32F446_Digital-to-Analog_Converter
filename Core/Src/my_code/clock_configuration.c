#include "main.h"
#include "math.h"
#include "my_headers/my_code.h"





void SWITCH_CLOCKS(int clock_num){
	if (clock_num == 0){
		// hsi
		RCC->CR|=RCC_CR_HSION;
		while(!(RCC->CR & RCC_CR_HSIRDY));
		RCC->CR&=~(RCC_CFGR_SW_Msk);

	}
	if (clock_num == 1){
		RCC->CR|=RCC_CR_PLLON;
		while(!(RCC->CR & RCC_CR_PLLRDY));
		FLASH->ACR |= FLASH_ACR_LATENCY_3WS;

		RCC->CFGR&=~(RCC_CFGR_SW_Msk);
		RCC->CFGR|=RCC_CFGR_SW_PLL;
		while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	}

	//if (clock_num == 2){
	//	RCC->CR|=RCC_CR_HSEON;
	//	while(!(RCC->CR & RCC_CR_HSERDY));
	//	RCC->CFGR&=~(RCC_CFGR_SW_Msk);
	//	RCC->CFGR|=RCC_CFGR_SW_HSE;
	//}
}

void configure_PLL(int hsi_hse_base, uint8_t plln_scaler){
	// SWITCH HSI_HSE
	if (hsi_hse_base == 0){
		//hsi_on
		RCC->PLLCFGR&=~(RCC_PLLCFGR_PLLSRC_Msk);
	}
	if (hsi_hse_base == 1){
		//hse_on
		RCC->PLLCFGR|=RCC_PLLCFGR_PLLSRC_HSE;

	}

	//====== PLL CONFIG ======
	RCC->PLLCFGR&=~(RCC_PLLCFGR_PLLM_Msk);
	// set PLLM to /8    ==1/(2**3)
	RCC->PLLCFGR|=RCC_PLLCFGR_PLLM_3;

	RCC->PLLCFGR&=~(RCC_PLLCFGR_PLLN_Msk);
	//set PLLN to 160
	if (plln_scaler<=432){
		RCC->PLLCFGR|=plln_scaler<<RCC_PLLCFGR_PLLN_Pos;
	}
	// set PLLP to /2
	RCC->PLLCFGR&=~(0b11<<RCC_PLLCFGR_PLLP_Pos);



	// Slow Down APB1 to 40 MHz. pre-scaler ==/4
	configure_APB1_prescaler(4);

	// Slow Down APB2 to 80MHz. pre-scaler ==/2
	configure_APB2_prescaler(2);

}




// ======== RCC PRESCALERS =======
void configure_APB1_prescaler(int div){
	// Can DIV by 1, 2, 4, 8, 16
	RCC->CFGR&=~(RCC_CFGR_PPRE1_Msk);
	if (div == 2){
		RCC->CFGR|=RCC_CFGR_PPRE1_DIV2;
	}
	if (div == 4){
		RCC->CFGR|=RCC_CFGR_PPRE1_DIV4;
	}
	if (div == 8){
		RCC->CFGR|=RCC_CFGR_PPRE1_DIV8;
	}

	if (div == 16){
		RCC->CFGR|=RCC_CFGR_PPRE1_DIV16;
	}
}

void configure_APB2_prescaler(int div){
	// Can DIV by 1, 2, 4, 8, 16
	RCC->CFGR&=~(RCC_CFGR_PPRE2_Msk);
	if (div == 2){
		RCC->CFGR|=RCC_CFGR_PPRE2_DIV2;
	}
	if (div == 4){
		RCC->CFGR|=RCC_CFGR_PPRE2_DIV4;
	}
	if (div == 8){
		RCC->CFGR|=RCC_CFGR_PPRE2_DIV8;
	}

	if (div == 16){
		RCC->CFGR|=RCC_CFGR_PPRE2_DIV16;
	}
}




void TURN_AHB1_GPIO_CLOCK_ON(void){
	RCC->AHB1ENR|=0xF;
}


void SET_APB1_PRESCALER(int freq_MHz){

}





void Enable_SysTick_Interrupt(void){
	SysTick->CTRL|=(SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_TICKINT_Msk);

	// SysTick is HCLK/8 in CortexM4:  So if 16MHz -> SysTick is just 2MHz
	// Normal is 2000000UL

	SysTick->LOAD = (uint32_t)(200000UL - 1UL);
	SysTick->VAL = 0UL;

	NVIC_EnableIRQ(SysTick_IRQn);


}

/// -==== BADDD =====
void SysTick_reset_value(int Scaler){

	unsigned long frequency = 200000UL/Scaler;
	SysTick->LOAD = (uint32_t)(frequency - 1UL);
}








//=============== ALGORITHMS ================

uint8_t counter=0;
float numerator=1;
uint32_t denumerator = 100;
uint8_t PWD_increasing_flag=1;

void SysTick_Handler(void){
	counter++;
	counter%=(denumerator);
	if (counter % 100 == 0 && PWD_increasing_flag==1){
		numerator+=0.4*((numerator+5)/40);
	}
	if (counter % 100 == 0 && PWD_increasing_flag==0){
		numerator-=0.4*((numerator+5)/40);
	}
	if (numerator >= denumerator-50){
		PWD_increasing_flag=0;
	}

	if (numerator < 0.5){
		PWD_increasing_flag=1;
	}

	if (counter < numerator){
		set_output(GPIOA, 5, 1);
		set_output(GPIOC, 0, 1);
		set_output(GPIOC, 3,0);
	} else {
		set_output(GPIOA, 5, 0);
		set_output(GPIOC, 0, 0);
		set_output(GPIOC, 3, 1);
	}
}


void change_systick_freq_ALGO(void){
	int Scaler_2n =1;
	int clicked_flag = 0;
	while (1)
	{
	  if (clicked_flag==0 && get_input(GPIOC, 13) == 0){
		  Scaler_2n*=2;
		  if (Scaler_2n == 128){
			  Scaler_2n = 1;
		  }
		  SysTick_reset_value(Scaler_2n);

		  clicked_flag = 1;
	  }

	  if (clicked_flag == 1 && get_input(GPIOC, 13)==1){
		  clicked_flag = 0;
	  }
	}
}



