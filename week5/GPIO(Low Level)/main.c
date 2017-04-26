#include "stm32f4xx.h"                  // Device header

int main(void){
	RCC->AHB1ENR |= 0x0001U | 0x0004U;	// GPIOA, GPIOC clock enable

	GPIOC->MODER &= ~(0x3U << 2 * 13);	// Set GPIOC Pin13 as input
	GPIOA->MODER |= (0x1U << 2 * 5);	// Set GPIOA Pin5 as output
	
	while(1){
		if(!(GPIOC->IDR & 0x2000U)){	// Button pressed
			GPIOA->BSRR = 0x1U << 5;	// Set LED
		}
		else{	// Button released
			GPIOA->BSRR = 0x1U << (5 + 16);	// Reset LED
		}
	}
	
	return 0;
}
