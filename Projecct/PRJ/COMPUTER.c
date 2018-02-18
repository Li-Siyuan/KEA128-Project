#include "COMPUTER.h"
                                 //SCIÊ¾²¨Æ÷Ê¾Êý

void sensor_DATA(int16 data1,int16 data2,int16 data3)
{
	  u8 temp[11] = {0};
		unsigned char databuf[11] = {0};
		 unsigned char j;
		  temp[0]=0x04;
			temp[1]=0xfa;
      temp[8]=0xfa;
	  	temp[9]=0x04;
      temp[2]  = (unsigned char)(data1%256);
      temp[3] = (unsigned char)(data1/256);
		  temp[4]  = (unsigned char)(data2%256);
      temp[5] = (unsigned char)(data2/256);
		  temp[6]  = (unsigned char)(data3%256);
      temp[7] = (unsigned char)(data3/256);
      
		  for(j=0;j<10;j++)
    {databuf[j]=temp[j];
        uart_putchar (DEBUG_PORT,(char)databuf[j]);
    }
}

