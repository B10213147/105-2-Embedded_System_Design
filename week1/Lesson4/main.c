#include "stm32f4xx.h"                  // Device header

void bubble_Sort(void);
void selection_Sort(void);

int nums[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

int main(void){
	bubble_Sort();
	selection_Sort();
	return 0;
}

void bubble_Sort(void){
	for(int i = 9; i > 0; i--){
		for(int j = 0; j < i; j++){
			if(nums[j] > nums[j+1]){
				int tmp = nums[j];
				nums[j] = nums[j+1];
				nums[j+1] = tmp;
			}
		}
	}
}

void selection_Sort(void){
	for(int i = 0; i < 10; i++){
		int max_loc = i;
		for(int j = i + 1; j < 10; j++){
			if(nums[max_loc] < nums[j]){
				max_loc = j;
			}
		}
		if(max_loc != i){
			int tmp = nums[max_loc];
			nums[max_loc] = nums[i];
			nums[i] = tmp;
		}
	}
}
