#ifndef __ANGLE_H
#define __ANGLE_H

#include "headfile.h"

//ѡ��ֱ�������㷨
#define USE_ANGLE_PD              //��ͨPD
//#define USE_ANGLE_MOVE_PD       //����PD
//#define  USE_ANGLE_EXPERT_PD    //ר��PD

//ԭʼ���ݴ������
#define gyro_offset_z            9//9              //��������ֵ        //�Ӵ���󣬼�����ǰ
#define gyro_offset_x            0              //��������ֵ 
#define Gyro_ratio             	 0.8// 0.01    //�����ǹ�һ��ϵ��
#define angle_offset             70             //���ٶȼ���ֵ
#define  Angle_ratio             11           //���ٶȼƹ�һ��ϵ��


//ֱ����PD����
#define P_ANGLE             P_A
#define D_ANGLE             D_A


//����PD����
#ifdef USE_ANGLE_MOVE_PD
	#define P_ANGLE_MAX             2*P_A
	#define D_ANGLE_MAX             2*D_A
	#define P_ANGLE_MIN             0.5*P_A
	#define D_ANGLE_MIN             0.5*D_A
#endif

//ר��PID����
#ifdef USE_ANGLE_EXPERT_PD
	#define  Angle_max      30
	#define  Angle_min      5
	#define  PWM_Angle_max  985
	#define  Angle_max2     10
	#define  K_max          2
	#define  K_min          0.5
	#define  I_ANGLE        1
	#endif




void duty_angle(void);

#endif


