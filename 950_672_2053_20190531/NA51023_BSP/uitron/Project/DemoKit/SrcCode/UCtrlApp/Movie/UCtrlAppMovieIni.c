#include "SysCommon.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIAppMovie.h"
#include "UCtrlAppMovie.h"

#if(IPCAM_MODE == ENABLE)
#include "UIAppIPCam_Param.h"
#else
#include "UIAppMovie_Param.h"
#endif

#define THIS_DBGLVL         6 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppMovieIni
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if (IPCAM_PARAM_INI == ENABLE)

UINT32 _SECTION(".kercfg_data") UCTRLAPPMOVIEINI_FLG_ID_0;

void UCtrlAppMovieIni_InstallID(void)
{
	OS_CONFIG_FLAG(UCTRLAPPMOVIEINI_FLG_ID_0);
}

void UCtrlAppMovieIni_WaitFinish(void)
{
	FLGPTN flag;
	wai_flg(&flag, UCTRLAPPMOVIEINI_FLG_ID_0, 0x1, TWF_ORW | TWF_CLR);
}

static INT32 param_begin(char *str, INT32 len)
{
	//DBG_IND("get: %s\r\n",str);
	return 0;
}

static INT32 param_end(UTOKEN_END_INFO *p_info)
{

	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

static INT32 param_stream0_width(INT32 argc, char *argv[])
{
	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();
	UIAPPIPCAM_BLOCK_BUFFER_INFO  *pBlockConfig = UIAppIPCam_get_BlockBufferInfo();

	int stream_no = 0;
	UINT32 width = atoi(argv[0]);

	pStreamConfig[stream_no].resolution.w = width;
	pVodMaxConfig[stream_no].uiMaxWidth = width;
	pBlockConfig[stream_no].sResolution.w = width;
	return 0;
}

static INT32 param_stream0_height(INT32 argc, char *argv[])
{
	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();
	UIAPPIPCAM_BLOCK_BUFFER_INFO  *pBlockConfig = UIAppIPCam_get_BlockBufferInfo();

	int stream_no = 0;
	UINT32 height = atoi(argv[0]);

	pStreamConfig[stream_no].resolution.h = height;
	pVodMaxConfig[stream_no].uiMaxHeight= height;
	pBlockConfig[stream_no].sResolution.h = height;
	return 0;
}

static INT32 param_stream0_max_bytes(INT32 argc, char *argv[])
{
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pFixQPConfig = UIAppIPCam_get_FixQPInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();

	int stream_no = 0;
	UINT32 byte_rate = atoi(argv[0]);
	pCBRConfig[stream_no].cbr_info.uiByteRate = byte_rate;
	pVBRConfig[stream_no].vbr_info.uiByteRate = byte_rate;
	pBRCNoneConfig[stream_no].uiTargetByterate = byte_rate;
	pEVBRConfig[stream_no].evbr_info.uiByteRate = byte_rate;
	pFixQPConfig[stream_no].sFixQPInfo.uiByteRate = byte_rate;
	pVodMaxConfig[stream_no].uiMaxByteRate = byte_rate;

	return 0;
}

static INT32 param_stream1_width(INT32 argc, char *argv[])
{
	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();
	UIAPPIPCAM_BLOCK_BUFFER_INFO  *pBlockConfig = UIAppIPCam_get_BlockBufferInfo();

	int stream_no = 2;
	UINT32 width = atoi(argv[0]);

	pStreamConfig[stream_no].resolution.w = width;
	pVodMaxConfig[stream_no].uiMaxWidth = width;
	pBlockConfig[stream_no].sResolution.w = width;
	return 0;
}

static INT32 param_stream1_height(INT32 argc, char *argv[])
{
	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();
	UIAPPIPCAM_BLOCK_BUFFER_INFO  *pBlockConfig = UIAppIPCam_get_BlockBufferInfo();

	int stream_no = 2;
	UINT32 height = atoi(argv[0]);

	pStreamConfig[stream_no].resolution.h = height;
	pVodMaxConfig[stream_no].uiMaxHeight= height;
	pBlockConfig[stream_no].sResolution.h = height;
	return 0;
}

static INT32 param_stream1_max_bytes(INT32 argc, char *argv[])
{
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pFixQPConfig = UIAppIPCam_get_FixQPInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();

	int stream_no = 2;
	UINT32 byte_rate = atoi(argv[0]);
	pCBRConfig[stream_no].cbr_info.uiByteRate = byte_rate;
	pVBRConfig[stream_no].vbr_info.uiByteRate = byte_rate;
	pBRCNoneConfig[stream_no].uiTargetByterate = byte_rate;
	pEVBRConfig[stream_no].evbr_info.uiByteRate = byte_rate;
	pFixQPConfig[stream_no].sFixQPInfo.uiByteRate = byte_rate;
	pVodMaxConfig[stream_no].uiMaxByteRate = byte_rate;

	return 0;
}

static INT32 param_stream2_width(INT32 argc, char *argv[])
{
	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();
	UIAPPIPCAM_BLOCK_BUFFER_INFO  *pBlockConfig = UIAppIPCam_get_BlockBufferInfo();

	int stream_no = 3;
	UINT32 width = atoi(argv[0]);

	pStreamConfig[stream_no].resolution.w = width;
	pVodMaxConfig[stream_no].uiMaxWidth = width;
	pBlockConfig[stream_no].sResolution.w = width;
	return 0;
}

static INT32 param_stream2_height(INT32 argc, char *argv[])
{
	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();
	UIAPPIPCAM_BLOCK_BUFFER_INFO  *pBlockConfig = UIAppIPCam_get_BlockBufferInfo();

	int stream_no = 3;
	UINT32 height = atoi(argv[0]);

	pStreamConfig[stream_no].resolution.h = height;
	pVodMaxConfig[stream_no].uiMaxHeight= height;
	pBlockConfig[stream_no].sResolution.h = height;
	return 0;
}

static INT32 param_stream2_max_bytes(INT32 argc, char *argv[])
{
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pFixQPConfig = UIAppIPCam_get_FixQPInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();

	int stream_no = 3;
	UINT32 byte_rate = atoi(argv[0]);
	pCBRConfig[stream_no].cbr_info.uiByteRate = byte_rate;
	pVBRConfig[stream_no].vbr_info.uiByteRate = byte_rate;
	pBRCNoneConfig[stream_no].uiTargetByterate = byte_rate;
	pEVBRConfig[stream_no].evbr_info.uiByteRate = byte_rate;
	pFixQPConfig[stream_no].sFixQPInfo.uiByteRate = byte_rate;
	pVodMaxConfig[stream_no].uiMaxByteRate = byte_rate;

	return 0;
}

static INT32 param_set_finish(INT32 argc, char *argv[])
{
	set_flg(UCTRLAPPMOVIEINI_FLG_ID_0, 1);
	return 0;
}


static UTOKEN_PARAM tbl_param[] = {
	{"stream0_width", param_stream0_width},
	{"stream0_height", param_stream0_height},
	{"stream0_max_bytes", param_stream0_max_bytes},
	{"stream1_width", param_stream1_width},
	{"stream1_height", param_stream1_height},
	{"stream1_max_bytes", param_stream1_max_bytes},
	{"stream2_width", param_stream2_width},
	{"stream2_height", param_stream2_height},
	{"stream2_max_bytes", param_stream2_max_bytes},
	{"set_finish", param_set_finish},
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_movie_ini = {
	"umovieini",
	tbl_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#endif
