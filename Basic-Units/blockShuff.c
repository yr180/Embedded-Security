/******************************
    0,1,2,3,4,5,6,7
	2,3,0,1,6,7,4,5
******************************/

#include <stdio.h>

void main()
{
	unsigned long int input=0x01234567,output=0;
	unsigned long int bShift[] = {2,3,0,1,6,7,4,5};
	int i;
	for(i=0;i<8;i++)
	{
		output |= ((input>>(i*4)) & 0xf) << (bShift[i]*4);
	}
	printf("Output: 0x%lx", output);
}
