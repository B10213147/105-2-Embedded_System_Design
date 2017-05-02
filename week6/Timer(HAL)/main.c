#include "stm32f4xx.h"                  // Device header
#include "gpio.h"
#include "rcc.h"
#include "timer.h"

int main(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);
	
	GPIO_Set_Direction(GPIOA, 0, 0x0020);
	GPIO_Set_Direction(GPIOC, 0x2000, 0);
	
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EGR_UG);
	
	int frequency = 5;	// unit: Hz
	float duty = 0.9;	
	int period = SystemCoreClock / 16000 / frequency;	// Unit: cycle
	uint32_t nH = period * duty;
	uint32_t nL = period - nH;	
	
	int pin = 0;
	GPIO_WriteBit(GPIOA, 5, pin);
	while(1){
		if(GPIO_ReadInputDataBit(GPIOC, 13) == 0){	// Button pressed
			TIM_Cmd(TIM2, ENABLE);
			uint32_t next = TIM_GetCounter(TIM2) + nL;	
			do{
				if(TIM_GetCounter(TIM2) > next){
					if(pin != 0){	// High period timeout
						next += nL;
					}
					else{	// Low period timeout
						next += nH;
					}
					pin ^= 1;
					GPIO_WriteBit(GPIOA, 5, pin);
				}
			}while(GPIO_ReadInputDataBit(GPIOC, 13) == 0);	// Button pressed
			
			pin = 0;
			GPIO_WriteBit(GPIOA, 5, pin);
			TIM_Cmd(TIM2, DISABLE);
			TIM_SetCounter(TIM2, 0);
		}
	}
	
	return 0;
}
