/*******************************************************************
    Author: Yash Rao
    Date  : 27/5/17
    This code has a sBox and a pBox in the form:
                   Input
              _______|_______
             |               |
            sBox            pBox
                \__________/
                /          \
            op1              op2
             |_______ _______|
                     |
                  Output
*******************************************************************/

#include <stdio.h>

/******************************************************************
    This function defines the sBox operation
    input  : 32 bit unsigned long int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int sBox(unsigned long int input)
{
    printf("sIn: %lx\n",input);
    unsigned long int output = 0;
	unsigned long int sBox[] = {0x03,0x0f,0x0e,0x01,
                                0x00,0x0a,0x05,0x08,
                                0x0c,0x04,0x0b,0x02,
                                0x09,0x07,0x06,0x0d};
	unsigned int i,length = 32;
	for(i=0;i<length;i=i+4)
    {
        output |= (sBox[(input>>(length-4-i))&0x0F])<<(length-4-i);
    }
	return output;
}

/******************************************************************
    This function defines the pBox operation
    input  : 32 bit unsigned long int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int pBox(unsigned long int input)
{
    printf("pIn: %lx\n",input);
	unsigned long int output = 0;
	unsigned long int pBox[] = {16,17,18,19,20,21,22,23,
                                 8, 9,10,11,12,13,14,15,
                                 0, 1, 2, 3, 4, 5, 6, 7,
                                24,25,26,27,28,29,30,31};
	unsigned int i;
	for(i=0;i<=31;i++)
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
	unsigned long long int input = 0x1234567890abcdef, sB=0, pB=0;
	sB = sBox((input>>32)&0xffffffff);
	pB = pBox((input)&0xffffffff);
	printf("Output: 0x%llx", (pB<<32)|sB);
}
