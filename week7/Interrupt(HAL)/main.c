#include "stm32f4xx.h"                  // Device header
#include "gpio.h"
#include "rcc.h"
#include "timer.h"

void startup(void);

int frequency = 5;	// unit: Hz
int duty = 50;	// unit: %
uint32_t nH, nL, next;
int main(void){
	startup();	
	while(1);
	
	return 0;
}

void EXTI15_10_IRQHandler(void){
	if((EXTI->PR & EXTI_PR_PR13) != 0){
		frequency = frequency % 10 + 1;
		int period = SystemCoreClock / 16000 / frequency;	// Unit: cycle
		nH = period * (float)duty / 100;
		nL = period - nH;
		
		EXTI->PR = EXTI_PR_PR13;
	}
}

int pin = 0;
void TIM2_IRQHandler(void){
	if((TIM2->SR & TIM_SR_CC1IF) != 0){
		TIM_SetCounter(TIM2, 0);
		if(pin != 0){	// High period timeout
			TIM2->CCR1 = nL;
		}
		else{	// Low period timeout
			TIM2->CCR1 = nH;
		}
		pin ^= 1;
		GPIO_WriteBit(GPIOA, 5, pin);
	
		TIM2->SR = ~TIM_SR_CC1IF;
	}
}

void startup(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);
	
	GPIO_Set_Direction(GPIOA, 0, 0x0020);
	GPIO_Set_Direction(GPIOC, 0x2000, 0);
	
	pin = 0;
	GPIO_WriteBit(GPIOA, 5, pin);
	int period = SystemCoreClock / 16000 / frequency;	// Unit: cycle
	nH = period * (float)duty / 100;
	nL = period - nH;
	
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EGR_UG);
	TIM2->DIER |= TIM_DIER_CC1IE;
	TIM2->CCR1 = 100;
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2, ENABLE);
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13 & SYSCFG_EXTICR4_EXTI13_PC;
	EXTI->IMR |= EXTI_IMR_MR13;
	EXTI->FTSR |= EXTI_FTSR_TR13;
	NVIC_EnableIRQ(EXTI15_10_IRQn);	
}
