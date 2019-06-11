#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIControl.h"
#include "UCtrlAppDisp.h"
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "GxVideo.h"
#include "dal_disp.h"
#include "Delay.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppSetup
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define SXCMD_TEST          0

typedef struct _UCtrlAppDispParmType {
	UINT32 isSet;
	UINT32 index;
} UCtrlAppDispParmType;

typedef struct _UCtrlAppDispInfoType {
	BOOL                    isErr;
} UCtrlAppDispInfoType;

#if(LINUX_NVT_DISP == ENABLE)
static UCtrlAppDispInfoType   dispInfo;
static UCtrlAppDispInfoType  *pDispInfo = &dispInfo;
#define ipc_getPhyAddr(addr) ((addr) & 0x1FFFFFFF)
#define ipc_getNonCacheAddr(addr) (((addr) & 0x1FFFFFFF)|0xA0000000)

static INT32 param_mem_alloc(INT32 argc, char *argv[])
{
	UINT32 DeviceID;
	UINT32 LyerID;
	DispMemSetting Setting = {0};
	char retString[64];

	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid DeviceID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		snprintf(retString, sizeof(retString), "no dev");
		NvtUctrl_SetRetString(retString);
		return -1;
	}

	if ((LyerID = atoi(argv[1])) >= NVTDISP_LYR_NUM) {
		DBG_ERR("invalid LyerID: %u (Max: %d)\r\n", LyerID, NVTDISP_LYR_NUM);
		return -1;
	}

	if(LyerID!=DAL_DISP_LAYER_OSD1) {
		DBG_ERR("no mem LyerID: %d)\r\n", LyerID);
		snprintf(retString, sizeof(retString), "no memd");
		NvtUctrl_SetRetString(retString);
		return -1;
	}

	Setting.addr = ipc_getPhyAddr(OS_GetMempoolAddr(POOL_ID_DISP_OSD1));
	Setting.size = OS_GetMempoolSize(POOL_ID_DISP_OSD1);

	NvtUctrl_SetConfigData((void *)&Setting, sizeof(DispMemSetting));
	return 0;
}

static INT32 param_dsize_get(INT32 argc, char *argv[])
{
	UINT32 DeviceID;
	ISIZE Setting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid DeviceID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		return -1;
	}

	Setting=GxVideo_GetDeviceSize(DeviceID);

	NvtUctrl_SetConfigData((void *)&Setting, sizeof(ISIZE));

	return 0;
}

static INT32 param_init(INT32 argc, char *argv[])
{
	UINT32 DeviceID;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid DeviceID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		return -1;
	}

	return dal_disp_init(DeviceID);
}

static INT32 param_uninit(INT32 argc, char *argv[])
{
	UINT32 DeviceID;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid DeviceID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		return -1;
	}

	return dal_disp_uninit(DeviceID);
}
static INT32 param_bufaddr_set(INT32 argc, char *argv[])
{
	UINT32 DeviceID;
	UINT32 LyerID;
	DispBufAddrSetting Setting = {0};
	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid DeviceID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		return -1;
	}

	if ((LyerID = atoi(argv[1])) >= NVTDISP_LYR_NUM) {
		DBG_ERR("invalid LyerID: %u (Max: %d)\r\n", LyerID, NVTDISP_LYR_NUM);
		return -1;
	}

	if(NvtUctrl_GetConfigData((void *)&Setting, sizeof(DispBufAddrSetting))==NVTUCTRL_RET_OK)
		return dal_disp_setlayerbufferaddr(DeviceID,LyerID,Setting.addr1,Setting.addr2,Setting.addr3,Setting.wait);
	else
		return -1;
}

static INT32 param_scalewin_set(INT32 argc, char *argv[])
{
	UINT32 DeviceID;
	UINT32 LyerID;
	DispScaleWinSetting Setting = {0};

	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid DeviceID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		return -1;
	}

	if ((LyerID = atoi(argv[1])) >= NVTDISP_LYR_NUM) {
		DBG_ERR("invalid LyerID: %u (Max: %d)\r\n", LyerID, NVTDISP_LYR_NUM);
		return -1;
	}

	if(NvtUctrl_GetConfigData((void *)&Setting, sizeof(DispScaleWinSetting))==NVTUCTRL_RET_OK)
		return dal_disp_setlayerscalewindow(DeviceID,LyerID,Setting.x,Setting.y,Setting.w,Setting.h);
	else
		return -1;
}

static INT32 param_buffmt_set(INT32 argc, char *argv[])
{
	UINT32 DeviceID;
	UINT32 LyerID;
	DispBufFmtSetting Setting = {0};

	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid LyerID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		return -1;
	}

	if ((LyerID = atoi(argv[1])) >= NVTDISP_LYR_NUM) {
		DBG_ERR("invalid LyerID: %u (Max: %d)\r\n", LyerID, NVTDISP_LYR_NUM);
		return -1;
	}

	if(NvtUctrl_GetConfigData((void *)&Setting, sizeof(DispBufFmtSetting))==NVTUCTRL_RET_OK)
		return dal_disp_setlayerbufferformat(DeviceID,LyerID,Setting.format,Setting.w,Setting.h,Setting.loff);
	else
		return -1;
}

static INT32 param_get(INT32 argc, char *argv[])
{
	UINT32 DeviceID;
	UINT32 LyerID;
	UINT32 param;
	UINT32 value =0;

	if (argc != 3) {
		DBG_ERR("argc!=3 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid DeviceID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		return -1;
	}

	if ((LyerID = atoi(argv[1])) >= NVTDISP_LYR_NUM) {
		DBG_ERR("invalid LyerID: %u (Max: %d)\r\n", LyerID, NVTDISP_LYR_NUM);
		return -1;
	}

	param = atoi(argv[2]);

	dal_disp_getlayer(DeviceID,LyerID,param,&value);

	return NvtUctrl_SetConfigData((void *)&value, sizeof(UINT32));
}

static INT32 param_set(INT32 argc, char *argv[])
{
	UINT32 DeviceID;
	UINT32 LyerID;
	UINT32 param;
	UINT32 value;

	if (argc != 4) {
		DBG_ERR("argc!=4 (%d)\r\n", argc);
		return -1;
	}

	if ((DeviceID = atoi(argv[0])) >= NVTDISP_DOUT_NUM) {
		DBG_ERR("invalid DeviceID: %u (Max: %d)\r\n", DeviceID, NVTDISP_DOUT_NUM);
		return -1;
	}

	if ((LyerID = atoi(argv[1])) >= NVTDISP_LYR_NUM) {
		DBG_ERR("invalid LyerID: %u (Max: %d)\r\n", LyerID, NVTDISP_LYR_NUM);
		return -1;
	}

	param= atoi(argv[2]);
	value= atoi(argv[3]);

	return dal_disp_setlayer(DeviceID,LyerID,param,value);
}

static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
	memset(pDispInfo, 0, sizeof(UCtrlAppDispInfoType));
	return 0;
}

static INT32 param_end(UTOKEN_END_INFO *p_info)
{
	char retStr[64];

	if (p_info->err_code < 0) {
		snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return p_info->err_code;
	}

	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_disp_param[] = {

	{"malloc", 		param_mem_alloc},
	{"dsizeGet", 	param_dsize_get},
	{"init", 		param_init},
	{"uninit", 		param_uninit},
	{"bufaddrSet", 	param_bufaddr_set},
	{"scalewinSet", param_scalewin_set},
	{"buffmtSet", 	param_buffmt_set},
	{"Get", 		param_get},
	{"Set", 		param_set},

	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_disp = {
	"udisp",
	tbl_disp_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#if SXCMD_TEST
static BOOL Cmd_udisp_set(CHAR *strCmd)
{

	uToken_Parse(strCmd, &module_setup);
	return TRUE;
}

SXCMD_BEGIN(udisp, "udisp")
SXCMD_ITEM("memSet %", Cmd_udisp_set, "")
SXCMD_END()

#endif

void UCtrlAppDisp_InstallCmd(void)
{
#if SXCMD_TEST
	SxCmd_AddTable(udisp);
#endif
}

#endif
