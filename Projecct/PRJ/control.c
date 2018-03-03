#include "control.h"
#include "math.h"

/*������ѹ*/

#define PWM_R_END 60//95
#define PWM_L_END 20//70

char Debug_Mode = 2;

extern int16 OutData[4];
extern u8 P_A,D_A,P_S,I_S,P_T,D_T;
extern float PWM_ANGLE,PWM_SPEED_OUT,PWM_TURN;
extern float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;                         //�����ԭʼ����


uint8 cnt = 0;
float PWM_L,PWM_R,PWM;
u8 P_A=28,D_A=11,P_S=18,I_S=10,P_T=11,D_T=18;

//PWM���
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
		PWM = PWM_ANGLE + PWM_SPEED_OUT;//�ٶȸ��������ṩ���
		PWM_L = PWM;
		PWM_R = PWM;
	}
	
	else if(Debug_Mode==2)
	{
	PWM = PWM_ANGLE + PWM_SPEED_OUT + 250;//�ٶȸ��������ṩ���
	PWM_L = PWM - PWM_TURN;
	PWM_R = PWM + PWM_TURN;
	}
	
	/***********************������ٶ�������985��PWM��******************************/
    if(PWM_L > (1000-PWM_L_END))  
			PWM_L = (1000-PWM_L_END);
    else if(PWM_L < -(1000-PWM_L_END)) 
			PWM_L = -(1000-PWM_L_END);
		
    if(PWM_R > (1000-PWM_R_END))  
			PWM_R = (1000-PWM_R_END);
    else if(PWM_R < -(1000-PWM_R_END)) 
			PWM_R = -(1000-PWM_R_END);
		
  /*****************����ռ�ձ�*********************************************************/
		if(PWM_R<0)
		{
			ftm_pwm_duty(ftm2,ftm_ch1,0);     //����ռ�ձ�Ϊ�ٷ�֮��100/FTM0_PRECISON*100��
			ftm_pwm_duty(ftm2,ftm_ch0,(u32)(-PWM_R+PWM_R_END)); //62
		}
		else
		{
			ftm_pwm_duty(ftm2,ftm_ch0,0);     //����ռ�ձ�Ϊ�ٷ�֮��100/FTM0_PRECISON*100��
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



//�жϷ���ص�����
void duty_5ms()
{
	
	cnt++;
	//5msһ��ֱ����
	duty_angle();
		
	
	if(Debug_Mode==1)
	{
		//100msһ���ٶȻ�
		if(cnt==20)
		{
			duty_speed();
			cnt = 0;
		}
	}
	
	
	else if(Debug_Mode==2)
	{
		//ת��
		duty_turn();
		
		//100msһ���ٶȻ�
		if(cnt==20)
		{
			duty_speed();
			cnt = 0;
		}
	}
	
	//������PWM
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
  FLASH_EraseSector(sector);                                      //�������һ������
  FLASH_WriteSector(sector,(const uint8 *)write,4,0);             //д���ĸ��ֽڵ����һ������ ƫ��0�ֽ�
  FLASH_WriteSector(sector,(const uint8 *)write+4,4,4);              //д���ĸ��ֽڵ����һ������ ƫ��4�ֽ�
}

void READ_8_DATA(void)
{
  
    P_A = (int16)flash_read(sector,0,uint8);                           //��ȡ����
    D_A = (int16)flash_read(sector,1,uint8);
    P_S = (int16)flash_read(sector,2,uint8);
    I_S = (int16)flash_read(sector,3,uint8);
		P_T = (int16)flash_read(sector,4,uint8);                           //��ȡ����
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

