#include "stm32f4xx.h"                  // Device header

int main(void){
	// ex1
/*
	int a = 5, b = 6;
	int i = 1, ans = 1;

	while(i <= a){
		ans *= i;
		i++;
	}
	
	ans = 1;
	for(i = 1; i <= b; i++){
		ans *= i;
	}
*/
	// ex2
	int flag, ans;
	
	for(int i = 3; i <= 12; i++){
		flag = 1;
		for(int j = 2; j < i; j++){
			if(i % j == 0){
				flag = 0;
				break;
			}
		}
		if(flag == 1){
			ans = i;
		}
	}
	
	return 0;
}
