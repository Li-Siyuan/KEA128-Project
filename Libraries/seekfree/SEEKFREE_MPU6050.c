/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MPU6050
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 * @note	
					L3G4200D接线定义
					------------------------------------ 
						SDA                 B4
						SCL                 B5
					------------------------------------ 
					int main(void)
{
    get_clk();              //获取时钟频率 必须执行

    pit_init_ms(pit0,1000);				
    set_irq_priority(PIT_CH0_IRQn,1);	
    enable_irq(PIT_CH0_IRQn);			
    EnableInterrupts;					
    
    IIC_init();
    InitMPU6050();
    for(;;)
    {
        Get_Gyro();
        Get_AccData();
        systick_delay_ms(10);
    }
        
	
}
 ********************************************************************************************************************/



#include "SEEKFREE_MPU6050.h"

int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;

#define USER_CTRL		0x6A	//用户控制寄存器

#define PWR_MGMT_2		0x6C	//设置正常工作模式还是唤醒模式
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68,只读)
#define FIFO_EN			0x23	//各功能的FIFO的使能或失能

#define INT_ENABLE		0x38	//中断使能寄存器
//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void InitMPU6050(void)
{  
    //初始化MPU6050，解除睡眠状态
	simiic_write_reg(MPU6050_DEV_ADDR,PWR_MGMT_1, 0x80);			//复位MPU6050
	delay_us(100);													//延迟100ms
	simiic_write_reg(MPU6050_DEV_ADDR,PWR_MGMT_1, 0x00);			//唤醒MPU6050
	
	simiic_write_reg(MPU6050_DEV_ADDR,GYRO_CONFIG, 0x18);			//设置陀螺仪量程为：±2000dps
	simiic_write_reg(MPU6050_DEV_ADDR,ACCEL_CONFIG, 0x10);			//设置加速度传感器量程为：±2g
	simiic_write_reg(MPU6050_DEV_ADDR,SMPLRT_DIV, 0x04);			//设置采样率的分频值为19，从而采样率为50Hz
	simiic_write_reg(MPU6050_DEV_ADDR,_CONFIG, 0x04);				//设置数字低通滤波值，滤波带宽为20Hz
	simiic_write_reg(MPU6050_DEV_ADDR,USER_CTRL, 0x00);				//关闭所有FIFO，并且关闭I2C主模式
	simiic_write_reg(MPU6050_DEV_ADDR,FIFO_EN, 0x00);				//关闭各功能的FIFO
	simiic_write_reg(MPU6050_DEV_ADDR,INT_PIN_CFG, 0x80);			//设置INT引脚（INT引脚低电平有效，推挽输出...)
	simiic_write_reg(MPU6050_DEV_ADDR,INT_ENABLE, 0x00);			//关闭所有中断
	
	simiic_write_reg(MPU6050_DEV_ADDR,PWR_MGMT_1, 0x01);			//使用X轴的PLL时钟作为工作时钟
	simiic_write_reg(MPU6050_DEV_ADDR,PWR_MGMT_2, 0x00);			//设置陀螺仪和加速计处于正常工作模式
}




//内部使用用户无需调用
int16 GetData(uint8 REG_Address)
{
    uint8 L;   uint16 H ;
    H=simiic_read_reg(MPU6050_DEV_ADDR, REG_Address, IIC);
    L=simiic_read_reg(MPU6050_DEV_ADDR, REG_Address+1, IIC);
    return (H<<8)+L;   //合成数据
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_AccData(void)
{
    mpu_acc_x = GetData(ACCEL_XOUT_H)>>2;
    mpu_acc_y = GetData(ACCEL_YOUT_H)>>2;
    mpu_acc_z = GetData(ACCEL_ZOUT_H)>>2;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_Gyro(void)
{
    mpu_gyro_z = GetData(GYRO_XOUT_H)>>3;
    mpu_gyro_y = GetData(GYRO_YOUT_H)>>3;
    mpu_gyro_x = GetData(GYRO_ZOUT_H)>>3;
}


