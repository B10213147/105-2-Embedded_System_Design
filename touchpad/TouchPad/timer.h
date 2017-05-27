#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"                  // Device header

/** @defgroup TIM_One_Pulse_Mode 
  * @{
  */
#define TIM_OPMode_Single                  ((uint16_t)0x0008)
#define TIM_OPMode_Repetitive              ((uint16_t)0x0000)

/** @defgroup TIM_interrupt_sources 
  * @{
  */
#define TIM_IT_Update		((uint16_t)0x0001)
#define TIM_IT_CC1			((uint16_t)0x0002)
#define TIM_IT_CC2			((uint16_t)0x0004)
#define TIM_IT_CC3			((uint16_t)0x0008)
#define TIM_IT_CC4			((uint16_t)0x0010)
#define TIM_IT_Trigger		((uint16_t)0x0040)

/** @defgroup TIM_Event_Source 
  * @{
  */
#define TIM_EventSource_Update             ((uint16_t)0x0001)
#define TIM_EventSource_CC1                ((uint16_t)0x0002)
#define TIM_EventSource_CC2                ((uint16_t)0x0004)
#define TIM_EventSource_CC3                ((uint16_t)0x0008)
#define TIM_EventSource_CC4                ((uint16_t)0x0010)
#define TIM_EventSource_Trigger            ((uint16_t)0x0040)

void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler);
void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter);
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload);
uint32_t TIM_GetCounter(TIM_TypeDef* TIMx);
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint16_t TIM_OPMode);
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource);
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2);
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint32_t Compare3);
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint32_t Compare4);
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState);
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT);
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT);

#endif	// __TIMER_H
