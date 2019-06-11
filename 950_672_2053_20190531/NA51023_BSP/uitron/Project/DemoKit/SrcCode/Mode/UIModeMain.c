////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMain.h"
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
//#include "UIMainWnd.h"
//UIInfo
#include "UIInfo.h"

#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif

int PRIMARY_MODE_MAIN = -1;      ///< Main

//#NT#2016/12/02#Niven Cho -begin
//#NT#MAIN mode use whole POOL_ID_APP as temp buffer
extern UINT32 User_GetTempBuffer(UINT32 uiSize);
//#NT#2016/12/02#Niven Cho -end

void ModeMain_Open(void);
void ModeMain_Close(void);

void ModeMain_Open(void)
{
	//#NT#2016/12/02#Niven Cho -begin
	//#NT#MAIN mode use whole POOL_ID_APP as temp buffer
	SxCmd_RegTempMemFunc(User_GetTempBuffer);
	//#NT#2016/12/02#Niven Cho -end
	Ux_SetActiveApp(&UISetupObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

#if(UI_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
#if (CALIBRATION_FUNC == ENABLE)
		if (EngineerMode_CheckEng())
		{
			EngineerMode_Open();

			// check if enter engineer mode
			if (IsEngineerModeOpened())
			{
				EngineerMode_WndOpen();
			}
		}
#endif
	    //Ux_OpenWindow((VControl *)(&UIMainWndCtrl), 0);
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnOpen) {
		    pCurSubMode->pfSubMode_OnOpen();
        }
	}
#endif

}
void ModeMain_Close(void)
{

#if(UI_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
#if (CALIBRATION_FUNC == ENABLE)
		// check if enter engineer mode
		if (IsEngineerModeOpened())
		{
			EngineerMode_WndClose();
		}
#endif
		//Ux_CloseWindow((VControl *)(&UIMainWndCtrl), 0);
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnClose) {
		    pCurSubMode->pfSubMode_OnClose();
        }
	}
#endif


	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

}

SYS_MODE gModeMain = {
	"MAIN",
	ModeMain_Open,
	ModeMain_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
