
/* Includes ------------------------------------------------------------------*/
#include "i2c_analog.h"

/*-------------------------------------------------------------------------------*/

void I2C_ANALOG_GPIO_Init(void)
{
	SET_SDA;
	SET_SCL; 
}

void I2C_ANALOG_Delay(void)
{
	uint16_t i = 10;
	while (i)
	{
		i--;
		__asm("nop");
	}
}

uint8_t I2C_ANALOG_Start(void)
{
     SET_SDA;
     I2C_ANALOG_DELAY;

     SET_SCL;
     I2C_ANALOG_DELAY;

     if ( !I2C_ANALOG_SDA_STATE )
     {
          return I2C_ANALOG_BUS_BUSY;
     }

     RESET_SDA;
     I2C_ANALOG_DELAY;

     RESET_SCL;
     I2C_ANALOG_DELAY;

     if (I2C_ANALOG_SDA_STATE)
     {
          return I2C_ANALOG_BUS_ERROR;
     }

     return I2C_ANALOG_BUS_READY;
}

void I2C_ANALOG_Stop(void)
{
     RESET_SDA;
     I2C_ANALOG_DELAY;
     SET_SCL;
     I2C_ANALOG_DELAY;
     SET_SDA;
     I2C_ANALOG_DELAY;
}


void I2C_ANALOG_SendNACK(void)
{
     RESET_SDA;
     I2C_ANALOG_DELAY;
     SET_SCL;
     I2C_ANALOG_DELAY;
     RESET_SCL;
     I2C_ANALOG_DELAY;
}

void I2C_ANALOG_SendACK(void)
{
     SET_SDA;
     I2C_ANALOG_DELAY;
     SET_SCL;
     I2C_ANALOG_DELAY;
     RESET_SCL;
     I2C_ANALOG_DELAY;
}

uint8_t I2C_ANALOG_WaitACK(void)
{
    RESET_SCL;
    I2C_ANALOG_DELAY;
    SET_SDA;
    I2C_ANALOG_DELAY;
    SET_SCL; 	 
    I2C_ANALOG_DELAY;
	
    if (I2C_ANALOG_SDA_STATE)
    {
        RESET_SCL;
        I2C_ANALOG_DELAY;
        return I2C_ANALOG_BUS_ERROR;
    }

    RESET_SCL;
    I2C_ANALOG_DELAY;
    
    return I2C_ANALOG_BUS_READY;
}

uint8_t I2C_ANALOG_SendByte(uint8_t Data)
{
     uint8_t i;
     RESET_SCL;
     for(i=0;i<8;i++)
     { 
		if(Data & 0x80)
		{
		    SET_SDA;
		}
		else
		{
		    RESET_SDA;
		}
		Data <<= 1;
		I2C_ANALOG_DELAY;

		SET_SCL;
		I2C_ANALOG_DELAY;
		RESET_SCL;
		I2C_ANALOG_DELAY;
     }

     SET_SDA;
     I2C_ANALOG_DELAY;
     SET_SCL;
     I2C_ANALOG_DELAY;  
     if(I2C_ANALOG_SDA_STATE)
     {
           RESET_SCL;
           return I2C_ANALOG_NACK;
     }
     else
     {
           RESET_SCL;
           return I2C_ANALOG_ACK; 
     }   
}

uint8_t I2C_ANALOG_RecvByte(void)
{
     uint8_t i,Dat = 0;
     SET_SDA;
     RESET_SCL;
     Dat = 0;
     for(i=0; i<8; i++)
     {
           SET_SCL;          
           I2C_ANALOG_DELAY;
           Dat <<= 1;
           if(I2C_ANALOG_SDA_STATE)
           {
                Dat|=0x01;
           }  
           RESET_SCL;         
           I2C_ANALOG_DELAY;               
     }
     return Dat;
}

void I2C_ANALOG_WriteData(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
     I2C_ANALOG_Start();                  
     I2C_ANALOG_SendByte(SlaveAddress);   
     I2C_ANALOG_SendByte(REG_Address);    
     I2C_ANALOG_SendByte(REG_data);       
     I2C_ANALOG_Stop();                   
}


uint8_t I2C_ANALOG_ReadData(uint8_t SlaveAddress,uint8_t REG_Address)
{ 
     uint8_t REG_data;
     I2C_ANALOG_Start();                          
     I2C_ANALOG_SendByte(SlaveAddress);           
     I2C_ANALOG_SendByte(REG_Address);              
     I2C_ANALOG_Start();                          
     I2C_ANALOG_SendByte(SlaveAddress+1);         
     REG_data = I2C_ANALOG_RecvByte();            
     I2C_ANALOG_SendACK();  
     I2C_ANALOG_Stop();                           
  	return REG_data;
}


uint8_t I2C_ANALOG_WriteMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) 
{

	#if 1
	while(count--) 
	{ 
		I2C_ANALOG_WriteData(address,reg,*data); 
		reg++; 
		data++; 
	}


	#else
    uint16_t i;
	
    if (I2C_ANALOG_Start()!=I2C_ANALOG_BUS_READY)
    {
        return I2C_ANALOG_GPIO_DeInit(I2C_ANALOG_GPIO_SDA,I2C_ANALOG_GPIO_SCL);
    }
    I2C_ANALOG_SendByte(address);
    if (!I2C_ANALOG_WaitACK())
    {
        I2C_ANALOG_Stop();
        return I2C_ANALOG_BUS_ERROR;
    }
    
    I2C_ANALOG_SendByte(reg);
    I2C_ANALOG_WaitACK();
    
    for (i = 0; i < count; i++) 
    {
        I2C_ANALOG_SendByte(data[i]);
        if (!I2C_ANALOG_WaitACK()) 
		{
            I2C_ANALOG_Stop();
            return I2C_ANALOG_GPIO_DeInit(I2C_ANALOG_GPIO_SDA,I2C_ANALOG_GPIO_SCL);
        }
    }
    
    I2C_ANALOG_Stop();
	#endif
    
    return I2C_ANALOG_BUS_READY; 
}

uint8_t I2C_ANALOG_ReadMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) 
{
	#if 1
	while(count) 
	{
		*data++=I2C_ANALOG_ReadData(address,reg++); 
		count--; 
	}
	
	#else
    if (I2C_ANALOG_Start()!=I2C_ANALOG_BUS_READY)
    {
        return I2C_ANALOG_GPIO_DeInit(I2C_ANALOG_GPIO_SDA,I2C_ANALOG_GPIO_SCL);
    }	
	
    I2C_ANALOG_SendByte(address);
    if (!I2C_ANALOG_WaitACK())
    {
        I2C_ANALOG_Stop();
        return I2C_ANALOG_GPIO_DeInit(I2C_ANALOG_GPIO_SDA,I2C_ANALOG_GPIO_SCL);
    }
    I2C_ANALOG_SendByte(reg);
    I2C_ANALOG_WaitACK();
    I2C_ANALOG_Start();
    I2C_ANALOG_SendByte(address+1);
    I2C_ANALOG_WaitACK();
    while (count) 
    {
        *data = I2C_ANALOG_RecvByte();
        if (count == 1)
        {
            I2C_ANALOG_SendNACK();
        }
        else
        {
            I2C_ANALOG_SendACK();
        }
        data++;
        count--;
    }
    I2C_ANALOG_Stop();
    #endif
	
    return I2C_ANALOG_BUS_READY;
}

