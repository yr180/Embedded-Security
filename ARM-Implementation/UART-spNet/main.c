#include <LPC21xx.h>
#include "uart.h"

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
int main()
{
  unsigned long long int key[] = {0,0}, keyNew[] = {0,0}, data = 0;
	unsigned int i,j;
	unsigned char temp=0,m=0;
	UART_Init(9600);
	
	
	/***************************Plain Text**************************/
	UART_transmit('p');
	UART_transmit('-');

	for (i=0;i<=15;i++)
			{
				m = UART_receive();
				UART_transmit(m);
					if (m>0x2f && m<0x3a)
					{
						temp = m-0x30;
					}
				 if(m>=0x61 && m<=0x66)
					{
					 temp = m-87;
					}
					data+=(temp&0xF)<<((15-i)*4);; 
			}	
	/***************************************************************/
	/***************************Key Value***************************/
	UART_transmit(' ');
	UART_transmit('k');
	UART_transmit('-');
	
	for (j=0; j<2; j++)
		{
			//k[j1] =0;
				for (i=0;i<=15;i++)
				{
					m = UART_receive();
					UART_transmit(m);
			 
					if (m>0x2F && m<0x3a)
					{
						temp = m-0x30;
					}	
					if(m>=0x61 && m<=0x66)
					{
						temp = m-87;
					}
					key[j] += (temp & 0xf) << ((15-i)*4); 
				}	
		}
	/***************************************************************/
	/************************Key Scheduling*************************/
	for(i=0;i<31;i++)
	{
	    //printf("\n--------Round: %d--------\n",i);

	    data = data^key[1];
	    data = sBox(data,64);
	    data = pBox(data);
	    //Shifting
			keyNew[0] = (key[0]<<13)|((key[1]>>51)&0x1fff);
			keyNew[1] = (key[1]<<13)|((key[0]>>51)&0x1fff);

			//s-box
			keyNew[1] = (keyNew[1]&0xffffffffffffff00) | (sBox((keyNew[1]&0x00000000000000ff),8)&0x00000000000000ff);

			//round count
			keyNew[1] = (keyNew[1]&0x07ffffffffffffff) | (((keyNew[1]>>59)^(i&0x0000001f))<<59);

			//printf("key LSB   : 0x%llx\n",keyNew[1]);
			//printf("Output %d: 0x%llx",i,data);

			//Update keys
			key[0] = keyNew[0];
			key[1] = keyNew[1];
			keyNew[0] = 0;
			keyNew[1] = 0;
	}
	
	/***************************************************************/
	/************************Tranmit cipher*************************/
	
	temp = 0;
	UART_transmit('\n');
	UART_transmit('c');
	UART_transmit('-');
	
	for(i=0;i<16;i++)
	{
		temp=(data>>(15-i)*4)&0xf;	
		if(temp<10)
			UART_transmit(0x30+temp);
		else
			UART_transmit(87+temp);
	}
	temp=0;
	UART_transmit('\n');
	/***************************************************************/
	return 0;
}
