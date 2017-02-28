#include "array_length.h"

int main()
{
    int nums[5] = {0, 1, 2, 3, 4};
    int foo[10];

    unsigned int a = sizeof(nums) / sizeof(*nums);
    cout << a << endl;
    int_array_Print(nums, ARRAY_LENGTH(nums));
    int_array_Print(foo, ARRAY_LENGTH(foo));
/*
    for(int i = 0; i < 5; i++){
        cout << nums[i] << " ";
    }
    cout << endl;
    for(int i = 0; i < 10; i++){
        cout << foo[i] << " ";
    }
    cout << endl;
*/
/*
    cout << nums[0] << " ";
    cout << nums[1] << " ";
    cout << nums[2] << " ";
    cout << nums[3] << " ";
    cout << nums[4] << " ";
*/
    return 0;
}

void int_array_Print(int *a, unsigned int length){
    for(unsigned int i = 0; i < length; i++){
        cout << a[i] << " ";
    }
    cout << endl;
}
