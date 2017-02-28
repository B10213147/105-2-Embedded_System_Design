#ifndef ARRAY_LENGTH_H_INCLUDED
#define ARRAY_LENGTH_H_INCLUDED

#include <iostream>

#define ARRAY_LENGTH(a)     (sizeof(a) / sizeof(*a))

using namespace std;

void int_array_Print(int *a, unsigned int length);

#endif // ARRAY_LENGTH_H_INCLUDED
