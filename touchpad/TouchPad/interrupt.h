#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 
void TIM2_IRQHandler(void);
void EXTI3_IRQHandler(void);	 

#ifdef __cplusplus
}
#endif

#endif /* __INTERRUPT_H */
