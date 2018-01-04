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
unsigned long long int sBox(unsigned long long int input, int length)
{
    unsigned long long int output = 0;
	unsigned long long int sBox[] = {0x06,0x03,0x0a,0x05,
                                     0x0c,0x08,0x01,0x0b,
                                     0x00,0x0d,0x09,0x0e,
                                     0x0f,0x02,0x07,0x04};
	unsigned int i;
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
	for(i=0;i<64;i++)
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
	unsigned long long int key[] = {0,0}, keyNew[] = {0,0}, input = 0x1234567890abcdef;
	unsigned int i;
	printf("Initial Input: 0x%llx\n", input);
	printf("Initial Key  : 0x%llx%llx\n", key[0],key[1]);
	for(i=0;i<31;i++)
	{
	    printf("\n--------Round: %d--------\n",i);

	    input = input^key[1];
	    input = sBox(input,64);
	    input = pBox(input);
	    //Shifting
        keyNew[0] = (key[0]<<13)|((key[1]>>51)&0x1fff);
        keyNew[1] = (key[1]<<13)|((key[0]>>51)&0x1fff);

        //s-box
        keyNew[1] = (keyNew[1]&0xffffffffffffff00) | (sBox((keyNew[1]&0x00000000000000ff),8)&0x00000000000000ff);

        //round count
        keyNew[1] = (keyNew[1]&0x07ffffffffffffff) | (((keyNew[1]>>59)^(i&0x0000001f))<<59);

        printf("key LSB   : 0x%llx\n",keyNew[1]);
        printf("Output %d: 0x%llx",i,input);

        //Update keys
        key[0] = keyNew[0];
        key[1] = keyNew[1];
        keyNew[0] = 0;
        keyNew[1] = 0;
	}
}
