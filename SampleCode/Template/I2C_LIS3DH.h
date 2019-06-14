
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIS3DH_API_H
#define __LIS3DH_API_H

#include "NuMicro.h"
#include <stdio.h>
#include <math.h>

#include "i2c_analog.h"

#define LIS3DH_SA0							(PC4)   
#define LIS3DH_ADDRESS                     	(0x18|LIS3DH_SA0) //(0x33>>1) //0x33

extern int16_t LIS3DH_accx,LIS3DH_accy,LIS3DH_accz;

#define LIS3DH_STATUS_REG_AUX 				0x07
#define LIS3DH_OUT_ADC1_L 					0x08
#define LIS3DH_OUT_ADC1_H 					0x09
#define LIS3DH_OUT_ADC2_L 					0x0A
#define LIS3DH_OUT_ADC2_H 					0x0B
#define LIS3DH_OUT_ADC3_L 					0x0C
#define LIS3DH_OUT_ADC3_H 					0x0D
#define LIS3DH_INT_COUNTER_REG 				0x0E
#define LIS3DH_WHO_AM_I 					0x0F
#define LIS3DH_TEMP_CFG_REG 				0x1F
#define LIS3DH_CTRL_REG1 					0x20
#define LIS3DH_CTRL_REG2 					0x21
#define LIS3DH_CTRL_REG3 					0x22
#define LIS3DH_CTRL_REG4 					0x23
#define LIS3DH_CTRL_REG5 					0x24
#define LIS3DH_CTRL_REG6 					0x25
#define LIS3DH_REFERENCE 					0x26
#define LIS3DH_STATUS_REG2 					0x27
#define LIS3DH_OUT_X_L 						0x28
#define LIS3DH_OUT_X_H 						0x29
#define LIS3DH_OUT_Y_L 						0x2A
#define LIS3DH_OUT_Y_H 						0x2B
#define LIS3DH_OUT_Z_L 						0x2C
#define LIS3DH_OUT_Z_H 						0x2D
#define LIS3DH_FIFO_CTRL_REG 				0x2E
#define LIS3DH_FIFO_SRC_REG 					0x2F
#define LIS3DH_INT1_CFG 						0x30
#define LIS3DH_INT1_SOURCE 					0x31
#define LIS3DH_LIS3DH_INT1_THS 				0x32
#define LIS3DH_INT1_DURATION 				0x33
#define LIS3DH_CLICK_CFG 					0x38
#define LIS3DH_CLICK_SRC 					0x39
#define LIS3DH_CLICK_THS 					0x3A
#define LIS3DH_TIME_LIMIT 					0x3B
#define LIS3DH_TIME_LATENCY 					0x3C
#define LIS3DH_TIME_WINDOW 				0x3D

#define LIS3DH_REG_TEMP       				0x00  /*!< Temperature Register of LIS3DH */
#define LIS3DH_REG_CONF       				0x01  /*!< Configuration Register of LIS3DH */
#define LIS3DH_REG_THYS       				0x02  /*!< Temperature Register of LIS3DH */
#define LIS3DH_REG_TOS        				0x03  /*!< Over-temp Shutdown threshold Register of LIS3DH */
#define LIS3DH_I2C_SPEED      				100000 /*!< I2C Speed */

/* Includes ------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void LIS3DH_Read(uint8_t DeviceAddr, uint8_t RegisterAddr,
                              uint16_t NumByteToRead,
                              uint8_t* pBuffer);

void LIS3DH_Write(uint8_t DeviceAddr, uint8_t RegisterAddr,
                               uint16_t NumByteToWrite,
                               uint8_t* pBuffer);

void SetupLIS3DH(void);
uint16_t GetDataLIS3DH(void);
void Angle_Calculate(void);

#endif /* __LIS3DH_API_H */

