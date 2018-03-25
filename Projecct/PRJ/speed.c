#include "speed.h"

float speedL , speedR ,speed_error,speed_need=1000,speed_now; 
double speed_I=0;
float I_MOVE=1;
float PWM_SPEED,PWM_SPEED_OUT,PWM_SPEED_AGO;
//限幅速度环输出最大为角度环的MOVE倍
float MOVE=6;
extern int16 Angle_Need;
extern uint8 cnt;
extern u8 P_S,I_S;
extern float PWM_ANGLE;

int16 SPEED_MAX = 0;

//速度环:一个角度对应一个速度，速度环通过输出反向加速度产生角度，从而产生对应速度 （PWM（电机的电压有效值）控制的是加速度，而不是速度）
/**************e车参数****************/
/*大齿轮直径 42.80mm 齿轮数 104齿
 *编码器齿轮 直径 16mm 齿轮数 40齿
*轮胎直径63.5mm pi取3.1415 周长为0.19949m
 *编码器参数 500线欧姆龙 取样时间 100ms
 */
/***********************************/
void duty_speed()                                  //测试下加速阶段时间，即时间常数T1
{
	speedL = ftm_count_get(ftm0);  //得到左轮脉冲数
  speedR = ftm_count_get(ftm1);  //得到右轮脉冲数
  ftm_count_clean(ftm0);         //清脉冲数
  ftm_count_clean(ftm1);
          
	if(gpio_get(DIR_L)==1)        //判断方向
		speedL = -speedL;
	if(gpio_get(DIR_R)==0)
		speedR = -speedR;
	
	speed_now = (speedL + speedR)/2;  //得到当前脉冲数
	speed_error = (speed_need - speed_now);    //得到速度偏差
        
//设置变速积分系数

	
	speed_I += speed_error*I_MOVE*0.01;          //变速积分          
	
//	if(speed_I>550)                          //积分限幅
//		speed_I = 550;
//	else if(speed_I<-550)
//		speed_I = -550;
	
	
	//20分三角形输出，修改原来的梯形曲线为斜坡折线
	PWM_SPEED = P_SPEED*speed_error+ I_SPEED*speed_I;//（输出加速度（期望角度））	，先调I，后调P
	PWM_SPEED_OUT = PWM_SPEED;
}





