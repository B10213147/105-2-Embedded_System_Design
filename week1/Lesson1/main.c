#include "stm32f4xx.h"                  // Device header

int main(void){
	int a = 1, b = 4;
	
	int ans = a + b;
	ans = a - b;
	ans = a * b;
	float ans_f = (float)a / b;
	ans = a % b;
	
	return 0;
}
