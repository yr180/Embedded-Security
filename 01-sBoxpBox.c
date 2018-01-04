/*******************************************************************
    Author: Yash Rao
    Date  : 26/5/17
    This code has a sBox and a pBox in the form:
                   Input
                     |
                    sBox
                     |
                    pBox
                     |
                   Output
*******************************************************************/


#include <stdio.h>

/******************************************************************
    This function defines the sBox operation
    input  : 64 bit unsigned long long int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int sBox(unsigned long long int input)
{
    unsigned long long int output = 0;
	unsigned long long int sBox[] = {0x06,0x03,0x0a,0x05,
                                     0x0c,0x08,0x01,0x0b,
                                     0x00,0x0d,0x09,0x0e,
                                     0x0f,0x02,0x07,0x04};
	unsigned int i,length = 64;
	for(i=0;i<=length;i=i+4)
    {
        output |= (sBox[(input>>(length-4-i))&0x0F])<<(length-4-i);
    }
	return output;
}

/******************************************************************
    This function defines the pBox operation
    input  : 64 bit unsigned long long int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int pBox(unsigned long long int input)
{
	unsigned long long int output = 0;
	unsigned long long int pBox[] = {0, 16,32,48,1, 17,33,49,
                                     2, 18,34,50,3, 19,35,51,
                                     4, 20,36,52,5, 21,37,53,
                                     6, 22,38,54,7, 23,39,55,
                                     8, 24,40,56,9, 25,41,57,
                                     10,26,42,58,11,27,43,59,
                                     12,28,44,60,13,29,45,61,
                                     14,30,46,62,15,31,47,63};
	unsigned int i;
	for(i=0;i<=63;i++)
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
	sB = sBox(input);
	pB = pBox(sB);
	printf("Output: 0x%llx", pB);
}
