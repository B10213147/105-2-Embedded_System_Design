#include "stm32f4xx.h"                  // Device header

void putty_Begin(int baud_rate);
void startup(void);

int frequency = 10;	// Unit: Hz
int duty = 50;	// Unit: %
uint32_t nH, nL;
int main(void){
	startup();
	putty_Begin(9600);
	while(1){
		if(USART2->SR & USART_SR_RXNE){
			char data = USART2->DR;	// Read data
			while(!(USART2->SR & USART_SR_TXE));
			USART2->DR = data;	// Write data
			
			if(data >= '1' && data <= '9'){
				frequency = data - '0';
				int period = SystemCoreClock / frequency;	// Unit: cycle
				nH = period * (float)duty / 100;
				nL = period - nH;
				TIM2->CR1 |= TIM_CR1_CEN;	// Enable Timer2 counter
			}
		}
	}
	return 0;
}

int pin = 0;
void EXTI15_10_IRQHandler(void){
	if((EXTI->PR & EXTI_PR_PR13) != 0){
		while(!(USART2->SR & USART_SR_TXE));
		USART2->DR = '\f';	// Send form feed
		pin = 0;
		GPIOA->BSRR = 0x1U << (5 + 16);	// Reset PA5
		TIM2->CR1 &= ~TIM_CR1_CEN;	// Disable Timer2 counter
		TIM2->CNT = 0;	// Reset Timer2 counter
	    EXTI->PR = EXTI_PR_PR13;	// Clear pending bit
	}
}

void TIM2_IRQHandler(void){
	if((TIM2->SR & TIM_SR_CC1IF) != 0){
		TIM2->CNT = 0;
		
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

void putty_Begin(int baud_rate){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	// Enable GPIOA clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;	// Enable USART2 clock
	
	GPIOA->AFR[0] &= ~((0xF << 4 * 2) | (0xF << 4 * 3));	
	GPIOA->AFR[0] |= (0x7 << 4 * 2) | (0x7 << 4 * 3);	// Set Pin2, Pin3 as AF7
	GPIOA->MODER &= ~((0x3 << 2 * 2) | (0x3 << 2 * 3)); 
	GPIOA->MODER |= (0x2 << 2 * 2) | (0x2 << 2 * 3);	// Set Pin2, Pin3 as alternate function pin
	
	USART2->BRR = SystemCoreClock / baud_rate;	// Baud rate setting
	USART2->CR1 &= ~(USART_CR1_M | USART_CR1_PCE);	// 8 Data bits, no Parity control
	USART2->CR2 &= ~USART_CR2_STOP;	// 1 Stop bit
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;	// RX, TX, USART2 enable
}

void startup(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;	// Enable GPIOA, GPIOC clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable Timer2 clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;	// Enable SYSCFG clock
	
	GPIOA->MODER |= 0x1U << 2 * 5;	// User LD2: PA5
	GPIOC->MODER &= ~(0x3U << 2 * 13);	// B1 USER: PC13
	
	GPIOA->BSRR = 0x1U << (5 + 16);	// Reset PA5
	int period = SystemCoreClock / frequency;	// Unit: cycle
	nH = period * (float)duty / 100;
	nL = period - nH;
	
	TIM2->CNT = 0;	// Reset Timer2 counter
	TIM2->DIER |= TIM_DIER_CC1IE;	// Capture/Compare 1 interrupt enable
	TIM2->CCR1 = nL;	// Give a match value to compare
	NVIC_EnableIRQ(TIM2_IRQn);	// Enable NVIC IRQ channel	
	TIM2->CR1 |= TIM_CR1_CEN;	// Enable Timer2 counter
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13 & SYSCFG_EXTICR4_EXTI13_PC;	// Select the source input for EXTI13
	EXTI->IMR |= EXTI_IMR_MR13;	// Set mask bit(EXTI_Line13)
	EXTI->FTSR |= EXTI_FTSR_TR13;	// Active when falling edge occur
	NVIC_EnableIRQ(EXTI15_10_IRQn);	// Enable NVIC IRQ channel	
}
