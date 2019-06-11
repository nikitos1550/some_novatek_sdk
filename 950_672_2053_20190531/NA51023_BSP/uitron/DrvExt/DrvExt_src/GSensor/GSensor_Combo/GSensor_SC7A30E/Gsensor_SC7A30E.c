#include "GSensor.h"
#include "GSensor_Combo.h"
#include "Gsensor_SC7A30E.h"
#include "SwTimer.h"
#include <stdlib.h>
#include "ErrorNo.h"
#include "Delay.h"

#define __MODULE__          GSensor_SC7A30E
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

//********************************************************************************
//
//GSENSOR SC7A30E
//
//********************************************************************************
static INT32 GsXData = 0;
static INT32 GsYData = 0;
static INT32 GsZData = 0;
static BOOL  g_bGsensorOpened = FALSE;
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;
static GSENSOR_SENSITIVITY g_GSensorSensitivityParking = GSENSOR_SENSITIVITY_PARKING_OFF;

static GSENSOR_OBJ g_GsensorSC7A30EObj =
	{GSensor_SC7A30E_open,
	GSensor_SC7A30E_close,
	GSensor_SC7A30E_GetStatus,
	GSensor_SC7A30E_ParkingMode,
	GSensor_SC7A30E_CrashMode,
	GSensor_SC7A30E_SetSensitivity,
	GSensor_SC7A30E_GetAxisValue,
	SC7A30E_ClearIntSts
	};
 void SC7A30E_ClearIntSts(void)
{
	// Since we didnot use interrupt mode here.
	return;
}

static UINT32 GSensor_SC7A30E_GetSensitivityLevel(void);
 void GSensor_SC7A30E_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
{
	UINT32 g_X_Data_L = 0, g_X_Data_H = 0, g_Y_Data_L = 0, g_Y_Data_H = 0, g_Z_Data_L = 0, g_Z_Data_H = 0;

	if (g_bGsensorOpened == FALSE)
		return;

    g_X_Data_L = GSensor_I2C_ReadReg(OUT_X_L_REG);
    g_Y_Data_L = GSensor_I2C_ReadReg(OUT_Y_L_REG);
    g_Z_Data_L = GSensor_I2C_ReadReg(OUT_Z_L_REG);

    g_X_Data_H = GSensor_I2C_ReadReg(OUT_X_H_REG);
    g_Y_Data_H = GSensor_I2C_ReadReg(OUT_Y_H_REG);
    g_Z_Data_H = GSensor_I2C_ReadReg(OUT_Z_H_REG);

	GsXData = ((g_X_Data_L & 0xF0) >> 4) | ((g_X_Data_H & 0xFF) << 4);
	if (GsXData > 0x7FF)
		GsXData = (-1)*(0xFFF - GsXData);
	//GsXData = (GsXData*98*2)/(2048*10);

	GsYData = ((g_Y_Data_L & 0xF0) >> 4) | ((g_Y_Data_H & 0xFF) << 4);
	if (GsYData > 0x7FF)
		GsYData = (-1)*(0xFFF - GsYData);
	//GsYData = (GsYData*98*2)/(2048*10);

	GsZData = ((g_Z_Data_L & 0xF0) >> 4) | ((g_Z_Data_H & 0xFF) << 4);
	if (GsZData > 0x7FF)
		GsZData = (-1)*(0xFFF - GsZData);
	//GsZData = (GsZData*98*2)/(2048*10);

	*pX = GsXData;
	*pY = GsYData;
	*pZ = GsZData;
}

BOOL GSensor_SC7A30E_CrashMode(void)
{
    return FALSE;
}

void GSensor_SC7A30E_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{

if(GSensorSensitivity<GSENSOR_SENSITIVITY_PARKING_BEGIN)
	{
	g_GSensorSensitivity = GSensorSensitivity;

	}
	else
	{
	g_GSensorSensitivityParking=GSensorSensitivity;
	}
	GSensor_SC7A30E_GetSensitivityLevel();
}

//**********************************************************************************
//
//
//**********************************************************************************
//static GSENSOR_INFO2 g_GsensorInfo;
static UINT32 SC7A30EThreshold=0xFF;/*1G= X/16;*/

INT32 GSensor_SC7A30E_GetAxisMsg(UINT32 uiInt)
{
    INT32 Data  = 0;

    if (uiInt >511) //0x3F
    {
        //negative number
        Data =-(uiInt-511);
        DBG_MSG("GSensor_SC7A30E_GetAxisMsg1  = %d\r\n",Data);

    }
    else
    {
        Data = uiInt;
        DBG_MSG("GSensor_SC7A30E_GetAxisMsg2  = %d\r\n",Data);
    }

    return Data;//(bzNegative)?(-1*Data):(Data);

}

static UINT32 GSensor_SC7A30E_GetSensitivityLevel(void)
{
    switch(g_GSensorSensitivity)
    {
        case GSENSOR_SENSITIVITY_OFF:
            SC7A30EThreshold =0xFF;
            break;
        case GSENSOR_SENSITIVITY_LOW:
            SC7A30EThreshold =0x45;//25
            break;
        case GSENSOR_SENSITIVITY_MED:
            SC7A30EThreshold =0x35;//20
            break;
        case GSENSOR_SENSITIVITY_HIGH:
            SC7A30EThreshold =0x25;//16
            break;
        default:
            SC7A30EThreshold =0xFF;
            break;
    }
    return SC7A30EThreshold;
}

static void GSensor_SC7A30E_OpenInterface(void)
{

}

static void GSensor_SC7A30E_CloseInterface(void)
{

}

PGSENSOR_OBJ GSensor_SC7A30E_getGSensorObj(void)
{
    return &g_GsensorSC7A30EObj;
}
static BOOL GSensor_Init(void)
{
	GSENSOR_INFO GsensorInfo;

	GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
	GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
	GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_200KHZ;//I2C_BUS_CLOCK_400KHZ;
	// SC7A20 GSensor I2C slave addresses
	GsensorInfo.I2C_Slave_WAddr = SC7A30E_WRITE_ID;
	GsensorInfo.I2C_Slave_RAddr = SC7A30E_READ_ID;
    //UINT32 Register ;

	if (GSensor_I2C_Init(GsensorInfo) == TRUE) {
#if 0	//only for test
		UINT32 retrycount;
		for(retrycount = 0; retrycount<2; retrycount++)
		{
			CHKPNT;
			GSensor_I2C_WriteReg(SW_RST_REG, SW_RST);						/// Soft reset
			Delay_DelayMs(50);
			//init
			if(GSensor_I2C_ReadReg(CHIP_ID_REG) != CHIP_ID) {				/// Check Chip ID
				DBG_ERR("[GSensor] retry count = %d\r\n",retrycount);
			}
			else
				break;
		}
		//init
		if(GSensor_I2C_ReadReg(CHIP_ID_REG) != CHIP_ID) {				/// Check Chip ID
			DBG_ERR("[GSensor] SC7A20 checked failed!!\r\n");
			GSensor_I2C_Exit();											/// If not matched, close I2C bus.
			return FALSE;
		}
#else
		//init
		if(GSensor_I2C_ReadReg(CHIP_ID_REG) != SC7A30E_DRV_ID) {				/// Check Chip ID
			DBG_ERR("[GSensor] SC7A20 checked failed!!CHIP_ID=0x%x \r\n", GSensor_I2C_ReadReg(CHIP_ID_REG));
			GSensor_I2C_Exit();											/// If not matched, close I2C bus.
			return FALSE;
		}
#endif
		// Init GSensor IC here

		//GSensor_I2C_WriteReg(0x39,0x00);			//使能一个或多个中断生成
		DBG_ERR(" ============SC7A30E_DEVICE IRT = OK=========\r\n");
        /*
        for(registers = 0;registers < 0X50;registers++)
        {
           debug_msg("register = 0x%02x   value = 0x%02x++++++++++++++\r\n",register, GSensor_I2C_ReadReg(register));

        }
        */
		//DBG_ERR("value = 0x%02X\r\n",value);
		GSensor_I2C_WriteReg(0x24,0x80);			//reboot
		GSensor_I2C_WriteReg(0x22,0x00);			//关闭中断
		GSensor_I2C_WriteReg(0x21,0x00);
		// GSensor_I2C_WriteReg(0x39,&value);		//使能一个或多个中断生成
		//debug_err((" ============SC7A30E_DEVICE IRT222 ==========\r\n",value));
		//debug_err(("value = 0x%02X\r\n",value));

		GSensor_I2C_WriteReg(0x20,0x37);			//设置时钟
		GSensor_I2C_WriteReg(0x23,0x90);			//当MSB和LSB被读是输出寄存器更新


		GSensor_I2C_WriteReg(0x25,0x00);			//中断低电平有效
		GSensor_I2C_WriteReg(0x21,0x0d);			//开通高通滤波，控制中断检测
		//GSensor_I2C_WriteReg(0x21,0x2d);			//开通高通滤波，控制中断检测

		GSensor_I2C_WriteReg(0x3a,0x20);			//XX为中断阈值(建议值 5,20,40)
													//范围0x01-0x7e
													//16mg/LSB
													//当值为0x7f时，永不触发
		GSensor_I2C_WriteReg(0x3b,0x7f);
		GSensor_I2C_WriteReg(0x3c,0x40);			//中断状态位保持时间(40ms/LSB)
		//GSensor_I2C_WriteReg(0x3c,0x10);			//中断状态位保持时间(40ms/LSB)
		GSensor_I2C_WriteReg(0x38,0x15);			//开启XYZ三轴中断
		GSensor_I2C_WriteReg(0x22,0x00);			//使能关闭中断

		GSensor_I2C_WriteReg(0x3b,0x7f);
		GSensor_I2C_WriteReg(0x3c,0x40);			//中断状态位保持时间(40ms/LSB)
		//MC3232_I2CWrByte(0x3c,0x10);			    //中断状态位保持时间(40ms/LSB)
		GSensor_I2C_WriteReg(0x38,0x00);			//开启XYZ三轴中断
		GSensor_I2C_WriteReg(0x22,0x00);			//使能中断

        /*
		 debug_msg("0x39 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x39));
		 debug_msg("0x22 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x22));
		 debug_msg("0x21 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x21));
		 debug_msg("0x24 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x24));
		 debug_msg("0x20 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x20));
		 debug_msg("0x23 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x23));
		 debug_msg("0x25 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x25));
		 debug_msg("0x21 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x21));
		 debug_msg("0x3a = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x3a));
		 debug_msg("0x3b = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x3b));
		 debug_msg("0x3c = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x3c));
		 debug_msg("0x38 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x38));
		 debug_msg("0x32 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x32));
		*/

		return TRUE;


	} else {
		DBG_ERR("GSensor init failed!!\r\n");
		return FALSE;
	}
}

//for  Gensor POWER UP
void GSensor_SC7A30E_PowerUp(INT32 sen)
{

}


BOOL GSensor_SC7A30E_open(void)
{
	//debug_msg("SC7A30E :　GSensor open!!!!!!!!!!!!!!\r\n");
   if (g_bGsensorOpened == TRUE)
   {
       DBG_ERR("GSensor open already\r\n");
       return TRUE;
   }

   g_bGsensorOpened = TRUE;
   // open Gsensor interface
   GSensor_SC7A30E_OpenInterface();

   // Gsensor init
   if(GSensor_Init()) {
	   g_bGsensorOpened = TRUE;
	   DBG_DUMP("[GSensor] SC7A20 is opened OK!!\r\n");
	   GSensor_SC7A30E_CrashMode();
	   return TRUE;
   }

   return TRUE;
}

BOOL GSensor_SC7A30E_close(void)
{
	 DBG_ERR("SC7A30E :　GSensor close!!!!!!!!!!!!!!\r\n");
    if (g_bGsensorOpened==FALSE)
    {
        DBG_ERR("I2C close already\r\n");
        return TRUE;
    }

    // close Gsensor interface
    GSensor_SC7A30E_CloseInterface();

	GSensor_I2C_Exit();
    g_bGsensorOpened = FALSE;

    return TRUE;
}


BOOL GSensor_SC7A30E_GetStatus(Gsensor_Data *GS_Data)
{
	UINT32 GsensorDat=0;
	INT32 x=0,y=0,z=0,level=0;

	level = SC7A30EThreshold;

	GsensorDat = GSensor_I2C_ReadReg(0x39);//使能一个或多个中断生成

	//DBG_ERR("GsensorDat39 = 0x%02X\r\n",GsensorDat);

	x = GSensor_I2C_ReadReg(0x29);
	if (x>= 0x80)
	x= x- 0xFF;
	//DBG_ERR("GsensorDat29 = 0x%02X  0x%02X\r\n",abs(x),x);

	y = GSensor_I2C_ReadReg(0x2b);//使能一个或多个中断生成
	if (y>= 0x80)
	y= y- 0xFF;
	//debug_msg("GsensorDat2b = 0x%02X\r\n",level);
	//debug_msg("GsensorDatXX = 0x%02XGsensorDatYY = 0x%02XGsensorDatZZ = 0x%02X\r\n",abs(x),abs(y),abs(z));
	//DBG_ERR("GsensorDat2b = 0x%02X  0x%02X\r\n",abs(y),y);

	z = GSensor_I2C_ReadReg(0x2d);
	if (z>= 0x80)
	z= z- 0xFF;

	//debug_msg("level = 0x%02XGsensorDatXX = 0x%02XGsensorDatYY = 0x%02XGsensorDatZZ = 0x%02X\r\n",level,abs(x),abs(y),abs(z));
	//DBG_ERR("GsensorDat2d = 0x%02X  0x%02X\r\n",abs(z),z);
	//DBG_ERR("GsensorDat = 0x%02X\r\n",GsensorDat);
	//DBG_ERR("level = 0x%02X\r\n",level);
	if(abs(x)>level || abs(y)>level || abs(z)>level)
	{
		DBG_ERR(" ============ 30E Interrupt !!!!!!!===========\r\n");
		return TRUE;
	}

	return FALSE;
}

BOOL GSensor_SC7A30E_ParkingMode(void)
{
	if (g_bGsensorOpened == FALSE)
		return FALSE;

	//GSensor_I2C_WriteReg(0x24,0x80);//启动内存的内容
	//GSensor_I2C_WriteReg(0x20,0x37);//设置时钟
	//GSensor_I2C_WriteReg(0x23,0x90);//当MSB和LSB被读是输出寄存器更新


	GSensor_I2C_WriteReg(0x25,0x00);			//中断di电平有效
	//GSensor_I2C_WriteReg(0x21,0x0d);


if(g_GSensorSensitivityParking == GSENSOR_SENSITIVITY_PARKING_OFF)
			GSensor_I2C_WriteReg(0x3a,0x7f);	//XX为中断阈值(建议值 5,20,40)
												//范围0x01-0x7e
												//16mg/LSB
												//当值为0x7f时，永不触发
if(g_GSensorSensitivityParking == GSENSOR_SENSITIVITY_PARKING_LOW)
			GSensor_I2C_WriteReg(0x3a,0x30);
if(g_GSensorSensitivityParking == GSENSOR_SENSITIVITY_PARKING_MED)
			GSensor_I2C_WriteReg(0x3a,0x20);
if(g_GSensorSensitivityParking == GSENSOR_SENSITIVITY_PARKING_HIGH)
			GSensor_I2C_WriteReg(0x3a,0x15);
	//debug_msg("0x3a = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x3a));

//CHKPNT;

	//GSensor_I2C_WriteReg(0x39,0x00);			//使能一个或多个中断生成
	GSensor_I2C_WriteReg(0x3b,0x7f);
	GSensor_I2C_WriteReg(0x3c,0x10);		//中断状态位保持时间(40ms/LSB)
	GSensor_I2C_WriteReg(0x38,0x15);			//开启XYZ三轴中断
	GSensor_I2C_WriteReg(0x22,0x80);			//使能中断
	//debug_msg("0x25 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x25));
	//debug_msg("0x22 = 0x%02x   ++++++++++++++\r\n", GSensor_I2C_ReadReg(0x22));


 //   DBG_MSG("----------SC7A30E GSensor_ParkingMode----------\n\r");
    return 1;
}

