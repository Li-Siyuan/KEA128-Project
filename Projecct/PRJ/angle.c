#include "angle.h"


float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;                         //处理后原始数据
float PWM_ANGLE;

extern u8 P_A,D_A;
extern int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;                 //原始数据
extern int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;

#ifdef QingHua
extern float g_fCarAngle;                 //融合滤波输出参数
#endif
#ifdef Kalman
extern float angle, angle_dot;          //卡尔曼滤波输出参数
#endif
#ifdef UPDATE
extern float AngleX;                    //新滤波方法输出参数
#endif

//int16 Angle_Need = 3;

//得到角度值
void angle_get()
{

	//原始数据
   Get_AccData();
   Get_Gyro();
        
	//数据处理
	 Gyro = (gyro_offset_z + mpu_gyro_z) * Gyro_ratio;                           //陀螺仪采集到的角速度归一化
	 Gyro_Turn = -(gyro_offset_x - mpu_gyro_x) * Gyro_ratio;                     //陀螺仪采集到的角速度归一化
   Angle = (mpu_acc_y+angle_offset) / Angle_ratio ;                          //将加速度计采集到的角度归一化，乘上0.375是为了归一化到0~90°

   if(Angle > 90)Angle = 90;                                              //防止加速度角度溢出
   if(Angle < -90)Angle = -90;
  
   Gyro_ago = Gyro_Last;
		
	//滤波
	#ifdef QingHua
	 QingHua_AngleCalaulate(Angle,Gyro);   //清华融合滤波
	 Angle_Last = g_fCarAngle;
	 Gyro_Last = Gyro;
	#endif
	
	#ifdef Kalman
	 Kalman_Filter(Angle,Gyro) ;      //卡尔曼滤波
	 Angle_Last = angle;
	 Gyro_Last = angle_dot;
	#endif
	
	#ifdef UPDATE
	 IMUupdate(angleX, angleY, angleZ, gyroX, gyroY, gyroZ); //新滤波方案
	 Angle_Last = AngleX;
	 Gyro_Last = Gyro;
	#endif
}



//直立环
void duty_angle()
{
	angle_get();
	
	/**************************************************************普通PD************************************************************************/
#ifdef USE_ANGLE_PD 
	PWM_ANGLE = P_ANGLE*Angle_Last + D_ANGLE*Gyro_Last/10;	
#endif
	
	/**************************************************************滑动PD************************************************************************/
#ifdef USE_ANGLE_MOVE_PD  
  
	//一、若角度偏差过大，使用较强控制
	if((Angle_Last>Angle_max)||(Angle_Last<-Angle_max))
		PWM_ANGLE = P_ANGLE_MAX*Angle_Last + D_ANGLE_MAX*Gyro_Last;
	
	//二、若角度偏差过小，使用较弱控制	
	else if((Angle_Last<Angle_min)||(Angle_Last>-Angle_min))
		PWM_ANGLE = P_ANGLE_MIN*Angle_Last + D_ANGLE_MIN*Gyro_Last;
	
	//三、否则、正常控制
	else
		PWM_ANGLE = P_ANGLE*Angle_Last + D_ANGLE*Gyro_Last;
	
#endif
	
	/**************************************************************专家PID***********************************************************************/
#ifdef USE_ANGLE_EXPERT_PD 	
	
	ANGLE_I += Angle_Last;  //积分项
	if(ANGLE_I>20)
		ANGLE_I = 20;
	else if(ANGLE_I<-20)
		ANGLE_I = -20;
	PWM_ANGLE_AGO = PWM_ANGLE;  
		
	//一、若角度偏差过大，直接开环最大输出
	if((Angle_Last>Angle_max)||(Angle_Last<-Angle_max))
		PWM_ANGLE = PWM_Angle_max;
	 
	//二、若角速度与角度同向（偏差向增大方向发展），或误差为一常值
	else if((Angle_Last*Gyro_Last>0)||(Gyro_Last==0))
	{
		if((Angle_Last>Angle_max2)||(Angle_Last<-Angle_max2))      //同时角度误差较大，则正常PD控制乘以增益放大系数
			PWM_ANGLE = K_max*(P_ANGLE*Angle_Last + D_ANGLE*Gyro_Last);
		else                                                       //同时角度较小，则正常PD
			PWM_ANGLE = P_ANGLE*Angle_Last + D_ANGLE*Gyro_Last;
	}
	
	//三、若角速度与角度同向（偏差向减小方向发展）
	else if(Angle_Last*Gyro_Last<0)
	{
		if((Gyro_Last*Gyro_ago>0)||Angle_Last==0)  //即将或已达到稳态，则保持输出值不变
			PWM_ANGLE = PWM_ANGLE_AGO;
		else if(Gyro_Last*Gyro_ago<0)               //处于临界状态
		{
			if((Angle_Last>Angle_max2)||(Angle_Last<-Angle_max2))    //同时偏差较大，乘以增益放大系数
				PWM_ANGLE = K_max*P_ANGLE*Angle_Last;
			else                                                     //同时偏差较小，乘以抑制系数
				PWM_ANGLE = K_min*P_ANGLE*Angle_Last;
		}		
	}
	
	//四、控制同时，若角度偏差非常小，加入积分控制
	if(Angle_Last<Angle_min)
		PWM_ANGLE += I_ANGLE*ANGLE_I;	
	
#endif
	
	
}



