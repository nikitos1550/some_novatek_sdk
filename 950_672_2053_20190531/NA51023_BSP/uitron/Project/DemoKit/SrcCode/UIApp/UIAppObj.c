#include "SysKer.h"
#include "SysCfg.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "Debug.h"
#include "AppLib.h"
#include "UIAppCommon.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppComm
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


extern VControl SystemObjCtrl;
extern VControl UISetupObjCtrl;

//the order of this array should be the same as APP_TYPE
VControl *gAppCtrlTypeList[APP_TYPE_MAX - APP_TYPE_MIN] = {
	&SystemObjCtrl,      //APP_BASE
	&UISetupObjCtrl,     //APP_SETUP
	0,                   //APP_PLAY
	0,                   //APP_PHOTO
	0,                   //APP_MOVIEPLAY
	0,                   //APP_MOVIEREC
	0,                   //APP_PRINT
	0,                   //APP_PCC,
	0,                   //APP_MSDC
#if(WIFI_AP_FUNC==ENABLE)
	0,                   //APP_WIFICMD
#endif
};

