#ifndef _NVTWIFI_IPC_H_
#define _NVTWIFI_IPC_H_

#if defined(__ECOS)
#include <cyg/nvtipc/NvtIpcAPI.h>
#include <cyg/nvtwifi/nvtwifi.h>
#else
#include <nvtipc.h>
#include <nvtwifi.h>
#endif

//Definition of IPC communication functions
int NvtWifi_Init(NVTWIFI_OPEN_OBJ* pObj);
int NvtWifi_Uninit(void);
int NvtWifi_ExeWifiAPI(NVTWIFI_CMDID CmdId, int *pOutRet);
int NvtWifi_SysReqAck(int AckValue);
int NvtWifi_WaitCmd(NVTWIFI_MSG *pRcvMsg);

#define P2P_UPDATE				(1)

extern void NvtWifi_Sta_Status_CB(char *pIntfName, char *pMacAddr, int status);
extern void NvtWifi_Link_Status_CB(char *pIntfName,int status);
extern void NvtWifi_WSC_Event_CB(int evt, int status);
extern int  NvtWifi_WSC_Flash_CB(nvt_wsc_flash_param *flash_param);
extern void NvtWifi_P2P_CB(char *pIntfName,  int status);

typedef struct _NVTWIFI_CMDID_IMPL
{
	int               (*Wifi_CmdId_Test)(void);
	int               (*Wifi_CmdId_Init)(int param);
	int               (*Wifi_CmdId_Is_Interface_Up)(const char *pIntfName);
	int               (*Wifi_CmdId_Interface_Up)(const char *pIntfName);
	int               (*Wifi_CmdId_Interface_Down)(const char *pIntfName);
	int               (*Wifi_CmdId_Interface_Config)(const char *pIntfName, char *addr, char *netmask);
	int               (*Wifi_CmdId_Interface_Delete)(char *pIntfName);
	int               (*Wifi_CmdId_Config)(nvt_wifi_settings *pwifi);
	int               (*Wifi_CmdId_Get_Mac)(char *mac);
	int               (*Wifi_CmdId_Set_Mac)(const char *pIntfName, char *mac);
	int               (*Wifi_CmdId_Run_Cmd)(char *param1, char *param2, char *param3, char *param4, char *param5, char *param6);
	int               (*Wifi_CmdId_RegisterStaCB)(char *pIntfName);
	int               (*Wifi_CmdId_RegisterLinkCB)(char *pIntfName);
	int               (*Wifi_CmdId_RegisterWSCCB)(void);
	int               (*Wifi_CmdId_RegisterWSCFlashCB)(void);
	int               (*Wifi_CmdId_RegisterP2PCB)(char *pIntfName);
	int               (*Wifi_CmdId_getWlSiteSurveyRequest)(char *pIntfName, int *pStatus);
	int               (*Wifi_CmdId_getWlSiteSurveyResult)(char *pIntfName, NVT_SS_STATUS_T *pSSStatus);
	int               (*Wifi_CmdId_getWlP2PScanRequest)(char *pIntfName, int *pStatus);
	int               (*Wifi_CmdId_getWlP2PScanResult)(char *pIntfName, NVT_SS_STATUS_T *pSSStatus);
	int               (*Wifi_CmdId_sendP2PProvisionCommInfo)(char *pIntfName, NVT_P2P_PROVISION_COMM_Tp pP2pProvision);
	int               (*Wifi_CmdId_create_wscd)(void);
	int               (*Wifi_CmdId_wsc_reinit)(void);
	int               (*Wifi_CmdId_wsc_stop_service)(void);
	int               (*Wifi_CmdId_wscd_status)(void);
	int               (*Wifi_CmdId_generate_pin_code)(char *pinbuf);
	int               (*Wifi_CmdId_ignore_down_up)(char *pIntfName, int val);
	int               (*Wifi_CmdId_get_connect_info)(const char *pIntfName, const char *pStaMac, NVT_Connect_Info info, unsigned char *buf, unsigned int size);
	int               (*Wifi_CmdId_Interface_Remove)(const char *pIntfName);
	int               (*Wifi_CmdId_set_country)(const char *country);
	int               (*Wifi_CmdId_set_frequency)(NVT_FREQUENCY_TYPE f);
	int               (*Wifi_CmdId_set_bandwidth)(NVT_BANDWIDTH_TYPE bw);
}NVTWIFI_CMDID_IMPL;

extern const NVTWIFI_CMDID_IMPL gCmdImplFp;

#endif //_NVTWIFI_IPC_H_

