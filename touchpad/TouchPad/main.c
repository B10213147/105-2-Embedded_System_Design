#include "stm32f4xx.h"                  // Device header
#include "gpio.h"
#include "rcc.h"
#include "timer.h"
#include "exti.h"

void startup(void);
void delay_us(int n);
void delay_ms(int n);
void functionality(void);

int led_freq = 1;
float duty = 0.5;
uint32_t led_nL, led_nH;
uint8_t led_pin = 0;
uint8_t keys[16];
int main(void){
	startup();

	while(1){
	}
	
	return 0;
}

void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != 0){
		TIM_SetCounter(TIM2, 0);
		if(led_pin != 0){	// High period timeout
			TIM_SetCompare1(TIM2, led_nL);
		}
		else{	// Low period timeout
			TIM_SetCompare1(TIM2, led_nH);
		}
		led_pin ^= 1;
		GPIO_WriteBit(GPIOA, 5, led_pin);
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
}

uint8_t DV = 0;
void EXTI3_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line3) != 0){
		if(DV == 0){
			DV = 1;
			TIM_SetCounter(TIM3, 0);
			TIM_SetCompare1(TIM3, 16 * 20);
			TIM_Cmd(TIM3, ENABLE);
		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}	
}

uint8_t scl_c = 0, scl_pin = 0;
uint32_t scl_nH, scl_nL;
uint32_t scl_freq = 80000;
void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != 0){
		TIM_SetCounter(TIM3, 0);
		
		if(scl_pin != 0){	// High period timeout
			keys[scl_c] = GPIO_ReadInputDataBit(GPIOC, 3);
			TIM_SetCompare1(TIM3, scl_nL);
			scl_c++;
		}
		else{	// Low period timeout
			TIM_SetCompare1(TIM3, scl_nH);
		}
		scl_pin ^= 1;
		GPIO_WriteBit(GPIOC, 2, scl_pin);
		
		if(scl_c == 16){
			TIM_Cmd(TIM3, DISABLE);
			functionality();
			scl_c = 0;
			DV = 0;
		}
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	}
}

void startup(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);

	GPIO_Set_Direction(GPIOA, 0, 0x0020);	// LD2: PA5
	GPIO_Set_Direction(GPIOC, 0x0008, 0x0004);	// SCL: PC2, SDO: PC3
	GPIOC->PUPDR |= (0x1 << 2 * 3);
	
	int period = SystemCoreClock / 16000 / led_freq;
	led_nH = period * duty;
	led_nL = period - led_nH;
	period = SystemCoreClock / scl_freq;
	scl_nH = period * duty;
	scl_nL = period - scl_nH;
	
	// Timer3 use for SCL clock generation
	TIM_SetCounter(TIM3, 0);
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	NVIC_EnableIRQ(TIM3_IRQn);
	
	// Timer2 use for LED blink
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_SetCompare1(TIM2, led_nL);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3 & SYSCFG_EXTICR1_EXTI3_PC;
	EXTI_Init(EXTI_Line3, EXTI_Line3, EXTI_Line3, ENABLE);	
	NVIC_EnableIRQ(EXTI3_IRQn);	
}

void functionality(void){
	int i;
	for(i = 0; keys[i] == 0; i++);
	switch(i){
		case 0:
			if(led_freq < 10){
				led_freq++;
			}
			break;
		case 3:
			if(led_freq > 1){
				led_freq--;
			}
			break;
		case 12:
			if(duty < 0.90){
				duty += 0.1;
			}
			break;
		case 14:
			if(duty > 0.2){
				duty -= 0.1;
			}
			break;
		default:
			break;
	}
	int period = SystemCoreClock / 16000 / led_freq;
	led_nH = period * duty;
	led_nL = period - led_nH;
}

void delay_us(int n){
	TIM_PrescalerConfig(TIM3, 16 - 1);
	TIM_GenerateEvent(TIM3, TIM_EventSource_Update);	
	TIM_Cmd(TIM3, ENABLE);

	while(TIM_GetCounter(TIM3) < n);	
	
	TIM_Cmd(TIM3, DISABLE);
	TIM_SetCounter(TIM3, 0);
}

void delay_ms(int n){
	TIM_PrescalerConfig(TIM3, 16000 - 1);
	TIM_GenerateEvent(TIM3, TIM_EventSource_Update);	
	TIM_Cmd(TIM3, ENABLE);

	while(TIM_GetCounter(TIM3) < n);	
	
	TIM_Cmd(TIM3, DISABLE);
	TIM_SetCounter(TIM3, 0);
}
