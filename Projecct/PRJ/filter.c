#include "filter.h"
#include "control.h"
#include "math.h"

/**************************************************************************
//   清华角度滤波方案
*************************************************************************
*
*  功能说明：清华角度滤波
*  参数说明：G_angle                       加速度计角度0-90内
*            Gyro                         陀螺仪角速度转花后的数值
*            GRAVITY_ADJUST_TIME_CONSTANT  时间校正系数
DT                             定时器时间 单位s
*  函数返回：无符号结果值
*  修改时间：2013-2-10
* 备注：参考清华源码
*/
//
//*************************************************************************
float g_fCarAngle,g_fGyroscopeAngleIntegral; //融合后的角度

void QingHua_AngleCalaulate(float G_angle,float Gyro)
{
    float fDeltaValue;

    g_fCarAngle = g_fGyroscopeAngleIntegral;   //最终融合角度
    fDeltaValue = (G_angle - g_fCarAngle) / GRAVITY_ADJUST_TIME_CONSTANT;  //时间系数矫正
    g_fGyroscopeAngleIntegral += (Gyro + fDeltaValue) * DT;                //融合角度
}



//**************************************************************************
//   Kalman滤波
//**************************************************************************

float angle, angle_dot;         //外部需要引用的变量
//-------------------------------------------------------
// 0.00015     //0.0001
const float Q_angle=0.001, Q_gyro=0.002/*0.003*/, R_angle=0.67/*0.5*/, dt=0.005;
//0.0001         //0.00015        //1.2
//注意：dt的取值为kalman滤波器采样时间;         //0.8
static float P[2][2] = {
    { 1, 0 },
    { 0, 1 }
};

static float Pdot[4] ={0,0,0,0};

static const char C_0 = 1;

static float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//-------------------------------------------------------
void Kalman_Filter(float angle_m,float gyro_m)          //gyro_m:gyro_measure
{
    angle+=(gyro_m-q_bias) * dt;
    
    Pdot[0]=Q_angle - P[0][1] - P[1][0];
    Pdot[1] = -P[1][1];
    Pdot[2] = -P[1][1];
    Pdot[3]=Q_gyro;
    
    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;
    
    
    angle_err = angle_m - angle;
    
    

    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];
    
    E = R_angle + C_0 * PCt_0;
    
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    
    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
    
    
    angle   += K_0 * angle_err;
    q_bias  += K_1 * angle_err;
    angle_dot = gyro_m-q_bias;
}

/**************************************************************************
//  新滤波方案
*************************************************************************/
float AngleX=0,AngleY=0;
#define	pi		3.14159265f                           
#define	Kp		0.8f                        
#define	Ki		0.001f                         
#define	halfT    	0.004f           

float  q0=1,q1=0,q2=0,q3=0;   
volatile float  exInt=0,eyInt=0,ezInt=0;  


void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	 float  norm=0;
         float  vx=0, vy=0, vz=0;
	 float  ex=0, ey=0, ez=0;

	norm = sqrt(ax*ax + ay*ay + az*az);	//把加速度计的三维向量转成单维向量   
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

		//	下面是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。 
		//	根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素
		//	所以这里的vx vy vz，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的
		//	重力单位向量。
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3 ;

	ex = (ay*vz - az*vy) ;
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;

	q0 = q0 + (-q1*gx - q2*gy - q3*gz) * halfT;
	q1 = q1 + ( q0*gx + q2*gz - q3*gy) * halfT;
	q2 = q2 + ( q0*gy - q1*gz + q3*gx) * halfT;
	q3 = q3 + ( q0*gz + q1*gy - q2*gx) * halfT;

	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	AngleX  = asin(2*(q0*q2 - q1*q3 )) * 57.2957795f; // 俯仰   换算成度
	AngleY = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll 

}

