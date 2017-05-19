#include "stm32f4xx.h"                  // Device header

void putty_Begin(int baud_rate);

int main(void){
	putty_Begin(9600);
	char data;
	while(1){
		if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)){
			data = USART_ReceiveData(USART2);
			if(USART_GetFlagStatus(USART2, USART_FLAG_TXE)){
				USART_SendData(USART2, data);
			}
		}
	}	
	return 0;
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
