#include "stm32f4xx.h"                  // Device header

void startup(void);

int frequency = 10;	// Unit: Hz
int duty = 50;	// Unit: %
uint32_t nH, nL, next;
int main(void){
	startup();
	while(1);
	
	return 0;
}

void EXTI15_10_IRQHandler(void){
	if((EXTI->PR & EXTI_PR_PR13) != 0){
		frequency = frequency % 10 + 1;
		
	    EXTI->PR = EXTI_PR_PR13;	// Clear pending bit
	}
}

int pin = 0;
void TIM2_IRQHandler(void){
	if((TIM2->SR & TIM_SR_CC1IF) != 0){
		TIM2->CNT = 0;
		
		int period = SystemCoreClock / frequency;	// Unit: cycle
		nH = period * (float)duty / 100;
		nL = period - nH;
		
		if(pin != 0){	// High period timeout
			TIM2->CCR1 = nL;
			GPIOA->BSRR = 0x1U << (5 + 16);	// Reset PA5
		}
		else{	// Low period timeout
			TIM2->CCR1 = nH;
			GPIOA->BSRR = 0x1U << 5;	// Set PA5
		}
		pin ^= 1;
		
		TIM2->SR = ~TIM_SR_CC1IF;	// Clear pending bit
	}
}

void startup(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;	// Enable GPIOA, GPIOC clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable Timer2 clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;	// Enable SYSCFG clock
	
	GPIOA->MODER |= 0x1U << 2 * 5;	// User LD2: PA5
	GPIOC->MODER &= ~(0x3U << 2 * 13);	// B1 USER: PC13
	
	TIM2->CNT = 0;	// Reset Timer2 counter
	TIM2->DIER |= TIM_DIER_CC1IE;	// Capture/Compare 1 interrupt enable
	TIM2->CCR1 = 100;	// Give a match value to compare
	NVIC_EnableIRQ(TIM2_IRQn);	// Enable NVIC IRQ channel	
	TIM2->CR1 |= TIM_CR1_CEN;	// Enable Timer2 counter
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13 & SYSCFG_EXTICR4_EXTI13_PC;	// Select the source input for EXTI13
	EXTI->IMR |= EXTI_IMR_MR13;	// Set mask bit(EXTI_Line13)
	EXTI->FTSR |= EXTI_FTSR_TR13;	// Active when falling edge occur
	NVIC_EnableIRQ(EXTI15_10_IRQn);	// Enable NVIC IRQ channel	
}
