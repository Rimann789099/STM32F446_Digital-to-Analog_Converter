/*
 * usart.c
 *
 *  Created on: Nov 21, 2024
 *      Author: luciuss
 */
#include "main.h"

void APB1_USART2_clock_on(){
	//*((uint32_t *)  (0x40023840)) |= (1<<17);
	RCC->APB1ENR |=RCC_APB1ENR_USART2EN;
}

// Check HSI clock Is_Working
void hsi_IS_ON(){

	int state = (*((uint32_t *) 0x40023800)&1);
	if (state==1){

		for (int i =0; i<1000000;i++){};
		set_output(GPIOA, 5,0);
		for (int i =0; i<1000000;i++){};
		set_output(GPIOA, 5, 1);
	}
}


int get_over8(USART_TypeDef *usart){
//USART_CR1 register
	return  (usart->CR1&(1<<15))>>15;
}

float set_usartdiv(USART_TypeDef *usart,int frequency_Hz, int baud_rate){

	int over8 = get_over8(usart);
	float usart_divisor  = frequency_Hz/baud_rate;//*(1/(8*(2-over8)));
	//take mantissa part 0dXX
	//take fraction part 0dXX
	usart->BRR=usart_divisor;
	return usart_divisor;
}

void usart_enable(USART_TypeDef *usart,int state){
	usart->CR1 &= ~(1 << 13);
	usart->CR1|= state << 13;
}

//Transmitter enable (TE)
void usart_transmitter_enable(USART_TypeDef *usart,int state){
	usart->CR1 &= ~(1 << 3);
	usart->CR1|= state << 3;
}

//Receiver enable (RE)
void usart_receiver_enable(USART_TypeDef *usart, int state){
	usart->CR1 &= ~(1 << 2);
	usart->CR1|= state << 2;
}



int get_TXE(USART_TypeDef *usart){
	int n =7;
	return (usart -> SR & (1<<n))>>n;
}


void choose_AF(int pin_n, int af_code){
	GPIOA->AFR[0]&=~(0b1111<<pin_n*4);
	// Set AFR for pin n to AFx peripheral
	GPIOA->AFR[0]|=af_code<<pin_n*4;
}


// SEND With CR
void send_message(USART_TypeDef *usart, char* message, int n){
	if (usart->SR & USART_SR_TXE){
		int i =0;
		while(i<n){
			if (usart->SR & USART_SR_TXE){
				usart->DR=message[i];
				i++;
			}
		}
// CR Symbol
		while (i<n+1){
			if (usart->SR & USART_SR_TXE){
				usart->DR='\r';
				i++;
			}
		}

	}
}


// Without CR symbol
int receive_message(int message_unread, int n, char* message_buffer){
	int i;
	char symbol;
	if(USART2->SR & USART_SR_RXNE){
		i =0;
		symbol = ' ';

		//set_output(GPIOA, 5, 1);
		while ((symbol!='\r')&&(i<n+1)){
			if(USART2->SR & USART_SR_RXNE){
				symbol = USART2->DR;
				if (symbol!='\r'){
					message_buffer[i] = symbol;
				}
				i++;
			}
		}
		return 1;
	}
	return message_unread;
}



int check_message(int n, char* message){
	if (message[0]=='p' && message[1]=='i' && message[2]=='n' && message[3]=='g' )
		return 0;
	if (message[0]=='l' && message[1]=='e' && message[2]=='o' && message[3]=='n' )
		return 1;
	if (message[0]=='l' && message[1]=='e' && message[2]=='o' && message[3]=='f')
		return 2;
	if (message[0]=='c' && message[1]=='h' && message[2]=='e' && message[3]=='c' && message[4]=='k'){
		return 3;
	}

	return -1;
}


void ping_message(char *message){
			send_message(USART2,message, 4);
}


void led_on_message(GPIO_TypeDef *gpio, int n){
	set_output(gpio, n, 1);
}

void led_off_message(GPIO_TypeDef *gpio, int n){
	set_output(gpio, n, 0);
}

void check_led_message(GPIO_TypeDef *gpio ,int n){
	if (check_output(gpio, n) == 1){
		char message[2] = {'O', 'N'};
		send_message(USART2, message, 2);
	} else{
		char message[3] = {'O','F','F'};
		send_message(USART2, message, 3);
	}
}






int read_exec_message(int message_unread, int n, char* message){
// If we have unread message => READ IT
	if (message_unread == 1){
		int message_code = check_message(n , message);

		if (message_code == 0){
			ping_message(message);
// Unread messages = 0
			message_unread = 0;
			return message_unread;
		}

		if (message_code == 1){
			led_on_message(GPIOA, message[4] - '0');
			message_unread = 0;
			return message_unread;
		}

		if (message_code == 2){
			led_off_message(GPIOA, message[4] - '0' );
			message_unread = 0;
			return message_unread;
		}

		if (message_code == 3){
			int led_n = message[5] - '0';
			check_led_message(GPIOA, led_n);

			message_unread = 0;
			return message_unread;
		}


	}
	// If wasn't read => Return Unread State
	return message_unread;

}




int button_message(USART_TypeDef *usart, int message_sent){
	char data[12] = {'I', ' ','L', 'O', 'V', 'E', ' ', 'K','S','U','!','\n'};

	int button_clicked = 0;


	if (get_input(GPIOC, 13) == 0 && button_clicked == 0 && message_sent == 0){
		button_clicked = 1;
		// If TXE is in HIGH state, then it is ready to get new data
		int i = 0;
		while(i<12){
			if (USART2->SR & USART_SR_TXE){
				USART2->DR=data[i];
				i++;
			}
		}

		if (i>=12){
			message_sent = 1;
		}
	}

	if (get_input(GPIOC, 13)==1){
		button_clicked = 0 ;
		message_sent = 0;
	}

	return message_sent;

}


int global_character;
void USART2_IRQHandler(void){


}
