#ifndef __RCC_H
#define __RCC_H

#include "stm32f4xx.h"                  // Device header

void RCC_AHB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState);

#endif	// __RCC_H
