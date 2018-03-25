#include "control.h"
#include "math.h"

/*死区电压*/

#define PWM_R_END 60//95
#define PWM_L_END 20//70

extern int16 OutData[4];
extern u8 P_A,D_A,P_S,I_S,P_T,D_T;
extern float PWM_ANGLE,PWM_TURN;
extern float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;      //处理后原始数据
extern uint16 adc1,adc2;

double cnt = 0;
float PWM_L,PWM_R,PWM;
u8 P_A=24,D_A=11,P_S=17,I_S=28,P_T=20,D_T=8;

//PWM输出
void duty_pwm()
{
	
	
	PWM = cnt;
	PWM=PWM_L;
	PWM=PWM_R;
  /*****************设置占空比*********************************************************/

}

//中断服务回调函数
void duty_5ms()
{
	duty_pwm();
	cnt=cnt+1*1;//使用时修改此处的参数1，使得PWM以不同的速率变化。可以测试一下带载和空转两种状态！看看曲线有什么区别。
	//最后使用的肯定是带载时的曲线
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

