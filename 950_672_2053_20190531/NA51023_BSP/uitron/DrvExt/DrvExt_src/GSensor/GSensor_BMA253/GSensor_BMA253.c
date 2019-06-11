#include "GSensor.h"
#include "GSensor_BMA253.h"
#include "SwTimer.h"
#include <stdlib.h>
#include "ErrorNo.h"
#include "Delay.h"

#define __MODULE__          GSensor_BMA253
#define __DBGLVL__          1      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    //*=All, [mark]=CustomClass
#include "DebugModule.h"

//********************************************************************************
//
//GSENSOR BMA253
//
//********************************************************************************
static INT32 GsXData = 0,OldGsXData = 0;
static INT32 GsYData = 0,OldGsYData = 0;
static INT32 GsZData = 0,OldGsZData = 0;
static BOOL  g_bGsensorOpened = FALSE;
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;


//**********************************************************************************
//
//
//**********************************************************************************
//static GSENSOR_INFO2 g_GsensorInfo;
static UINT32 MBA253Threshold=0xFFFFFFFF;

static UINT32 GSensor_GetSensitivityLevel(void)
{
	switch(g_GSensorSensitivity) {
	case GSENSOR_SENSITIVITY_OFF:
		MBA253Threshold = 0xFFFFFFFF;
		break;
	case GSENSOR_SENSITIVITY_LOW:
		MBA253Threshold = 0xFF;
		break;
	case GSENSOR_SENSITIVITY_MED:
		MBA253Threshold = 0x1FF;
		break;
	case GSENSOR_SENSITIVITY_HIGH:
		MBA253Threshold = 0x3FF;
		break;
	default:
		MBA253Threshold = 0xFFFFFFFF;
		break;
	}
	return MBA253Threshold;
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
	// BMA253 GSensor I2C slave addresses
	GsensorInfo.I2C_Slave_WAddr = BMA253_I2C_WR_ADDR;
	GsensorInfo.I2C_Slave_RAddr = BMA253_I2C_RD_ADDR;
	if (GSensor_I2C_Init(GsensorInfo) == TRUE) {
		//init
		if(GSensor_I2C_ReadReg(CHIP_ID_REG) != CHIP_ID) {				/// Check Chip ID
			DBG_ERR("GSensor init(0x%x) failed!!\r\n",GSensor_I2C_ReadReg(CHIP_ID_REG));
			GSensor_I2C_Exit();											/// If not matched, close I2C bus.
			return FALSE;
		}

		// Init GSensor IC here
		GSensor_I2C_WriteReg(SW_RST_REG, SW_RST);						/// Soft reset
		Delay_DelayMs(2);												/// Delay wake-Up Time (1.8 ms)
		GSensor_I2C_WriteReg(PMU_RANGE_REG, PMU_RANGE_2G);				/// Set PMU_RANGE = +/- 2g
		GSensor_I2C_WriteReg(PMU_BW_REG, PMU_BW_31_25);					/// Set PMU_BW = 31.25 Hz

		DBG_DUMP("GSensor init OK!!\r\n");
		return TRUE;
	} else {
		DBG_ERR("GSensor init failed!!\r\n");
		return FALSE;
	}
}

BOOL GSensor_open(void)
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
	}
	return TRUE;
}

BOOL GSensor_close(void)
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

BOOL GSensor_GetStatus(Gsensor_Data *GS_Data)
{
	UINT32	abs_cal_x = 0, abs_cal_y = 0, abs_cal_z = 0;
	UINT32	G_INT_COUNT = 0;
	UINT32	Threshold;
	UINT32	g_X_Data_L = 0, g_X_Data_H = 0, g_Y_Data_L = 0, g_Y_Data_H = 0, g_Z_Data_L = 0, g_Z_Data_H = 0;

	if (g_bGsensorOpened == FALSE)
		return FALSE;
	if(g_GSensorSensitivity == GSENSOR_SENSITIVITY_OFF)
		return FALSE;

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

	OldGsXData = GsXData;
	OldGsYData = GsYData;
	OldGsZData = GsZData;

	DBG_IND("..........X = %d,Y = %d,Z = %d\r\n", GsXData, GsYData, GsZData);
	DBG_IND("..........DX = %d,DY = %d,DZ = %d th=%d \r\n", abs_cal_x, abs_cal_y, abs_cal_z, Threshold);

	if(G_INT_COUNT >= 2)
		return TRUE;
	else
		return FALSE;
}

BOOL GSensor_ParkingMode(void)
{
	if (g_bGsensorOpened == FALSE)
		return FALSE;
	if(g_GSensorSensitivity == GSENSOR_SENSITIVITY_OFF)
		return FALSE;

	//DBG_DUMP("BMA253 Enter parking mode..\r\n");
	DBG_DUMP("BMA253 Parking mode not implemented!\r\n");

    return TRUE;
}

void ClearIntSts(void)
{
	// Since we didnot use interrupt mode here.
	return;
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

