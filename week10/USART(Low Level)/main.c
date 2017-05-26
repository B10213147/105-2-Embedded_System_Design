#include "stm32f4xx.h"                  // Device header

void putty_Begin(int baud_rate);

int main(void){
	putty_Begin(9600);
	while(1){
		if(USART2->SR & USART_SR_RXNE){
			char data = USART2->DR;	// Read data
			while(!(USART2->SR & USART_SR_TXE));
			USART2->DR = data;	// Write data
		}
	}
	return 0;
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
