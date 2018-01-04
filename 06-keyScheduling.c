/*******************************************************************
    Author: Yash Rao
    Date  : 27/5/17
*******************************************************************/

#include <stdio.h>

/******************************************************************
    This function defines the sBox operation
    input  :  8 bit unsigned int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int sBox(unsigned int input)
{
    printf("sBox input: 0x%lx\n",input);
    unsigned long long int output = 0;
	unsigned int sBox[] = {0x03,0x0f,0x0e,0x01,
                           0x00,0x0a,0x05,0x08,
                           0x0c,0x04,0x0b,0x02,
                           0x09,0x07,0x06,0x0d};
	unsigned int i,length = 8;
	for(i=0;i<length;i=i+4)
    {
        output |= (sBox[(input>>(length-4-i))&0x0F])<<(length-4-i);
    }
    printf("sBox output: 0x%llx\n",output);
	return output;
}

/******************************************************************
    This is the main function
******************************************************************/
void main()
{
	unsigned long long int key[] = {0, 0}, keyNew[] = {0,0};
	unsigned int i;
	printf("Input: 0x%llx%llx\n", key[0],key[1]);
	for(i=0;i<31;i++)
	{
	    printf("\n--------Round: %d--------\n",i);
	    //Shifting
        keyNew[0] = (key[0]<<13)|((key[1]>>51)&0x1fff);
        keyNew[1] = (key[1]<<13)|((key[0]>>51)&0x1fff);
        printf("keyShift: 0x%llx %llx\n", keyNew[0],keyNew[1]);

        //s-box
        keyNew[1] = (keyNew[1]&0xffffffffffffff00) | sBox(keyNew[1]&0x00000000000000ff);
        printf("keyS-box: 0x%llx %llx\n", keyNew[0],keyNew[1]);

        //round count
        keyNew[1] = (keyNew[1]&0x07ffffffffffffff) | (((keyNew[1]>>59)^(i&0x0000001f))<<59);

        printf("key RC: 0x%llx %llx\n", keyNew[0],keyNew[1]);
        key[0] = keyNew[0];
        key[1] = keyNew[1];
        keyNew[0] = 0;
        keyNew[1] = 0;
	}
}
