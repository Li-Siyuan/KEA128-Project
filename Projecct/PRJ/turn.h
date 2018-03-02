#ifndef  __TURN_H
#define  __TURN_H

#include "headfile.h"


//转向环参数
#define    P_TURN                       P_T
#define    D_TURN                       D_T

#define offet_1              10
#define offet_0               5
#define offet_2              15
#define adc_max              20
#define turn_radio            1

//使用普通PD
#define  USE_TURN_PD
//使用模糊PD
//#define  USE_TURN_BLUR_PD
//使用分组PD
//#define  USE_TURN_MOVE_PD
//使用最大转弯半径限制
//#define  MAX_RADIUS





void duty_turn(void);






#endif


