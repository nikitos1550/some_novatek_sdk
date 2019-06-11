////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
#include "NvtUctrlAPI.h"
#if defined(__ECOS)  //for CPU1 ecos use nvtuctrl
#include "nvtuctrl.h"
#endif
#include "uToken.h"
#include "uctrlapp_nvt_status.h"
////////////////////////////////////////////////////////////////////////////////

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysUctrlExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern UTOKEN_MODULE module_gxstrg;
extern UTOKEN_MODULE module_mode;
#if (PHOTO_MODE==ENABLE)
extern UTOKEN_MODULE module_photo;
#endif
extern UTOKEN_MODULE module_setup;
extern UTOKEN_MODULE module_sys;
extern UTOKEN_MODULE module_movie;
extern UTOKEN_MODULE module_movie_ini;
extern UTOKEN_MODULE module_onvif;
extern UTOKEN_MODULE module_image;
extern UTOKEN_MODULE module_event;
extern UTOKEN_MODULE module_iqtool;
//extern void register_event(void);

static int NvtUctrl_Cmd(char *cmd)
{
	char retString[64];

	if (cmd && strlen(cmd)) {
		//INT32 hr=-1;

		DBG_IND("%s\r\n", cmd);
		UTOKEN_MODULE *pModule = uToken_GetModule(cmd);
		if (pModule != NULL) {
			//hr = uToken_Parse(cmd,pModule);
			uToken_Parse(cmd, pModule);
			return 0;
		} else {
			sprintf(retString,"err module:%s",(char *)cmd);
			NvtUctrl_SetRetString(retString);
			return -2;
		}
	} else {
		snprintf(retString, sizeof(retString), "no cmd");
		NvtUctrl_SetRetString(retString);
		return -3;
	}
}


void System_OnUctrl(void)
{
	NVTUCTRL_OPEN uctrl_mem = {0};
	uctrl_mem.sharedMemAddr = OS_GetMempoolAddr(POOL_ID_NVTUCTRL);
	uctrl_mem.sharedMemSize = OS_GetMempoolSize(POOL_ID_NVTUCTRL);
	DBGH(uctrl_mem.sharedMemAddr);
	if (E_OK != NvtUctrl_Open(&uctrl_mem)) {
		DBG_ERR("NvtUctrl_Open fail\r\n");
		return;
	}
	// register token table
#if (FS_FUNC==ENABLE)
	uToken_Register(&module_gxstrg);
#endif
	uToken_Register(&module_mode);
#if (PHOTO_MODE==ENABLE)
	uToken_Register(&module_photo);
#endif
	uToken_Register(&module_setup);
	uToken_Register(&module_sys);
	uToken_Register(&module_movie);
#if (IPCAM_PARAM_INI == ENABLE)
	uToken_Register(&module_movie_ini);
#endif
	uToken_Register(&module_onvif);
	uToken_Register(&module_image);

#if _TODO
	uToken_Register(&module_event);
	uToken_Register(&module_iqtool);
#else
	debug_msg("_TODO -> %s:%d, task(%x,%s) caller(%pF)\r\n", __FUNCTION__, __LINE__,
			  OS_GetTaskID(), OS_GetTaskName(), __builtin_return_address(0));

#endif

	//register nvt_status
#if (UCTRL_FUNC==ENABLE)
	uctrlapp_nvt_status_init(
		OS_GetMempoolAddr(POOL_ID_UCTRL_NVT_STATUS),
		OS_GetMempoolSize(POOL_ID_UCTRL_NVT_STATUS));
	uToken_Register(uctrlapp_nvt_status_get_utoken_module());
#endif

	//#NT#2018/05/15#YongChang Qui -begin
	//#NT#Add ramoops to print linux panic message after watchdog reboot
	uToken_Register(ramoops_get_utoken_module());
	//#NT#2018/05/15#YongChang Qui -end

	//
	NvtUctrl_RegCmdCB(NvtUctrl_Cmd);
    #if defined(__ECOS)  //for CPU1 ecos use nvtuctrl
    NvtUctrl_NoIPC_RegCB(nvtuctrl_setnotify);
    #endif
	//register_event();//register_event alarm input
}

