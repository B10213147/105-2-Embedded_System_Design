#ifndef __RCC_H
#define __RCC_H

#include "stm32f4xx.h"                  // Device header

/** @defgroup RCC_AHB1_Peripherals 
  * @{
  */ 
#define RCC_AHB1Periph_GPIOA		((uint32_t)0x00000001)
#define RCC_AHB1Periph_GPIOB		((uint32_t)0x00000002)
#define RCC_AHB1Periph_GPIOC		((uint32_t)0x00000004)
#define RCC_AHB1Periph_GPIOD		((uint32_t)0x00000008)
#define RCC_AHB1Periph_GPIOE		((uint32_t)0x00000010)
#define RCC_AHB1Periph_GPIOH		((uint32_t)0x00000080)
#define RCC_AHB1Periph_CCR			((uint32_t)0x00000100)
#define RCC_AHB1Periph_DMA1			((uint32_t)0x00000004)
#define RCC_AHB1Periph_DMA2			((uint32_t)0x00000004)

/** @defgroup RCC_APB1_Peripherals 
  * @{
  */ 
#define RCC_APB1Periph_TIM2			((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3			((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4			((uint32_t)0x00000004)
#define RCC_APB1Periph_TIM5			((uint32_t)0x00000008)
#define RCC_APB1Periph_WWDG			((uint32_t)0x00000080)
#define RCC_APB1Periph_SPI2			((uint32_t)0x00004000)
#define RCC_APB1Periph_SPI3			((uint32_t)0x00008000)
#define RCC_APB1Periph_USART2		((uint32_t)0x00020000)
#define RCC_APB1Periph_I2C1			((uint32_t)0x00200000)
#define RCC_APB1Periph_I2C2			((uint32_t)0x00400000)
#define RCC_APB1Periph_I2C3			((uint32_t)0x00800000)
#define RCC_APB1Periph_PWR			((uint32_t)0x10000000)

/** @defgroup RCC_APB2_Peripherals 
  * @{
  */ 
#define RCC_APB2Periph_TIM1			((uint32_t)0x00000001)
#define RCC_APB2Periph_USART1		((uint32_t)0x00000010)
#define RCC_APB2Periph_USART6		((uint32_t)0x00000020)
#define RCC_APB2Periph_ADC1			((uint32_t)0x00000100)
#define RCC_APB2Periph_SDIO			((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1			((uint32_t)0x00001000)
#define RCC_APB2Periph_SPI4			((uint32_t)0x00002000)
#define RCC_APB2Periph_SYSCFG		((uint32_t)0x00004000)
#define RCC_APB2Periph_TIM9			((uint32_t)0x00010000)
#define RCC_APB2Periph_TIM10		((uint32_t)0x00020000)
#define RCC_APB2Periph_TIM11		((uint32_t)0x00040000)

void RCC_AHB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_Periph, FunctionalState NewState);

#endif	// __RCC_H
