#include "control.h"
#include "math.h"

/*死区电压*/

#define PWM_R_END 60//95
#define PWM_L_END 20//70

char Debug_Mode = 2;

extern int16 OutData[4];
extern u8 P_A,D_A,P_S,I_S,P_T,D_T;
extern float PWM_ANGLE,PWM_SPEED_OUT,PWM_TURN;
extern float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;                         //处理后原始数据


uint8 cnt = 0;
float PWM_L,PWM_R,PWM;
u8 P_A=24,D_A=11,P_S=18,I_S=10,P_T=11,D_T=18;

//PWM输出
void duty_pwm()
{
	if(Debug_Mode==0)
	{
		PWM = PWM_ANGLE;
    PWM_L = PWM;
		PWM_R = PWM;
	}
	else if(Debug_Mode==1)
	{
		PWM = PWM_ANGLE + PWM_SPEED_OUT;//速度负反馈，提供倾角
		PWM_L = PWM;
		PWM_R = PWM;
	}
	
	else if(Debug_Mode==2)
	{
	PWM = PWM_ANGLE + PWM_SPEED_OUT + 250;//速度负反馈，提供倾角
	PWM_L = PWM - PWM_TURN;
	PWM_R = PWM + PWM_TURN;
	}
	
	/***********************将最大速度限制在985个PWM内******************************/
    if(PWM_L > (1000-PWM_L_END))  
			PWM_L = (1000-PWM_L_END);
    else if(PWM_L < -(1000-PWM_L_END)) 
			PWM_L = -(1000-PWM_L_END);
		
    if(PWM_R > (1000-PWM_R_END))  
			PWM_R = (1000-PWM_R_END);
    else if(PWM_R < -(1000-PWM_R_END)) 
			PWM_R = -(1000-PWM_R_END);
		
  /*****************设置占空比*********************************************************/
		if(PWM_R<0)
		{
			ftm_pwm_duty(ftm2,ftm_ch1,0);     //设置占空比为百分之（100/FTM0_PRECISON*100）
			ftm_pwm_duty(ftm2,ftm_ch0,(u32)(-PWM_R+PWM_R_END)); //62
		}
		else
		{
			ftm_pwm_duty(ftm2,ftm_ch0,0);     //设置占空比为百分之（100/FTM0_PRECISON*100）
			ftm_pwm_duty(ftm2,ftm_ch1,(u32)(PWM_R+PWM_R_END)); 
		}
		if(PWM_L<0)
		{
			ftm_pwm_duty(ftm2,ftm_ch3,0);
			ftm_pwm_duty(ftm2,ftm_ch2,(u32)(-PWM_L+PWM_L_END));//62
		}
		else
		{
			ftm_pwm_duty(ftm2,ftm_ch2,0);
			ftm_pwm_duty(ftm2,ftm_ch3,(u32)(PWM_L+PWM_L_END));
		}
		
		
}



//中断服务回调函数
void duty_5ms()
{
	
	cnt++;
	//5ms一次直立环
	duty_angle();
		
	
	if(Debug_Mode==1)
	{
		//100ms一次速度环
		if(cnt==20)
		{
			duty_speed();
			cnt = 0;
		}
	}
	
	
	else if(Debug_Mode==2)
	{
		//转向环
		duty_turn();
		
		//100ms一次速度环
		if(cnt==20)
		{
			duty_speed();
			cnt = 0;
		}
	}
	
	//电机输出PWM
	duty_pwm();
	
}


void WRITE_8_DATA(void)
{
  uint8 write[8] = {0};
  
  write[0] = P_A;
  write[1] = D_A;
  write[2] = P_S;
  write[3] = I_S;
	write[4] = P_T;
  write[5] = D_T;
  write[6] = 0;
  write[7] = 0;
  FLASH_EraseSector(sector);                                      //擦除最后一个扇区
  FLASH_WriteSector(sector,(const uint8 *)write,4,0);             //写入四个字节到最后一个扇区 偏移0字节
  FLASH_WriteSector(sector,(const uint8 *)write+4,4,4);              //写入四个字节到最后一个扇区 偏移4字节
}

void READ_8_DATA(void)
{
  
    P_A = (int16)flash_read(sector,0,uint8);                           //读取数据
    D_A = (int16)flash_read(sector,1,uint8);
    P_S = (int16)flash_read(sector,2,uint8);
    I_S = (int16)flash_read(sector,3,uint8);
		P_T = (int16)flash_read(sector,4,uint8);                           //读取数据
    D_T = (int16)flash_read(sector,5,uint8);
  /*  read[6] = (int16)flash_read(sector,6,uint8);
    read[7] = (int16)flash_read(sector,7,uint8);*/
    
}

void DISPLAY(void)
{
  DisableInterrupts; 


  OLED_ShowString(0,4,(uint8*)"AP:     ",16);	
  OLED_MY_ShowNum(25,4,P_A,16);
	
  OLED_ShowString(0,2,(uint8*)"AD:     ",16);
  OLED_MY_ShowNum(25,2,D_A,16);
	
  OLED_ShowString(50,4,(uint8*)"SP:     ",16);
  OLED_MY_ShowNum(75,4,P_S,16);
	
  OLED_ShowString(50,2,(uint8*)"SI:     ",16);
  OLED_MY_ShowNum(75,2,I_S,16);

  EnableInterrupts;
}

