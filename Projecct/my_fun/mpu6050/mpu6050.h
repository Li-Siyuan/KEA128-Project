#ifndef __MPU6050_I2CMONI_H
#define __MPU6050_I2CMONI_H

//#include "stm32f0xx.h"
#include "iic_moni.h"

#include "common.h"
//#define u8 unsigned char
//#define u16 unsigned short int
//#define u32 unsigned int 

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))



//typedef struct
//{
//  __IO uint32_t CR1;      /*!< I2C Control register 1,            Address offset: 0x00 */
//  __IO uint32_t CR2;      /*!< I2C Control register 2,            Address offset: 0x04 */
//  __IO uint32_t OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
 // __IO uint32_t OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
 // __IO uint32_t TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
 // __IO uint32_t TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
 // __IO uint32_t ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
//  __IO uint32_t ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
 // __IO uint32_t PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
 // __IO uint32_t RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
//  __IO uint32_t TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */
//}I2C_TypeDef;







void MPU6050_MoniI2c_Config(void);
void MPU6050_MoniI2c_ITConfig(void);
u8 MPU6050_MoniI2c_GetId(void);
int16 MPU6050_MoniI2c_GetAccX(void);
int16 MPU6050_MoniI2c_GetAccY(void);
int16 MPU6050_MoniI2c_GetAccZ(void);
int16 MPU6050_MoniI2c_GetGyroX(void);
int16 MPU6050_MoniI2c_GetGyroY(void);
int16 MPU6050_MoniI2c_GetGyroZ(void);
float MPU6050_MoniI2c_GetTemperature(void);

#endif
