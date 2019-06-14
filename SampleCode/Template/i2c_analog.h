
#ifndef _I2C_ANALOG_H_
#define _I2C_ANALOG_H_

#include "NuMicro.h"
#include <stdio.h>


#define SET_SDA              			(PC0 = 1)
#define RESET_SDA         				(PC0 = 0)
#define SET_SCL              			(PC1 = 1)
#define RESET_SCL        				(PC1 = 0)
#define I2C_ANALOG_SDA_STATE 			(PC0)
#define I2C_ANALOG_SCL_STATE 			(PC1)
#define I2C_ANALOG_DELAY     			(I2C_ANALOG_Delay())

enum I2C_ANALOG_REPLAY_ENUM
{
     I2C_ANALOG_NACK = 0,
     I2C_ANALOG_ACK = 1
};

enum I2C_ANALOG_BUS_STATE_ENUM
{
     I2C_ANALOG_BUS_READY = 0,
     I2C_ANALOG_BUS_BUSY=1,
     I2C_ANALOG_BUS_ERROR=2
};

void I2C_ANALOG_GPIO_Init(void);
void I2C_ANALOG_Delay(void);
uint8_t I2C_ANALOG_Start(void);
void I2C_ANALOG_Stop(void);
void I2C_ANALOG_SendACK(void);
void I2C_ANALOG_SendNACK(void);
uint8_t I2C_ANALOG_SendByte(uint8_t Data);
uint8_t I2C_ANALOG_RecvByte(void);
void I2C_ANALOG_WriteData(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
uint8_t I2C_ANALOG_ReadData(uint8_t SlaveAddress,uint8_t REG_Address);

uint8_t I2C_ANALOG_WriteMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count); 
uint8_t I2C_ANALOG_ReadMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count); 


#endif


