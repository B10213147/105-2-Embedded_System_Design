#include "stm32f4xx.h"                  // Device header

int main(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;	// Enable GPIOA, GPIOC clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable Timer2 clock
	
	GPIOA->MODER |= 0x1U << 2 * 5;	// User LD2: PA5
	GPIOC->MODER &= ~(0x3U << 2 * 13);	// B1 USER: PC13
	
	TIM2->CNT = 0;	// Reset Timer2 counter
	
	int frequency = 10;	// Unit: Hz
	float duty = 0.5;	
	int period = SystemCoreClock / frequency;	// Unit: cycle
	uint32_t nH = period * duty;
	uint32_t nL = period - nH;	
	
	int pin = 0;
	GPIOA->BSRR = 0x1U << (5 + 16);	// Reset PA5
	while(1){
		if(!(GPIOC->IDR & GPIO_IDR_IDR_13)){	// Button pressed
			TIM2->CR1 |= TIM_CR1_CEN;	// Enable Timer2 counter
			uint32_t next = TIM2->CNT + nL;			
			do{
				if(TIM2->CNT > next){
					if(pin != 0){	// High period timeout
						next += nL;
						GPIOA->BSRR = 0x1U << (5 + 16);	// Reset PA5
					}
					else{	// Low period timeout
						next += nH;
						GPIOA->BSRR = 0x1U << 5;	// Set PA5
					}
					pin ^= 1;
				}
			}while(!(GPIOC->IDR & GPIO_IDR_IDR_13));	// Button pressed
			
			pin = 0;
			GPIOA->BSRR = 0x1U << (5 + 16);	// Reset PA5
			TIM2->CR1 &= ~TIM_CR1_CEN;
			TIM2->CNT = 0;	// Reset Timer2 counter
		}
	}
	
	return 0;
}
