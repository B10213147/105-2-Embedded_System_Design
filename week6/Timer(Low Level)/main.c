#include "stm32f4xx.h"                  // Device header

int main(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	GPIOA->MODER |= 0x1U << 2 * 5;
	
	TIM2->CNT = 0;
	TIM2->CR1 |= TIM_CR1_CEN;
	
	int frequency = 1;	// unit: Hz
	float duty = 0.5;	
	uint32_t nH = SystemCoreClock / frequency * duty;
	uint32_t nL = SystemCoreClock / frequency - nH;
	uint32_t next = TIM2->CNT + nL;
	
	int pin = 0;
	GPIOA->BSRR = 0x1U << (5 + 16);
	while(1){
		if(TIM2->CNT > next){
			if(pin != 0){
				next += nL;
				GPIOA->BSRR = 0x1U << (5 + 16);
			}
			else{
				next += nH;
				GPIOA->BSRR = 0x1U << 5;
			}
			pin ^= 1;
		}
	}
	
	return 0;
}
