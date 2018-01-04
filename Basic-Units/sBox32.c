/***************************************************
char			8 bit	-127 - 127
short int		16 bit
int				32 bit
long int
long long int 	64 bit

unsigned char			8 bit	0 - 255
unsigned short int		16 bit
unsigned int
unsigned long int
unsigned long long int	64 bit
***************************************************/
#include <stdio.h>

void main()
{
	unsigned int input = 0xEF;
	unsigned int sBox[] = {0x03,0x0F, 0x0E, 0x01, 0x00, 0x0A, 0x05, 0x08, 0x0C, 0x04, 0x0B, 0x02, 0x09, 0x07, 0x06, 0x0D};
	unsigned int output = 0;
	unsigned int i,length = 32;
	printf("Enter plain text: ");
	scanf("%x", &input);
	//printf("%x\n",input);
	for(i=0;i<=length;i=i+4)
    {
        output |= (sBox[(input>>(length-4-i))&0x0F])<<(length-4-i);
    }
	printf("output: 0x%x", output);
}
