#ifndef  __SPEED_H
#define  __SPEED_H

#include "headfile.h"



//速度环参数
#define speed_ratio             0.1
#define P_SPEED                 P_S
#define I_SPEED                 I_S
#define I_MIN                   50           //变速积分下限
#define I_MAX                  800          //变速积分上限





void duty_speed(void); 







#endif


