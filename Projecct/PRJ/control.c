#include "control.h"
#include "math.h"

/*������ѹ*/

#define PWM_R_END 60//95
#define PWM_L_END 20//70

extern int16 OutData[4];
extern u8 P_A,D_A,P_S,I_S,P_T,D_T;
extern float PWM_ANGLE,PWM_TURN;
extern float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;      //�����ԭʼ����
extern uint16 adc1,adc2;

double cnt = 0;
float PWM_L,PWM_R,PWM;
u8 P_A=24,D_A=11,P_S=17,I_S=28,P_T=20,D_T=8;

//PWM���
void duty_pwm()
{
	
	
	PWM = cnt;
	PWM=PWM_L;
	PWM=PWM_R;
  /*****************����ռ�ձ�*********************************************************/

}

//�жϷ���ص�����
void duty_5ms()
{
	duty_pwm();
	cnt=cnt+1*1;//ʹ��ʱ�޸Ĵ˴��Ĳ���1��ʹ��PWM�Բ�ͬ�����ʱ仯�����Բ���һ�´��غͿ�ת����״̬������������ʲô����
	//���ʹ�õĿ϶��Ǵ���ʱ������
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

