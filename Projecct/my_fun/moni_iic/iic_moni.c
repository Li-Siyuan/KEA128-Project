/***************************************************************************************
									����
����Ŀ�����������ѧϰʹ�ã�����������ֲ�޸ģ������뱣����������Ϣ����ֲ�����г�����������
������BUG����Զ�ǿز����κ����Ρ��������ã�

����汾�ţ�	2.0
���ڣ�			2017-1-1
���ߣ�			�������ꡢС���
��Ȩ���У�		��Զ�ǿ�N0.1ʵ����
****************************************************************************************/
/************************************************************************
ģ��I2C
ֻ�������Ҫ�޸������޸����е�I2C_PORT��I2C_SCL��I2C_SDA��RCC_I2C_PORT
��Ӧ�ĹܽźͶ˿ڼ���

ע�⣺
1.��ʼ��I2C_MoniConfig()����ǰ�����ȳ�ʼ��SysTick_init()���Ա����ʹ���ӳٺ���
************************************************************************/
#include "iic_moni.h"
#include "KEA128_systick.h"
#include "KEA128_gpio.h"
#include "headfile.h"
/************************************************************************
������ֲ�޸���
ֻ��Ҫ�޸�ģ��I2Cʹ�õ���ʹ�õĵĶ˿ں�SCL/SDA���ż���
************************************************************************/
//#define I2C_PORT 				GPIOA
#define I2C_SCL 				G0
#define I2C_SDA 				G1
//#define RCC_I2C_PORT 			RCC_AHBPeriph_GPIOA
/***********************************************************************/
//���Ʒ���
#define IIC_SDA_IN        gpio_ddr(I2C_SDA, GPI);            //�������ŷ���
#define IIC_SDA_OUT       gpio_ddr(I2C_SDA, GPO);   //���ö˿ڷ���Ϊ���
#define IIC_SCL_OUT        gpio_ddr(I2C_SCL, GPO);
#define IIC_SCL_IN         gpio_ddr(I2C_SCL, GPI);
//���ƹܽ�����ߵ͵�ƽ
#define I2C_SCL_LOW 			gpio_set(I2C_SCL,0) //PD_ODR_ODR3 = 0   //����
#define I2C_SCL_HIGH 			gpio_set(I2C_SCL,1)//PD_ODR_ODR3 = 1   //����
#define I2C_SDA_LOW 			gpio_set(I2C_SDA,0)// PC_ODR_ODR7 = 0   //����
#define I2C_SDA_HIGH 			gpio_set(I2C_SDA,1)//PC_ODR_ODR7 = 1   //����

//��ȡ�ܽŵĵ�ƽ״̬
#define I2C_SDA_READ 			gpio_get(I2C_SDA) //PC_IDR_IDR7       //DQֵ
//#define delay_us                    delay_iic(1);
//==========================================================================
extern volatile struct GPIO_Type *GPIOx[3];

void delay_iic(int i)
{
  i=10;
  while(i--);
}

/****************************������*******************************************/
//I2C��ʼ������
void I2C_MoniConfig(void){
	
	gpio_init(I2C_SCL ,GPO,1);
  gpio_init(I2C_SDA ,GPO,1);  
        
        
	I2C_SCL_LOW;
	delay_iic(1);
	I2C_SDA_HIGH;
	delay_iic(1);
}


//�ڲ��������ж������Ƿ��ڿ���״̬�������ǣ���ѭ�����ṩSCLʱ��������ֱ���ӻ��ͷ�SDA��
void _I2C_IsBusy(void)
{
      //  I2C_SDA_HIGH;
	//��ȡSDA���ϵĵ�ƽ״̬,��Ϊ�͵�ƽ��˵�����߱��ӻ����ƣ���Ϊ�ߵ�ƽ��˵�����߿��У�����׼�����Ϳ�ʼ����
        IIC_SDA_IN;
	while(I2C_SDA_READ==0){
		IIC_SDA_OUT;
           //    I2C_SDA_HIGH;
		I2C_SCL_LOW;
		delay_iic(3);
		I2C_SCL_HIGH;
		delay_iic(3);
                IIC_SDA_IN;
	}
        IIC_SDA_OUT;
}	


//������ʼ�ź�
void I2C_Start(void){
	//�ж��������Ƿ��ڿ���״̬
	_I2C_IsBusy();
	
	//����SCL�еĵ�ƽΪ�ͣ���ֹ��ΪSCL���ڸߵ�ƽ��ʹ���潫SDA����ʱ�����ܻᴥ��һ��stop�ź�
	I2C_SCL_LOW;
	delay_iic(1);
	
	I2C_SDA_HIGH;
	delay_iic(1);
	I2C_SCL_HIGH;
	delay_iic(1);
	I2C_SDA_LOW;
	delay_iic(1);
	
	I2C_SCL_LOW;
	delay_iic(1);
}

//���������ź�
void I2C_Stop(void){
	I2C_SCL_LOW;
	delay_iic(1);
	
	I2C_SDA_LOW;
	delay_iic(1);
	I2C_SCL_HIGH;
	delay_iic(1);
	I2C_SDA_HIGH;
	delay_iic(1);
	
}

//����Ӧ���źŻ��߷�Ӧ���ź�,ackState->ΪENABLEʱ�������Ӧ���ź�
void I2C_SetAck(FunctionalState ackState){
	I2C_SCL_LOW;
	delay_iic(1);
	
	if(ackState == ENABLE){
		I2C_SDA_LOW;
		delay_iic(1);
	}else{
		I2C_SDA_HIGH;
		delay_iic(1);
	}
	
	I2C_SCL_HIGH;
	delay_iic(1);
	
	I2C_SCL_LOW;
	delay_iic(1);
	
	I2C_SDA_HIGH;
	delay_iic(1);
}

//���Ӧ���źţ�ENABLE�����߷�Ӧ���źţ�DISABLE��
FunctionalState I2C_GetAck(void){
	FunctionalState ask;
	
	I2C_SCL_HIGH;
	delay_iic(1);
	//��ȡSDA���ϵĵ�ƽ״̬
        IIC_SDA_IN;
	if(I2C_SDA_READ){
          IIC_SDA_OUT;
		ask = DISABLE;
	}else{
          IIC_SDA_OUT;
		ask = ENABLE;
	}
	
	I2C_SCL_LOW;
	delay_iic(1);
	
	return ask;
}

//д�����ݸ��ӻ���������Ӧ����߷�Ӧ���ź�
FunctionalState MoniI2C_WriteByte(u8 data){
	u8 i;
	
	//������λ�Ĵ����Ĺ��ܣ�������ͨ��I/O�ڷ��ͳ�ȥ
	for(i=0;i<8;i++){
		I2C_SCL_LOW;
		delay_iic(1);
		
		if(data & 0x80){
			I2C_SDA_HIGH;
		}else{
			I2C_SDA_LOW;
		}
		data <<=1;
		delay_iic(1);
		
		I2C_SCL_HIGH;
		delay_iic(2);
	}
	
	I2C_SCL_LOW;
	delay_iic(1);
	I2C_SDA_HIGH;
	delay_iic(1);
	
	return I2C_GetAck();
}

//��ȡ�ӻ����͵�����,��������Ӧ���Ƿ�Ӧ��
u8 MoniI2C_ReadByte(FunctionalState ackState){
	u8 i;
	u8 data = 0x00;
	
	//������λ�Ĵ����Ĺ��ܣ������ݴ�I/O���ж�ȡ����
	for(i=0;i<8;i++){
		I2C_SCL_HIGH;
		delay_iic(1);
		
		//����λ����ֵ
		data <<=1;
                IIC_SDA_IN;
		if(I2C_SDA_READ){
			data |= 0x01;
		}
		IIC_SDA_OUT;
		I2C_SCL_LOW;
		delay_iic(2);
	}
	
	//������Ӧ�������Ӧ����MPU6050
	I2C_SetAck(ackState);
	
	return data;
}


/********************************************************
���ӻ��еļĴ���д��һ���ֽڵ�����
*addr:�豸��ַ
*reg���Ĵ�����ַ
*data��Ҫд�������
*����ֵ��0Ϊ����д�룬1Ϊд���쳣
**********************************************************/
u8 MoniI2c_WriteByteToSlave(u8 addr,u8 reg,u8 data)
{
	FunctionalState state;							//������¼ACK����NACK
	                                                
	I2C_Start();                                    //������ʼ�ź�
	state = MoniI2C_WriteByte(addr<<1|0);           //���ӻ���ַ��д����д������
	if(state == ENABLE){                            //�жϴӻ��Ƿ���Ӧ�˵�ַ
		state = MoniI2C_WriteByte(reg);     		//��Ҫд�����ݵļĴ�����ַ
		if(state == ENABLE){                        
			MoniI2C_WriteByte(data);                //�����ݷ��͸��ӻ�
			
			//����д�룬����ֹͣ�źţ�������0
			I2C_Stop();
			return 0;
		}
	}
	
	//д���쳣������ֹͣ�źţ�������1
	I2C_Stop();
	return 1;
}                                                  


/************************************************************
���ӻ��еļĴ���д��������
*addr�������ĵ�ַ
*reg���׸�д�����ݵļĴ�����ַ
*len��Ҫд������ݵĸ���
*buf��Ҫд������������׵�ַ
*����ֵ���ɹ�д���򷵻�0��ʧ���򷵻�1
**************************************************************/
u8 MoniI2c_WriteSomeDataToSlave(u8 addr,u8 reg,u8 len,u8 *buf)
{
	FunctionalState state;								//������¼ACK����NACK
	u8 i;
	
	I2C_Start();                                    	//������ʼ�ź�
	state = MoniI2C_WriteByte(addr<<1|0);               //���ӻ���ַ��д����д������
	if(state == ENABLE){                            	//�жϴӻ��Ƿ���Ӧ�˵�ַ��
		state = MoniI2C_WriteByte(reg);      			//��Ҫд�����ݵ��׸��Ĵ�����ַ���͸��ӻ�
		if(state == ENABLE){
			for(i=0;i<len;i++){							//��ʼ�������������ݶ����͸��ӻ�
				state = MoniI2C_WriteByte(*(buf+i));    //�����ݷ��͸��ӻ�
				if(state == DISABLE){					
					I2C_Stop();                         //�ӻ�δӦ�𣬲���ֹͣ�źŽ������ݴ��䣬������1
					return 1;
				}
			}
			
			//��������ֹͣ�źţ�����0
			I2C_Stop();                                      	
			return 0;
		}                                          
	}                              
	
	//д���쳣�����������źţ�����1
	I2C_Stop();
	return 1;
}                                                  


/********************************************************
���ӻ��еļĴ�����ȡһ���ֽڵ�����
*addr:�豸��ַ
*reg���Ĵ�����ַ
*buf:��ȡ�������ݴ洢���ڴ���
*����ֵ��0Ϊ������ȡ��1Ϊ��ȡ�쳣
**********************************************************/
u8 MoniI2c_ReadFromSlave(u8 addr,u8 reg,u8 *buf)
{
	FunctionalState state;
	
	I2C_Start();
	state = MoniI2C_WriteByte(addr<<1|0);			//���ʹӻ���ַ��д����
	if(state == ENABLE){
		state = MoniI2C_WriteByte(reg);				//����Ҫ��ȡ���ݵļĴ����ĵ�ַ
		if(state == ENABLE){
			I2C_Start();							//����Sr�źţ�
			state = MoniI2C_WriteByte(addr<<1|1);	//���ʹӻ���ַ�Ͷ�����
			if(state == ENABLE){
				*buf = MoniI2C_ReadByte(DISABLE);	//��ȡ��һ�����ݺ���ӻ�����һ��NACK�źţ������ӻ���������
				
				//�������ݶ�ȡ������ֹͣ�źţ�������0
				I2C_Stop();
				return 0;
			}
		}
	}
	
	//���ݶ�ȡ�쳣������stop�źţ�������1
	I2C_Stop();
	return 1;		//���ݶ�ȡʧ�ܣ�����0
}


/************************************************************************
�Ӵӻ��ж�ȡ�������ĸ�8λ�͵�8λ������

*addr�������ĵ�ַ
*reg��Ҫ��ȡ���ݵ��׸����ݵļĴ�����ַ
*len����ȡ�����ݸ���
*buf����ȡ����8λ���ݴ�����ĵ�ַ����ͨ��ָ��ķ�ʽ����ȡ�������ݷ��ظ������ߡ�
*����ֵ���ɹ���ȡ�򷵻�0��ʧ���򷵻�1
*************************************************************************/
u8 MoniI2c_ReadSomeDataFromSlave(u8 addr,u8 reg,u8 len,u8 *buf)
{
	FunctionalState state;
	
	I2C_Start();
	state = MoniI2C_WriteByte(addr<<1|0);							//д��ӻ���ַ��д����
	if(state == ENABLE){
		state = MoniI2C_WriteByte(reg);								//д��Ҫ��ȡ���ݵ��׸��Ĵ����ĵ�ַ
		if(state == ENABLE){
			I2C_Start();
			state = MoniI2C_WriteByte(addr<<1|1);					//���·�����ʼ�źţ�תΪ�����򣬿�ʼ��ȡ����
			if(state == ENABLE){
				while(len){
					if(len==1){										
						*buf = MoniI2C_ReadByte(DISABLE);			//�����һ�����ݽ���ʱ�����ӻ�����һ��NACK�ź�
					}else{
						*buf = MoniI2C_ReadByte(ENABLE);			//��һ�����ݽ���ʱ�����ӻ�����һ��ACK�ź�
					}
					len--;
					buf++;
				}
				
				//����������ȡ������ֹͣ�źţ������˴����ݵĴ��䣬������0
				I2C_Stop();
				return 0;
			}
		}
	}
	
	//���ݶ�ȡ�쳣������stop�źţ������˴����ݶ�ȡ
	I2C_Stop();
	return 1;		//���ݶ�ȡʧ�ܣ�����1
}

