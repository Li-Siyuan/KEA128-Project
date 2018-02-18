#include "protoco.h"

int16 OutData[4] = { 0 };                                    //SCI示波器示数

//**************************************************************************
/*
*  功能说明：SCI示波器CRC校验
内部调用函数
*  参数说明： 无
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//**************************************************************************
static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

//************************************************
//
/*
*  功能说明：SCI示波器发送函数

*  参数说明：
OutData[]  需要发送的数值赋予该数组
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//****************************************************
void OutPut_Data(void)
{
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
		u8 send_buff[32]={0};
		
    for(i=0;i<4;i++)
    {

        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];

    }

    for(i=0;i<4;i++)
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }

    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;

		#ifdef USE_UART
    for(i=0;i<10;i++)
    {
        uart_putchar (DEBUG_PORT,(char)databuf[i]);			
    }
		#endif
		
		#ifdef  USE_NRF
		send_buff[0] = 10;
		for(i=0;i<10;i++)
    {
        send_buff[i+1] = databuf[i];		
    }
		SEND_BUF(send_buff);
		#endif
}



void Send_Wave_char(void)
{
	u8 send_buff[32]={0};
	int i=0;
	
	  #ifdef USE_UART
		uart_putchar (DEBUG_PORT,0x03);
	  uart_putchar (DEBUG_PORT,0xFC);
    for(i=0;i<4;i++)
    {
        uart_putchar (DEBUG_PORT,(char)OutData[i]);			
    }
		uart_putchar (DEBUG_PORT,0xFC);
	  uart_putchar (uart0,0x03);
		#endif
		
		#ifdef  USE_NRF
		send_buff[0] = 8;
		
		send_buff[1] = 0x03;
		send_buff[2] = 0xFC;
		for(i=3;i<7;i++)
    {
        send_buff[i+1] = OutData[i];		
    }
		send_buff[7] = 0xFC;
		send_buff[8] = 0x03;
		SEND_BUF(send_buff);
		#endif
}

void Send_Wave_int(void)
{
	u8 send_buff[32]={0};
	int i=0;
	
	  #ifdef USE_UART
		uart_putchar (DEBUG_PORT,0x03);
	  uart_putchar (DEBUG_PORT,0xFC);
    for(i=0;i<4;i++)
    {
        uart_putint(DEBUG_PORT,(int)OutData[i]);			
    }
		uart_putchar (DEBUG_PORT,0xFC);
	  uart_putchar (DEBUG_PORT,0x03);
		#endif
		
		#ifdef  USE_NRF
		send_buff[0] = 8;
		
		send_buff[1] = 0x03;
		send_buff[2] = 0xFC;
		for(i=3;i<11;i+=2)
    {
        send_buff[i+2] = (int)OutData[i]&0xff;
				send_buff[i+1] = ((int)OutData[i]>>8)&0xff;
    }
		send_buff[7] = 0xFC;
		send_buff[8] = 0x03;
		SEND_BUF(send_buff);
		#endif
}

void vcan_sendware(uint8 *wareaddr, uint32 waresize)
{
#define CMD_WARE     3
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //yy_摄像头串口调试 使用的命令
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //yy_摄像头串口调试 使用的命令

    uart_putbuff(DEBUG_PORT, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(DEBUG_PORT, wareaddr, waresize); //再发送图像

    uart_putbuff(DEBUG_PORT, cmdr, sizeof(cmdr));    //先发送命令
}


