/*****************************************
	x 	0 1 2 3
   p(x)	2 3 0 1
*****************************************/

#include <stdio.h>

void main()
{
	unsigned long long int input = 0x0123456789abcdef, output = 0;
	unsigned long long int pBox[] = {0,16,32,48,1,17,33,49,
                                     2,18,34,50,3,19,35,51,
                                     4,20,36,52,5,21,37,53,
                                     6,22,38,54,7,23,39,55,
                                     8,24,40,56,9,25,41,57,
                                     10,26,42,58,11,27,43,59,
                                     12,28,44,60,13,29,45,61,
                                     14,30,46,62,15,31,47,63};
	unsigned int i;
	//printf("Enter plain text: ");
	//scanf("%llx",&input);

	for(i=0;i<=63;i++)
    {
        output |= ((input>>i)&0x01)<<pBox[i];
    }
	printf("Output: 0x%llx", output);
}
