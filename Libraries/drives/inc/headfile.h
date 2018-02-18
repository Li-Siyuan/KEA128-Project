/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		ͷ�ļ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/
#ifndef  __HEADFILE_H
#define  __HEADFILE_H

#include "common.h"
#include "KEA128_port_cfg.h"
#include "stdio.h"

//--------������--------
#include "KEA128_ftm.h"
#include "KEA128_pit.h"
#include "KEA128_gpio.h"
#include "KEA128_port.h"
#include "KEA128_adc.h"
#include "KEA128_irq.h"
#include "KEA128_uart.h"
#include "KEA128_kbi.h"
#include "KEA128_flash.h"
#include "KEA128_systick.h"
#include "KEA128_i2c.h"
#include "KEA128_spi.h"



//--------��ɿƼ���Ʒ���̿�--------
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_MPU6050.h"
//#include "SEEKFREE_NRF24L01.h"

//my_fun.h
#include "iic_moni.h"
#include "mpu6050.h"
#include "nvic.h"
#include "codetab.h"
#include "oled_iic.h"
#include "my_nrf.h"
#include "init.h"
#include "filter.h"
#include "protoco.h"
#include "control.h"
#include "speed.h"
#include "angle.h"
#include "turn.h"
#include "key.h"
#include "master.h"
#include "COMPUTER.h"


#endif
