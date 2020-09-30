#include <stdio.h>
#include <lpc21xx.h>
#include <rtl.h>
#include "lcd.h"

void unlock(void);
void lock(void);
void cmd(unsigned int val);
void data(unsigned int dat);
void delay(unsigned int x);

unsigned char msg1[]="unlocked\n";
unsigned char msg2[]="locked\n";

void unlock(void)
{
	int i, j;
	unsigned int c[]={0x30,0x30,0x20,0x20,0x28,0x01,0x06,0x0e,0x80};
	PINSEL0=0X00000000;
	IODIR0=0X000000FC;
	IOCLR0=0X000000FC;
	for(i=0;i<9;i++)
	{
	cmd(c[i]);
	delay(10000);
	}


	 cmd(0x80);
	 delay(10000);
	 for(j=0;j<8;j++)
	 {
		data(msg1[j]);
	delay(500000);
	delay(500000);
	delay(500000);
	}
	delay(65000);
	 delay(65000);
	delay(65000);
	//delay(65000);
	//delay(65000);
	//delay(65000);
	//delay(65000);

	cmd(0x01);
	delay(65000);
	 delay(65000);
	delay(65000);
	//delay(65000);
	//delay(65000);
	//delay(65000);
	//delay(65000);
}

void lock(void)
{
	unsigned int i, j;
	unsigned int c[]={0x30,0x30,0x20,0x20,0x28,0x01,0x06,0x0e,0x80};
	PINSEL0 = 0X00000000;
	IODIR0 = 0X000000FC;
	IOCLR0 = 0X000000FC;
	for(i=0;i<6;i++)
	{
	cmd(c[i]);
	delay(10000);
	}


	 cmd(0x80);
	 delay(10000);
	 for(j=0;j<7;j++)
	 {
		data(msg2[j]);
	delay(500000);
	delay(500000);
	delay(500000);
	}
	delay(65000);
	 delay(65000);
	delay(65000);
	//delay(65000);
	//delay(65000);
	//delay(65000);
	//delay(65000);

	cmd(0x01);
	delay(65000);
	 delay(65000);
	delay(65000);
	//delay(65000);
	//delay(65000);
	//delay(65000);
	//delay(65000);
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
	unsigned int d;
	for(d=0;d<x;d++);
}