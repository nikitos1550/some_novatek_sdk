#include "GSensor.h"
#include "GSensor_KXTJ2.h"
#include "SwTimer.h"
#include "Debug.h"
#include <stdlib.h>
#include "ErrorNo.h"

#define __MODULE__          GSensor_KXTJ2
#define __DBGLVL__          1      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    //*=All, [mark]=CustomClass
#include "DebugModule.h"

//********************************************************************************
//
//GSENSOR KXTJ2
//
//********************************************************************************
static INT32 GsXData = 0,OldGsXData = 0;
static INT32 GsYData = 0,OldGsYData = 0;
static INT32 GsZData = 0,OldGsZData = 0;
static BOOL  g_bGsensorOpened = FALSE;
static UINT32 KXTJ2Threshold=0xFF;/*1G= X/16;*/
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;

static INT32 GSensor_KXTJ2_GetAxisMsg(UINT32 uiInt)
{
    return ((INT8)(uiInt));
}

static UINT32 GSensor_KXTJ2_GetSensitivityLevel(void)
{
    switch(g_GSensorSensitivity)
    {
        case GSENSOR_SENSITIVITY_OFF:
            KXTJ2Threshold =0xFF;
            break;
        case GSENSOR_SENSITIVITY_LOW:
            KXTJ2Threshold =0x25;//25
            break;
        case GSENSOR_SENSITIVITY_MED:
            KXTJ2Threshold =0x15;//20
            break;
        case GSENSOR_SENSITIVITY_HIGH:
            KXTJ2Threshold =0x10;//16
            break;
        default:
            KXTJ2Threshold =0xFF;
            break;
    }
    return KXTJ2Threshold;
}

static void GSensor_OpenInterface(void)
{

}

static void GSensor_CloseInterface(void)
{

}

static BOOL GSensor_Init(void)
{
    GSENSOR_INFO GsensorInfo;

    GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
    GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
    GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    // KXTJ2 GSensor I2C slave addresses
    GsensorInfo.I2C_Slave_WAddr = 0x1C;
    GsensorInfo.I2C_Slave_RAddr = 0x1D;
    if (GSensor_I2C_Init(GsensorInfo) == TRUE)
    {
        //KXTJ2 init register setting
        GSensor_I2C_WriteReg(0x1B,0x00);
        GSensor_I2C_WriteReg(0x1B,0x40);
        GSensor_I2C_WriteReg(0x21,0x02);
        GSensor_I2C_WriteReg(0x1B,0xC0);
        return TRUE;
    }
    else
    {
        DBG_MSG("GSensor init failed!!\r\n");
        return FALSE;
    }
}

BOOL GSensor_open(void)
{
   DBG_ERR("KXTJ2\r\n");

   if (g_bGsensorOpened == TRUE)
   {
       DBG_MSG("GSensor open already\r\n");
       return TRUE;
   }

   g_bGsensorOpened = TRUE;

   // open Gsensor interface
   GSensor_OpenInterface();

   // Gsensor init
   GSensor_Init();

   return TRUE;
}

BOOL GSensor_close(void)
{
    if (g_bGsensorOpened==FALSE)
    {
        DBG_MSG("GSensor close already\r\n");
        return TRUE;
    }

    // close Gsensor interface
    GSensor_CloseInterface();

    GSensor_I2C_Exit();

    g_bGsensorOpened = FALSE;

    return TRUE;
}


BOOL GSensor_GetStatus(Gsensor_Data *GS_Data)
{
    UINT8  abs_cal_x=0,abs_cal_y=0,abs_cal_z=0;
    UINT8 G_INT_COUNT=0;
    UINT8 KXTJ2Threshold;
    UINT32 g_X_Data_L=0,g_X_Data_H=0,g_Y_Data_L=0,g_Y_Data_H=0,g_Z_Data_L=0,g_Z_Data_H=0;

    if(g_bGsensorOpened==FALSE)
    {
        return FALSE;
    }
    if(g_GSensorSensitivity == GSENSOR_SENSITIVITY_OFF)
    {
        return FALSE;
    }

    KXTJ2Threshold = GSensor_KXTJ2_GetSensitivityLevel();

    g_X_Data_L=GSensor_I2C_ReadReg(OUT_X_L_REG);
    g_Y_Data_L=GSensor_I2C_ReadReg(OUT_Y_L_REG);
    g_Z_Data_L=GSensor_I2C_ReadReg(OUT_Z_L_REG);

    g_X_Data_H=GSensor_I2C_ReadReg(OUT_X_H_REG);
    g_Y_Data_H=GSensor_I2C_ReadReg(OUT_Y_H_REG);
    g_Z_Data_H=GSensor_I2C_ReadReg(OUT_Z_H_REG);

    if(((g_X_Data_H&0x80)>>7)==0x01)
        GsXData =-(2046-((((g_X_Data_L&0xF0)>>4) +((g_X_Data_H&0x7F)<<4))));
    else
        GsXData =(((g_X_Data_L&0xF0)>>4) +((g_X_Data_H&0x7F)<<4));

    if(((g_Y_Data_H&0x80)>>7)==0x01)
        GsYData =-(2046-(((g_Y_Data_L&0xF0)>>4) +((g_Y_Data_H&0x7F)<<4)));
    else
        GsYData =(((g_Y_Data_L&0xF0)>>4) +((g_Y_Data_H&0x7F)<<4));

      if(((g_Z_Data_H&0x80)>>7)==0x01)
        GsZData =-(2046-(((g_Z_Data_L&0xF0)>>4) +((g_Z_Data_H&0x7F)<<4)));
    else
        GsZData =(((g_Z_Data_L&0xF0)>>4) +((g_Z_Data_H&0x7F)<<4));

    //DBG_ERR("GsXData=0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n\r",g_X_Data_H,g_X_Data_L,g_Y_Data_H,g_Y_Data_L,g_Z_Data_H,g_Z_Data_L);

    //DBG_ERR("GsXData=%d, %d, %d\n\r",GsXData,GsYData,GsZData);


    abs_cal_x =abs(GsXData-OldGsXData);
    if(abs_cal_x>KXTJ2Threshold)   //30
        G_INT_COUNT++;

    abs_cal_y =abs(GsYData-OldGsYData);
    if(abs_cal_y>KXTJ2Threshold) //20
        G_INT_COUNT++;

    abs_cal_z =abs(GsZData-OldGsZData);
    if(abs_cal_z>KXTJ2Threshold) //20
        G_INT_COUNT++;

    OldGsXData = GsXData;
    OldGsYData = GsYData;
    OldGsZData = GsZData;

    //debug_msg("..........X = %d,Y = %d,Z = %d\r\r",GsXData,GsYData,GsZData);
    //debug_msg("..........DX = %d,DY = %d,DZ = %d th=%d \r\n",abs_cal_x,abs_cal_y,abs_cal_z,D07Threshold);

    if(G_INT_COUNT>=2)
    {
        DBG_MSG("----------KXTJ2_isr1 EMG----------\n\r");
        return TRUE;
    }
    else
       return FALSE;
}

BOOL GSensor_ParkingMode(void)
{
    UINT32  abs_cal_x=0,abs_cal_y=0,abs_cal_z=0;
    UINT32 P_INT_COUNT=0;
    UINT32 g_KXTJ2ParkenThreshold=0x07;

    GsXData = GSensor_KXTJ2_GetAxisMsg(GSensor_I2C_ReadReg(OUT_X_REG));
    GsYData = GSensor_KXTJ2_GetAxisMsg(GSensor_I2C_ReadReg(OUT_Y_REG));
    GsZData = GSensor_KXTJ2_GetAxisMsg(GSensor_I2C_ReadReg(OUT_Z_REG));

/*
      if(GsXData <(g_KXTJ2ParkenThreshold>>1))
      {
        GsXData  =OldGsXData;
      }
      if(GsYData <(g_KXTJ2ParkenThreshold>>1))
      {
        GsYData  =OldGsYData;
      }
      if(GsZData <(g_KXTJ2ParkenThreshold>>1))
      {
        GsZData  =OldGsZData;
      }
*/
    abs_cal_x=abs(GsXData-OldGsXData);
    if(abs_cal_x>g_KXTJ2ParkenThreshold)   //30
        P_INT_COUNT++;

    abs_cal_y =abs(GsYData-OldGsYData);
    if(abs_cal_y>g_KXTJ2ParkenThreshold) //20
        P_INT_COUNT++;

    abs_cal_z =abs(GsZData-OldGsZData);
    if(abs_cal_z>g_KXTJ2ParkenThreshold) //20
        P_INT_COUNT++;

    OldGsXData = GsXData;
    OldGsYData = GsYData;
    OldGsZData = GsZData;

    //DBG_MSG("..........X = %d,Y = %d,Z = %d\r\r",GsXData,GsYData,GsZData);
    //DBG_MSG("..........DX = %d,DY = %d,DZ = %d th=%d \r\n",abs_cal_x,abs_cal_y,abs_cal_z,D07Threshold);

    if(P_INT_COUNT>=1)
    {
        DBG_MSG("----------KXTJ2 GSensor_ParkingMode----------\n\r");
        return 1;
    }
    else
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

