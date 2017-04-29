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
uint32_t nL, nH, next;
int pin = 0;
uint8_t keys[16];
int main(void){
	startup();
	
	int period = SystemCoreClock / 16000 / frequency;
	nH = period * duty;
	nL = period - nH;
	next = TIM_GetCounter(TIM3) + nL;
	
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
			functionality();
		}
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

void startup(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN, ENABLE);

	GPIO_Set_Direction(GPIOA, 0, 0x0020);
	GPIO_Set_Direction(GPIOC, 0x0008, 0x0004);
	//GPIOC->PUPDR |= (0x1 << 2 * 2) | (0x1 << 2 * 3);
	GPIOC->PUPDR |= (0x1 << 2 * 3);
	
	TIM_SetCounter(TIM3, 0);
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1);
	TIM_GenerateEvent(TIM2, TIM_EGR_UG);	
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
		case 15:
			if(duty > 0.1){
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
	TIM_GenerateEvent(TIM3, TIM_EGR_UG);	
	TIM_Cmd(TIM3, ENABLE);

	while(TIM3->CNT < n);	
	
	TIM_Cmd(TIM3, DISABLE);
	TIM3->CNT = 0;
}

void delay_ms(int n){
	TIM_PrescalerConfig(TIM3, 16000 - 1);
	TIM_GenerateEvent(TIM3, TIM_EGR_UG);	
	TIM_Cmd(TIM3, ENABLE);

	while(TIM3->CNT < n);	
	
	TIM_Cmd(TIM3, DISABLE);
	TIM3->CNT = 0;
}
