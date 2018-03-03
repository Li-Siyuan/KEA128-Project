#include "control.h"
//中断服务函数在isr.c
extern float adc1_max,adc1_min;
extern u8 max,min;
extern uint16 adc1,adc2,adc_value1[5];
extern float speed_now,speed_need,speed_error;//现在、需要、偏差速度
extern double speed_I;//速度的积分
extern int16 turn_error,OutData[4],sensor[3];
extern float PWM_TURN,Gyro_Turn,PWM_ANGLE,PWM_SPEED,PWM_SPEED_OUT;
extern float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;
int16 sensor1=0;
extern float abc;
extern char key_flag;
extern float PWM;
extern int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;                 //Ô­Ê¼Êý¾Ý
extern int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;
extern char Debug_Mode;
int main()
{
	
	get_clk();              //获取时钟频率 上电必须执行
	
	DisableInterrupts;        //关闭总中断
	
	NVIC_Conf();              //NVIC优先级配置
        
	ALL_INIT();              //所有硬件初始化	
	
	READ_8_DATA();           //从flash中读出PID参数
	
	DISPLAY();               //OLED显示当前参数
         
	enable_irq(PIT_CH0_IRQn);			//开启pit0中断
        
	EnableInterrupts;					//开启总中断
	
	abc = P_A;key_flag=1;OLED_ShowString(0,6,(uint8*)"PA:    ",16);OLED_MY_ShowNum(25,6,P_A,16);
	
	while(1)
	{	
		KEY_SCAN();	   //按键扫描
		
		//由于pit中断会打断与oled的iic通信，故显示较缓慢且闪烁
		OLED_MY_ShowNum(0,0,max,16);
		OLED_MY_ShowNum(25,0,adc1/10,16);
		
		OLED_MY_ShowNum(50,0,adc2/10,16);
		OLED_MY_ShowNum(75,0,min,16);

	//	speed_I = 3000;
		
		//发送波形到上位机		
#if 1	
		 OutData[0] = (int16)adc1;//speed_I;  //speed_now
      // OutData[0] = (int16)Angle;                   //红
		
	  	OutData[1] = (int16)turn_error;	//PWM_ANGLE
    			                  		         //黄
      OutData[2] = (int16)adc2;//PWM_SPEED_OUT;//PWM_SPEED;              		 //蓝
      OutData[3] = (int16)speed_now;//P_SPEED*speed_error*0.01;//turn_error;             			 //紫
      OutPut_Data();

#endif
#if 0
		  sensor_DATA(sensor[0],sensor[1],sensor1);
		  send_road((char)turn_error);
#endif


	}	
}

 


