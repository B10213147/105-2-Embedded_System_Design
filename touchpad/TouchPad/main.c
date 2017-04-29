#include "stm32f4xx.h"                  // Device header
#include "gpio.h"
#include "rcc.h"
#include "timer.h"

void startup(void);
void delay_us(int n);
void delay_ms(int n);

uint8_t keys[16];
int main(void){
	startup();
	
	while(1){
		if(GPIO_ReadInputDataBit(GPIOC, 3)){
			for(int i = 0; i < 16; i++){
				delay_us(20);
				GPIO_WriteBit(GPIOC, 2, 1);
				delay_us(5);
				keys[i] = GPIO_ReadInputDataBit(GPIOC, 3);
				GPIO_WriteBit(GPIOC, 2, 0);
				delay_us(5);	
			}
		}
		//delay_ms(5);
	}
	
	return 0;
}

void startup(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);

	GPIO_Set_Direction(GPIOA, 0, 0x0020);
	GPIO_Set_Direction(GPIOC, 0x0008, 0x0004);
	//GPIOC->PUPDR |= (0x1 << 2 * 2) | (0x1 << 2 * 3);
	GPIOC->PUPDR |= (0x1 << 2 * 3);
	TIM_SetCounter(TIM2, 0);
}

void delay_us(int n){
	TIM_PrescalerConfig(TIM2, 16 - 1);
	TIM_GenerateEvent(TIM2, TIM_EGR_UG);	
	TIM_Cmd(TIM2, ENABLE);

	while(TIM2->CNT < n);
	TIM2->CNT = 0;
	
	TIM_Cmd(TIM2, DISABLE);
}

void delay_ms(int n){
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EGR_UG);	
	TIM_Cmd(TIM2, ENABLE);

	while(TIM2->CNT < n);
	TIM2->CNT = 0;
	
	TIM_Cmd(TIM2, DISABLE);
}
