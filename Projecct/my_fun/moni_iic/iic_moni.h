#ifndef __I2C_MONI_H
#define __I2C_MONI_H

//#include "stm32f0xx.h"
#include "common.h"
//#define u8 unsigned char
//#define u16 unsigned short int
//#define u32 unsigned int 

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))


typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;




#define ERROR       0
#define SUCCESS     1
 
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



//#define IIC_PORT_CLOCK    kCLOCK_PortD

	
void I2C_MoniConfig(void);
void I2C_Start(void);
void I2C_Stop(void);

FunctionalState MoniI2C_WriteByte(u8 data);
u8 MoniI2C_ReadByte(FunctionalState ackState); 

u8 MoniI2c_WriteByteToSlave(u8 addr,u8 reg,u8 data);
u8 MoniI2c_WriteSomeDataToSlave(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MoniI2c_ReadFromSlave(u8 addr,u8 reg,u8 *buf);
u8 MoniI2c_ReadSomeDataFromSlave(u8 addr,u8 reg,u8 len,u8 *buf);

#endif

