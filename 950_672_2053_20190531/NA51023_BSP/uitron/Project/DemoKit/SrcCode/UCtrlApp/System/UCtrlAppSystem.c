#include "AppControl.h"
#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UIControl.h"
#include "UIControlWnd.h"
#include "UCtrlAppSystem.h"
#include "NvtUctrlAPI.h"
#include "NvtIpcAPI.h"
#include "UCtrlMain.h"
#include "UISetup.h"
#include "nvtmpp.h"
#if (FWS_FUNC == ENABLE)
#include "FwSrvApi.h"
#endif
#if (HFS_FUNC == ENABLE)
#include "HfsNvtAPI.h"
#endif
#include "UIAppNetwork.h"
#include "UIAppMovie.h"
#include "ImageUnit_NetRTSP.h"
#include "ImageApp_IPCam.h"
#include "UIAppIPCam_Param.h"
#include "ProjectInfo.h"
#include "modelext_info.h"
#include "bin_info.h"
#include "DrvExt.h"
#include "ipl_ctrl_infor.h"
#if(WIFI_FUNC==ENABLE)
//#NT#2016/06/15#Charlie Chang -begin
//#NT# support wifi cmd
#include "WifiCmdParser.h"
#include "WifiAppCmd.h"
//#NT#2016/06/15#Charlie Chang -end
#endif
#if (MSDCVENDOR_NVT == ENABLE)
#include "MsdcNvtCb.h"
#endif
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppSys
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define SXCMD_TEST          0

#define UCTRLAPPSYS_FLG_NET_INITED  FLGPTN_BIT(0)   // notified from linux said net init is ok


#define WIFIRET_LAST_DATA '1'
#define WIFIRET_CONTINUE_DATA '2'
#define WIFI_CMD_ROOT        '/'
#define WIFI_CMD_CUSTOM_TAG  '?'

typedef struct _UCtrlSysParmType {
	UINT32 isSet;
} UCtrlSysParmType;

typedef struct _UCtrlSysParmFWType {
	UINT32 isSet;
	UINT32 fwSize;
} UCtrlSysParmFWType;

typedef struct _UCtrlPortsetParmType {
	UINT32 isSet;
	UINT32 http_port;
	UINT32 https_port;
	UINT32 rtsp_port;
} UCtrlPortsetParmType;

typedef struct _UCtrlAppDisableType {
	UINT32 isSet;
	BOOL http_server;
	BOOL rtsp_server;
} UCtrlAppDisableType;

typedef struct _UCtrlMsdcNvtType {
	UINT32 isSet;
	BOOL bEn;
} UCtrlMsdcNvtType;


typedef struct _CAP_INFO {
	UINT32	Valid;
	UINT32  Width;
	UINT32  Height;
	UINT32  FrameRate;
	UINT32  ScanMethod;
	UINT32  Feature;	// 0:undefined 1:encode only 2:liveview only
	UINT32  MaxByteRate;
	IPCAM_SENSOR    SensorId;
	UINT32  SensorFrontFps;
	IPCAM_IME       ImeId;
	UINT32  ImeCompressed;
	UINT32  Ime3DNR;
	UINT32  ImeFps;
	UINT32  BufferCount;
	UINT32  PathId;
} CAP_INFO;

typedef struct _LCD_INFO {
	UINT32	Valid;
	UINT32  Width;
	UINT32  Height;
	UINT32  FrameRate;
	UINT32  ScanMethod;
	UINT32  VoutId;
} LCD_INFO;

#define	MAX_ENV_INFO_STRING_LEN	31

typedef struct _ENV_INFO {
	char SensorName[MAX_ENV_INFO_STRING_LEN + 1];
	char DisplayName[MAX_ENV_INFO_STRING_LEN + 1];
	char EmbeddedMemoryName[MAX_ENV_INFO_STRING_LEN + 1];
	char DSPFeature[MAX_ENV_INFO_STRING_LEN + 1];
	char BuildDate[MAX_ENV_INFO_STRING_LEN + 1];

	struct {
		unsigned long  addr;
		unsigned long  size;
	} mem_partition[8];	//0:ipc 1:linux 2:uboot 3:dsp1 4:uitron 5:ramdisk 6:loader 7:extdram
	struct {
		unsigned long  addr;
		unsigned long  size;
		unsigned long  free;
		unsigned long  max_free_block;
	} mem_status[2];  //0:ddr-1, 1:ddr-2

	unsigned int  fd_facescale;
	unsigned int  direct_mode : 1;
	unsigned int  define_product: 4;
	unsigned int  define_new_trigger : 1;
	unsigned int  define_ipl_direct : 1;
	unsigned int  define_ipl_3dnr : 1;
	unsigned int  define_enc_3dnr : 1;
	unsigned int  define_disp_wait : 1;
	unsigned int  define_dis : 1;
	unsigned int  define_md_func : 1;
	unsigned int  define_smartroi : 1;
	unsigned int  define_vqa_func : 1;
	unsigned int  define_pd_func : 1;
	unsigned int  define_fd_func : 1;
	unsigned int  define_aec_func : 1;
	unsigned int  define_anr_func : 1;
	unsigned int  define_ltr_func : 1;
	unsigned int  define_wdr_func : 1;
	unsigned int  define_hdr_func : 1;
	unsigned int  define_ftg_func : 1;
	unsigned int  define_buf_level : 4;
	unsigned int  define_reserved : 6;
	unsigned int  md_path;

	unsigned int  audio_enc_pcm : 1;
	unsigned int  audio_enc_aac : 1;
	unsigned int  audio_enc_alaw : 1;
	unsigned int  audio_enc_ulaw : 1;
	unsigned int  audio_enc_reserved : 12;
	unsigned int  audio_dec_pcm : 1;
	unsigned int  audio_dec_aac : 1;
	unsigned int  audio_dec_alaw : 1;
	unsigned int  audio_dec_ulaw : 1;
	unsigned int  audio_dec_reserved : 12;

	unsigned int  max_video_buf_time_ms;
	unsigned int  max_audio_in_buf_time_ms;
	unsigned int  max_audio_out_buf_time_ms;

} ENV_INFO;

#define AU_INFO_SAMPE_RATE_8K       (1 << 0)    /*  8000 Hz */
#define AU_INFO_SAMPE_RATE_11K      (1 << 1)    /* 11025 Hz */
#define AU_INFO_SAMPE_RATE_12K      (1 << 2)    /* 12000 Hz */
#define AU_INFO_SAMPE_RATE_16K      (1 << 3)    /* 16000 Hz */
#define AU_INFO_SAMPE_RATE_22K      (1 << 4)    /* 22050 Hz */
#define AU_INFO_SAMPE_RATE_24K      (1 << 5)    /* 24000 Hz */
#define AU_INFO_SAMPE_RATE_32K      (1 << 6)    /* 32000 Hz */
#define AU_INFO_SAMPE_RATE_44K      (1 << 7)    /* 44100 Hz */
#define AU_INFO_SAMPE_RATE_48K      (1 << 8)    /* 48000 Hz */

#define AU_INFO_SAMPE_SIZE_8BIT     (1 << 0)    /* 8 Bit   */
#define AU_INFO_SAMPE_SIZE_16BIT    (1 << 1)    /* 16 bits */
#define AU_INFO_SAMPE_SIZE_32BIT    (1 << 2)    /* 32 bits */

#define AU_INFO_CHANNEL_LEFT        (1 << 0)
#define AU_INFO_CHANNEL_RIGHT       (1 << 1)
#define AU_INFO_CHANNEL_STEREO      (1 << 2)

typedef struct _AUDIO_IN_INFO {
	UINT32	Valid;      // 0: not used
	UINT32  SampleRate_Support_bitmap;
	UINT32  SampleSize_Support_bitmap;
	UINT32  Channel_Support_bitmap;
	UINT32  Source;

} AUDIO_IN_INFO;

typedef struct _AUDIO_OUT_INFO {
	UINT32	Valid;      // 0: not used
	UINT32  SampleRate_Support_bitmap;
	UINT32  SampleSize_Support_bitmap;
	UINT32  Channel_Support_bitmap;
	UINT32  Source;
} AUDIO_OUT_INFO;

#define MAX_CAP_PATH          8
#define MAX_DISP_OUT          1
#define MAX_AUDIO_IN_COUNT    1
#define MAX_AUDIO_OUT_COUNT   1
typedef struct _SYS_INFO {
	CAP_INFO CapInfo[MAX_CAP_PATH];
	LCD_INFO LCDInfo[MAX_DISP_OUT];
	AUDIO_IN_INFO AuInInfo[MAX_AUDIO_IN_COUNT];
	AUDIO_OUT_INFO AuOutInfo[MAX_AUDIO_OUT_COUNT];
	ENV_INFO EnvInfo;
} SYS_INFO;

typedef struct _UCtrlImeInfoType {
	UINT32 isSet;
	BOOL bEn;
} UCtrlImeInfoType;



typedef struct _UCtrlSysInfoType {
	BOOL                  isErr;
	UCtrlSysParmType      fwupdate;
	UCtrlSysParmType      shutdown;
	UCtrlSysParmType      reboot;
	UCtrlSysParmType      builddate;
	UCtrlSysParmType      core2fwaddr;
	UCtrlSysParmFWType    core2fwready;
	UCtrlSysParmType      core2fwupflag;
	UCtrlPortsetParmType  portset;
	UCtrlAppDisableType   app_disable;
	UCtrlMsdcNvtType      msdcnvt;
	int                   paramcnt;
} UCtrlSysInfoType;

static UCtrlSysInfoType   sysInfo;
static UCtrlSysInfoType  *pSysInfo = &sysInfo;

extern UINT32 _SECTION(".kercfg_data") UCTRLAPPSYS_FLG_ID;
UINT32 UCTRLAPPSYS_FLG_ID = 0;

static INT32 param_portset(INT32 argc, char *argv[])
{
	if (argc != 3) {
		DBG_ERR("need [rtsp port] [http port] [https port]\r\n");
		return -1;
	}
	pSysInfo->portset.rtsp_port = atoi(argv[0]);
	pSysInfo->portset.https_port = atoi(argv[1]);
	pSysInfo->portset.http_port = atoi(argv[2]);
	pSysInfo->portset.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

static INT32 param_rtsp_portget(INT32 argc, char *argv[])
{
#if(FORCE_UNPLUG_RTSP != ENABLE)
	char retStr[64] = {0};

	UINT32 rtsp_port = ImageUnit_GetParam(&ISF_NetRTSP, ISF_CTRL, NETRTSP_PARAM_PORT);

	DBG_DUMP("rtsp port=%d\r\n", rtsp_port);
	snprintf(retStr, sizeof(retStr), "%d", rtsp_port);
	NvtUctrl_SetRetString(retStr);
#else
	DBG_ERR("not support !!\r\n");
#endif
	return 0;
}

static INT32 param_fwupdate(INT32 argc, char *argv[])
{
	pSysInfo->fwupdate.isSet = 1;
	pSysInfo->paramcnt++;

	return 0;
}

static INT32 param_shutdown(INT32 argc, char *argv[])
{
	pSysInfo->shutdown.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

static INT32 param_reboot(INT32 argc, char *argv[])
{
	pSysInfo->reboot.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

static INT32 param_notify(INT32 argc, char *argv[])
{
	if (strcmp("net_init_ok", argv[0]) == 0) {
		set_flg(UCTRLAPPSYS_FLG_ID, UCTRLAPPSYS_FLG_NET_INITED);
	}
	return 0;
}

/* Build date query */
static INT32 param_builddate(INT32 argc, char *argv[])
{
	pSysInfo->builddate.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

/* Core2 query firmware buffer address */
static INT32 param_core2fwaddr(INT32 argc, char *argv[])
{
	pSysInfo->core2fwaddr.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

/* Core2 notify firmware is ready in buffer */
static INT32 param_core2fwready(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	pSysInfo->core2fwready.isSet = 1;
	pSysInfo->core2fwready.fwSize = atoi(argv[0]);
	pSysInfo->paramcnt++;
	return 0;
}

/* Core2 query firmware upgrade flag, check if firmware upgraded */
static INT32 param_core2fwupflag(INT32 argc, char *argv[])
{
	pSysInfo->core2fwupflag.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}
//#NT#2016/05/18#David Tsai -begin
//#NT# Support http and rtsp service disable in booting time
static INT32 param_app_disable(INT32 argc, char *argv[])
{
	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}

	pSysInfo->app_disable.isSet = 1;
	pSysInfo->app_disable.http_server = atoi(argv[0]);
	pSysInfo->app_disable.rtsp_server = atoi(argv[1]);

	return 0;
}
//#NT#2016/05/18#David Tsai -end

//#NT#2017/08/14#Charlie Chang -begin
//#NT# support hot plug mem to linux
static INT32 param_dynamic_mem(INT32 argc, char *argv[]){


	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	UINT32 ret =0;
	ret = nvtmpp_vb_init();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}
	UINT32 uiAlignSize = atoi(argv[0]);
	char RetStr[32];
	DBG_DUMP("hotplug alignSize=%d\r\n",uiAlignSize);
	INT32  iFreeSize = nvtmpp_vb_get_max_free_size(NVTMPP_DDR_1) - HOT_PLUG_REMAIN_SIZE;
	NVTMPP_VB_POOL vb_pool;
	NVTMPP_VB_BLK  blk;
	UINT32 buf_addr=0;
	UINT32 uiPhyAddr = 0;
	//UINT32 uiFinalAddr = 0;

	DBG_DUMP("free:%d align:%d\r\n",iFreeSize,uiAlignSize);
	if(iFreeSize <=0){
		DBG_WRN("not enough mem!!!\r\n");
		return-1;
	}

	vb_pool = nvtmpp_vb_create_pool("LinuxPool", iFreeSize, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == vb_pool) {
		DBG_ERR("create private pool err\r\n");
		return -1;
	}
	blk = nvtmpp_vb_get_block(0, vb_pool, iFreeSize, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_BLK == blk) {
		if( nvtmpp_vb_destroy_pool(vb_pool)!=NVTMPP_ER_OK ){
			DBG_ERR("destroy pool fail\r\n");
		}
		DBG_ERR("get vb block err\r\n");
		return -1;
	}
	buf_addr = nvtmpp_vb_block2addr(blk);
	uiPhyAddr = NvtIPC_GetPhyAddr(buf_addr);
	sprintf(RetStr,"%d %d",uiPhyAddr,iFreeSize);
	#if 0
	////check alignment
		if((uiPhyAddr%uiAlignSize) != 0){

			uiFinalAddr = ((uiPhyAddr/uiAlignSize)+1)*uiAlignSize;
			iFreeSize = iFreeSize - (uiFinalAddr - uiPhyAddr);

			if(iFreeSize <=0){
				DBG_WRN("aling(%x)  original_addr=%x final_addr=%x size=%d, buf size not enough\r\n",
					uiPhyAddr,uiFinalAddr,iFreeSize);
				nvtmpp_vb_destroy_pool(vb_pool);
				sprintf(RetStr,"0 0");

			}
			else{
				iDevideValue = iFreeSize/uiAlignSize;
				if(iDevideValue == 0){
					DBG_WRN("aling(%x) original_addr=%x final_addr=%x size=%d, buf size not enough!\r\n",
					uiPhyAddr,uiFinalAddr,iFreeSize);
					nvtmpp_vb_destroy_pool(vb_pool);
					sprintf(RetStr,"0 0");
				}
				else{
					sprintf(RetStr,"%d %d",uiFinalAddr,iDevideValue*uiAlignSize);
				}
			}
		}
		else{
			sprintf(RetStr,"%d %d",NvtIPC_GetPhyAddr(buf_addr),uiAlignSize*iDevideValue);
		}


	}
	#endif
	DBG_DUMP("(addr,size):%s\r\n",RetStr);
	NvtUctrl_SetRetString(RetStr);
    return 0;
}
//#NT#2017/08/14#Charlie Chang -end

//#NT#2016/08/05#Niven Cho -begin
//#NT#MSDC-NET
static INT32 param_msdcnvt(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	pSysInfo->msdcnvt.isSet = 1;
	pSysInfo->msdcnvt.bEn = atoi(argv[0]);

	return 0;
}
//#NT#2016/08/05#Niven Cho -end

static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
	memset(pSysInfo, 0, sizeof(UCtrlSysInfoType));
	return 0;
}
static void system_poweroff(void)
{
	DBG_IND("system_poweroff\r\n");
	Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}
static void system_reboot(void)
{
	DBG_IND("system_reboot\r\n");
#if !defined(_CPU1_ECOS_) && (IPCAM_FUNC == ENABLE)
	//IPCam, do nothing
#elif defined(_CLOCK_RTC_)
	GxSystem_EnableHWReset(0);
#elif defined(_CLOCK_DRTC_)
	GxSystem_EnableSWReset(0); //it may cause damage on peripheral, cause their power are not be reset here.
#else
	#error (unknown CLOCK setting?)
#endif
	Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}
static INT32 param_end(UTOKEN_END_INFO *p_info)
{
	char retStr[64];
#if ONVIF_FUNC
	static BOOL onvifStartFlag = FALSE;
#endif

	if (p_info->err_code < 0) {
		snprintf(retStr, sizeof(retStr) - 1, "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return p_info->err_code;
	}

	DBG_IND("param cnt %d\r\n", pSysInfo->paramcnt);

	if (pSysInfo->paramcnt > 1) {
		snprintf(retStr, sizeof(retStr) - 1, "ERR: parm cnt %d > 1", pSysInfo->paramcnt);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return -1;
	}

	if (pSysInfo->fwupdate.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_FWUPDATE, 0);
	}
	if (pSysInfo->shutdown.isSet) {
		system_poweroff();

	}
	if (pSysInfo->reboot.isSet) {
		system_reboot();
	}

	/* Reture build date string to core 2 */
	if (pSysInfo->builddate.isSet) {
		char retStr[64];

		snprintf(retStr, sizeof(retStr), "%s", Prj_GetCheckinDate());
		DBG_IND("reStr= %s\r\n", retStr);
		NvtUctrl_SetRetString(retStr);
	}

	/* Return firmware buffer address to core2 */
	if (pSysInfo->core2fwaddr.isSet) {
		char retStr[64];

		if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
			DBG_ERR("Not in Main Mode.\r\n");
			snprintf(retStr, sizeof(retStr), "0 0");
		} else {
			//because FwSrv need 4 word alignment source bin address
			UINT32 uiAddr = ALIGN_CEIL_16(OS_GetMempoolAddr(POOL_ID_APP));
			UINT32 uiSize = OS_GetMempoolSize(POOL_ID_APP) - (uiAddr - OS_GetMempoolAddr(POOL_ID_APP));
			snprintf(retStr, sizeof(retStr), "%lu %lu", NvtIPC_GetPhyAddr(uiAddr), uiSize);
		}

		DBG_IND("reStr= %s\r\n", retStr);
		NvtUctrl_SetRetString(retStr);
	}

	/* Core2 upload firmware ready notify */
	if (pSysInfo->core2fwready.isSet) {
#if (FWS_FUNC == ENABLE)
#if _TODO
		FWSRV_CMD Cmd = {0};
		/* Firmware upload ready, start upgrade */
		DBG_DUMP("receive firmware ready notify, fwSize is %lu!\r\n", pSysInfo->core2fwready.fwSize);
		System_CPU2_Stop();
		FWSRV_BIN_UPDATE_ALL_IN_ONE FwInfo = {0};
		FwInfo.uiSrcBufAddr = ALIGN_CEIL_16(OS_GetMempoolAddr(POOL_ID_APP));
		FwInfo.uiSrcBufSize = pSysInfo->core2fwready.fwSize;
		FwInfo.uiWorkBufAddr = FwInfo.uiSrcBufAddr + ALIGN_CEIL_16(FwInfo.uiSrcBufSize); //because FwSrv need 4 word alignment source bin address
		FwInfo.uiWorkBufSize = OS_GetMempoolSize(POOL_ID_APP) - (FwInfo.uiSrcBufAddr - OS_GetMempoolAddr(POOL_ID_APP)) - ALIGN_CEIL_16(FwInfo.uiSrcBufSize); //for FwSrv reload to check sum
		FwInfo.fpItemCb = NULL;
		memset(&Cmd, 0, sizeof(Cmd));
		Cmd.Idx = FWSRV_CMD_IDX_BIN_UPDATE_ALL_IN_ONE;
		Cmd.In.pData = &FwInfo;
		Cmd.In.uiNumByte = sizeof(FwInfo);
		Cmd.Prop.bExitCmdFinish = TRUE;
		FWSRV_ER fws_er = FwSrv_Cmd(&Cmd);
		if (fws_er != FWSRV_ER_OK) {
			DBG_ERR("FW bin write failed %d\r\n", fws_er);
			system_reboot();
			return fws_er;
		}
		system_reboot();
#else
		debug_msg("_TODO -> %s:%d, task(%x,%s) caller(%pF)\r\n", __FUNCTION__, __LINE__,
				  OS_GetTaskID(), OS_GetTaskName(), __builtin_return_address(0));

#endif
#endif
	}

	if (pSysInfo->core2fwupflag.isSet) {
		char retStr[64];
#if(BOOT_RESET_MENU_INFO == ENABLE)
		if (SysGetFlag(FL_CPU2_DEFAULT)) {
			SysSetFlag(FL_CPU2_DEFAULT, 0);
			Save_MenuInfo();
			snprintf(retStr, sizeof(retStr), "1");
		} else {
			snprintf(retStr, sizeof(retStr), "0");
		}

		DBG_IND("reStr= %s\r\n", retStr);
#else
		MODELEXT_HEADER *head = NULL;
		BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &head);
		UINT32 uiFWUpdate = (pBinInfo->ld.LdCtrl2 & LDCF_UPDATE_FW);
		if (uiFWUpdate == 0) {
			snprintf(retStr, sizeof(retStr), "0");
		} else {
			snprintf(retStr, sizeof(retStr), "1");
		}
#endif
		NvtUctrl_SetRetString(retStr);
	}
	if (pSysInfo->portset.isSet) {

		DBG_DUMP("set rtsp port=%d http_port=%d https_port=%d\r\n",
				 pSysInfo->portset.rtsp_port,
				 pSysInfo->portset.http_port,
				 pSysInfo->portset.https_port);
		////set hfs server
#if (HFS_FUNC == ENABLE)
		UINet_HFSUnInit();
		UINet_HFSPortSet(pSysInfo->portset.http_port,
						 pSysInfo->portset.https_port);
		UINet_HFSInit();
#endif

		/////set rtsp server
#if(FORCE_UNPLUG_RTSP != ENABLE)
		UINT32 rtsp_port = ImageUnit_GetParam(&ISF_NetRTSP, ISF_CTRL, NETRTSP_PARAM_PORT);
		if (rtsp_port != (UINT32)pSysInfo->portset.rtsp_port) {
			ImageUnit_Begin(&ISF_NetRTSP, 0);
			ImageUnit_SetParam(ISF_CTRL, NETRTSP_PARAM_PORT, pSysInfo->portset.rtsp_port);
			ImageUnit_End();
			ImageApp_IPCam_update_all_path();
		}
		snprintf(retStr, sizeof(retStr), "OK");
		NvtUctrl_SetRetString(retStr);
#endif

		/////starup onvif
#if ONVIF_FUNC
		if (onvifStartFlag == FALSE) {
			UINet_OnvifInit();
			onvifStartFlag = TRUE;
		}
#endif
	}

	if (pSysInfo->app_disable.isSet) {
		if (pSysInfo->app_disable.http_server != 0) {
			DBG_DUMP("\r\n!!!!!Disable hfs!!!!!\r\n");
#if (HFS_FUNC == ENABLE)
			UINet_DisableHFS(FALSE);
#endif
		}
		if (pSysInfo->app_disable.rtsp_server != 0) {
			DBG_DUMP("\r\n!!!!!Disable rtsp!!!!!\r\n");
			ImageApp_IPCam_Config(IPCAM_CFG_PLUG_RTSP, 0);
		}
	}

#if (MSDCVENDOR_NVT == ENABLE)
	if (pSysInfo->msdcnvt.isSet && System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
		MsdcNvtCb_Net(pSysInfo->msdcnvt.bEn);
	}
#endif

	return 0;
}

//#NT#2016/10/18#Ben Wang -begin
//#NT#Refine uctrl wifi cmd
static INT32 param_wificmdinit(INT32 argc, char *argv[])
{
#if _TODO
	DBG_IND("\r\n");
	WifiAppCmd_init();
	WifiCmd_ReceiveCmd(1);
#else
	debug_msg("_TODO -> %s:%d, task(%x,%s) caller(%pF)\r\n", __FUNCTION__, __LINE__,
			  OS_GetTaskID(), OS_GetTaskName(), __builtin_return_address(0));

#endif
	return 0;
}
static INT32 param_wificmd(INT32 argc, char *argv[])
{
#if _TODO
	char RetStr[16];
	char   *pch;
	char buf[2048] = {0};
	//char *pCmdStr;

	int buf_size;

	char mimeType[64] = {0};
	sprintf(mimeType, "text/xml");
	int ret = 0;
	static int index = 0;

	if (argc < 1 || argv[0] == NULL) {
		DBG_ERR("argc error  argc=%d\r\n", argc);
		return 0;
	}
	DBG_IND("cmd=%s\r\n", argv[0]);

	pch = strchr((char *)argv[0], WIFI_CMD_CUSTOM_TAG);
	if (NULL == pch || *argv[0] != WIFI_CMD_ROOT) {
		DBG_WRN("Not a valid cmd (%s)\r\n", argv[0]);
		return 0;
	}
	//the argument "path" of WifiCmd_GetData is the string prior to '?'
	*pch = 0;
	buf_size = sizeof(buf);
//	memset(buf,0,sizeof(buf));
	buf_size--; //reserve the first for continue flag
	ret = WifiCmd_GetData(argv[0], pch + 1, (UINT32)buf + 1, (UINT32 *)&buf_size, mimeType, index);
	buf_size++;//add first byte
	if (ret == 0) { ///not continue data
		index = 0;
		memset(buf, WIFIRET_LAST_DATA, 1); ////set continue flag: 1 means last data
	} else if (ret == 1) { /// continue data
		index ++;
		memset(buf, WIFIRET_CONTINUE_DATA, 1); ////set continue flag:2 means continue data
	} else {
		index = 0;
		buf_size = 0;
		DBG_ERR("WifiCmd_GetDataerror ret=%d\r\n", ret);
	}
	DBG_IND("buf_size=%d\r\n", buf_size);
	NvtUctrl_SetConfigData((void *)&buf, buf_size);
	snprintf(RetStr, sizeof(RetStr), "%d", buf_size);
	NvtUctrl_SetRetString(RetStr);
	if (THIS_DBGLVL > 1) {
		*(buf + sizeof(buf) - 1)  = 0;
		DBG_DUMP("XML Return:\r\n%s\r\n", buf);
	}
#else
	debug_msg("_TODO -> %s:%d, task(%x,%s) caller(%pF)\r\n", __FUNCTION__, __LINE__,
			  OS_GetTaskID(), OS_GetTaskName(), __builtin_return_address(0));


#endif
	return 0;
}
//#NT#2016/10/18#Ben Wang -end

#if(IPCAM_FUNC==ENABLE)

static UINT32 get_supported_sample_rate_bitmap(AUDIO_SR SampleRate)
{
	UINT32 bitmap;
	if (SampleRate == AUDIO_SR_8000)
		bitmap = AU_INFO_SAMPE_RATE_8K;
	else if (SampleRate == AUDIO_SR_11025)
		bitmap = AU_INFO_SAMPE_RATE_8K | AU_INFO_SAMPE_RATE_11K;
	else if (SampleRate == AUDIO_SR_12000)
		bitmap = AU_INFO_SAMPE_RATE_8K | AU_INFO_SAMPE_RATE_11K | AU_INFO_SAMPE_RATE_12K;
	else if (SampleRate == AUDIO_SR_16000)
		bitmap = AU_INFO_SAMPE_RATE_8K | AU_INFO_SAMPE_RATE_11K | AU_INFO_SAMPE_RATE_12K | AU_INFO_SAMPE_RATE_16K;
	else if (SampleRate == AUDIO_SR_22050)
		bitmap = AU_INFO_SAMPE_RATE_8K | AU_INFO_SAMPE_RATE_11K | AU_INFO_SAMPE_RATE_12K | AU_INFO_SAMPE_RATE_16K |
				AU_INFO_SAMPE_RATE_22K;
	else if (SampleRate == AUDIO_SR_24000)
		bitmap = AU_INFO_SAMPE_RATE_8K | AU_INFO_SAMPE_RATE_11K | AU_INFO_SAMPE_RATE_12K | AU_INFO_SAMPE_RATE_16K |
				AU_INFO_SAMPE_RATE_22K | AU_INFO_SAMPE_RATE_24K;
	else if (SampleRate == AUDIO_SR_32000)
		bitmap = AU_INFO_SAMPE_RATE_8K | AU_INFO_SAMPE_RATE_11K | AU_INFO_SAMPE_RATE_12K | AU_INFO_SAMPE_RATE_16K |
				AU_INFO_SAMPE_RATE_22K | AU_INFO_SAMPE_RATE_24K | AU_INFO_SAMPE_RATE_32K;
	else if (SampleRate == AUDIO_SR_44100)
		bitmap = AU_INFO_SAMPE_RATE_8K | AU_INFO_SAMPE_RATE_11K | AU_INFO_SAMPE_RATE_12K | AU_INFO_SAMPE_RATE_16K |
				AU_INFO_SAMPE_RATE_22K | AU_INFO_SAMPE_RATE_24K | AU_INFO_SAMPE_RATE_32K | AU_INFO_SAMPE_RATE_44K;
	else if (SampleRate == AUDIO_SR_48000)
		bitmap = AU_INFO_SAMPE_RATE_8K | AU_INFO_SAMPE_RATE_11K | AU_INFO_SAMPE_RATE_12K | AU_INFO_SAMPE_RATE_16K |
				AU_INFO_SAMPE_RATE_22K | AU_INFO_SAMPE_RATE_24K | AU_INFO_SAMPE_RATE_32K | AU_INFO_SAMPE_RATE_44K |
				AU_INFO_SAMPE_RATE_48K;
	else
		bitmap = 0;
	return bitmap;
}

static UINT32 get_supported_bit_bitmap(UINT32 bits)
{
	UINT32 bitmap;
	if (bits == 8)
		bitmap = AU_INFO_SAMPE_SIZE_8BIT;
	else if (bits == 16)
		bitmap = AU_INFO_SAMPE_SIZE_16BIT;
	else if (bits == 32)
		bitmap = AU_INFO_SAMPE_SIZE_32BIT;
	else
		bitmap = 0;
	return bitmap;
}

static UINT32 get_supported_channels_bitmap(AUDIO_CH channels)
{
	UINT32 bitmap;
	if (channels == AUDIO_CH_STEREO)
		bitmap = AU_INFO_CHANNEL_LEFT | AU_INFO_CHANNEL_RIGHT | AU_INFO_CHANNEL_STEREO;
	else
		bitmap = AU_INFO_CHANNEL_LEFT | AU_INFO_CHANNEL_RIGHT;
	return bitmap;
}
#endif


static INT32 param_sysinfo(INT32 argc, char *argv[])
{
#if(IPCAM_FUNC==ENABLE)
	char tmpStr[256];
	SYS_INFO sSysInfo = {0};
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_SENSOR_INFO *SensorInfo = UIAppIPCam_get_SensorInfo();
	UIAPPIPCAM_BLOCK_BUFFER_INFO  *pBlockConfig;
	FPS_PARAM fps_info = {0};
	UINT32 SensorId, ImeId, mode;
	NVTMPP_VB_CONF_S vb_conf = {0};
	WAVSTUD_INFO_SET *pAudioInfo_max = UIAppIPCam_get_AudioInMaxInfo();
	WAVSTUD_INFO_SET *pAudioOutInfo_max = UIAppIPCam_get_AudioOutMaxInfo();

	memset(&sSysInfo, 0, sizeof(SYS_INFO));

	STATIC_ASSERT(SENSOR_ID_1 == IPCAM_SENSOR1); //Ensure both definitions have the same value

	//The CapInfo width/height means the max resolution that can be achieved.
	// Those values should be the max resolution of frame reserved for IPL outputting,
	// and are used for buffer allocation in IPCamExe_InitCommonMem()
	pBlockConfig = UIAppIPCam_get_BlockBufferInfo();
	mode = IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_PRVFLOW);

	strncpy(sSysInfo.EnvInfo.SensorName, ProductSensorName, MAX_ENV_INFO_STRING_LEN);
	snprintf(tmpStr, sizeof(tmpStr), "%s,%s,%s\n", ProductDisplay1Name, ProductDisplay2Name ,ProductDisplay3Name);
	strncpy(sSysInfo.EnvInfo.DisplayName, ProductDisplay3Name, MAX_ENV_INFO_STRING_LEN);
	strncpy(sSysInfo.EnvInfo.EmbeddedMemoryName, ProductEmbeddedMemoryName, MAX_ENV_INFO_STRING_LEN);
	strncpy(sSysInfo.EnvInfo.DSPFeature, ProductDSPFeatureName, MAX_ENV_INFO_STRING_LEN);
	snprintf(sSysInfo.EnvInfo.BuildDate, MAX_ENV_INFO_STRING_LEN, __DATE__","__TIME__);

	sSysInfo.EnvInfo.direct_mode = (mode == ICF_FLOW_DIRECT);
	sSysInfo.EnvInfo.mem_partition[0].addr = _BOARD_IPC_ADDR_;
	sSysInfo.EnvInfo.mem_partition[0].size = _BOARD_IPC_SIZE_;
	sSysInfo.EnvInfo.mem_partition[1].addr = _BOARD_LINUX_ADDR_;
	sSysInfo.EnvInfo.mem_partition[1].size = _BOARD_LINUX_SIZE_;
	sSysInfo.EnvInfo.mem_partition[2].addr = _BOARD_UBOOT_ADDR_;
	sSysInfo.EnvInfo.mem_partition[2].size = _BOARD_UBOOT_SIZE_;
	sSysInfo.EnvInfo.mem_partition[3].addr = _BOARD_DSP1_ADDR_;
	sSysInfo.EnvInfo.mem_partition[3].size = _BOARD_DSP1_SIZE_;
	sSysInfo.EnvInfo.mem_partition[4].addr = _BOARD_UITRON_ADDR_;
	sSysInfo.EnvInfo.mem_partition[4].size = _BOARD_UITRON_SIZE_;
	sSysInfo.EnvInfo.mem_partition[5].addr = _BOARD_RAMDISK_ADDR_;
	sSysInfo.EnvInfo.mem_partition[5].size = _BOARD_RAMDISK_SIZE_;
	sSysInfo.EnvInfo.mem_partition[6].addr = _BOARD_LOADER_ADDR_;
	sSysInfo.EnvInfo.mem_partition[6].size = _BOARD_LOADER_SIZE_;
	sSysInfo.EnvInfo.mem_partition[7].addr = 0;
	sSysInfo.EnvInfo.mem_partition[7].size = 0;

	extern UINT32  nvtmpp_heap_get_free_size(NVTMPP_DDR ddr);
	extern UINT32  nvtmpp_heap_get_max_free_block_size(NVTMPP_DDR ddr);

	if (NVTMPP_ER_OK == nvtmpp_vb_get_conf(&vb_conf)) {
		sSysInfo.EnvInfo.mem_status[0].addr = vb_conf.ddr_mem[NVTMPP_DDR_1].Addr;
		sSysInfo.EnvInfo.mem_status[0].size = vb_conf.ddr_mem[NVTMPP_DDR_1].Size;
		sSysInfo.EnvInfo.mem_status[0].free = nvtmpp_heap_get_free_size(NVTMPP_DDR_1);
		sSysInfo.EnvInfo.mem_status[0].max_free_block = nvtmpp_heap_get_max_free_block_size(NVTMPP_DDR_1);
		sSysInfo.EnvInfo.mem_status[1].addr = vb_conf.ddr_mem[NVTMPP_DDR_1].Addr;
		sSysInfo.EnvInfo.mem_status[1].size = vb_conf.ddr_mem[NVTMPP_DDR_1].Size;
		sSysInfo.EnvInfo.mem_status[1].free = nvtmpp_heap_get_free_size(NVTMPP_DDR_1);
		sSysInfo.EnvInfo.mem_status[1].max_free_block = nvtmpp_heap_get_max_free_block_size(NVTMPP_DDR_1);
	}

	extern int IPCam_GetFDFaceScale(void);
	sSysInfo.EnvInfo.fd_facescale = IPCam_GetFDFaceScale();

	sSysInfo.EnvInfo.define_product = IPCAM_PRODUCT;
	sSysInfo.EnvInfo.define_new_trigger = 0;
	sSysInfo.EnvInfo.define_ipl_direct = IPCAM_IPL_DIRECT;
	sSysInfo.EnvInfo.define_ipl_3dnr = IPCAM_IPL_3DNR;
	sSysInfo.EnvInfo.define_enc_3dnr = IPCAM_ENC_3DNR;
	sSysInfo.EnvInfo.define_disp_wait = IPCAM_DISP_WAIT;
	sSysInfo.EnvInfo.define_dis = IPCAM_DIS_COMP_FUNC;// IPCAM_DIS_MV_FUNC ??
	sSysInfo.EnvInfo.define_md_func = IPCAM_MD_FUNC;
	sSysInfo.EnvInfo.define_smartroi = IPCAM_SMARTROI_FUNC;
	sSysInfo.EnvInfo.define_vqa_func = IPCAM_VQA_FUNC;
	sSysInfo.EnvInfo.define_pd_func = IPCAM_PD_FUNC;
	sSysInfo.EnvInfo.define_fd_func = IPCAM_FD_FUNC;
	sSysInfo.EnvInfo.define_aec_func = IPCAM_AEC_FUNC;
	sSysInfo.EnvInfo.define_anr_func = IPCAM_ANR_FUNC;
	sSysInfo.EnvInfo.define_ltr_func = IPCAM_LONG_TERM_REF_FUNC;

	sSysInfo.EnvInfo.define_wdr_func = IPCAM_WDR_FUNC;
	sSysInfo.EnvInfo.define_hdr_func = IPCAM_SHDR_FUNC;
	sSysInfo.EnvInfo.define_ftg_func = IPCAM_FTG_FUNC;
	sSysInfo.EnvInfo.define_buf_level = IPCAM_BUFFER_LEVEL;
	sSysInfo.EnvInfo.md_path = IPCAM_MD_PATH;

	sSysInfo.EnvInfo.audio_enc_pcm = 1;
	sSysInfo.EnvInfo.audio_enc_aac = AUDIO_FUNC_AAC;
	sSysInfo.EnvInfo.audio_enc_alaw = 1;
	sSysInfo.EnvInfo.audio_enc_ulaw = 1;
	sSysInfo.EnvInfo.audio_dec_pcm = 1;
	sSysInfo.EnvInfo.audio_dec_aac = 0;
	sSysInfo.EnvInfo.audio_dec_alaw = 1;
	sSysInfo.EnvInfo.audio_dec_ulaw = 1;

	sSysInfo.EnvInfo.max_video_buf_time_ms = IPCAM_VDOENC_MAX_BUF_MS;
	sSysInfo.EnvInfo.max_audio_in_buf_time_ms = pAudioInfo_max->bufCount * pAudioInfo_max->unitTime * 100;
	sSysInfo.EnvInfo.max_audio_out_buf_time_ms = pAudioOutInfo_max->bufCount * pAudioOutInfo_max->unitTime * 100;

	SensorId = pStreamInfo[0].sensor;
	ImeId = pStreamInfo[0].ime;
	sensor_get_info(SensorId, SEN_GET_FPS, (void *)&fps_info);
	sSysInfo.CapInfo[0].Valid = IPCAM_MAIN_ON_OFF;
	sSysInfo.CapInfo[0].Width = IPCAM_MAIN_WIDTH;
	sSysInfo.CapInfo[0].Height = IPCAM_MAIN_HEIGHT;
	sSysInfo.CapInfo[0].FrameRate = 30;//fps_info.dft_fps;
	sSysInfo.CapInfo[0].ScanMethod = 1; // 1=progressive
	sSysInfo.CapInfo[0].Feature = IPCAM_MAIN_ON_OFF ? 1 : 0;    // 1=encode only
	sSysInfo.CapInfo[0].MaxByteRate = IPCAM_MAIN_MAX_BYTES;
	sSysInfo.CapInfo[0].SensorId = SensorId;
	sSysInfo.CapInfo[0].SensorFrontFps = SensorInfo[SensorId].uiFrontFPS;
	sSysInfo.CapInfo[0].ImeId = ImeId;
	sSysInfo.CapInfo[0].ImeCompressed = SensorInfo[SensorId].bIME1Compress;
	sSysInfo.CapInfo[0].Ime3DNR = SensorInfo[SensorId].bIME3DNR;
	sSysInfo.CapInfo[0].ImeFps = SensorInfo[SensorId].uiIMEFPS[ImeId];
	sSysInfo.CapInfo[0].BufferCount = pBlockConfig[0].count;
	sSysInfo.CapInfo[0].PathId = pStreamInfo[0].path;

	SensorId = pStreamInfo[1].sensor;
	ImeId = pStreamInfo[1].ime;
	sensor_get_info(SensorId, SEN_GET_FPS, (void *)&fps_info);
	sSysInfo.CapInfo[1].Valid = IPCAM_DISP_ON_OFF;
	sSysInfo.CapInfo[1].Width = IPCAM_DISP_WIDTH;
	sSysInfo.CapInfo[1].Height = IPCAM_DISP_HEIGHT;
	sSysInfo.CapInfo[1].FrameRate = 30;//fps_info.dft_fps;
	sSysInfo.CapInfo[1].ScanMethod = 1; // 1=progressive
	sSysInfo.CapInfo[1].Feature = IPCAM_DISP_ON_OFF ? 2 : 0;    // 2=liveview only
	sSysInfo.CapInfo[1].MaxByteRate = 0;
	sSysInfo.CapInfo[1].SensorId = SensorId;
	sSysInfo.CapInfo[1].SensorFrontFps = SensorInfo[SensorId].uiFrontFPS;
	sSysInfo.CapInfo[1].ImeId = ImeId;
	sSysInfo.CapInfo[1].ImeCompressed = SensorInfo[SensorId].bIME1Compress;
	sSysInfo.CapInfo[1].Ime3DNR = SensorInfo[SensorId].bIME3DNR;
	sSysInfo.CapInfo[1].ImeFps = SensorInfo[SensorId].uiIMEFPS[ImeId];
	sSysInfo.CapInfo[1].BufferCount = pBlockConfig[1].count;
	sSysInfo.CapInfo[1].PathId = pStreamInfo[1].path;

	SensorId = pStreamInfo[2].sensor;
	ImeId = pStreamInfo[2].ime;
	sensor_get_info(SensorId, SEN_GET_FPS, (void *)&fps_info);
	sSysInfo.CapInfo[2].Valid = IPCAM_SUB1_ON_OFF;
	sSysInfo.CapInfo[2].Width = IPCAM_SUB1_WIDTH;
	sSysInfo.CapInfo[2].Height = IPCAM_SUB1_HEIGHT;
	sSysInfo.CapInfo[2].FrameRate = 30;//fps_info.dft_fps;
	sSysInfo.CapInfo[2].ScanMethod = 1; // 1=progressive
	sSysInfo.CapInfo[2].Feature = IPCAM_SUB1_ON_OFF ? 1 : 0;    // 1=encode only
	sSysInfo.CapInfo[2].MaxByteRate = IPCAM_SUB1_MAX_BYTES;
	sSysInfo.CapInfo[2].SensorId = SensorId;
	sSysInfo.CapInfo[2].SensorFrontFps = SensorInfo[SensorId].uiFrontFPS;
	sSysInfo.CapInfo[2].ImeId = pStreamInfo[2].ime;
	sSysInfo.CapInfo[2].ImeCompressed = SensorInfo[SensorId].bIME1Compress;
	sSysInfo.CapInfo[2].Ime3DNR = SensorInfo[SensorId].bIME3DNR;
	sSysInfo.CapInfo[2].ImeFps = SensorInfo[SensorId].uiIMEFPS[ImeId];
	sSysInfo.CapInfo[2].BufferCount = pBlockConfig[2].count;
	sSysInfo.CapInfo[2].PathId = pStreamInfo[2].path;

	SensorId = pStreamInfo[3].sensor;
	ImeId = pStreamInfo[3].ime;
	sensor_get_info(SensorId, SEN_GET_FPS, (void *)&fps_info);
	sSysInfo.CapInfo[3].Valid = IPCAM_SUB2_ON_OFF;
	sSysInfo.CapInfo[3].Width = IPCAM_SUB2_WIDTH;
	sSysInfo.CapInfo[3].Height = IPCAM_SUB2_HEIGHT;
	sSysInfo.CapInfo[3].FrameRate = 30;//fps_info.dft_fps;
	sSysInfo.CapInfo[3].ScanMethod = 1; // 1=progressive
	sSysInfo.CapInfo[3].Feature = IPCAM_SUB2_ON_OFF ? 1 : 0;    // 1=encode only
	sSysInfo.CapInfo[3].MaxByteRate = IPCAM_SUB2_MAX_BYTES;
	sSysInfo.CapInfo[3].SensorId = SensorId;
	sSysInfo.CapInfo[3].SensorFrontFps = SensorInfo[SensorId].uiFrontFPS;
	sSysInfo.CapInfo[3].ImeId = ImeId;
	sSysInfo.CapInfo[3].ImeCompressed = SensorInfo[SensorId].bIME1Compress;
	sSysInfo.CapInfo[3].Ime3DNR = SensorInfo[SensorId].bIME3DNR;
	sSysInfo.CapInfo[3].ImeFps = SensorInfo[SensorId].uiIMEFPS[ImeId];
	sSysInfo.CapInfo[3].BufferCount = pBlockConfig[3].count;
	sSysInfo.CapInfo[3].PathId = pStreamInfo[3].path;


	SensorId = pStreamInfo[4].sensor;
	ImeId = pStreamInfo[4].ime;
	sensor_get_info(SensorId, SEN_GET_FPS, (void *)&fps_info);
	sSysInfo.CapInfo[4].Valid = IPCAM_SUB3_ON_OFF;
	sSysInfo.CapInfo[4].Width = IPCAM_SUB3_WIDTH;
	sSysInfo.CapInfo[4].Height = IPCAM_SUB3_HEIGHT;
	sSysInfo.CapInfo[4].FrameRate = 30;//fps_info.dft_fps;
	sSysInfo.CapInfo[4].ScanMethod = 1; // 1=progressive
	sSysInfo.CapInfo[4].Feature = IPCAM_SUB3_ON_OFF ? 1 : 0;    // 1=encode only
	sSysInfo.CapInfo[4].MaxByteRate = IPCAM_SUB3_MAX_BYTES;
	sSysInfo.CapInfo[4].SensorId = SensorId;
	sSysInfo.CapInfo[4].SensorFrontFps = SensorInfo[SensorId].uiFrontFPS;
	sSysInfo.CapInfo[4].ImeId = ImeId;
	sSysInfo.CapInfo[4].ImeCompressed = SensorInfo[SensorId].bIME1Compress;
	sSysInfo.CapInfo[4].Ime3DNR = SensorInfo[SensorId].bIME3DNR;
	sSysInfo.CapInfo[4].ImeFps = SensorInfo[SensorId].uiIMEFPS[ImeId];
	sSysInfo.CapInfo[4].BufferCount = pBlockConfig[4].count;
	sSysInfo.CapInfo[4].PathId = pStreamInfo[4].path;

#if SENSOR2
	SensorId = pStreamInfo[5].sensor;
	ImeId = pStreamInfo[5].ime;
	sensor_get_info(SensorId, SEN_GET_FPS, (void *)&fps_info);
	sSysInfo.CapInfo[5].Valid = SENSOR2_MAIN_ON_OFF;
	sSysInfo.CapInfo[5].Width = SENSOR2_MAIN_WIDTH;
	sSysInfo.CapInfo[5].Height = SENSOR2_MAIN_HEIGHT;
	sSysInfo.CapInfo[5].FrameRate = 30;//fps_info.dft_fps;
	sSysInfo.CapInfo[5].ScanMethod = 1; // 1=progressive
	sSysInfo.CapInfo[5].Feature = SENSOR2_MAIN_ON_OFF ? 1 : 0;    // 1=encode only
	sSysInfo.CapInfo[5].MaxByteRate = SENSOR2_MAIN_MAX_BYTES;
	sSysInfo.CapInfo[5].SensorId = SensorId;
	sSysInfo.CapInfo[5].SensorFrontFps = SensorInfo[SensorId].uiFrontFPS;
	sSysInfo.CapInfo[5].ImeId = ImeId;
	sSysInfo.CapInfo[5].ImeCompressed = SensorInfo[SensorId].bIME1Compress;
	sSysInfo.CapInfo[5].Ime3DNR = SensorInfo[SensorId].bIME3DNR;
	sSysInfo.CapInfo[5].ImeFps = SensorInfo[SensorId].uiIMEFPS[ImeId];
	sSysInfo.CapInfo[5].BufferCount = pBlockConfig[5].count;
	sSysInfo.CapInfo[5].PathId = pStreamInfo[5].path;

	SensorId = pStreamInfo[6].sensor;
	ImeId = pStreamInfo[6].ime;
	sensor_get_info(SensorId, SEN_GET_FPS, (void *)&fps_info);
	sSysInfo.CapInfo[6].Valid = SENSOR2_SUB1_ON_OFF;
	sSysInfo.CapInfo[6].Width = SENSOR2_SUB1_WIDTH;
	sSysInfo.CapInfo[6].Height = SENSOR2_SUB1_HEIGHT;
	sSysInfo.CapInfo[6].FrameRate = 30;//fps_info.dft_fps;
	sSysInfo.CapInfo[6].ScanMethod = 1; // 1=progressive
	sSysInfo.CapInfo[6].Feature = SENSOR2_SUB1_ON_OFF ? 1 : 0;    // 1=encode only
	sSysInfo.CapInfo[6].MaxByteRate = SENSOR2_SUB1_MAX_BYTES;
	sSysInfo.CapInfo[6].SensorId = SensorId;
	sSysInfo.CapInfo[6].SensorFrontFps = SensorInfo[SensorId].uiFrontFPS;
	sSysInfo.CapInfo[6].ImeId = ImeId;
	sSysInfo.CapInfo[6].ImeCompressed = SensorInfo[SensorId].bIME1Compress;
	sSysInfo.CapInfo[6].Ime3DNR = SensorInfo[SensorId].bIME3DNR;
	sSysInfo.CapInfo[6].ImeFps = SensorInfo[SensorId].uiIMEFPS[ImeId];
	sSysInfo.CapInfo[6].BufferCount = pBlockConfig[6].count;
	sSysInfo.CapInfo[6].PathId = pStreamInfo[6].path;

	SensorId = pStreamInfo[7].sensor;
	ImeId = pStreamInfo[7].ime;
	sensor_get_info(SensorId, SEN_GET_FPS, (void *)&fps_info);
	sSysInfo.CapInfo[7].Valid = SENSOR2_SUB2_ON_OFF;
	sSysInfo.CapInfo[7].Width = SENSOR2_SUB2_WIDTH;
	sSysInfo.CapInfo[7].Height = SENSOR2_SUB2_HEIGHT;
	sSysInfo.CapInfo[7].FrameRate = 30;//fps_info.dft_fps;
	sSysInfo.CapInfo[7].ScanMethod = 1; // 1=progressive
	sSysInfo.CapInfo[7].Feature = SENSOR2_SUB2_ON_OFF ? 1 : 0;    // 1=encode only
	sSysInfo.CapInfo[7].MaxByteRate = SENSOR2_SUB2_MAX_BYTES;
	sSysInfo.CapInfo[7].SensorId = SensorId;
	sSysInfo.CapInfo[7].SensorFrontFps = SensorInfo[SensorId].uiFrontFPS;
	sSysInfo.CapInfo[7].ImeId = ImeId;
	sSysInfo.CapInfo[7].ImeCompressed = SensorInfo[SensorId].bIME1Compress;
	sSysInfo.CapInfo[7].Ime3DNR = SensorInfo[SensorId].bIME3DNR;
	sSysInfo.CapInfo[7].ImeFps = SensorInfo[SensorId].uiIMEFPS[ImeId];
	sSysInfo.CapInfo[7].BufferCount = pBlockConfig[7].count;
	sSysInfo.CapInfo[7].PathId = pStreamInfo[7].path;
#endif

	/* Collect LCD information*/
	sSysInfo.LCDInfo[0].Valid = 1;
	sSysInfo.LCDInfo[0].Width = 720;
	sSysInfo.LCDInfo[0].Height = 576;
	sSysInfo.LCDInfo[0].VoutId = 0;


	/* Collect Audio-In information*/
	sSysInfo.AuInInfo[0].Valid = 1;

	sSysInfo.AuInInfo[0].SampleRate_Support_bitmap = get_supported_sample_rate_bitmap(pAudioInfo_max->audInfo.audSR);
	sSysInfo.AuInInfo[0].SampleSize_Support_bitmap = get_supported_bit_bitmap(pAudioInfo_max->audInfo.bitPerSamp);
	sSysInfo.AuInInfo[0].Channel_Support_bitmap = get_supported_channels_bitmap(pAudioInfo_max->audInfo.ch_num);
	sSysInfo.AuInInfo[0].Source = 7;

	/* Collect Audio-Out information*/
	sSysInfo.AuOutInfo[0].Valid = 1;
	sSysInfo.AuOutInfo[0].SampleRate_Support_bitmap = get_supported_sample_rate_bitmap(pAudioOutInfo_max->audInfo.audSR);
	sSysInfo.AuOutInfo[0].SampleSize_Support_bitmap = get_supported_bit_bitmap(pAudioOutInfo_max->audInfo.bitPerSamp);
	sSysInfo.AuOutInfo[0].Channel_Support_bitmap = get_supported_channels_bitmap(pAudioOutInfo_max->audInfo.ch_num);
	sSysInfo.AuOutInfo[0].Source = 0;


	NvtUctrl_SetConfigData((void *)&sSysInfo, sizeof(SYS_INFO));
	snprintf(tmpStr, sizeof(tmpStr), "Get Sysinfo OK\n");
	NvtUctrl_SetRetString(tmpStr);

#endif

	return 0;
}



static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_sys_param[] = {
	{"fwupdate", param_fwupdate},
	{"shutdown", param_shutdown},
	{"reboot", param_reboot},
	{"notify", param_notify},
	{"builddate", param_builddate},
	{"core2fwaddr", param_core2fwaddr},
	{"core2fwready", param_core2fwready},
	{"core2fwupflag", param_core2fwupflag},
	{"portset", param_portset},
	{"rtsp_portget", param_rtsp_portget},
	{"app_disable", param_app_disable},
	{"wificmdinit", param_wificmdinit},
	{"wificmd", param_wificmd},
	{"msdcnvt", param_msdcnvt},
	{"hotplugmem", param_dynamic_mem}, //// get dynamic mem for linux
	{"sysinfo", param_sysinfo},

	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_sys = {
	"usys",
	tbl_sys_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#if SXCMD_TEST

static BOOL Cmd_usys_set(CHAR *strCmd)
{
	uToken_Parse(strCmd, &module_sys);
	return TRUE;
}

SXCMD_BEGIN(usys, "usys")
SXCMD_ITEM("set %", Cmd_usys_set, "")
SXCMD_END()

#endif

void UCtrlAppSys_InstallCmd(void)
{
#if SXCMD_TEST
	SxCmd_AddTable(usys);
#endif
}

void UCtrlAppSys_InstallID(void)
{
	OS_CONFIG_FLAG(UCTRLAPPSYS_FLG_ID);
}

void UCtrlAppSys_WaitNetInitFinish(void)
{
	if (UCTRLAPPSYS_FLG_ID == 0) {
		DBG_ERR("need to UCtrlAppSys_InstallID()\r\n");
		return;
	}

	FLGPTN FlgPtn;
	wai_flg(&FlgPtn, UCTRLAPPSYS_FLG_ID, UCTRLAPPSYS_FLG_NET_INITED, TWF_ORW | TWF_CLR);
}
