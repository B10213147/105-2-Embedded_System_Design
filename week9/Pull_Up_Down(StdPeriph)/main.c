#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"

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
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, pin);
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
}

void startup(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	pin = 0;
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, pin);
	int period = SystemCoreClock / 16000 / frequency;	// Unit: cycle
	nH = period * (float)duty / 100;
	nL = period - nH;
	
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1, TIM_PSCReloadMode_Immediate);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_SetCompare1(TIM2, nL);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource10);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line10 | EXTI_Line13;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	NVIC_EnableIRQ(EXTI15_10_IRQn);	
}
