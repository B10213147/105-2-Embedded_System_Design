#include "stm32f4xx.h"                  // Device header
#include "gpio.h"
#include "rcc.h"
#include "timer.h"

void startup(void);
void delay_us(int n);
void delay_ms(int n);
void functionality(void);

int frequency = 1;
float duty = 0.5;
uint32_t nL, nH;
uint8_t pin = 0;
uint8_t keys[16];
int main(void){
	startup();

	while(1){
		if(GPIO_ReadInputDataBit(GPIOC, 3)){
			delay_us(20);
			for(int i = 0; i < 16; i++){				
				GPIO_WriteBit(GPIOC, 2, 1);
				delay_us(5);
				keys[i] = GPIO_ReadInputDataBit(GPIOC, 3);
				GPIO_WriteBit(GPIOC, 2, 0);
				delay_us(5);	
			}
			functionality();
		}
	}
	
	return 0;
}

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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);

	GPIO_Set_Direction(GPIOA, 0, 0x0020);	// LD2: PA5
	GPIO_Set_Direction(GPIOC, 0x0008, 0x0004);	// SCL: PC2, SDO: PC3
	GPIOC->PUPDR |= (0x1 << 2 * 3);
	
	int period = SystemCoreClock / 16000 / frequency;
	nH = period * duty;
	nL = period - nH;
	
	TIM_SetCounter(TIM3, 0);
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_SetCompare1(TIM2, nL);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2, ENABLE);
}

void functionality(void){
	int i;
	for(i = 0; keys[i] == 0; i++);
	switch(i){
		case 0:
			if(frequency < 10){
				frequency++;
			}
			break;
		case 3:
			if(frequency > 1){
				frequency--;
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
	int period = SystemCoreClock / 16000 / frequency;
	nH = period * duty;
	nL = period - nH;
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
