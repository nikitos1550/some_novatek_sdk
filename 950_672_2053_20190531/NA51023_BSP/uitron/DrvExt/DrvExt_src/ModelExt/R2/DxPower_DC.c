#include "DxCfg.h"
#include "IOCfg.h"
#include "DxPower.h"
#include "Utility.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "Dx.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxPwr
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


static UINT32 DrvPowerGetcaps(UINT32 CapID, UINT32 Param1);
static UINT32 DrvPowercfgs(UINT32 CfgID, UINT32 Param1);
static UINT32 DrvPowerInit(void *pInitParam);
static UINT32 DrvPowerOpen(void);
static UINT32 DrvPowerClose(void);
static UINT32 DrvPowerState(UINT32 StateID, UINT32 Value);
static UINT32 DrvPowerControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);
static UINT32 DrvPowerCommand(CHAR *pcCmdStr);
static UINT32 DrvPower_GetControl(DRVPWR_CTRL DrvPwrCtrl);
static void   DrvPower_SetControl(DRVPWR_CTRL DrvPwrCtrl, UINT32 value);

DX_OBJECT gDevPowerDC = {
	DXFLAG_SIGN,
	DX_CLASS_POWER_EXT,
	POWER_VER,
	"PowerDC",
	0, 0, 0, 0,
	DrvPowerGetcaps,
	DrvPowercfgs,
	DrvPowerInit,
	DrvPowerOpen,
	DrvPowerClose,
	DrvPowerState,
	DrvPowerControl,
	DrvPowerCommand,
	0,
};




static void   DrvPower_SetControl(DRVPWR_CTRL DrvPwrCtrl, UINT32 value)
{
	DBG_IND("DrvPwrCtrl(%d), value(%d)", DrvPwrCtrl, value);
	switch (DrvPwrCtrl) {
	case DRVPWR_CTRL_BATTERY_CALIBRATION_EN:
		break;

	case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
		break;

#if USB_CHARGE_FUNCTION
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
		break;
	case DRVPWR_CTRL_ENABLE_CHARGEIC:
		break;
#endif

	default:
		DBG_ERR("DrvPwrCtrl(%d)", DrvPwrCtrl);
		break;
	}
}

static UINT32  DrvPower_GetControl(DRVPWR_CTRL DrvPwrCtrl)
{
	UINT32 rvalue = 0;
	switch (DrvPwrCtrl) {
	case DRVPWR_CTRL_BATTERY_LEVEL:
		break;
	case DRVPWR_CTRL_BATTERY_ADC_VALUE:
		break;
	case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
		break;
	case DRVPWR_CTRL_IS_DUMMUYLOAD_POWEROFF:
		break;
	case DRVPWR_CTRL_IS_BATT_OVERHEAT:
		break;
#if USB_CHARGE_FUNCTION
	case DRVPWR_CTRL_IS_BATT_INSERT:
		break;
	case DRVPWR_CTRL_IS_DEAD_BATT:
		break;
	case DRVPWR_CTRL_IS_NEED_RECHARGE:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_OK:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
		break;
	case DRVPWR_CTRL_BATTERY_TEMPERATURE:
		break;
#else
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		rvalue = FALSE;
		break;
#endif
	default:
		DBG_ERR("DrvPwrCtrl(%d)", DrvPwrCtrl);
		break;
	}
	return rvalue;
}
/*
void DrvPower_RegCB(DX_CALLBACK_PTR fpDxPowerCB)
{

    g_fpDxPowerCB = fpDxPowerCB;
}
*/
///////////DX object
static UINT32 DrvPowerGetcaps(UINT32 CapID, UINT32 Param1)
{
	UINT32 v = 0;
	switch (CapID & 0x0000ffff) {
	case POWER_CAPS_BASE:
		v = POWER_BF_DRTC;
		break;
	default:
		break;
	}
	return v;
}

static UINT32 DrvPowercfgs(UINT32 CfgID, UINT32 Param1)
{

	return DX_OK;
}
static UINT32 DrvPowerInit(void *pInitParam)
{
	DBG_IND("Init\r\n");
	return DX_OK;
}

static UINT32 DrvPowerOpen(void)
{
	DBG_IND("open\r\n");
	return DX_OK;
}

static UINT32 DrvPowerClose(void)
{
	DBG_IND("close\r\n");
	return DX_OK;
}

static UINT32 DrvPowerState(UINT32 StateID, UINT32 Value)
{
	if (StateID & DXGET) {
		StateID &= ~DXGET;
		return DrvPower_GetControl(StateID);
	} else if (StateID & DXSET) {
		StateID &= ~DXSET;
		DrvPower_SetControl(StateID, Value);
	}
	return DX_OK;
}
static UINT32 DrvPowerControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)
{

	return DX_OK;
}

static UINT32 DrvPowerCommand(CHAR *pcCmdStr)  //General Command Console
{

	switch (*pcCmdStr) {
	case 'd':
		/*if (!strncmp(pcCmdStr, "Nand dump", 9))
		{
		    return TRUE;
		}*/
		break;
	}
	return FALSE;
}
/*
static void DrvPowerCallback(UINT32 EventID, UINT32 Param1, UINT32 Param2){

    if(g_fpDxPowerCB){
        g_fpDxPowerCB(EventID,Param1,Param2);
    }
}
*/




