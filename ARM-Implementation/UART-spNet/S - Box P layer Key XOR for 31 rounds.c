#include<LPC21xx.h>
unsigned long long int sbox(unsigned long long int);
unsigned long long int player(unsigned long long int);
void tx(unsigned int c); //function that transmit char (user see on uart terminal)
char rx(void); // function that receive anything
unsigned int P[64]={ 0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51,4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55,8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59,12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63};
unsigned int s[16]= {6,3,0xa,5,0xc,8,1,0xb,0,0xd,9,0xe,0xf,2,7,4};


int main()
 {
	unsigned long long int k[2]={0x00,0x00},data = 0x00,temp;
	unsigned int j1,i1,m,r,i;
	
	PINSEL0 = 0x05; // configuration for tx and rx in pinsel
	U0LCR = 0x83; // configure the line control register
	U0DLL = 0x61;
	U0DLM = 0x00;// config baud rate 9600 (calculate in bps ---> convert into hex --> then load into DLL & DLM)
	U0LCR = 0x03; // lock baud rate by resetting baud rate bit in U0LCR register	 
	
	//______________Enter Plain Text 64 bit from UART___________________________
	
	tx('P');
	tx('-');		
			for (i1=0;i1<=15;i1++)
			{
				m = rx();
				tx(m);
					if (m>0x2f && m<0x3a)
					{
						temp = m-0x30;
					}
				 if(m>=0x61 && m<=0x66)
					{
					 temp = m-87;
					}
					data+=(temp&0xF)<<((15-i1)*4);; 
			}	
			
	//______________Enter Key Value 128 bit from UART___________________________			
			
		tx(' ');
		tx('K');
		tx('-'); 
		for (j1=0; j1<2; j1++)
		{
			//k[j1] =0;
				for (i1=0;i1<=15;i1++)
				{
					m = rx();
					tx(m);
			 
					if (m>0x2F && m<0x3a)
					{
						temp = m-0x30;
					}	
					if(m>=0x61 && m<=0x66)
					{
						temp = m-87;
					}
					k[j1] += (temp & 0xf) << ((15-i1)*4); 
				}	
		}
//_______________Main Design Logic Starts from here___________________
		
	for(i1=0; i1<31; i1++)
	{
	data = data^k[1];
	data=sbox(data);
	data=player(data);
	
	//__________key scheduling___________________
			temp=k[1];
		k[1]= (temp<<13)|(k[0]>>(64-13));
		k[0]= (k[0]<<13)|(temp>>(64-13));
		
		//Sbox	
		m=0;	
		for(i=0; i<2; i++)
		{
			m|=(s[(k[1]>>(i*4))&0XF]<<i*4)&0xff;
		}
		k[1] &= (0Xffffffffffffff00);  
		k[1] |= ((m&0xFF));
		
		//RC xor	
		temp = (((k[1]&0XF800000000000000)>>(64-5))^(0x1f&i1));
		k[1] &= 0X07FFFFFFFFFFFFFF;
		k[1] |= temp<<(64-5);
		
	}
		
//_______________Output Data print on UART________________________

	 tx(' ');
	 tx('C');
	 tx('-');
	for (i1=0; i1<=15; i1++)
	{
		temp=(data>>(15-i1)*4)&0xf;	
			if(temp<10)
			tx(0x30+temp);
			else
			tx(87+temp);
	}
 temp =0;

//__________________________________________________________________	 
	
	
}

unsigned long long int sbox(unsigned long long int c)
{
	unsigned long long int i,z=0,z1=0,z2=0;
	for (i=0;i<8;i++)
	{
		z1 |= s[((c>>(i*4))&0xf)]<<(i*4);
		z2 |= s[(c>>((i+8)*4)&0xf)]<<(i*4);
		
	}
	z = ((z2<<32)&0xffffffff00000000)|(z1 & 0xffffffff);
	 return (z);
}
unsigned long long int player(unsigned long long int b)
{
	unsigned long long int c=0,i;
	for(i=0;i<64;i++)
	{
		c |=((b>>i) & 0x1) << P[i];
	}
	return(c);
}
void tx(unsigned int c)
{
 while(!(U0LSR & 0x20)); // check the transmitter data available bit in U0LSR
 U0THR = c; // if data is available it will transmit by jumping from above line (while !)
}
 char rx(void)
{
	unsigned char m;
	while(!(U0LSR & 0x01)); // check the transmitter data available bit in U0LSR
  m = U0RBR; // if data is available it will transmit by jumping from above line (while !)
  return(m);
}
