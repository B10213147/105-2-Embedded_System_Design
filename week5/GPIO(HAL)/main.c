#include "stm32f4xx.h"                  // Device header
#include "gpio.h"

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

