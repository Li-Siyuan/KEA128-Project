#ifndef  __TURN_H
#define  __TURN_H

#include "headfile.h"


//ת�򻷲���
#define    P_TURN                       P_T
#define    D_TURN                       D_T

#define offet_1              10
#define offet_0               5
#define offet_2              15
#define adc_max              20
#define turn_radio            1

//ʹ����ͨPD
#define  USE_TURN_PD
//ʹ��ģ��PD
//#define  USE_TURN_BLUR_PD
//ʹ�÷���PD
//#define  USE_TURN_MOVE_PD





void duty_turn(void);






#endif


