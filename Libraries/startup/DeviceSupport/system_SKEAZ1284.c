/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		system
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#include <stdint.h>
#include "SKEAZ1284.h"


//-------------------------------------------------------------------------------------------------------------------
//  KEA128_CLOCK Ϊ 0  �ں�Ƶ�� 40M  ����Ƶ��20M   ��ʱ��Ƶ��40M
//  KEA128_CLOCK Ϊ 1  �ں�Ƶ�� 80M  ����Ƶ��40M   ��ʱ��Ƶ��80M
//-------------------------------------------------------------------------------------------------------------------
#define KEA128_CLOCK    1



void disable_wdog(void)
{
    /* WDOG->TOVAL: TOVAL=0xE803 */
    WDOG->TOVAL = WDOG_TOVAL_TOVAL(0xE803); /* Timeout value */
    /* WDOG->CS2: WIN=0,FLG=0,??=0,PRES=0,??=0,??=0,CLK=1 */
    WDOG->CS2 = WDOG_CS2_CLK(0x01);       /* 1-kHz clock source */
    /* WDOG->CS1: EN=0,INT=0,UPDATE=1,TST=0,DBG=0,WAIT=1,STOP=1 */
    WDOG->CS1 = WDOG_CS1_UPDATE_MASK |
                WDOG_CS1_TST(0x00) |
                WDOG_CS1_WAIT_MASK |
                WDOG_CS1_STOP_MASK;
}

void Start (void) 
{
    disable_wdog();

    
   #if 1   
      SIM->CLKDIV = SIM_CLKDIV_OUTDIV1(0x00) | SIM_CLKDIV_OUTDIV2_MASK; //???????
    //???PEE??
    ICS->C2 = ICS_C2_BDIV(0x00) | ICS_C2_LP_MASK;
    OSC->CR = OSC_CR_OSCEN_MASK | OSC_CR_OSCOS_MASK | OSC_CR_RANGE_MASK | OSC_CR_HGO_MASK;
    ICS->C1 = ICS_C1_CLKS(0x00) | ICS_C1_RDIV(0x04-KEA128_CLOCK) | ICS_C1_IRCLKEN_MASK;
    while((ICS->S & ICS_S_IREFST_MASK) != 0x00U) {};    //??????????????
    while((ICS->S & ICS_S_LOCK_MASK) == 0x00U){};       //??FLL??
    while((ICS->S & 0x0CU) != 0x00U) {};                //????FLL??
#endif   
  
#if 0
    /* the following code segment demonstrates setting the ICS to FEI mode using a custom trim
    value provided by a programming tool. The resulting ICSOUT frequency is fint_t*1280/BDIV. */
    ICS_C3 = *((uint8_t*) 0x03FF); // trim internal reference clock
    ICS_C4 = *((uint8_t*) 0x03FE); // fine trim internal reference clock
    ICS_C2 = 0x20; // BDIV=divide by 2 � use default until clock dividers configured
    ICS_C1 = 0x04; // internal reference clock as source to FLL
 //   ICS_C1 |=  ICS_C1_RDIV(0x04-KEA128_CLOCK);
    while ((ICS_S & ICS_S_LOCK_MASK) == 0); // wait for FLL to lock
    SIM_CLKDIV = 0x01100000; // core clock = ICSOUT/1 and bus clock = core clock/2
    ICS_C2 = 0x00; // BDIV=divide by 1 � allows max core and bus clock frequencies
     OSC->CR = OSC_CR_OSCEN_MASK | OSC_CR_OSCOS_MASK | OSC_CR_RANGE_MASK | OSC_CR_HGO_MASK;
#endif
}

