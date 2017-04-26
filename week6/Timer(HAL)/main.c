#include "stm32f4xx.h"                  // Device header
#include "gpio.h"
#include "rcc.h"
#include "timer.h"

int main(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);
	
	GPIO_Set_Direction(GPIOA, 0, 0x0020);
	
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EGR_UG);
	TIM_Cmd(TIM2, ENABLE);
	
	int frequency = 50;	// unit: Hz
	float duty = 0.05;	
	int period = SystemCoreClock / 16000 / frequency;
	uint32_t nH = period * duty;
	uint32_t nL = period - nH;
	uint32_t next = TIM_GetCounter(TIM2) + nL;
	
	int pin = 0;
	GPIO_WriteBit(GPIOA, 5, pin);
	while(1){
		if(TIM_GetCounter(TIM2) > next){
			if(pin != 0){
				next += nL;
			}
			else{
				next += nH;
			}
			pin ^= 1;
			GPIO_WriteBit(GPIOA, 5, pin);		
		}
	}
	return 0;
}
