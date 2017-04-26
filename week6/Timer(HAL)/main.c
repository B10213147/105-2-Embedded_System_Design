#include "stm32f4xx.h"                  // Device header
#include "gpio.h"

void RCC_AHB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState);
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler);
void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter);
uint32_t TIM_GetCounter(TIM_TypeDef* TIMx);
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource);

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

void RCC_AHB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState){
	if(NewState != DISABLE){
		RCC->AHB1ENR |= RCC_Periph;
	}
	else{
		RCC->AHB1ENR &= ~RCC_Periph;
	}	
}

void RCC_APB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState){
	if(NewState != DISABLE){
		RCC->APB1ENR |= RCC_Periph;
	}
	else{
		RCC->APB1ENR &= ~RCC_Periph;
	}	
}

void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler){
	TIMx->PSC = Prescaler;
}

void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter){
	TIMx->CNT = Counter;
}

uint32_t TIM_GetCounter(TIM_TypeDef* TIMx){
	return TIMx->CNT;
}

void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState){
	if(NewState != DISABLE){
		TIMx->CR1 |= TIM_CR1_CEN;
	}
	else{
		TIMx->CR1 &= ~TIM_CR1_CEN;
	}
}

void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource){
	TIMx->EGR = TIM_EventSource;
}
