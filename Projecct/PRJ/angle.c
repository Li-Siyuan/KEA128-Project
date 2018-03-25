#include "angle.h"


float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;                         //处理后原始数据
float PWM_ANGLE;

extern u8 P_A,D_A;
extern int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;                 //原始数据
extern int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;


extern float PWM_SPEED_OUT;//串级速度环输出


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



//直立环普通PD+串级
void duty_angle()
{
	angle_get();
	
	/**************************************************************普通PD************************************************************************/
#ifdef USE_ANGLE_PD 
	PWM_ANGLE = P_ANGLE*(Angle_Last+PWM_SPEED_OUT/P_ANGLE) + D_ANGLE*Gyro_Last/10;	
#endif
	
	/**************************************************************滑动PD************************************************************************/
	
	/**************************************************************专家PID***********************************************************************/

}



