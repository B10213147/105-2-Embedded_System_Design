#include "stm32f4xx.h"                  // Device header

void max_Sort(void);
int max3(int a, int b, int c);
int max2(int a, int b);

int first, second, third;
int x = 5, y = 6, z = 7;
int main(void){
	max_Sort();	
	return 0;
}

void max_Sort(void){	
	first = max3(y, z, x);
	if(first == x){
		second = max2(y, z);
		if(second == y) third = z;
		else third = y;
	}
	else if(first == y){
		second = max2(x, z);
		if(second == x) third = z;
		else third = x;
	}
	else{	// temp == z
		second = max2(x, y);
		if(second == x) third = y;
		else third = x;
	}
}

int max3(int a, int b, int c){
	if(a >= b && a >= c) return a;
	else if(b >= a && b >= c) return b;
	else return c;
}

int max2(int a, int b){
	if(a >= b) return a;
	else return b;
}
