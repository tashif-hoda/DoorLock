#include<stdio.h>
#include<rtl.h>
#include<lpc21xx.h>
#include<string.h>
#define IR (IO1PIN & (1<<21))

void __swi(8) unlock(void);
void __swi(9) wrong_pwd(void);
void __swi(10) lock(void);
void __swi(11) enter_pass(void);
void __swi(12) enter_pass_uart(void);
void __swi(13) motor_unlock(void);
void __swi(14) motor_lock(void);

void lcd_display(char var[16], unsigned char n);
void cmd(unsigned int val);
void data(unsigned int dat);
void delay(unsigned int x);

void init_serial(void);

OS_TID t1, t2;

unsigned char msg1[]="Unlocked";
unsigned char msg2[]="Locked";
unsigned char msg3[]="Wrong Password";
unsigned char msg4[]="Enter Password:";

//char password[];
__task void task1(void);
__task void task2(void);



void __SWI_8(void)
{//unlock lcd display code
	unsigned int i;
	unsigned int c[]={0x30,0x30,0x20,0x20,0x28,0x01,0x06,0x0e,0x80};
	PINSEL0=0X00000000;
	IODIR0=0X000000FC;
	IOCLR0=0X000000FC;
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
	 for(i=0;i<8;i++)
	 {
		data(msg1[i]);
	delay(500000);
	delay(500000);
	delay(500000);
	}
	delay(65000);
	 delay(65000);
	delay(65000);
}

void __SWI_9(void)
{
	//wrong_pwd lcd display code
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
	 for(i=0;i<14;i++)
	 {
		data(msg3[i]);
	delay(500000);
	delay(500000);
	delay(500000);
	}
	delay(65000);
	 delay(65000);
	delay(65000);
}

void __SWI_10(void)
{	//lcd display lock function
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
	 for(i=0;i<6;i++)
	 {
		data(msg2[i]);
	delay(500000);
	delay(500000);
	delay(500000);
	}
	delay(65000);
	 delay(65000);
	delay(65000);
}

void __SWI_11(void)
{//enter password lcd display
	unsigned int i;
	unsigned int c[] = {0x30,0x30,0x20,0x20,0x28,0x01,0x06,0x0e,0x80};
	PINSEL0 = 0X00000000;
	IODIR0 = 0X000000FC;
	IOCLR0 = 0X000000FC;
	for(i=0;i<6;i++)
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
	 for(i=0;i<15;i++)
	 {
		data(msg4[i]);
	delay(500000);
	delay(500000);
	delay(500000);
	}
	delay(65000);
	 delay(65000);
	delay(65000);
}

void __SWI_12(void)
{
	//UART enter passwrod prompt
	unsigned char i;
	for(i=0; i<16; i++)
	{
		while(!(U0LSR & 0x20));
		U0THR = msg4[i];
	}
}

void __SWI_13(void)
{	//motor unlock
	IOCLR0 = 0x00000C00;
	IOSET0 = 0x00000400;
	lcd_display("unlocking, wait", 15);
	delay(65000);
	delay(65000);
	delay(65000);
	delay(65000);
	IOCLR0 = 0x00000C00;
}

void __SWI_14(void)
{ //motor lock
	IOCLR0 = 0x00000C00;
	IOSET0 = 0x00000800;
	lcd_display("locking, wait", 13);
	delay(65000);
	delay(65000);
	delay(65000);
	delay(65000);
	IOCLR0 = 0x00000C00;
}

__task void task2(void)
{
	//make these variables global if task doesn't work
	unsigned int z ;
	char input[20];// hold_str[10], count;
	char password[] = "unlock";
	while(1)
	{	
		while(IR==0)
		{
			for(z=0;1;z++)
			{
				while(!(U0LSR & 0x01));
				input[z] = U0RBR;
				if(input[z] == '\n')
				{
					U0THR = '\n';
					break;
				}
				U0THR = '*';
			}
			input[z] = '\0';
			if(strcmp(input, password)==0)
			{
				unlock();
				motor_unlock();
			}
			
			else
				wrong_pwd();
		os_tsk_prio_self(1);
		os_tsk_prio(t1, 2);
		}
	}
}


__task void task1(void)
{	
	IO1DIR = 0x01000000;
	t1 = os_tsk_self();
	t2 = os_tsk_create(task2, 0);
	while(1)
	{
		if(IR == 0)
		{
			enter_pass();
			enter_pass_uart();
			os_tsk_prio_self(1);
			os_tsk_prio(t2, 2);
		}
		lock();
	}
}

void lcd_display(char message[16], unsigned char n)
{
	unsigned int i;
	unsigned int c[] = {0x30,0x30,0x20,0x20,0x28,0x01,0x06,0x0e,0x80};
	PINSEL0 = 0X00000000;
	IODIR0 = 0X000000FC;
	IOCLR0 = 0X000000FC;
	for(i=0;i<6;i++)
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
	for(i=0;i<n;i++)
	{
		data(message[i]);
		delay(500000);
		delay(500000);
		delay(500000);
	}
	delay(65000);
	delay(65000);
	delay(65000);
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
	IO1DIR = 0x00000000;//IR sensor
	IODIR0 = 0x00000C00;//CN1 3 = P0.10 and 4 = P0.11 for motor
	os_sys_init(task1);
}