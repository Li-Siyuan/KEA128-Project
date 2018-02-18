
#include "init.h"
#include "control.h"



void ALL_INIT()
{
	
	//LED初始化（H2）
	//gpio_init(LED,GPO,1);
	
#ifdef USE_UART
	//UART1初始化
	uart_init(DEBUG_PORT,DEBUG_BAUD);
#endif
  

        
#ifdef USE_OLED
	//模拟IIC与OLED初始化（H3，H4）
	OLED_Conf();
	OLED_Clear();//清屏
#endif
	
	//按键初始化(E4\E5\E6/D2/D3)
	gpio_init(KEY1,GPI,1);
	gpio_init(KEY2,GPI,1);
	gpio_init(KEY3,GPI,1);
 // gpio_init(KEY4,GPI,1);
//  gpio_init(KEY5,GPI,1);
	
	//模拟IIC与MPU6050初始化(G0、G1)
  IIC_init();
  InitMPU6050();
        
	
	
#ifdef USE_NRF
	//模拟SPI与NRF初始化（D0\D1\H5\H6\H7\E3）
	SPI_GPIO_Init();
	while(NRF24L01_Check())//自检
	{
#ifdef USE_OLED
		OLED_ShowString(0,0,(uint8*)"NRF ERROR!",16);
#endif
	}
	NRF24L01_RT_Init();
#endif
	
	//Flash初始化
	FLASH_Init();
	
	//电机PWM初始化（H0 H1 G4 G5）    //引脚切换可以在KEA128_port_cfg.h设置
  ftm_pwm_init(ftm2,ftm_ch0,14000,0);  //输出50HZ PWM 占空比为百分之（500/FTM0_PRECISON*100）
	ftm_pwm_init(ftm2,ftm_ch1,14000,0); 
	ftm_pwm_init(ftm2,ftm_ch2,14000,0); 
	ftm_pwm_init(ftm2,ftm_ch3,14000,0); 
  gpio_init(B0,GPO,1);   //两使能管脚
  gpio_init(B1,GPO,1);
		
#ifdef SPEED_CONTROL	
	//编码器输入捕捉初始化(E0\E7\G2\G3)
	ftm_count_init(ftm0);  //对E0引脚输入的脉冲进行计数
	ftm_count_init(ftm1);  //E7
	gpio_init(DIR_L,GPI,1);
	gpio_init(DIR_R,GPI,1);
#endif
	
#ifdef TURN_CONTROL
	//编码器输入捕捉初始化(E0\E7\G2\G3)
	ftm_count_init(ftm0);  //对E0引脚输入的脉冲进行计数
	ftm_count_init(ftm1);  //E7
	gpio_init(DIR_L,GPI,1);
	gpio_init(DIR_R,GPI,1);
	//ADC采集初始化（A0、A1、A6）
	adc_init(ADC0_SE0);     //初始化ADC ADC引脚通过ADC通道号查看KEA128_adc.h文件得知
	adc_init(ADC0_SE1);
	adc_init(ADC0_SE2);
#endif
	
	//PIT定时器初始化
	pit_init_ms(pit0,5);		//5ms
	 
}

