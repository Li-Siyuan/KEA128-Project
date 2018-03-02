#ifndef __ANGLE_H
#define __ANGLE_H

#include "headfile.h"

//选择直立控制算法
#define USE_ANGLE_PD              //普通PD
//#define USE_ANGLE_MOVE_PD       //滑动PD
//#define  USE_ANGLE_EXPERT_PD    //专家PD

//原始数据处理参数
#define gyro_offset_z            9//9              //陀螺仪中值        //加大向后，减少向前
#define gyro_offset_x            0              //陀螺仪中值 
#define Gyro_ratio             	 0.8// 0.01    //陀螺仪归一化系数
#define angle_offset             170             //加速度计中值
#define  Angle_ratio             11           //加速度计归一化系数


//直立环PD参数
#define P_ANGLE             P_A
#define D_ANGLE             D_A


//滑动PD参数
#ifdef USE_ANGLE_MOVE_PD
	#define P_ANGLE_MAX             2*P_A
	#define D_ANGLE_MAX             2*D_A
	#define P_ANGLE_MIN             0.5*P_A
	#define D_ANGLE_MIN             0.5*D_A
#endif

//专家PID参数
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


