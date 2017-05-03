#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f4xx.h"                  // Device header

void EXTI_Init(uint32_t EXTI_Line, 
				uint32_t Rising_Mask, 
				uint32_t Falling_Mask, 
				FunctionalState NewState);
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);

#endif	// __EXTI_H
