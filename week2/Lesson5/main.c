#include "stm32f4xx.h"                  // Device header
#include <stdbool.h>

bool acomp(int a, int b){ return a > b; }
bool bcomp(int a, int b){ return a < b; }
void selection_Sort(int *a, int length);
void bubble_Sort(int *a_start, int *a_end);
void insertion_Sort(int *a_start, int *a_end, bool (*compare)(int, int));
void swap(int *a, int *b);

int nums[5] = {4, 3, 2, 1, 0};
int foo[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int main(void){
	bubble_Sort(nums, nums + 5);
	insertion_Sort(foo + 2, foo + 9, bcomp);
	//selection_Sort(nums, 5);
	//selection_Sort(foo, 10);
	return 0;
}

void selection_Sort(int *a, int length){
	for(int i = 0; i < length; i++){
		int min_loc = i;
		for(int j = i + 1; j < length; j++){
			if(a[j] < a[min_loc]){
				min_loc = j;
			}
		}
		if(min_loc != i){
			swap(a + min_loc, a + i);
		}
	}
}

void bubble_Sort(int *a_start, int *a_end){
	int length = a_end - a_start;
	int i = length - 1, newi;
	do{
		newi = 0;
		for(int j = 0; j < i; j++){
			if(a_start[j] > a_start[j+1]){
				swap(a_start + j, a_start + j + 1);
				newi = j;
			}
		}
		i = newi;
	}while(i != 0);
}

void insertion_Sort(int *a_start, int *a_end, bool (*compare)(int, int)){
	int length = a_end - a_start;
	for(int i = 1; i < length; i++){
		int tmp = a_start[i];
		int j = i - 1;
		while(j > -1 && compare(tmp, a_start[j])){
			a_start[j+1] = a_start[j];
			j--;
		}
		a_start[j+1] = tmp;
	}
}

void swap(int *a, int *b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
