
/* Includes ------------------------------------------------------------------*/
#include "I2C_LIS3DH.h"

#if defined (I2C_ANALOG)
#include "i2c_analog.h"
#endif


//float powf(float __x,float __y);
#define LIS3DH_BIT3			(0x1<<3)
#define PI 					((float)3.14159265f)
float fNormAcc,fSinRoll,fCosRoll,fSinPitch,fCosPitch = 0.0f, RollAng = 0.0f, PitchAng = 0.0f;

int16_t LIS3DH_accx=0,LIS3DH_accy=0,LIS3DH_accz=0;
uint8_t FlagSwitch = 0;


void I2C_readBytes(I2C_T *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) 
{
	uint8_t i, tmp;
	I2C_START(i2c);                         //Start
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, devAddr);         //send slave address+W
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, regAddr);        //send index
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);

	I2C_SET_CONTROL_REG(i2c, I2C_CTL_STA | I2C_CTL_SI);	//Start
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, (devAddr+1));    //send slave address+R
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);							

	for (i=0; i<length; i++) {
		I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
		I2C_WAIT_READY(i2c);							
		tmp = I2C_GET_DATA(i2c);           //read data   
		data[i]=tmp;
	}
	I2C_STOP(i2c);										 //Stop
}

void I2C_writeBytes(I2C_T *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) 
{
	uint8_t i;
	uint32_t tmp;
	I2C_START(i2c);                    //Start
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, devAddr);        //send slave address
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, regAddr);        //send index
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);	

	for (i=0; i<length; i++) {
		tmp = data[i];
		I2C_SET_DATA(i2c, tmp);            //send Data
		I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
		I2C_WAIT_READY(i2c);
	}

	I2C_STOP(i2c);										 //Stop
}



void LIS3DH_Delay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

void LIS3DH_Read(uint8_t DeviceAddr, uint8_t RegisterAddr,
                              uint16_t NumByteToRead,
                              uint8_t* pBuffer)
{

	#if defined (I2C_ANALOG)

	I2C_ANALOG_ReadMulti(DeviceAddr, RegisterAddr, pBuffer, NumByteToRead);

	#else
	
	I2C_ReadMultiBytesOneReg(I2C0, DeviceAddr, RegisterAddr, pBuffer, NumByteToRead);	
//	*pBuffer = I2C_ReadByteOneReg(I2C0, DeviceAddr, RegisterAddr);
//	I2C_readBytes(I2C0, DeviceAddr, RegisterAddr, NumByteToRead, pBuffer);
	
	#endif
}
void LIS3DH_Write(uint8_t DeviceAddr, uint8_t RegisterAddr,
                               uint16_t NumByteToWrite,
                               uint8_t* pBuffer)
{
	#if defined (I2C_ANALOG)
	
	I2C_ANALOG_WriteMulti(DeviceAddr, RegisterAddr, pBuffer, NumByteToWrite);
	
	#else

	I2C_WriteMultiBytesOneReg(I2C0, DeviceAddr, RegisterAddr, pBuffer, NumByteToWrite);	
//	I2C_WriteByteOneReg(I2C0, DeviceAddr, RegisterAddr,*pBuffer);
//	I2C_writeBytes(I2C0, DeviceAddr, RegisterAddr,NumByteToWrite,pBuffer);
	
	#endif
}

void SetupLIS3DH(void)
{
	uint8_t data;

	LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_WHO_AM_I, 1,&data);
	printf("LIS3DH_WHO_AM_I_ADDR = 0x%X\r\n",data);
	LIS3DH_Delay(5);

	LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_CTRL_REG1, 1,&data);
	LIS3DH_Delay(5);	
	
	data = 0x47;	
	LIS3DH_Write(LIS3DH_ADDRESS, LIS3DH_CTRL_REG1, 1,&data);// ODR : 50Hz
	LIS3DH_Delay(5);
}	

uint16_t GetDataLIS3DH(void)
{
	uint8_t tmpxl, tmpxh, tmpyl, tmpyh, tmpzl, tmpzh, tmp;
	int16_t ax_s,ay_s,az_s;
//	uint16_t i=0;
	uint8_t u8WaitCnt=0;

	do{
		LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_STATUS_REG2, 1,&tmp);	
		if (u8WaitCnt++>30)
			break;		
	}while(!(tmp & LIS3DH_BIT3));

	LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_OUT_X_H, 1, &tmpxh);
	LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_OUT_X_L, 1, &tmpxl);
	ax_s=(((int16_t) ((tmpxh << 8) | tmpxl)) >> 4);

	LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_OUT_Y_H, 1, &tmpyh);
	LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_OUT_Y_L, 1, &tmpyl);
	ay_s=(((int16_t) ((tmpyh << 8) | tmpyl)) >> 4);

	LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_OUT_Z_H, 1, &tmpzh);
	LIS3DH_Read(LIS3DH_ADDRESS, LIS3DH_OUT_Z_L, 1, &tmpzl);
	az_s=(((int16_t) ((tmpzh << 8) | tmpzl)) >> 4);

	#if 0	//debug
	printf("Accelerometer(mg):     X-axis : %5d, ",(int16_t)ax_s);
	printf(" Y-axis : %5d,  ",(int16_t)ay_s);
	printf(" Z-axis :  %5d \n\r",(int16_t)az_s);
	#endif

	LIS3DH_accx = ax_s ;
	LIS3DH_accy = ay_s ;
	LIS3DH_accz = az_s ;
	
//	LIS3DH_Delay(0xFFFF);
	LIS3DH_Delay(5);
	return 0;
}

void Angle_Calculate(void)
{  
	#if 1	//Tilt Test.xlsx @Yiven

	if (FlagSwitch)
	{
		printf("Accelerometer(mg):     X-axis : %5d, ",(int16_t)LIS3DH_accx);
		printf(" Y-axis : %5d,  ",(int16_t)LIS3DH_accy);
		printf(" Z-axis :  %5d \n\r",(int16_t)LIS3DH_accz);
	}
	else
	{
		float s1 = 0;
		float s2 = 0;	

		s1 = sqrt((float)((LIS3DH_accy*LIS3DH_accy)+(LIS3DH_accz*LIS3DH_accz)));
		s2 = sqrt((float)((LIS3DH_accx*LIS3DH_accx)+(LIS3DH_accz*LIS3DH_accz)));

		PitchAng = atan(LIS3DH_accx/s1)*180/PI;
		RollAng = atan(LIS3DH_accy/s2)*180/PI;

		printf("%s:RollAng(%10.3lf),PitchAng(%10.3lf)\r\n",__FUNCTION__,RollAng,PitchAng);
	}
	
	#endif
	
	#if 0
/*
	http://stackoverflow.com/questions/3755059/3d-accelerometer-calculate-the-orientation

	Roll = atan2(Y, Z) * 180/M_PI;
	Pitch = atan2(-X, sqrt(Y*Y + Z*Z)) * 180/M_PI;
	Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf 
*/


	RollAng= atan2(LIS3DH_accy, LIS3DH_accz) * 180/PI;
	PitchAng = atan2(-LIS3DH_accz, sqrt(LIS3DH_accy*LIS3DH_accy + LIS3DH_accz*LIS3DH_accz)) * 180/PI;

	printf("%s:RollAng(%10.3lf),PitchAng(%10.3lf)\r\n",__FUNCTION__,RollAng,PitchAng);
	#endif
	
	#if 0
    RollAng  = 180.0*atan(LIS3DH_accy/sqrt((float)(LIS3DH_accx*LIS3DH_accx+LIS3DH_accz*LIS3DH_accz)))/PI;       
    PitchAng = -180.0*atan(LIS3DH_accx/sqrt((float)(LIS3DH_accz*LIS3DH_accz+LIS3DH_accy*LIS3DH_accy)))/PI;
    if(LIS3DH_accz<0)
	{
        if(LIS3DH_accy>0)
		{
			RollAng  = 180.0 -RollAng;
		}
        else 
		{
			RollAng  = -(RollAng+180.0);
		}
    }
	printf("%s:RollAng(%10.3lf),PitchAng(%10.3lf)\r\n",__FUNCTION__,RollAng,PitchAng);
	#endif
	
	#if 0
	LIS3DH_accx /= 100.0f;
	LIS3DH_accy /= 100.0f;
	LIS3DH_accz /= 100.0f;	

	fNormAcc = sqrt((LIS3DH_accx*LIS3DH_accx)+(LIS3DH_accy*LIS3DH_accy)+(LIS3DH_accz*LIS3DH_accz));

	fSinRoll = -LIS3DH_accy/fNormAcc;
	fCosRoll = sqrt(1.0-(fSinRoll * fSinRoll));
	fSinPitch = LIS3DH_accx/fNormAcc;
	fCosPitch = sqrt(1.0-(fSinPitch * fSinPitch));
	if ( fSinRoll >0)
	{
		if (fCosRoll>0)
		{
			RollAng = acos(fCosRoll)*180/PI;
		}
		else
		{
			RollAng = acos(fCosRoll)*180/PI + 180;
		}
	}
	else
	{
		if (fCosRoll>0)
		{
			RollAng = acos(fCosRoll)*180/PI + 360;
		}
		else
		{
			RollAng = acos(fCosRoll)*180/PI + 180;
		}
	}

	if ( fSinPitch >0)
	{
		if (fCosPitch>0)
		{
			PitchAng = acos(fCosPitch)*180/PI;
		}
		else
		{
			PitchAng = acos(fCosPitch)*180/PI + 180;
		}
	}
	else
	{
		if (fCosPitch>0)
		{
			PitchAng = acos(fCosPitch)*180/PI + 360;
		}
		else
		{
			PitchAng = acos(fCosPitch)*180/PI + 180;
		}
	}

	if (RollAng >=360)
	{
		RollAng = RollAng - 360;
	}

	if (PitchAng >=360)
	{
		PitchAng = PitchAng - 360;
	}

	printf("%s:RollAng(%10.3lf),PitchAng(%10.3lf)\r\n",__FUNCTION__,RollAng,PitchAng);

	#endif
}
