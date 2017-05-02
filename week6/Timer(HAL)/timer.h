#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"                  // Device header

void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler);
void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter);
uint32_t TIM_GetCounter(TIM_TypeDef* TIMx);
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource);

#endif	// __TIMER_H
