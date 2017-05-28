#include "timer.h"

void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler){
	TIMx->PSC = Prescaler;
}

void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter){
	TIMx->CNT = Counter;
}

void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload){
	TIMx->ARR = Autoreload;
}

uint32_t TIM_GetCounter(TIM_TypeDef* TIMx){
	return TIMx->CNT;
}

void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState){
	if(NewState != DISABLE){
		TIMx->CR1 |= TIM_CR1_ARPE;
	}
	else{
		TIMx->CR1 &= ~TIM_CR1_ARPE;
	}	
}

void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint16_t TIM_OPMode){
	TIMx->CR1 &= ~TIM_CR1_OPM;
	TIMx->CR1 |= TIM_OPMode;
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

void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1){
	TIMx->CCR1 = Compare1;
}

void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2){
	TIMx->CCR2 = Compare2;
}

void TIM_SetCompare3(TIM_TypeDef* TIMx, uint32_t Compare3){
	TIMx->CCR3 = Compare3;
}

void TIM_SetCompare4(TIM_TypeDef* TIMx, uint32_t Compare4){
	TIMx->CCR4 = Compare4;
}

void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState){
	if(NewState != DISABLE){
		TIMx->DIER |= TIM_IT;
	}
	else{
		TIMx->DIER &= ~TIM_IT;
	}	
}

ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT){
	if(((TIMx->DIER & TIM_IT) && (TIMx->SR & TIM_IT)) != 0){
		return SET;
	}
	else{
		return RESET;
	}
}

void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT){
	TIMx->SR = ~TIM_IT;
}
