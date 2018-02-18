
#include "init.h"
#include "control.h"



void ALL_INIT()
{
	
	//LED��ʼ����H2��
	//gpio_init(LED,GPO,1);
	
#ifdef USE_UART
	//UART1��ʼ��
	uart_init(DEBUG_PORT,DEBUG_BAUD);
#endif
  

        
#ifdef USE_OLED
	//ģ��IIC��OLED��ʼ����H3��H4��
	OLED_Conf();
	OLED_Clear();//����
#endif
	
	//������ʼ��(E4\E5\E6/D2/D3)
	gpio_init(KEY1,GPI,1);
	gpio_init(KEY2,GPI,1);
	gpio_init(KEY3,GPI,1);
 // gpio_init(KEY4,GPI,1);
//  gpio_init(KEY5,GPI,1);
	
	//ģ��IIC��MPU6050��ʼ��(G0��G1)
  IIC_init();
  InitMPU6050();
        
	
	
#ifdef USE_NRF
	//ģ��SPI��NRF��ʼ����D0\D1\H5\H6\H7\E3��
	SPI_GPIO_Init();
	while(NRF24L01_Check())//�Լ�
	{
#ifdef USE_OLED
		OLED_ShowString(0,0,(uint8*)"NRF ERROR!",16);
#endif
	}
	NRF24L01_RT_Init();
#endif
	
	//Flash��ʼ��
	FLASH_Init();
	
	//���PWM��ʼ����H0 H1 G4 G5��    //�����л�������KEA128_port_cfg.h����
  ftm_pwm_init(ftm2,ftm_ch0,14000,0);  //���50HZ PWM ռ�ձ�Ϊ�ٷ�֮��500/FTM0_PRECISON*100��
	ftm_pwm_init(ftm2,ftm_ch1,14000,0); 
	ftm_pwm_init(ftm2,ftm_ch2,14000,0); 
	ftm_pwm_init(ftm2,ftm_ch3,14000,0); 
  gpio_init(B0,GPO,1);   //��ʹ�ܹܽ�
  gpio_init(B1,GPO,1);
		
#ifdef SPEED_CONTROL	
	//���������벶׽��ʼ��(E0\E7\G2\G3)
	ftm_count_init(ftm0);  //��E0���������������м���
	ftm_count_init(ftm1);  //E7
	gpio_init(DIR_L,GPI,1);
	gpio_init(DIR_R,GPI,1);
#endif
	
#ifdef TURN_CONTROL
	//���������벶׽��ʼ��(E0\E7\G2\G3)
	ftm_count_init(ftm0);  //��E0���������������м���
	ftm_count_init(ftm1);  //E7
	gpio_init(DIR_L,GPI,1);
	gpio_init(DIR_R,GPI,1);
	//ADC�ɼ���ʼ����A0��A1��A6��
	adc_init(ADC0_SE0);     //��ʼ��ADC ADC����ͨ��ADCͨ���Ų鿴KEA128_adc.h�ļ���֪
	adc_init(ADC0_SE1);
	adc_init(ADC0_SE2);
#endif
	
	//PIT��ʱ����ʼ��
	pit_init_ms(pit0,5);		//5ms
	 
}

