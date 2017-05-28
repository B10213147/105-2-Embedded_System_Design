#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f4xx.h"                  // Device header

/** @defgroup EXTI_Lines 
  * @{
  */
#define EXTI_Line0		((uint32_t)0x00000001)
#define EXTI_Line1		((uint32_t)0x00000002)
#define EXTI_Line2		((uint32_t)0x00000004)
#define EXTI_Line3		((uint32_t)0x00000008)
#define EXTI_Line4		((uint32_t)0x00000010)
#define EXTI_Line5		((uint32_t)0x00000020)
#define EXTI_Line6		((uint32_t)0x00000040)
#define EXTI_Line7		((uint32_t)0x00000080)
#define EXTI_Line8		((uint32_t)0x00000100)
#define EXTI_Line9		((uint32_t)0x00000200)
#define EXTI_Line10		((uint32_t)0x00000400)
#define EXTI_Line11		((uint32_t)0x00000800)
#define EXTI_Line12		((uint32_t)0x00001000)
#define EXTI_Line13		((uint32_t)0x00002000)
#define EXTI_Line14		((uint32_t)0x00004000)
#define EXTI_Line15		((uint32_t)0x00008000)
#define EXTI_Line16		((uint32_t)0x00010000)
#define EXTI_Line17		((uint32_t)0x00020000)
#define EXTI_Line18		((uint32_t)0x00040000)
#define EXTI_Line19		((uint32_t)0x00080000)
#define EXTI_Line20		((uint32_t)0x00100000)
#define EXTI_Line21		((uint32_t)0x00200000)
#define EXTI_Line22		((uint32_t)0x00400000)

void EXTI_Init(uint32_t EXTI_Line, \
				uint32_t Rising_Mask, \
				uint32_t Falling_Mask, \
				FunctionalState NewState);
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);

#endif	// __EXTI_H
