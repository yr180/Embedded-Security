#include <LPC21xx.h>
#include "uart.h"

/******************************************************************
    This function defines the sBox operation
    data  : 32 bit unsigned long long int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int sBox(unsigned long int data, int length)
{
  unsigned long long int output = 0;
	unsigned long int sBox[] = {0x02,0x09,0x07,0x0e,
														  0x01,0x0c,0x0a,0x00,
														  0x04,0x03,0x08,0x0d,
														  0x0f,0x06,0x05,0x0b};

	unsigned int i;
  for(i=0;i<=length;i=i+4)
  {
		output |= sBox[(data>>(length-4-i))&0x0F]<<(length-4-i);
  }
 	return output;
}

/******************************************************************
    This function defines the pBox operation
    data  : 64 bit unsigned long long int
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int pBox(unsigned long int data)
{
	unsigned long long int output = 0;
	unsigned long long int pBox[] = {20,16,28,24,17,21,25,29,
																	 22,18,30,26,19,23,27,31,
                                   11,15, 3, 7,14,10, 6, 2, 
																		9,13, 1, 5,12, 8, 4, 0};
	unsigned int i;
	for(i=0;i<32;i++)
    {
        output |= ((data>>i)&0x01)<<pBox[i];
    }
	return output;
}

/******************************************************************
    This function defines the circular shift operation
    data  : 32 bit unsigned long int,
    flag   : integer value to select lShift(0)/rShift(1)
    shift  : specifies the number of shifts
    returns: 64 bit unsigned long long int
******************************************************************/
unsigned long long int circShift(unsigned long int data, int flag, int shift)
{
    if(flag == 0)   //lShift
	{
	    data = (((data << shift)|(data >> (32-shift))) & 0xFFFFFFFF);
	    //printf("lshift: %lx\n",data);
	}
	else            //rShift
    {
	    data = (((data >> shift)|(data << (32-shift))) & 0xFFFFFFFF);
	    //printf("rshift: %lx\n",data);
	}
	return data&0x00000000ffffffff;
}

/******************************************************************
    This is the main function
******************************************************************/
int main()
{
  unsigned long long int key[] = {0,0}, keyNew[] = {0,0}, data = 0, 
												 temp1=0, temp2=0;
	unsigned int i,j;
	unsigned char temp=0,m=0;
	UART_Init(9600);
	
	
	/***************************Plain Text**************************/
	UART_transmit('p');
	UART_transmit('-');

	for(i=0;i<16;i++)
	{
		m = UART_receive();
		UART_transmit(m);
		if(m>0x2f && m<0x3a)
		{
			temp = m-0x30;
		}
		if(m>=0x61 && m<=0x66)
		{
			temp = m-87;
		}
		data += (temp&0xF)<<((15-i)*4);		//data-> 64 bit plain text
	}
	/***************************************************************/
	/***************************Key Value***************************/
	
	for(j=0;j<2;j++)
	{
		for(i=0;i<16;i++)
		{
			m = UART_receive();
			UART_transmit(m);
			if(m>0x2f && m<0x3a)
			{
				temp = m-0x30;
			}
			if(m>=0x61 && m<=0x66)
			{
				temp = m-87;
			}
			key[j] += (temp&0xF)<<((15-i)*4);		//key -> 2x64 bit key
		}
	}
	/***************************************************************/
	/************************Key Scheduling*************************/
	
	for(i=0;i<25;i++)
	{
			//printf("\n--------Round: %d--------\n",i);

			temp1 = (data>>32)&0x00000000ffffffff;
			temp2 = (data)&0x00000000ffffffff;

			temp2 = sBox((circShift(temp1,0,3)&0x00000000ffffffff),32)^
							(temp2&0x00000000ffffffff);
			temp2 = sBox((circShift(temp1,1,8)&0x00000000ffffffff),32)^
							(temp2&0x00000000ffffffff)^((key[1])&0x00000000ffffffff);

			temp1 = pBox(temp1)&0x00000000ffffffff;
			temp2 = pBox(temp2)&0x00000000ffffffff;

			//Shifting
			keyNew[0] = (key[0]<<13)|((key[1]>>51)&0x1fff);
			keyNew[1] = (key[1]<<13)|((key[0]>>51)&0x1fff);

			//s-box
			keyNew[1] = (keyNew[1]&0xffffffffffffff00) | 
									(sBox((keyNew[1]&0x00000000000000ff),8)&0x00000000000000ff);

			//round count
			keyNew[1] = (keyNew[1]&0x07ffffffffffffff) | 
								  (((keyNew[1]>>59)^(i&0x0000001f))<<59);

			//printf("key LSB: 0x%llx\n",keyNew[1]);

			//Update keys
			key[0] = keyNew[0];
			key[1] = keyNew[1];
			keyNew[0] = 0;
			keyNew[1] = 0;

			data = ((temp2<<32)&0xffffffff00000000)|(temp1&0x00000000ffffffff);
			//printf("Output %d: 0x%llx",i,data);
	}
	
	/***************************************************************/
	/************************Tranmit cipher*************************/
	
	temp = 0;
	UART_transmit(' ');
	UART_transmit('c');
	UART_transmit('-');
	
	for(i=0;i<16;i++)
	{
		temp = (data>>(15-i)*4)&0xF;
		if(temp<10)
			UART_transmit(0x30+temp);
		else
			UART_transmit(87 + temp);
	}
	temp=0;
	
	/***************************************************************/
	return 0;
}
