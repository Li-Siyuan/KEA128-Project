#include "my_nrf.h"
#include "KEA128_gpio.h"
/*********     24L01���ͽ������ݿ�ȶ���	  ***********/
#define TX_ADR_WIDTH    5     //5�ֽڵ�ַ���
#define RX_ADR_WIDTH    5     //5�ֽڵ�ַ���
#define TX_PLOAD_WIDTH  32    //32�ֽ���Ч���ݿ��
#define RX_PLOAD_WIDTH  32    //32�ֽ���Ч���ݿ��

const uchar TX_ADDRESS[TX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //���͵�ַ
const uchar RX_ADDRESS[RX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //���͵�ַ

#define NRF_CSN_HIGH   gpio_set(D0,1)  //Ƭѡ�źţ��������  
#define NRF_CSN_LOW    gpio_set(D0,0) 
#define NRF_IRQ        gpio_get(E3)//PDin(3)  //IRQ������������,��������
#define NRF_SCK_HIGH   gpio_set(H7,1)  //���ͽ���ģʽѡ���������
#define NRF_SCK_LOW   gpio_set(H7,0)
#define NRF_CE_HIGH    gpio_set(D1,1)  //PC5 �������
#define NRF_CE_LOW     gpio_set(D1,0)
#define NRF_MOSI_HIGH  gpio_set(H6,1)//PBout(15)  //PC6 MOSI ����������� ���������ʳ�ʼ��PC_CR2�Ĵ�����
#define NRF_MOSI_LOW   gpio_set(H6,0)
#define NRF_MISO       gpio_get(H5);//PBin(14)  //PC7 MISO

//void delay_us(uchar num)
//{
//	uchar i,j; 
//	for(i=0;i>num;i++)
// 	for(j=100;j>0;j--);
//}
void delay_150us(void)
{
	uint i;
	for(i=0;i>600;i++);
}

void SPI_GPIO_Init()
{
	gpio_init(D0,GPO,1);
	gpio_init(E3,GPI,1);
	gpio_init(H7,GPO,1);
	gpio_init(D1,GPO,1);
	gpio_init(H6,GPO,1);
	gpio_init(H5,GPI,1);
}


uchar SPI_RW(uchar byte)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // ���8λ
	{
		if((uchar)(byte&0x80)==0x80)
		NRF_MOSI_HIGH; 			// MSB TO MOSI
		else 
			NRF_MOSI_LOW; 
		byte=(byte<<1);					// shift next bit to MSB
		NRF_SCK_HIGH;
		byte|=NRF_MISO;	        		// capture current MISO bit
		NRF_SCK_LOW;
	}
	return byte;
}
/*********************************************/
/* �������ܣ���24L01�ļĴ���дֵ��һ���ֽڣ� */
/* ��ڲ�����reg   Ҫд�ļĴ�����ַ          */
/*           value ���Ĵ���д��ֵ            */
/* ���ڲ�����status ״ֵ̬                   */
/*********************************************/
uchar NRF24L01_Write_Reg(uchar reg,uchar value)
{
	uchar status;

	NRF_CSN_LOW;                  //CSN=0;   
  	status = SPI_RW(reg);		//���ͼĴ�����ַ,����ȡ״ֵ̬
	SPI_RW(value);
	NRF_CSN_HIGH;                  //CSN=1;

	return status;
}
/*************************************************/
/* �������ܣ���24L01�ļĴ���ֵ ��һ���ֽڣ�      */
/* ��ڲ�����reg  Ҫ���ļĴ�����ַ               */
/* ���ڲ�����value �����Ĵ�����ֵ                */
/*************************************************/
uchar NRF24L01_Read_Reg(uchar reg)
{
 	uchar value;

	NRF_CSN_LOW;              //CSN=0;   
  	SPI_RW(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	value = SPI_RW(NOP);
	NRF_CSN_HIGH;             	//CSN=1;

	return value;
}
/*********************************************/
/* �������ܣ���24L01�ļĴ���ֵ������ֽڣ�   */
/* ��ڲ�����reg   �Ĵ�����ַ                */
/*           *pBuf �����Ĵ���ֵ�Ĵ������    */
/*           len   �����ֽڳ���              */
/* ���ڲ�����status ״ֵ̬                   */
/*********************************************/
uchar NRF24L01_Read_Buf(uchar reg,uchar *pBuf,uchar len)
{
	uchar status,u8_ctr;
	NRF_CSN_LOW;                   	//CSN=0       
	status=SPI_RW(reg);				//���ͼĴ�����ַ,����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=SPI_RW(0XFF);		//��������
	NRF_CSN_HIGH;                 		//CSN=1
	return status;        			//���ض�����״ֵ̬
}
/**********************************************/
/* �������ܣ���24L01�ļĴ���дֵ������ֽڣ�  */
/* ��ڲ�����reg  Ҫд�ļĴ�����ַ            */
/*           *pBuf ֵ�Ĵ������               */
/*           len   �����ֽڳ���               */
/**********************************************/
uchar NRF24L01_Write_Buf(uchar reg, uchar *pBuf, uchar len)
{
	uchar status,u8_ctr;
	NRF_CSN_LOW;
	status = SPI_RW(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	SPI_RW(*pBuf++); 				//д������
	NRF_CSN_HIGH;
  return status;          		//���ض�����״ֵ̬
}							  					   

/*********************************************/
/* �������ܣ�24L01��������                   */
/* ��ڲ�����rxbuf ������������              */
/* ����ֵ�� 0   �ɹ��յ�����                 */
/*          1   û���յ�����                 */
/*********************************************/
uchar NRF24L01_RxPacket(uchar *rxbuf)
{
	uchar state;
	state=NRF24L01_Read_Reg(STATUS);  			//��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(nRF_WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&RX_OK)								//���յ�����
	{
		NRF_CE_LOW;
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);					//���RX FIFO�Ĵ���
		NRF_CE_HIGH;
		delay_150us(); 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}
/**********************************************/
/* �������ܣ�����24L01Ϊ����ģʽ              */
/* ��ڲ�����txbuf  ������������              */
/* ����ֵ�� 0x10    �ﵽ����ط�����������ʧ��*/
/*          0x20    �ɹ��������              */
/*          0xff    ����ʧ��                  */
/**********************************************/
uchar NRF24L01_TxPacket(uchar *txbuf)
{
	uchar state;
   
	NRF_CE_LOW;												//CE���ͣ�ʹ��24L01����
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//д���ݵ�TX BUF  32���ֽ�
 	NRF_CE_HIGH;												//CE�øߣ�ʹ�ܷ���	   
	while(NRF_IRQ==1);										//�ȴ��������
	state=NRF24L01_Read_Reg(STATUS);  						//��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(nRF_WRITE_REG+STATUS,state); 			//���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_TX)										//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);					//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(state&TX_OK)											//�������
	{
		return TX_OK;
	}
	return 0xff;											//����ʧ��
}

/********************************************/
/* �������ܣ����24L01�Ƿ����              */
/* ����ֵ��  0  ����                        */
/*           1  ������                      */
/********************************************/ 	  
uchar NRF24L01_Check(void)
{
	uchar check_in_buf[5]={0x11,0x22,0x33,0x44,0x55};
	uchar check_out_buf[5]={0x00};

	NRF_SCK_LOW;
	NRF_CSN_HIGH;    
	NRF_CE_LOW;

	NRF24L01_Write_Buf(nRF_WRITE_REG+TX_ADDR, check_in_buf, 5);

	NRF24L01_Read_Buf(nRF_READ_REG+TX_ADDR, check_out_buf, 5);

	if((check_out_buf[0] == 0x11)&&\
	   (check_out_buf[1] == 0x22)&&\
	   (check_out_buf[2] == 0x33)&&\
	   (check_out_buf[3] == 0x44)&&\
	   (check_out_buf[4] == 0x55))return 0;
	else return 1;
}			


void NRF24L01_RT_Init(void)
{	
	NRF_CE_LOW;		  
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
		NRF24L01_Write_Reg(FLUSH_RX,0xff);									//���RX FIFO�Ĵ���    
  	NRF24L01_Write_Buf(nRF_WRITE_REG+TX_ADDR,(uchar*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(nRF_WRITE_REG+RX_ADDR_P0,(uchar*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(nRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_CH,0);        //����RFͨ��Ϊ2.400GHz  Ƶ��=2.4+0GHz
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_SETUP,0x0F);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG,0x0f);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF_CE_HIGH;									  //CE�øߣ�ʹ�ܷ���
}

void SEND_BUF(uchar *buf)
{
	NRF_CE_LOW;
	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG,0x0e);
	NRF_CE_HIGH;
	delay_us(15);
	NRF24L01_TxPacket(buf);
	NRF_CE_LOW;
	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG, 0x0f);
	NRF_CE_HIGH;	
}
