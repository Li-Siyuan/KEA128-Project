#ifndef  __CONTROL_H
#define  __CONTROL_H

#include "headfile.h"


//滤波模式选择
#define QingHua
//#define Kalman
//#define UPDATE


//调试模式选择
//#define  ANGLE_CONTROL    //只调节直立环
//#define  SPEED_CONTROL    //只调节直立环和速度环
#define  TURN_CONTROL       //直立+速度+转向
#define DEBUG_MODE

//其他功能选择
//#define USE_NRF            //使用NRF
#define USE_UART          //使用串口UART
#define USE_OLED           //使用OLED


//管脚分配
#define    LED         H2
#define    KEY1        E4
#define    KEY2        E5
#define    KEY3        E6
//#define    KEY4        D2
//#define    KEY5        D3
#define    DIR_L       G2
#define    DIR_R       G3




#define  sector  (FLASH_SECTOR_NUM - 1)





void ALL_INIT(void);
void duty_5ms(void);
void angle_get(void);
void DISPLAY(void);
void READ_8_DATA(void);
void WRITE_8_DATA(void);

#endif


