#include "stm32f4xx.h"                  // Device header
#include <math.h>

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

int main(void){
	struct point p0 = {0, 0};
	struct point p1 = {5, 4};
	struct package info1 = {p0, p1};
	int dx = info1.b.x - info1.a.x;
	int dy = info1.b.y - info1.a.y;
	info1.slope = (float)dy / dx;
	info1.distance = sqrt(pow(dx, 2) + pow(dy, 2));
	return 0;
}