#include "exti.h"

void EXTI_Init(uint32_t EXTI_Line, 
				uint32_t Rising_Mask, 
				uint32_t Falling_Mask, 
				FunctionalState NewState){
	/* EXTI line configuration */
	if(NewState != DISABLE){
		EXTI->IMR |= EXTI_Line;
	}
	else{
		EXTI->IMR &= ~EXTI_Line;
	}
	/* Rising Falling edge configuration */
	EXTI->RTSR |= Rising_Mask;
	EXTI->FTSR |= Falling_Mask;					
}

ITStatus EXTI_GetITStatus(uint32_t EXTI_Line){
	if(((EXTI->IMR & EXTI_Line) && (EXTI->PR & EXTI_Line)) != 0){
		return SET;
	}
	else{
		return RESET;
	}
}

void EXTI_ClearITPendingBit(uint32_t EXTI_Line){
	EXTI->PR = EXTI_Line;
}
