#include "master.h"
extern u8 P_A,D_A,P_S,I_S,P_T,D_T;
void order(void)
{
    u8 temp[3] = {0};
    unsigned char databuf[3] = {0};
    unsigned char i;
    uart_getchar (DEBUG_PORT,databuf);
		  for(i=0;i<2;i++)
    {
			  temp[i]=(unsigned int)databuf[i];
    }
		if((temp[0]==0x01)&&(temp[1]==0xfd))
		{
			send_pid();
    }
}
void get_pid(void)
{
    u8 temp[8] = {0};
    unsigned char databuf[8] = {0};
    unsigned char i;
        uart_getchar (DEBUG_PORT,databuf);
			  for(i=0;i<8;i++)
    {
			 	  temp[i]=(unsigned int)databuf[i];
    }
		if((temp[0]==0x03)&&(temp[1]==0xfb))
		{
			P_A=temp[2];
	   	D_A=temp[4];
			P_S=temp[5];
			I_S=temp[6];
    }
}
void send_pid(void)
{
    u8 temp1[10] = {0};
    unsigned char databuf1[9] = {0};
    unsigned char j;
			temp1[0]=0x02;
			temp1[1]=0xfc;
			temp1[2]=P_A;
		  temp1[4]=D_A;
			temp1[5]=P_S;
			temp1[6]=I_S;
      temp1[8]=0xfc;
	  	temp1[9]=0x02;
		  for(j=0;j<9;j++)
    {databuf1[j]=temp1[j];
        uart_putchar (DEBUG_PORT,(char)databuf1[j]);
    }
}
void send_sensor(void)
{
	 u8 temp[10] = {0};
    unsigned char databuf[10] = {0};
    unsigned char j;
		  for(j=0;j<9;j++)
    {databuf[j]=temp[j];
        uart_putchar (DEBUG_PORT,(char)databuf[j]);
    }
}
void send_road( char data )
{	 u8 temp[6] = {0};
    unsigned char databuf[9] = {0};
    unsigned char j;
			temp[0]=0x05;
			temp[1]=0xf9;
		  temp[3]=data;
      temp[4]=0xf9;
	  	temp[5]=0x05;
		  for(j=0;j<5;j++)
    {databuf[j]=temp[j];
        uart_putchar (DEBUG_PORT,(char)databuf[j]);
    }
	}



	
