#if defined(__ECOS)

#include "axp-cfg.h"
#include "Delay.h"
#include "i2c.h"	//#include "I2C_driver.h"	//平台i2C读写函数头文件

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          AxpCfg
#define __DBGLVL__          2 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


static POWERIC_INFO g_PowerICInfo;

//PowerIC AXP216 Initial Module
/**
  Initialize PowerIC IIC bus

  @param void
  @return void
*/
//extern BOOL SysInit_GetMTPW(void);
BOOL PowerIC_I2C_Init(POWERIC_INFO PowerICInfo)
{
    UINT erReturn;
    g_PowerICInfo.I2C_RegBytes = PowerICInfo.I2C_RegBytes;
    g_PowerICInfo.I2C_PinMux = PowerICInfo.I2C_PinMux;
    g_PowerICInfo.I2C_BusClock = PowerICInfo.I2C_BusClock;
    g_PowerICInfo.I2C_Slave_WAddr = PowerICInfo.I2C_Slave_WAddr;
    g_PowerICInfo.I2C_Slave_RAddr = PowerICInfo.I2C_Slave_RAddr;

	if(1)//(SysInit_GetMTPW())
	{
	    if (g_PowerICInfo.I2C_RegBytes > 2)
	    {
	        DBG_ERR("Error register Byte Count !!\r\n");
	        return FALSE;
	    }

	    if (g_PowerICInfo.I2C_PinMux > I2C_PINMUX_2ND)
	    {
	        DBG_ERR("Error I2C Pin mux !!\r\n");
	        return FALSE;
	    }

	    if (g_PowerICInfo.I2C_BusClock > I2C_BUS_CLOCK_1MHZ)
	    {
	        DBG_ERR("Error I2C Bus Clock !!\r\n");
	        return FALSE;
	    }

	    erReturn = i2c3_open(&(g_PowerICInfo.I2C_Channel));
	    if(erReturn != I2C_STS_OK)
	    {
	        DBG_ERR("Error open I2C driver!!\r\n");
	        return FALSE;
	    }
		#if 0
	    i2c_setConfig(g_PowerICInfo.I2C_Channel, I2C_CONFIG_ID_MODE,        I2C_MODE_MASTER);
	    i2c_setConfig(g_PowerICInfo.I2C_Channel, I2C_CONFIG_ID_PINMUX, g_PowerICInfo.I2C_PinMux); //select to control I2C 1st device
	    i2c_setConfig(g_PowerICInfo.I2C_Channel, I2C_CONFIG_ID_BUSCLOCK, g_PowerICInfo.I2C_BusClock); //up to 400KHZ
		#else
		i2c3_setConfig(g_PowerICInfo.I2C_Channel, I2C_CONFIG_ID_MODE,        I2C_MODE_MASTER);
		i2c3_setConfig(g_PowerICInfo.I2C_Channel, I2C_CONFIG_ID_BUSCLOCK,    I2C_BUS_CLOCK_200KHZ);
		//i2c3_setConfig(g_PowerICInfo.I2C_Channel, I2C_CONFIG_ID_PINMUX,      I2C_PINMUX_1ST);
		#endif
	    return TRUE;
	}
	else
		return FALSE;
}

static void PowerIC_I2C_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
	I2C_DATA i2cData;
	I2C_BYTE i2cByte[3];

	i2cData.VersionInfo = DRV_VER_96660;
	i2cData.pByte       = i2cByte;
	i2cData.ByteCount   = 3;
	i2cByte[0].uiValue  = g_PowerICInfo.I2C_Slave_WAddr;    // slave address
	i2cByte[0].Param    = I2C_BYTE_PARAM_START;
	i2cByte[1].uiValue  = uiAddr;
	i2cByte[1].Param    = I2C_BYTE_PARAM_NONE;
	i2cByte[2].uiValue  = uiValue;
	i2cByte[2].Param    = I2C_BYTE_PARAM_STOP;

	// Lock I2C
	i2c3_lock(g_PowerICInfo.I2C_Channel);

	if (i2c3_transmit(&i2cData) != I2C_STS_OK) {
		DBG_ERR("i2c_transmit err\r\n");
	}

	// Unlock I2C
	i2c3_unlock(g_PowerICInfo.I2C_Channel);
}


static UINT32 PowerIC_I2C_ReadReg(UINT32 uiReg)
{
	I2C_DATA i2cData;
	I2C_BYTE i2cByte[3];
	I2C_STS ret;

	static UINT32 ulData;

	i2cData.VersionInfo = DRV_VER_96660;
	i2cData.pByte       = i2cByte;
	i2cData.ByteCount   = 3;
	i2cByte[0].uiValue  = g_PowerICInfo.I2C_Slave_WAddr;    // slave address
	i2cByte[0].Param    = I2C_BYTE_PARAM_START;
	i2cByte[1].uiValue  = uiReg;
	i2cByte[1].Param    = I2C_BYTE_PARAM_NONE;
	i2cByte[2].uiValue  = g_PowerICInfo.I2C_Slave_RAddr;//AXP_DEVICES_ADDR | 0x01;    // slave address with READ
	i2cByte[2].Param    = I2C_BYTE_PARAM_START;

	// Lock I2C
	i2c3_lock(g_PowerICInfo.I2C_Channel);

	do {
		if (i2c3_transmit(&i2cData) != I2C_STS_OK) {
			break;
		}

		i2cData.ByteCount = 1;
		i2cByte[0].Param = I2C_BYTE_PARAM_STOP | I2C_BYTE_PARAM_NACK;
		ret = i2c3_receive(&i2cData);
		if (ret != I2C_STS_OK) {
			DBG_ERR("i2c_receive err\r\n");
		}

	} while (0);

	// Unlock I2C
	i2c3_unlock(g_PowerICInfo.I2C_Channel);

	ulData = i2cByte[0].uiValue;
		return ulData;
}
#if 0
void PowerIC_I2C_Exit(void)
{
    i2c_close(g_PowerICInfo.I2C_Channel);
}

void PowerIC_I2C_Lock(void)
{
    i2c_lock(g_PowerICInfo.I2C_Channel);
}

void PowerIC_I2C_Unlock(void)
{
    i2c_unlock(g_PowerICInfo.I2C_Channel);
}
#endif
/*AXP216 读函数配置*/

uint8_t AXP216_read( uint8_t readAddr, uint8_t byteNum , uint8_t *readData)
		{
		uint8_t i;
		if(byteNum<=0)
			return 0;

			/*此处，填写用户定义的I2C读函数*/
 			//I2C_Read(AXP_DEVICES_ADDR,readData, readAddr, byteNum);

			for(i=0;i<byteNum;i++)
			{
				readData[i] = PowerIC_I2C_ReadReg(readAddr);
				//debug_msg("AXP216_read Addr=0x%x  : 0x%x,%d\r\n",readAddr,readData[i],i);
				readAddr++;
			}
 	   return byteNum;
       }



/*AXP216 写函数配置	*/

uint8_t AXP216_write(const uint8_t writeAddr, const uint8_t writeData)
	{



	    /*此处，填写用户定义的I2C 写函数*/
  		PowerIC_I2C_WriteReg(writeAddr,writeData);//I2C_Write(AXP_DEVICES_ADDR,writeAddr,writeData);

	return 1;
	}

#if 0
void delay_ms(uint16_t nms)
	{	    /*此处，填写用户定义的mS 延时函数*/

	 Delay_DelayMs(nms);//STM32_delay_ms(nms)  ;

	}
#endif



void axp_set_bits(int reg, uint8_t bit_mask)
{
	uint8_t reg_val;

	AXP216_read( reg,1, &reg_val);

	if ((reg_val & bit_mask) != bit_mask) {
		reg_val |= bit_mask;
		AXP216_write(reg, reg_val)	 ;

   		}

}



void  axp_clr_bits(int reg, uint8_t bit_mask)
{
	 uint8_t reg_val;

     AXP216_read( reg,1, &reg_val);

	if (reg_val & bit_mask) {
		reg_val &= ~bit_mask;
		AXP216_write(reg, reg_val)	 ;
    	}

}



void  axp_update(int reg, uint8_t val, uint8_t mask)
{

	uint8_t reg_val;

	AXP216_read(reg, 1, &reg_val);


	if ((reg_val & mask) != val) {
		reg_val = (reg_val & ~mask) | val;
    	AXP216_write(reg, reg_val);
	 }
}

#endif
