#include "rcc.h"

void RCC_AHB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState){
	if(NewState != DISABLE){
		RCC->AHB1ENR |= RCC_Periph;
	}
	else{
		RCC->AHB1ENR &= ~RCC_Periph;
	}	
}

void RCC_APB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState){
	if(NewState != DISABLE){
		RCC->APB1ENR |= RCC_Periph;
	}
	else{
		RCC->APB1ENR &= ~RCC_Periph;
	}	
}
