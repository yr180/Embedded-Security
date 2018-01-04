/*******************************************************************
    Author: Yash Rao
    Date  : 29/5/17
*******************************************************************/

#include <stdio.h>
unsigned int count=0;
/******************************************************************
    This function defines the sBox operation
    input  : 32 bit unsigned long long int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int sBox(unsigned long long int input, int length)
{
    unsigned long long int output = 0;
	unsigned long long int sBox[] = {0x00,0x05,0x03,0x02,
                                     0x05,0x08,0x04,0x08,
                                     0x09,0x08,0x02,0x08,
                                     0x01,0x01,0x01,0x01};
	unsigned int i;
	input = input&0x00000000ffffffff;
	//printf("input Sbox: %llx\n",input);
    for(i=0;i<=length;i=i+4)
    {
        output |= sBox[(input>>(length-4-i))&0x0F]<<(length-4-i);
        if(((input>>(length-4-i))&0x0F) > 0)
        {
            count++;
        }
    }
    //printf("Output Sbox: %llx, Count: %d\n",output,count);
	return output;
}

unsigned long long int circShift(unsigned long int input, int flag, int shift)
{
    if(flag == 0)   //lShift
	{
	    input = (((input << shift)|(input >> (32-shift))) & 0xFFFFFFFF);
	    //printf("lshift: %lx\n",input);
	}
	else            //rShift
    {
	    input = (((input >> shift)|(input << (32-shift))) & 0xFFFFFFFF);
	    //printf("rshift: %lx\n",input);
	}
	return input;
}

void main()
{
    unsigned long long int input = 0x0000000000000000, temp1=0, temp2=0;
	unsigned int i,j;
	printf("Initial Input: 0x%llx\n", input);
	for(j=1;j<16;j++)
    {
        printf("\n--------Round: %d--------\n",j);
        input = (0x0000000000000000|j)<<(4*(j-1));
        printf("Input: 0x%llx\n", input);
        for(i=1;i<6;i++)
        {
            temp1 = sBox((input>>32),32)&0x00000000ffffffff;
            temp2 = temp1^(input&0x00000000ffffffff);
            temp2 = circShift(temp2,0,3)&0x00000000ffffffff;
            temp1 = temp1^temp2;
            temp1 = circShift(temp1,1,7)&0x00000000ffffffff;
            input = temp2<<32|temp1;
            //printf("Sub-round: %d  Active S-Box count: %d\n",i,count);
            //printf("Output: 0x%llx %llx\n",temp2,temp1);
            temp1 = 0;
            temp2 = 0;
        }
        printf("Active S-Box count: %d",count);
        count = 0;
    }
}
