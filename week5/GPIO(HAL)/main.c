#include "stm32f4xx.h"                  // Device header

void GPIO_Set_Direction(GPIO_TypeDef* GPIOx, uint16_t inputMask, uint16_t outputMask);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, int Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, int Pin, uint8_t BitVal);

int main(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;	// Enable GPIOA, GPIOC clock
	
	GPIO_Set_Direction(GPIOA, 0, 0x0020);	// User LD2: PA5
	GPIO_Set_Direction(GPIOC, 0x2000, 0);	// B1 USER: PC13
	
	while(1){
		if(!GPIO_ReadInputDataBit(GPIOC, 13)){	// Button pressed
			GPIO_WriteBit(GPIOA, 5, 1);
		}
		else{	// Button released
			GPIO_WriteBit(GPIOA, 5, 0);
		}
	}
	
	return 0;
}

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
