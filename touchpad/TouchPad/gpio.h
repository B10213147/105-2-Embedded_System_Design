#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"                  // Device header

void GPIO_Set_Direction(GPIO_TypeDef* GPIOx, uint16_t inputMask, uint16_t outputMask);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, int Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, int Pin, uint8_t BitVal);

#endif	// __GPIO_H
