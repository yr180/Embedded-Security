/******************************
    0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
	8,9,a,b,c,d,e,f,0,1,2,3,4,5,6,7
******************************/

#include <stdio.h>

void main()
{
	unsigned long long int input=0x12345678abcdef09,output=0;
	unsigned long long int bShift[] = {8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};
	int i;
	for(i=0;i<16;i++)
	{
		output |= ((input>>(i*4)) & 0xf) << (bShift[i]*4);
	}
	printf("Output: 0x%llx", output);
}
