#include <stdio.h>

void main()
{
	unsigned long long int input = 0, output = 0;
	unsigned long long int sBox[] = {0x03,0x0F, 0x0E, 0x01, 0x00, 0x0A, 0x05, 0x08, 0x0C, 0x04, 0x0B, 0x02, 0x09, 0x07, 0x06, 0x0D};
	unsigned int i,length = 64;
	printf("Enter plain text: ");
	scanf("%llx", &input);
	//printf("%x\n",input);
	for(i=0;i<=length;i=i+4)
    {
        output |= (sBox[(input>>(length-4-i))&0x0F])<<(length-4-i);
    }
	printf("output: 0x%llx", output);
}
