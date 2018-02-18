#include "angle.h"


float Gyro,Angle,Angle_Last,Gyro_Last,Gyro_ago,ANGLE_I,Gyro_Turn,PWM_ANGLE_AGO;                         //�����ԭʼ����
float PWM_ANGLE;

extern u8 P_A,D_A;
extern int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;                 //ԭʼ����
extern int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;

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
	 IMUupdate(angleX, angleY, angleZ, gyroX, gyroY, gyroZ); //���˲�
	 Angle_Last = AngleX;
	 Gyro_Last = Gyro;
	#endif
}



//ֱ����
void duty_angle()
{
	angle_get();
	
	/**************************************************************��ͨPD************************************************************************/
#ifdef USE_ANGLE_PD 
	PWM_ANGLE = P_ANGLE*Angle_Last + D_ANGLE*Gyro_Last/4;	
#endif
	
	/**************************************************************����PD************************************************************************/
#ifdef USE_ANGLE_MOVE_PD  
  
	//һ�����Ƕ�ƫ�����ʹ�ý�ǿ����
	if((Angle_Last>Angle_max)||(Angle_Last<-Angle_max))
		PWM_ANGLE = P_ANGLE_MAX*Angle_Last + D_ANGLE_MAX*Gyro_Last;
	
	//�������Ƕ�ƫ���С��ʹ�ý�������	
	else if((Angle_Last<Angle_min)||(Angle_Last>-Angle_min))
		PWM_ANGLE = P_ANGLE_MIN*Angle_Last + D_ANGLE_MIN*Gyro_Last;
	
	//����������������
	else
		PWM_ANGLE = P_ANGLE*Angle_Last + D_ANGLE*Gyro_Last;
	
#endif
	
	/**************************************************************ר��PID***********************************************************************/
#ifdef USE_ANGLE_EXPERT_PD 	
	
	ANGLE_I += Angle_Last;  //������
	if(ANGLE_I>20)
		ANGLE_I = 20;
	else if(ANGLE_I<-20)
		ANGLE_I = -20;
	PWM_ANGLE_AGO = PWM_ANGLE;  
		
	//һ�����Ƕ�ƫ�����ֱ�ӿ���������
	if((Angle_Last>Angle_max)||(Angle_Last<-Angle_max))
		PWM_ANGLE = PWM_Angle_max;
	 
	//���������ٶ���Ƕ�ͬ��ƫ����������չ���������Ϊһ��ֵ
	else if((Angle_Last*Gyro_Last>0)||(Gyro_Last==0))
	{
		if((Angle_Last>Angle_max2)||(Angle_Last<-Angle_max2))      //ͬʱ�Ƕ����ϴ�������PD���Ƴ�������Ŵ�ϵ��
			PWM_ANGLE = K_max*(P_ANGLE*Angle_Last + D_ANGLE*Gyro_Last);
		else                                                       //ͬʱ�ǶȽ�С��������PD
			PWM_ANGLE = P_ANGLE*Angle_Last + D_ANGLE*Gyro_Last;
	}
	
	//���������ٶ���Ƕ�ͬ��ƫ�����С����չ��
	else if(Angle_Last*Gyro_Last<0)
	{
		if((Gyro_Last*Gyro_ago>0)||Angle_Last==0)  //�������Ѵﵽ��̬���򱣳����ֵ����
			PWM_ANGLE = PWM_ANGLE_AGO;
		else if(Gyro_Last*Gyro_ago<0)               //�����ٽ�״̬
		{
			if((Angle_Last>Angle_max2)||(Angle_Last<-Angle_max2))    //ͬʱƫ��ϴ󣬳�������Ŵ�ϵ��
				PWM_ANGLE = K_max*P_ANGLE*Angle_Last;
			else                                                     //ͬʱƫ���С����������ϵ��
				PWM_ANGLE = K_min*P_ANGLE*Angle_Last;
		}		
	}
	
	//�ġ�����ͬʱ�����Ƕ�ƫ��ǳ�С��������ֿ���
	if(Angle_Last<Angle_min)
		PWM_ANGLE += I_ANGLE*ANGLE_I;	
	
#endif
	
	
}



