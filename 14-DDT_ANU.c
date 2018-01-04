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
	unsigned long long int sBox[] = {0,9,0xc,1,5,8,8,2,0xa,1,4,0xa,2,8,8,1};
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

/******************************************************************
    This function defines the circular shift operation
    input  : 32 bit unsigned long int,
    flag   : integer value to select lShift(0)/rShift(1)
    shift  : specifies the number of shifts
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int pBox(unsigned long int input)
{
	unsigned long long int output = 0;
	//input = input&0x00000000ffffffff;
	unsigned long long int pBox[] = {20,16,28,24,17,21,25,29,22,18,30,26,19,23,27,31,
                                     11,15,3, 7, 14,10,6 ,2, 9, 13,1, 5, 12,8,4,0};
	unsigned int i;
	for(i=0;i<32;i++)
    {
        output |= ((input>>i)&0x01)<<pBox[i];
    }
	return output;
}

/******************************************************************
    This is the main function
******************************************************************/
void main()
{
    unsigned long long int input = 0x0000000000000000, temp1=0, temp2=0;
	unsigned int i,j;
	printf("Initial Input: 0x%llx\n", input);
	for(j=0;j<8;j++)
    {
        printf("\n--------Round: %d--------\n",j);
        input = (0x0000000000000001)*j;
        printf("Input: 0x%llx\n", input);
        for(i=1;i<5;i++)
        {
            temp1 = (input>>32)&0x00000000ffffffff;
            temp2 = (input)&0x00000000ffffffff;

            temp2 = sBox((circShift(temp1,0,3)&0x00000000ffffffff),32)^(temp2&0x00000000ffffffff);
            temp2 = sBox((circShift(temp1,1,8)&0x00000000ffffffff),32)^(temp2&0x00000000ffffffff);

            temp1 = pBox(temp1)&0x00000000ffffffff;
            temp2 = pBox(temp2)&0x00000000ffffffff;
            input = temp2<<32|temp1;
            printf("Sub-round: %d  Active S-Box count: %d\n",i,count);
            printf("Output: 0x%llx %llx\n",temp2,temp1);
            temp1 = 0;
            temp2 = 0;
        }
        printf("Active S-Box count: %d",count);
        count = 0;
    }
}

