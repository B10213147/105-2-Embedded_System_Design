#include "stm32f4xx.h"                  // Device header
#include "gpio.h"
#include "rcc.h"
#include "timer.h"
#include "exti.h"

void startup(void);

int frequency = 5;	// unit: Hz
int duty = 50;	// unit: %
uint32_t nH, nL;
int main(void){
	startup();	
	while(1);
	
	return 0;
}

void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line10) != 0){
		if(frequency > 1){
			frequency--;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	else if(EXTI_GetITStatus(EXTI_Line13) != 0){
		if(frequency < 10){
			frequency++;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	
	int period = SystemCoreClock / 16000 / frequency;	// Unit: cycle
	nH = period * (float)duty / 100;
	nL = period - nH;
}

int pin = 0;
void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != 0){
		TIM_SetCounter(TIM2, 0);
		if(pin != 0){	// High period timeout
			TIM_SetCompare1(TIM2, nL);
		}
		else{	// Low period timeout
			TIM_SetCompare1(TIM2, nH);
		}
		pin ^= 1;
		GPIO_WriteBit(GPIOA, 5, pin);
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
}

void startup(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_Set_Direction(GPIOA, 0, 0x0020);
	GPIO_Set_Direction(GPIOC, 0x2400, 0);
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR10_0;
	
	pin = 0;
	GPIO_WriteBit(GPIOA, 5, pin);
	int period = SystemCoreClock / 16000 / frequency;	// Unit: cycle
	nH = period * (float)duty / 100;
	nL = period - nH;
	
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_SetCompare1(TIM2, nL);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10 & SYSCFG_EXTICR3_EXTI10_PC;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13 & SYSCFG_EXTICR4_EXTI13_PC;
	EXTI_Init(EXTI_Line10 | EXTI_Line13, EXTI_Line10, EXTI_Line13, ENABLE);
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);	
}
