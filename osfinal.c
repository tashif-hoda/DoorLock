#include<stdio.h>
#include<rtl.h>
#include<lpc21xx.h>
#include<string.h>
#define IR (IO1PIN & (1<<24))

void lcd_init(void);
void __swi(8) unlock(void);
void __swi(9) wrong_pwd(void);
void cmd(unsigned int val);
void data(unsigned int dat);
void delay(unsigned int x);
void lock(void);
void init_serial(void);

unsigned char msg1[]="Unlocked";
unsigned char msg2[]="Locked";
unsigned char msg3[]="Wrong Password";

__task void task1(void);

void __SWI_8(void)
{
	int j;
	lcd_init();
	for(j=0;j<8;j++)
	{
	data(msg1[j]);
	delay(500000);
	}
	delay(65000);
}

void __SWI_9(void)
{
	 unsigned int j;
	 lcd_init();
	 for(j=0;j<14;j++)
	 {
		data(msg3[j]);
		delay(500000);
	 }
	 delay(65000);
}

void lock(void)
{
	 unsigned int j;
	 lcd_init();
	 for(j=0;j<6;j++)
	 {
		data(msg2[j]);
	  delay(500000);
	 }
	 delay(65000);
}

__task void task1(void)
{
	unsigned int z ;
	char input[10];
	lock();
	IODIR1 = 0x00000000;
	while(1)
	{	
		init_serial();
		while(!(IR==0));
		for(z=0;z<6;z++)
		{
		  while(!(U0LSR & 0x01));
			input[z] = U0RBR;
			U0THR = '*';
		}
		delay(15000);
		if(strcmp(input, "abcdef")==0)
		{
			unlock();
		}
		else
			wrong_pwd();
	}
}
void init_serial(void)
{
	PINSEL0 = 0x00000005;
	U0LCR = 0x83;
	U0DLL = 0x61;
	U0LCR = 0x03;
}

void cmd(unsigned int val)
{
 unsigned int y;
 y=val;
 y=y& 0xf0;
 IOCLR0=0X000000FC;
 IOCLR0=0X00000004;
 IOSET0=y;
 IOSET0=0X00000008;
 delay(10);
 IOCLR0=0X00000008;

 y=val;
 y=y&0x0f;
 y=y<<4;
 IOCLR0=0X000000FC;
 IOCLR0=0X00000004;
 IOSET0=y;
 IOSET0=0X00000008;
 delay(10);
 IOCLR0=0X00000008;
}
 
void data(unsigned int dat)
{
	unsigned int y;
	y=dat;
	y=y& 0xf0;
	IOCLR0=0X000000FC;
	IOSET0=0X00000004;
	IOSET0=y;
	IOSET0=0X00000008;
	delay(10);
	IOCLR0=0X00000008;

	y=dat;
	y=y&0x0f;
	y=y<<4;
	IOCLR0=0X000000FC;
	IOSET0=0X00000004;
	IOSET0=y;
	IOSET0=0X00000008;
	delay(10);
	IOCLR0=0X00000008;
}
void delay(unsigned int x)
{
	for(;x>0;x--);
}

int main(void)
{
	init_serial();
	os_sys_init(task1);
}
void lcd_init()
{
	unsigned int i;
	unsigned int c[] = {0x30,0x30,0x20,0x20,0x28,0x01,0x06,0x0e,0x80};
	PINSEL0 = 0X00000000;
	IODIR0 = 0X000000FC;
	IOCLR0 = 0X000000FC;
	for(i=0;i<9;i++)
	{
	 cmd(c[i]);
	 delay(10000);
	}
  cmd(0x01);
	delay(65000);
	delay(65000);
	delay(65000);
  cmd(0x80);
	delay(10000);
}