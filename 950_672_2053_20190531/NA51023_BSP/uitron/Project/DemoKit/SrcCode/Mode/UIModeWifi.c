
#include "PrjCfg.h"
#include "UIMenuWndWiFiModuleLink.h"
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#include "UIMenuWndWiFiMobileLinkOK.h"
//#NT#2016/03/23#Isiah Chang -end
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "SysMain.h"

int SYS_SUBMODE_WIFI = -1;      ///< WiFi

#if(WIFI_FUNC==ENABLE)

void SubModeWiFi_Open(void)
{
#if (WIFI_AP_FUNC==ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) != System_GetState(SYS_STATE_PREVSUBMODE)) {
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
		Ux_OpenWindow(&UIMenuWndWiFiModuleLinkCtrl, 0);
#elif(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
		Ux_OpenWindow(&UIMenuWndWiFiMobileLinkOKCtrl, 0);
#endif
	}
#endif

//#NT#2016/03/23#Isiah Chang -end
}

void SubModeWiFi_Close(void)
{
#if (WIFI_AP_FUNC==ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) != System_GetState(SYS_STATE_NEXTSUBMODE)) {
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
		Ux_CloseWindow(&UIMenuWndWiFiModuleLinkCtrl, 0);
#elif(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
		Ux_CloseWindow(&UIMenuWndWiFiMobileLinkOKCtrl, 0);
#endif
//#NT#2016/03/23#Isiah Chang -end
	}
#endif
}

SYS_SUBMODE gSubModeWiFi = {
	"WIFI",
	SubModeWiFi_Open,
	SubModeWiFi_Close,
};

#endif
