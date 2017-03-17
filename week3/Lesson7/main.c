#include "stm32f4xx.h"                  // Device header
#include <math.h>
#include "array_length.h"

struct point{
	int x;
	int y;
};

struct package{
    struct point a;
    struct point b;
    float distance;
    float slope;
};

void selection_Sort(struct package *a, int length);
void bubble_Sort(struct package *a_start, struct package *a_end);
void swap(struct package *a, struct package *b);

struct point p_array[8] = {
	{0, 100}, {1270, 280}, {1560, 220}, {2946, 350},
	{10453, 120}, {12690, 285}, {15034, 789}, {16656, 751}
};
struct package info[7];
int main(void){
	for(int i = 0; i < 7; i++){
		info[i].a = p_array[i];
		info[i].b = p_array[i+1];
	}
	for(int i = 0; i < 7; i++){
		int dx = info[i].b.x - info[i].a.x;
		int dy = info[i].b.y - info[i].a.y;
		info[i].slope = (float)dy / dx;
		info[i].distance = sqrt(pow(dx, 2) + pow(dy, 2));
	}
	bubble_Sort(info + 2, info + 7);
	selection_Sort(info, ARRAY_LENGTH(info));
	return 0;
}

void selection_Sort(struct package *a, int length){
	for(int i = 0; i < length; i++){
		int min_loc = i;
		for(int j = i + 1; j < length; j++){
			if(a[j].slope < a[min_loc].slope){
				min_loc = j;
			}
		}
		if(min_loc != i){
			swap(a + min_loc, a + i);
		}
	}
}

void bubble_Sort(struct package *a_start, struct package *a_end){
	int length = a_end - a_start;
	int i = length - 1, newi;
	do{
		newi = 0;
		for(int j = 0; j < i; j++){
			if(a_start[j].distance > a_start[j+1].distance){
				swap(a_start + j, a_start + j + 1);
				newi = j;
			}
		}
		i = newi;
	}while(i != 0);
}

void swap(struct package *a, struct package *b){
	struct package tmp = *a;
	*a = *b;
	*b = tmp;
}
