#include "GSensor.h"
#include "GSensor_LIS3DH.h"
#include "SwTimer.h"
#include "Debug.h"
#include <stdlib.h>

#define __MODULE__          GSensor_LIS3DH
#define __DBGLVL__          1      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    //*=All, [mark]=CustomClass
#include "DebugModule.h"

//static INT32 GsXData = 0,OldGsXData = 0;
//static INT32 GsYData = 0,OldGsYData = 0;
//static INT32 GsZData = 0,OldGsZData = 0;
static BOOL  g_bGsensorOpened = FALSE;
//static UINT32 DRAM07Threshold=0xFF;/*1G= X/16;*/
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;

#if 0
static INT32 GSensor_LIS3DH_GetAxisMsg(UINT32 uiInt)
{
#if 1
//  UINT32 buf[2];
//  INT32 mgStep;
//  BOOL  bzNegative=FALSE;
/*
    if (uiInt>=0x80)
    {
      // negative number
      uiInt = ~uiInt;
      uiInt++;
      bzNegative = TRUE;
    }
*/
    return ((INT8)(uiInt));

#else
    UINT32 Data  = 0;

    if (uiInt >=128) //0x3F
    {
        //negative number
        Data =(uiInt-128);
        //debug_msg("DMARD07_GetAxisMg1  = %d\r\n",Data);

    }
    else
    {
        Data = uiInt;
        //debug_msg("DMARD07_GetAxisMg2  = %d\r\n",Data);
    }

    return Data;//(bzNegative)?(-1*Data):(Data);
#endif
}
#endif

#if 0
static UINT32 GSensor_LIS3DH_GetSensitivityLevel(void)
{
    switch(g_GSensorSensitivity)
    {
    case GSENSOR_SENSITIVITY_OFF:
        DRAM07Threshold =0xFF;
        break;
    case GSENSOR_SENSITIVITY_LOW:
        DRAM07Threshold =0x25;//25
        break;
    case GSENSOR_SENSITIVITY_MED:
        DRAM07Threshold =0x15;//20
        break;
    case GSENSOR_SENSITIVITY_HIGH:
        DRAM07Threshold =0x10;//16
        break;
    default:
        DRAM07Threshold =0xFF;
        break;
    }
    return DRAM07Threshold;
}
#endif

static void GSensor_LIS3DH_OpenInterface(void)
{

}

static void GSensor_LIS3DH_CloseInterface(void)
{

}

static BOOL GSensor_Init(void)
{
    GSENSOR_INFO GsensorInfo;

    GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
    GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
    GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    // DMARD07 GSensor I2C slave addresses
    GsensorInfo.I2C_Slave_WAddr = 0x32;
    GsensorInfo.I2C_Slave_RAddr = 0x33;
    if (GSensor_I2C_Init(GsensorInfo) == TRUE)
    {
        //DARMD07 init register setting
        GSensor_I2C_WriteReg(0x20,0x55);//Normal Mode, 342Hz,XYZ Int enable

        debug_msg("%s: devID:%x\r\n",__func__, GSensor_I2C_ReadReg(0x0F));
        debug_msg("%s: reg 0x20:%x\r\n",__func__,GSensor_I2C_ReadReg(0x20));
        return TRUE;
    } else {
        debug_msg("G Sensor Init failed !!\r\n");
        return FALSE;
    }
}

BOOL GSensor_open(void)
{
   if (g_bGsensorOpened == TRUE)
   {
       debug_msg("Gsensor open already\r\n");
       return TRUE;
   }

   g_bGsensorOpened = TRUE;

   // open Gsensor interface
   GSensor_LIS3DH_OpenInterface();

   // Gsensor init
   GSensor_Init();

   return TRUE;
}

BOOL GSensor_close(void)
{

    if (g_bGsensorOpened==FALSE)
    {
        debug_msg("I2C close already\r\n");
        return TRUE;
    }

    // close Gsensor interface
    GSensor_LIS3DH_CloseInterface();

    g_bGsensorOpened = FALSE;

    return TRUE;
}

BOOL GSensor_GetStatus(Gsensor_Data *GS_Data)
{
    debug_msg("LIS3DH GSensor_GetStatus..........\n\r");
	return FALSE;
}

BOOL GSensor_ParkingMode(void)
{
     return 0;
}

BOOL GSensor_CrashMode(void)
{
    return FALSE;
}

void GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
    g_GSensorSensitivity = GSensorSensitivity;
}

void GSensor_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
{
}
