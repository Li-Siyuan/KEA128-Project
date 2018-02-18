#ifndef  __CONTROL_H
#define  __CONTROL_H

#include "headfile.h"


//�˲�ģʽѡ��
#define QingHua
//#define Kalman
//#define UPDATE


//����ģʽѡ��
//#define  ANGLE_CONTROL    //ֻ����ֱ����
//#define  SPEED_CONTROL    //ֻ����ֱ�������ٶȻ�
#define  TURN_CONTROL       //ֱ��+�ٶ�+ת��
#define DEBUG_MODE

//��������ѡ��
//#define USE_NRF            //ʹ��NRF
#define USE_UART          //ʹ�ô���UART
#define USE_OLED           //ʹ��OLED


//�ܽŷ���
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


