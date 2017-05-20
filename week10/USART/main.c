#include "stm32f4xx.h"                  // Device header

void startup(void);
void putty_Begin(int baud_rate);

int frequency = 5;	// unit: Hz
int duty = 50;	// unit: %
uint32_t nH, nL;
int main(void){
	startup();
	putty_Begin(9600);
	
	char data;
	while(1){
		if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)){
			data = USART_ReceiveData(USART2);
			while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
			USART_SendData(USART2, data);			
			
			if(data >= '1' && data <= '9'){
				frequency = data - '0';
				int period = SystemCoreClock / 16000 / frequency;	// Unit: cycle
				nH = period * (float)duty / 100;
				nL = period - nH;
				TIM_Cmd(TIM2, ENABLE);
			}
		}
	}	
	return 0;
}

uint8_t pin = 0;
void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line13) != 0){
		TIM_Cmd(TIM2, DISABLE);
		pin = 0;
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, pin);
		
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
		USART_SendData(USART2, '\f');	

		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != 0){
		TIM_SetCounter(TIM2, 0);
		if(pin != 0){	// High period timeout
			TIM_SetCompare1(TIM2, nL);
		}
		else{	// Low period timeout
			TIM_SetCompare1(TIM2, nH);
		}
		pin ^= 1;
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, pin);
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
}

void startup(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	pin = 0;
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, pin);
	int period = SystemCoreClock / 16000 / frequency;	// Unit: cycle
	nH = period * (float)duty / 100;
	nL = period - nH;
	
	TIM_SetCounter(TIM2, 0);
	TIM_PrescalerConfig(TIM2, 16000 - 1, TIM_PSCReloadMode_Immediate);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_SetCompare1(TIM2, nL);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	NVIC_EnableIRQ(EXTI15_10_IRQn);	
}

void putty_Begin(int baud_rate){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = baud_rate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStruct);
	
	USART_Cmd(USART2, ENABLE);
}
