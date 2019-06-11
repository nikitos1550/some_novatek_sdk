#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utility.h"

#include "DrvExt.h"

#include "adc.h" //for adc_readData()
#include "top.h" //for pinmux_select_ick(), pinmux_select_goice(), PINMUX_ICK, PINMUX_GOICE
#include "DxApi.h"
#include "Driver.h" //for drv_getVerInfo(), drv_getBuildDate()
#include "IOCfg.h" //for USE_VIO
#include "SwTimer.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#include "SxCmd.h"

static BOOL Cmd_dx_lcd(CHAR *strCmd)
{
	DX_HANDLE devLCDObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
	return Dx_Command(devLCDObj, strCmd);
}

static BOOL Cmd_dx_tv(CHAR *strCmd)
{
	DX_HANDLE devTVObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_TVOUT);
	return Dx_Command(devTVObj, strCmd);
}
#if _TODO
static BOOL Cmd_dx_hdmi(CHAR *strCmd)
{
	DX_HANDLE devHDMIObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_HDMIOUT);
	return Dx_Command(devHDMIObj, strCmd);
}
static BOOL Cmd_dx_lcd2(CHAR *strCmd)
{
	DX_HANDLE devLCD2Obj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD2);
	return Dx_Command(devLCD2Obj, strCmd);
}
static BOOL Cmd_dx_snr1(CHAR *strCmd)
{
	switch (*strCmd) {
#if (USE_VIO == ENABLE)
	case 'v':
		if (!strncmp(strCmd, "vdet", 4)) {
			char ts[10];
			UINT32 v1;
			sscanf_s(strCmd, "%s %d", ts, &v1); //fix for CID 44011
			DBG_IND("v1 = %d\r\n", v1);
			vio_setpin(VIO_SENSOR1_DET, v1);
			return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}
static BOOL Cmd_dx_snr2(CHAR *strCmd)
{
	switch (*strCmd) {
#if (USE_VIO == ENABLE)
	case 'v':
		if (!strncmp(strCmd, "vdet", 4)) {
			char ts[10];
			UINT32 v1;
			sscanf_s(strCmd, "%s %d", ts, &v1); //fix for CID 44012
			DBG_IND("v1 = %d\r\n", v1);
			vio_setpin(VIO_SENSOR2_DET, v1);
			return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}
#endif
static BOOL Cmd_dx_aout(CHAR *strCmd)
{
	DX_HANDLE devAudObj = Dx_GetObject(DX_CLASS_AUDIO_EXT);
	return Dx_Command(devAudObj, strCmd);
}


SXCMD_BEGIN(dx, "driver extern debug using")
SXCMD_ITEM("lcd %", Cmd_dx_lcd, "lcd relay command")
SXCMD_ITEM("tv %", Cmd_dx_tv, "tv relay command")
#if _TODO
SXCMD_ITEM("hdmi %", Cmd_dx_hdmi, "hdmi relay command")
SXCMD_ITEM("lcd2 %", Cmd_dx_lcd2, "lcd2 relay command")
SXCMD_ITEM("snr1 %", Cmd_dx_snr1, "sensor1 relay command")
SXCMD_ITEM("snr2 %", Cmd_dx_snr2, "sensor2 relay command")
#endif
SXCMD_ITEM("aout %", Cmd_dx_aout, "audio out relay command")
SXCMD_END()


static BOOL Cmd_drv_all(CHAR *Name)
{
	DBG_DUMP("@ITEM_BEGIN [%s]\r\n", Name);

	drv_begin_dump();

	// CPU
	drv_dumpCPUInfo();

	// DRAM
	drv_dumpDRAMInfo();

#if (!defined(_Disp_VIRTUAL_LCD1_OFF_) || !defined(_TV_OFF_))
 	// IDE
	drv_dumpIDEInfo();
#endif

	// DSI
	//drv_dumpDSIInfo();

#if (!defined(_CARD1_NONE_) || !defined(_CARD3_NONE_))
	// SDIO
	drv_dumpSDIOInfo();
#endif

#if (!defined(_EMBMEM_NONE_)) && (!defined(_EMBMEM_UITRON_OFF_))
	// NAND
	drv_dumpNANDInfo();
#endif

#if !defined(_USB1_OFF_)
	// USB
	drv_dumpUSBInfo();
#endif

	// PWM
	drv_dumpPWMInfo();

#if !defined(_AUDIO_OFF_)
	// EAC
	drv_dumpEACInfo();
#endif

	// SIF
	drv_dumpSIFInfo();

	// SIE
	drv_dumpSIEInfo();

	// Sensor
	drv_dumpSensorInfo();

#if !defined(_TV_OFF_)
	// TV
	drv_dumpTVInfo();
#endif

	// PWBC
	drv_dumpPWBCInfo();

	// HDMI
	//drv_dumpHDMIInfo();

	// PLL
	drv_dumpPllInfo();

	// Misc
	drv_dumpMISCInfo();

	drv_end_dump();

	DBG_DUMP("@ITEM_END\r\n");
	return TRUE;
}


static BOOL Cmd_drv_ide(CHAR *strCmd)
{
#if (!defined(_Disp_VIRTUAL_LCD1_OFF_) || !defined(_TV_OFF_))
	idec_dumpInfo(0);
#endif
	return TRUE;
}

static BOOL Cmd_drv_pwm(CHAR *strCmd)
{
	pwm_dumpInfo();
	return TRUE;
}


SXCMD_BEGIN(drvdump, "Dump driver information")
SXCMD_ITEM("ide", Cmd_drv_ide, "Dump IDE")
SXCMD_ITEM("pwm", Cmd_drv_pwm, "Dump PWM")
SXCMD_ITEM("all", Cmd_drv_all, "Dump current status in any status")
SXCMD_END()


