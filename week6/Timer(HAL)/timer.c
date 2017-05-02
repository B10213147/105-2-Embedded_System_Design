#include "timer.h"

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
