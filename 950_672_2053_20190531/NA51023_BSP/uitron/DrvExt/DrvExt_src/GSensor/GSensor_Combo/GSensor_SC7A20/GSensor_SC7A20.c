#include "GSensor.h"
#include "GSensor_Combo.h"
#include "GSensor_SC7A20.h"
#include "SwTimer.h"
#include <stdlib.h>
#include "ErrorNo.h"
#include "Delay.h"

#define __MODULE__          GSensor_SC7A20
#define __DBGLVL__          1      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    //*=All, [mark]=CustomClass
#include "DebugModule.h"

static BOOL GSensor_SC7A20_open(void);
static BOOL GSensor_SC7A20_close(void);
static BOOL GSensor_SC7A20_GetStatus(Gsensor_Data *GS_Data);
static BOOL GSensor_SC7A20_ParkingMode(void);
static BOOL GSensor_SC7A20_CrashMode(void);
static void GSensor_SC7A20_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity);
static void GSensor_SC7A20_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ);
static void SC7A20_ClearIntSts(void);

static GSENSOR_OBJ g_GsensorSC7A20Obj = {
	GSensor_SC7A20_open,
	GSensor_SC7A20_close,
	GSensor_SC7A20_GetStatus,
	GSensor_SC7A20_ParkingMode,
	GSensor_SC7A20_CrashMode,
	GSensor_SC7A20_SetSensitivity,
	GSensor_SC7A20_GetAxisValue,
	SC7A20_ClearIntSts
};

PGSENSOR_OBJ GSensor_SC7A20_getGSensorObj(void)
{
    return &g_GsensorSC7A20Obj;
}

//********************************************************************************
//
//GSENSOR SC7A20
//
//********************************************************************************
static INT32 GsXData = 0,OldGsXData = 0;
static INT32 GsYData = 0,OldGsYData = 0;
static INT32 GsZData = 0,OldGsZData = 0;
static BOOL  g_bGsensorOpened = FALSE;
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;
static GSENSOR_SENSITIVITY g_GSensorSensitivityParking = GSENSOR_SENSITIVITY_PARKING_OFF;


//**********************************************************************************
//
//
//**********************************************************************************
//static GSENSOR_INFO2 g_GsensorInfo;
static UINT32 SC7A20Threshold=0xFFFFFFFF;

/*----------------------------------------------------
//20170525
H:600
M:400
L:300
----------------------------------------------------*/
static UINT32 GSensor_GetSensitivityLevel(void)
{
	switch(g_GSensorSensitivity) {
	case GSENSOR_SENSITIVITY_OFF:
		SC7A20Threshold = 0xFFFFFFFF;
		break;
	case GSENSOR_SENSITIVITY_LOW:
		SC7A20Threshold = 600;//70;
		break;
	case GSENSOR_SENSITIVITY_MED:
		SC7A20Threshold = 400;//50;
		break;
	case GSENSOR_SENSITIVITY_HIGH:
		SC7A20Threshold = 300;//20;
		break;
	default:
		SC7A20Threshold = 0xFFFFFFFF;
		break;
	}
	return SC7A20Threshold;
}

static void GSensor_OpenInterface(void)
{

}

static void GSensor_CloseInterface(void)
{
#if 0
	GSensor_I2C_WriteReg(0x22,0x00);
#endif
}

static BOOL GSensor_Init(void)
{
	GSENSOR_INFO GsensorInfo;

	GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
	GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
	GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_200KHZ;//I2C_BUS_CLOCK_400KHZ;
	// SC7A20 GSensor I2C slave addresses
	GsensorInfo.I2C_Slave_WAddr = SC7A20_I2C_WR_ADDR;
	GsensorInfo.I2C_Slave_RAddr = SC7A20_I2C_RD_ADDR;
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
		if(GSensor_I2C_ReadReg(CHIP_ID_REG) != CHIP_ID) {				/// Check Chip ID
			DBG_ERR("[GSensor] SC7A20 checked failed!!CHIP_ID=0x%x \r\n", GSensor_I2C_ReadReg(CHIP_ID_REG));
			GSensor_I2C_Exit();											/// If not matched, close I2C bus.
			return FALSE;
		}
#endif
		// Init GSensor IC here

#if 1
		INT32 temp1;
		temp1 = GSensor_I2C_ReadReg(0x57);
		if(!(temp1 & 0x0c))
		{
			GSensor_I2C_WriteReg(0x1e,0x05);
			temp1 |= 0x0c;
			GSensor_I2C_WriteReg(0x57,temp1);
			GSensor_I2C_WriteReg(0x1e,0x15);

			DBG_ERR("GSensor I2C had set!!\r\n");
		}
#endif
		GSensor_I2C_WriteReg(0x25,0x00);
		GSensor_I2C_WriteReg(0x22,0x00);
		GSensor_I2C_WriteReg(0x38,0x00);
		GSensor_I2C_WriteReg(0x24,0x80);

		GSensor_I2C_WriteReg(0x20,0x37);
		//GSensor_I2C_ReadReg(0x26);
		GSensor_I2C_WriteReg(0x21,0x0d);
		GSensor_I2C_WriteReg(0x23,0x80);

		return TRUE;
	} else {
		DBG_ERR("GSensor init failed!!\r\n");
		return FALSE;
	}
}

static BOOL GSensor_SC7A20_open(void)
{
	if (g_bGsensorOpened == TRUE) {
		DBG_ERR("GSensor open already\r\n");
		return TRUE;
	}
	// open Gsensor interface
	GSensor_OpenInterface();
	// Gsensor init
	if(GSensor_Init()) {
		g_bGsensorOpened = TRUE;
		DBG_DUMP("[GSensor] SC7A20 is opened OK!!\r\n");

		GSensor_SC7A20_CrashMode();
		return TRUE;
	}
	return FALSE;
}

static BOOL GSensor_SC7A20_close(void)
{
	if (g_bGsensorOpened == FALSE) {
		DBG_ERR("I2C close already\r\n");
		return TRUE;
	}

	// close Gsensor interface
	GSensor_CloseInterface();

	GSensor_I2C_Exit();

	g_bGsensorOpened = FALSE;

	return TRUE;
}

static BOOL GSensor_SC7A20_GetStatus(Gsensor_Data *GS_Data)
{
	UINT32  abs_cal_x = 0, abs_cal_y = 0, abs_cal_z = 0;
	UINT8 G_INT_COUNT = 0;
	UINT8 G_INT_COUNT02 = 0;
	UINT32 Threshold;
	UINT32 ShakeThreshold;
	UINT32 g_X_Data_L = 0, g_X_Data_H = 0, g_Y_Data_L = 0, g_Y_Data_H = 0, g_Z_Data_L = 0, g_Z_Data_H = 0;

	if (g_bGsensorOpened == FALSE)
		return FALSE;
	//if(g_GSensorSensitivity == GSENSOR_SENSITIVITY_OFF)
	//	return FALSE;

	Threshold = GSensor_GetSensitivityLevel();

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

	abs_cal_x = abs(GsXData-OldGsXData);
	if(abs_cal_x > Threshold)
		G_INT_COUNT++;

	abs_cal_y = abs(GsYData-OldGsYData);
	if(abs_cal_y > Threshold)
		G_INT_COUNT++;

	abs_cal_z = abs(GsZData-OldGsZData);
	if(abs_cal_z > Threshold)
		G_INT_COUNT++;


	//check shake state!!!
	{
		ShakeThreshold = 120;	//GSENSOR_SENSITIVITY_HIGH

		if(abs_cal_x > ShakeThreshold)
			G_INT_COUNT02++;
		if(abs_cal_y > ShakeThreshold)
			G_INT_COUNT02++;
		if(abs_cal_z > ShakeThreshold)
			G_INT_COUNT02++;

		if(G_INT_COUNT02 >= 2)	// 2
		{
			GS_Data->ShakeState =1;
		}
		else
		{
			GS_Data->ShakeState =0;
		}
	}

	OldGsXData = GsXData;
	OldGsYData = GsYData;
	OldGsZData = GsZData;

	DBG_IND("..........X %d, Y %d, Z %d\r\n", GsXData, GsYData, GsZData);
	DBG_IND("..........DX %d, DY %d, DZ %d, th %d\r\n", abs_cal_x, abs_cal_y, abs_cal_z, Threshold);

	if(g_GSensorSensitivity == GSENSOR_SENSITIVITY_OFF)
		return FALSE;

	if(G_INT_COUNT >= 2)
		return TRUE;
	else
		return FALSE;
}

static BOOL GSensor_SC7A20_ParkingMode(void)
{
	if (g_bGsensorOpened == FALSE)
		return FALSE;
	if(g_GSensorSensitivityParking == GSENSOR_SENSITIVITY_PARKING_OFF)
		return FALSE;

	if(g_GSensorSensitivityParking == GSENSOR_SENSITIVITY_PARKING_LOW)
	{
		DBG_DUMP("[GSensor] SC7A20 Enter parking mode. .LOW\r\n");

		GSensor_I2C_WriteReg(0x3A,0x13);
		GSensor_I2C_WriteReg(0x3B,0x1F);
		GSensor_I2C_WriteReg(0x3C,0x6a);
		GSensor_I2C_WriteReg(0x38,0x15);
		GSensor_I2C_WriteReg(0x22,0x80);
	}
	else if(g_GSensorSensitivityParking == GSENSOR_SENSITIVITY_PARKING_MED)
	{
		DBG_DUMP("[GSensor] SC7A20 Enter parking mode..MED\r\n");

		GSensor_I2C_WriteReg(0x3A,0x10);
		GSensor_I2C_WriteReg(0x3B,0x1F);
		GSensor_I2C_WriteReg(0x3C,0x6a);
		GSensor_I2C_WriteReg(0x38,0x15);
		GSensor_I2C_WriteReg(0x22,0x80);
	}
	else if(g_GSensorSensitivityParking == GSENSOR_SENSITIVITY_PARKING_HIGH)
	{
		DBG_DUMP("[GSensor] SC7A20 Enter parking mode..HIGH\r\n");

		GSensor_I2C_WriteReg(0x3A,0x08);
		GSensor_I2C_WriteReg(0x3B,0x1F);
		GSensor_I2C_WriteReg(0x3C,0x6a);
		GSensor_I2C_WriteReg(0x38,0x15);
		GSensor_I2C_WriteReg(0x22,0x80);
	}

	#if 0 	//only for gsensor test
	UINT8 datatemp=0;
	UINT8 dataAddr=0x3F;
	UINT8 i ;

	DBG_DUMP("[GSensor] SC7A20 Enter parking mode..read DATA\r\n");
    for(i=0;i<0x40;i++)
    	{
		datatemp=GSensor_I2C_ReadReg(dataAddr);
		DBG_DUMP("D 0x%x = 0x%x\r\n",dataAddr,datatemp);
		dataAddr	--;
    	}
	#endif

    return TRUE;
}

static void SC7A20_ClearIntSts(void)
{
	// Since we didnot use interrupt mode here.
	return;
}

static BOOL GSensor_SC7A20_CrashMode(void)
{
	if (g_bGsensorOpened == FALSE)
		return FALSE;

	DBG_DUMP("[GSensor] SC7A20 Enter crash mode..\r\n");
#if 0
	GSensor_I2C_WriteReg(0x22,0x00);
#endif

    return TRUE;
}

static void GSensor_SC7A20_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
	if (GSensorSensitivity <= GSENSOR_SENSITIVITY_END) {
		g_GSensorSensitivity = GSensorSensitivity;
	}
	if (GSensorSensitivity >= GSENSOR_SENSITIVITY_PARKING_BEGIN && GSensorSensitivity <= GSENSOR_SENSITIVITY_PARKING_END) {
		g_GSensorSensitivityParking = GSensorSensitivity;
	}
}

static void GSensor_SC7A20_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
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

