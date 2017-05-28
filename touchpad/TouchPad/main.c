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

void startup(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);

	GPIO_Set_Direction(GPIOA, 0, 0x0020);	// LD2: PA5
	GPIO_Set_Direction(GPIOC, 0x0008, 0x0004);	// SCL: PC2, SDO: PC3
	GPIOC->PUPDR |= (0x1 << 2 * 3);
	
	int period = SystemCoreClock / 16000 / led_freq;
	led_nH = period * duty;
	led_nL = period - led_nH;
	
	// Timer3 use for SCL clock generation
	TIM_SetCounter(TIM3, 0);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_SelectOnePulseMode(TIM3, TIM_OPMode_Single);
	
	// Timer2 use for LED blink
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_SetAutoreload(TIM2, led_nL);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3 & SYSCFG_EXTICR1_EXTI3_PC;
	EXTI_Init(EXTI_Line3, EXTI_Line3, 0, ENABLE);	
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
		case 15:
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
	TIM_SetAutoreload(TIM3, n);
	TIM_GenerateEvent(TIM3, TIM_EventSource_Update);	
	TIM_Cmd(TIM3, ENABLE);
	while(TIM_GetCounter(TIM3));
}

void delay_ms(int n){
	TIM_PrescalerConfig(TIM3, 16000 - 1);
	TIM_SetAutoreload(TIM3, n);
	TIM_GenerateEvent(TIM3, TIM_EventSource_Update);	
	TIM_Cmd(TIM3, ENABLE);
	while(TIM_GetCounter(TIM3));
}
