#include "interrupt.h"
#include "gpio.h"
#include "timer.h"
#include "exti.h"

extern void functionality(void);
extern void delay_us(int n);

extern char keys[16];
extern uint8_t led_pin, led_nH, led_nL;
void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != 0){
		if(led_pin != 0){	// High period timeout
			TIM_SetAutoreload(TIM2, led_nL);
		}
		else{	// Low period timeout
			TIM_SetAutoreload(TIM2, led_nH);
		}
		led_pin ^= 1;
		GPIO_WriteBit(GPIOA, 5, led_pin);
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void EXTI3_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line3) != 0){
		delay_us(100);
		for(int i = 0; i < 16; i++){				
			GPIO_WriteBit(GPIOC, 2, 1);
			delay_us(1);
			keys[i] = GPIO_ReadInputDataBit(GPIOC, 3);
			GPIO_WriteBit(GPIOC, 2, 0);
			delay_us(1);	
		}
		functionality();
		
		EXTI_ClearITPendingBit(EXTI_Line3);
	}	
}
