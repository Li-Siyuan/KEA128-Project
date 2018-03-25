#include "angle.h"


float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;                         //�����ԭʼ����
float PWM_ANGLE;

extern u8 P_A,D_A;
extern int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;                 //ԭʼ����
extern int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;


extern float PWM_SPEED_OUT;//�����ٶȻ����


#ifdef QingHua
extern float g_fCarAngle;                 //�ں��˲��������
#endif
#ifdef Kalman
extern float angle, angle_dot;          //�������˲��������
#endif
#ifdef UPDATE
extern float AngleX;                    //���˲������������
#endif

//int16 Angle_Need = 3;

//�õ��Ƕ�ֵ
void angle_get()
{

	//ԭʼ����
   Get_AccData();
   Get_Gyro();
        
	//���ݴ���
	 Gyro = (gyro_offset_z + mpu_gyro_z) * Gyro_ratio;                           //�����ǲɼ����Ľ��ٶȹ�һ��
	 Gyro_Turn = -(gyro_offset_x - mpu_gyro_x) * Gyro_ratio;                     //�����ǲɼ����Ľ��ٶȹ�һ��
   Angle = (mpu_acc_y+angle_offset) / Angle_ratio ;                          //�����ٶȼƲɼ����ĽǶȹ�һ��������0.375��Ϊ�˹�һ����0~90��

   if(Angle > 90)Angle = 90;                                              //��ֹ���ٶȽǶ����
   if(Angle < -90)Angle = -90;
  
   Gyro_ago = Gyro_Last;
		
	//�˲�
	#ifdef QingHua
	 QingHua_AngleCalaulate(Angle,Gyro);   //�廪�ں��˲�
	 Angle_Last = g_fCarAngle;
	 Gyro_Last = Gyro;
	#endif
	
	#ifdef Kalman
	 Kalman_Filter(Angle,Gyro) ;      //�������˲�
	 Angle_Last = angle;
	 Gyro_Last = angle_dot;
	#endif
	
	#ifdef UPDATE
	 IMUupdate(angleX, angleY, angleZ, gyroX, gyroY, gyroZ); //���˲�����
	 Angle_Last = AngleX;
	 Gyro_Last = Gyro;
	#endif
}



//ֱ������ͨPD+����
void duty_angle()
{
	angle_get();
	
	/**************************************************************��ͨPD************************************************************************/
#ifdef USE_ANGLE_PD 
	PWM_ANGLE = P_ANGLE*(Angle_Last+PWM_SPEED_OUT/P_ANGLE) + D_ANGLE*Gyro_Last/10;	
#endif
	
	/**************************************************************����PD************************************************************************/
	
	/**************************************************************ר��PID***********************************************************************/

}



