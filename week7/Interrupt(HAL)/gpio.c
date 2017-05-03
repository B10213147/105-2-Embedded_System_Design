#include "gpio.h"

void GPIO_Set_Direction(GPIO_TypeDef* GPIOx, uint16_t inputMask, uint16_t outputMask){
	uint32_t pin_pos, cur_pin;
	for(pin_pos = 0x00; pin_pos < 0x10; pin_pos++){
		cur_pin = 0x01 << pin_pos;
		if(inputMask & cur_pin){
			GPIOx->MODER &= ~(0x3 << 2 * pin_pos);
		}
		else if(outputMask & cur_pin){
			GPIOx->MODER |= 0x1 << 2 * pin_pos;
		}
	}
}

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, int Pin){
	uint32_t cur_pin = 0x1 << Pin;
	if(GPIOx->IDR & cur_pin){
		return 1;
	}
	else{
		return 0;
	}
}

void GPIO_WriteBit(GPIO_TypeDef* GPIOx, int Pin, uint8_t BitVal){
	if(BitVal != 0){
		GPIOx->BSRR = 0x1 << Pin;
	}
	else{
		GPIOx->BSRR = 0x1 << (Pin + 16);
	}
}
