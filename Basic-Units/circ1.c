/*****************************
10101011    AB
*****************************/

#include <stdio.h>
#define shift 4
void main()
{
	unsigned long int input = 0xcccf1234;
	int flag = 1;

	if(flag == 0)   //lShift
	{
	    input = (((input << shift)|(input >> (32-shift))) & 0xFFFFFFFF);
        printf("Left Shift Output: 0x%lx",input);
	}
	else            //rShift
    {
	    input = (((input >> shift)|(input << (32-shift))) & 0xFFFFFFFF);
        printf("Right Shift Output: 0x%lx",input);
	}
}
