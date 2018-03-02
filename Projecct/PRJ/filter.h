#ifndef __FILTER_H
#define __FILTER_H


//清华融合滤波参数
#define GRAVITY_ADJUST_TIME_CONSTANT     2     //1.6 //0.1 //波形偏下减小该值   2
#define DT                               0.005    //0.005





void QingHua_AngleCalaulate(float G_angle,float Gyro);
void Kalman_Filter(float angle_m,float gyro_m);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);


#endif

