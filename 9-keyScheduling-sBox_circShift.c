/*******************************************************************
    Author: Yash Rao
    Date  : 27/5/17
*******************************************************************/

#include <stdio.h>

/******************************************************************
    This function defines the sBox operation
    input  : 64 bit unsigned long long int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int sBox(unsigned long int input, int length)
{
    unsigned long int output = 0;
	unsigned long int sBox[] = {0x03,0x0f,0x0e,0x01,
                                0x00,0x0a,0x05,0x08,
                                0x0c,0x04,0x0b,0x02,
                                0x09,0x07,0x06,0x0d};
	unsigned int i;
	for(i=0;i<=length;i=i+4)
    {
        output |= (sBox[(input>>(length-4-i))&0x0F])<<(length-4-i);
    }
	return output;
}

/******************************************************************
    This function defines the circular shift operation
    input  : 32 bit unsigned long int,
    flag   : integer value to select lShift(0)/rShift(1)
    shift  : specifies the number of shifts
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int circShift(unsigned long int input, int flag, int shift)
{
    if(flag == 0)   //lShift
	{
	    input = (((input << shift)|(input >> (32-shift))) & 0xFFFFFFFF);
	    printf("lshift: %lx\n",input);
	}
	else            //rShift
    {
	    input = (((input >> shift)|(input << (32-shift))) & 0xFFFFFFFF);
	    printf("rshift: %lx\n",input);
	}
	return input;
}

/******************************************************************
    This is the main function
******************************************************************/
void main()
{
    unsigned long long int key[] = {0,0}, keyNew[] = {0,0}, input = 0x0000000000000000, temp1=0, temp2=0;
	unsigned int i;
	printf("Initial Input: 0x%llx\n", input);
	printf("Initial Key  : 0x%llx%llx\n", key[0],key[1]);
	for(i=0;i<31;i++)
	{
	    printf("\n--------Round: %d--------\n",i);

	    temp1 = sBox((input>>32),32)&0x00000000ffffffff;
	    temp2 = temp1^(key[1]&0x00000000ffffffff)^(input&0x00000000ffffffff);
	    temp2 = circShift(temp2,0,3);
	    temp1 = temp1^temp2^((key[1]>>32)&0x00000000ffffffff);
	    temp1 = circShift(temp1,1,7);

	    //Shifting
        keyNew[0] = (key[0]<<13)|((key[1]>>51)&0x1fff);
        keyNew[1] = (key[1]<<13)|((key[0]>>51)&0x1fff);

        //s-box
        keyNew[1] = (keyNew[1]&0xffffffffffffff00) | (sBox((keyNew[1]&0x00000000000000ff),8)&0x00000000000000ff);

        //round count
        keyNew[1] = (keyNew[1]&0x07ffffffffffffff) | (((keyNew[1]>>59)^(i&0x0000001f))<<59);

        printf("key LSB   : 0x%llx\n",keyNew[1]);

        //Update keys
        key[0] = keyNew[0];
        key[1] = keyNew[1];
        keyNew[0] = 0;
        keyNew[1] = 0;
        input = ((temp2<<32)&0xffffffff00000000)|(temp1&0x00000000ffffffff);
        printf("Output %d: 0x%llx",i,input);
	}
}
