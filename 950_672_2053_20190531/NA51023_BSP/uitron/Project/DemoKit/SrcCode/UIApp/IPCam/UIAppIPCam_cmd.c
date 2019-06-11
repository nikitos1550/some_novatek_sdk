//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#include "SysCommon.h"
#include "SysCfg.h"
#include "AppLib.h"
#include "UIAppCommon.h"
#include "UIModeIPCam.h"
#include "UIAppIPCam.h"
#include "UIAppIPCam_Param.h"

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppIpcamCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_VdoOut.h"
extern UINT32 System_GetEnableSensor(void);
extern UINT32 System_GetEnableDisp(void);
static void t_size_value(int x, int y)
{
	if (System_GetEnableSensor() & SENSOR_1) {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetVdoAspectRatio(ISF_IN1, x, y);
		ImageUnit_End();
	}
	#if (DISPLAY_FUNC == ENABLE)
	if (System_GetEnableDisp() & DISPLAY_1) {
		ImageUnit_Begin(&ISF_VdoOut1, 0);
		ImageUnit_SetVdoImgSize(ISF_IN1, 0, 0); //buffer size = full device size
		ImageUnit_SetVdoAspectRatio(ISF_IN1, x, y);//charlie hard code,need to_do
		ImageUnit_SetVdoPreWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
		ImageUnit_End();
	}
	if (System_GetEnableDisp() & DISPLAY_2) {
		ImageUnit_Begin(&ISF_VdoOut2, 0);
		ImageUnit_SetVdoImgSize(ISF_IN1, 0, 0); //buffer size = full device size
		ImageUnit_SetVdoAspectRatio(ISF_IN1, x, y);//charlie hard code,need to_do
		ImageUnit_SetVdoPreWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
		ImageUnit_End();
	}
	#endif
}
static BOOL Cmd_sys_size(CHAR *strCmd)
{
	static int t_size = 0;
	//size
	if (t_size == 0) {
		t_size_value(4, 3);
		t_size = 1;
	} else if (t_size == 1) {
		t_size_value(16, 9);
		t_size = 0;
	}
	ImageStream_UpdateAll(&ISF_Stream[0]);
	//ImageStream_UpdateAll(&ISF_Stream[1]);
	return TRUE;
}
#include "ImageUnit_VdoEnc.h"
static BOOL Cmd_sys_t1(CHAR *strCmd)
{
	ImageStream_Begin(&ISF_Stream[0], 0);
	ImageStream_SyncOutputDownstream(ImageUnit_In(&ISF_VdoEnc, ISF_IN1), ISF_PORT_STATE_RUN); //do RUN time sync
	ImageStream_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_t2(CHAR *strCmd)
{
	ImageStream_Begin(&ISF_Stream[0], 0);
	ImageStream_SyncOutputDownstream(ImageUnit_In(&ISF_VdoEnc, ISF_IN1), ISF_PORT_STATE_READY); //do READY time sync
	ImageStream_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_t3(CHAR *strCmd)
{
	ImageStream_Begin(&ISF_Stream[0], 0);
	ImageStream_SyncOutputDownstream(ImageUnit_In(&ISF_VdoEnc, ISF_IN1), ISF_PORT_STATE_OFF); //do OFF time sync
	ImageStream_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_o1(CHAR *strCmd)
{
	ImageStream_Begin(&ISF_Stream[0], 0);
	ImageStream_SetOutputDownstream(ImageUnit_In(&ISF_VdoEnc, ISF_IN1), ISF_PORT_STATE_READY); //do STOP
	ImageStream_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);

	ImageStream_Begin(&ISF_Stream[0], 0);
	ImageStream_SetOutputDownstream(ImageUnit_In(&ISF_VdoEnc, ISF_IN1), ISF_PORT_STATE_RUN); //do START
	ImageStream_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_o2(CHAR *strCmd)
{
	ImageStream_Begin(&ISF_Stream[0], 0);
	ImageStream_SetOutputDownstream(ImageUnit_In(&ISF_VdoEnc, ISF_IN1), ISF_PORT_STATE_OFF); //do STOP,CLOSE
	ImageStream_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);

	ImageStream_Begin(&ISF_Stream[0], 0);
	ImageStream_SetOutputDownstream(ImageUnit_In(&ISF_VdoEnc, ISF_IN1), ISF_PORT_STATE_RUN); //do OPEN,START
	ImageStream_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
/*
ImageUnit_SetParam(ISF_OUT1, ISF_VDOENC_PARAM_WIDTH, 1920);
ImageUnit_SetParam(ISF_OUT1, ISF_VDOENC_PARAM_HEIGHT, 1080);
ImageUnit_SetParam(ISF_OUT1, ISF_VDOENC_PARAM_FRAMERATE, 30);
ImageUnit_SetParam(ISF_OUT1, ISF_VDOENC_PARAM_CODEC, MEDIAVIDENC_H264);
ImageUnit_SetParam(ISF_OUT1, ISF_VDOENC_PARAM_TARGETRATE, 3*1024*1024);
*/
static BOOL Cmd_sys_b1(CHAR *strCmd)
{
	ImageUnit_Begin(&ISF_VdoEnc, 0);
	ImageUnit_SetParam(ISF_OUT1, VDOENC_PARAM_TARGETRATE, 3 * 1024 * 1024);
	ImageUnit_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_b2(CHAR *strCmd)
{
	ImageUnit_Begin(&ISF_VdoEnc, 0);
	ImageUnit_SetParam(ISF_OUT1, VDOENC_PARAM_TARGETRATE, 2 * 1024 * 1024);
	ImageUnit_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_c1(CHAR *strCmd)
{
	ImageUnit_Begin(&ISF_VdoEnc, 0);
	ImageUnit_SetParam(ISF_OUT1, VDOENC_PARAM_CODEC, MEDIAVIDENC_H264);
	ImageUnit_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_c2(CHAR *strCmd)
{
	ImageUnit_Begin(&ISF_VdoEnc, 0);
	ImageUnit_SetParam(ISF_OUT1, VDOENC_PARAM_CODEC, MEDIAVIDENC_H265);
	ImageUnit_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_s1(CHAR *strCmd)
{
	UIAppIPCam_set_DisplayPath(0);
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	return TRUE;
}
static BOOL Cmd_sys_s2(CHAR *strCmd)
{
	UIAppIPCam_set_DisplayPath(1);
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	return TRUE;
}
static BOOL Cmd_sys_s3(CHAR *strCmd)
{
	UIAppIPCam_set_DisplayPath(2);
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	return TRUE;
}
static BOOL Cmd_sys_s4(CHAR *strCmd)
{
	UIAppIPCam_set_DisplayPath(3);
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	return TRUE;
}
static BOOL Cmd_sys_x1(CHAR *strCmd)
{
	ImageUnit_Begin(ISF_IPL(0), 0);
	    ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORX);
	ImageUnit_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}
static BOOL Cmd_sys_x2(CHAR *strCmd)
{
	ImageUnit_Begin(ISF_IPL(0), 0);
	    ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORY);
	ImageUnit_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return TRUE;
}


#include "ipl_cmd.h"
#include "ipl_alg_infor.h"
#include "ipl_ctrl_infor.h"
#include "ImageUnit_UserProc.h"
#include "ImageApp_IPCam.h"
#include "UIAppIPCam_Param.h"

static UINT32 sensor_id = 0;
static UINT32 ime_path_id = 0;
static ISF_DATA    isf_data;
static ISF_PORT    *pSrc;

#define NEW_CAPTURE ENABLE

static BOOL Cmd_sys_yuv(CHAR *strCmd)
{
	UINT32 isf_out = ISF_OUT1;
	ISF_UNIT    *pUnit;

#if(IPCAM_FUNC==ENABLE)
	//DBG_DUMP("Get YUV at IPCAM interface!!");
	IPCAM_STREAM_INFO *pStreamInfo = (IPCAM_STREAM_INFO *)UIAppIPCam_get_streamInfo();
	UINT32 path_index = 0;
	for (path_index = UIAPPIPCAM_PATH_ID_0; path_index < UIAPPIPCAM_PATH_MAX_ID; path_index++) {
        if((pStreamInfo[path_index].enable == ENABLE)&&(pStreamInfo[path_index].sensor == sensor_id)&&(pStreamInfo[path_index].ime == ime_path_id)) {
            isf_out = pStreamInfo[path_index].path;
            break;
        }
        if(path_index + 1 == UIAPPIPCAM_PATH_MAX_ID){
            DBG_ERR("IPCAM path match error!!");
        }
	}
#else
	//DBG_DUMP("Get YUV at DVCAM interface!!");
#endif
	DBG_DUMP("get yuv start!!!!\r\n");

	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(isf_out, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, isf_out);
#if (NEW_CAPTURE == ENABLE)
	{
	// pause sie
	IPL_PAUSEDMA_INFO SIE_Pause;

	SIE_Pause.id = sensor_id;
	SIE_Pause.pnext = NULL;

	DBG_DUMP("ipl pause dma!!!!\r\n");
	if (IPL_SetCmd(IPL_SET_PAUSEDMA, (void *)&(SIE_Pause)) != E_OK) {
		DBG_ERR("ipl set sie pause command fail\r\n");
		return ISF_OK;
	}
	IPL_WaitCmdFinish();
	}
#endif
	// pull data
	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		//yuv->ipcam_path_id = isf_out;
		//yuv->size = sizeof(*yuv);
		//yuv->yuv_buf = *((IMG_BUF*)&isf_data.Desc[0]);

		//release UserProc's PULL queue
		ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(isf_out, USERPROC_PARAM_ALLOW_PULL_IMM, FALSE);
		ImageUnit_End();
	} else {
		DBG_ERR("get ime rdy info fail\r\n");
	}

	DBG_DUMP("get yuv finish!!!!\r\n");

	ImageUnit_ReleaseData((ISF_DATA *)&isf_data);

#if (NEW_CAPTURE == ENABLE)
	{
	// resume sie
	IPL_RESUMEDMA_INFO SIE_resume;

	SIE_resume.id = sensor_id;
	SIE_resume.pnext = NULL;

	DBG_DUMP("ipl resume dma!!!!\r\n");
	if (IPL_SetCmd(IPL_SET_RESUMEDMA, (void *)&(SIE_resume)) != E_OK) {
		DBG_ERR("ipl set sie resume command fail\r\n");
		return ISF_OK;
	}
	IPL_WaitCmdFinish();
	}
#endif

	DBG_DUMP("get yuv end!!!!\r\n");
	
	return ISF_OK;
}

SXCMD_BEGIN(ipcam, "ipcam command")
SXCMD_ITEM("size", Cmd_sys_size, "change size")
SXCMD_ITEM("t1", Cmd_sys_t1, "t1")
SXCMD_ITEM("t2", Cmd_sys_t2, "t2")
SXCMD_ITEM("t3", Cmd_sys_t3, "t3")
SXCMD_ITEM("o1", Cmd_sys_o1, "o1")
SXCMD_ITEM("o2", Cmd_sys_o2, "o2")
SXCMD_ITEM("b1", Cmd_sys_b1, "b1")
SXCMD_ITEM("b2", Cmd_sys_b2, "b2")
SXCMD_ITEM("c1", Cmd_sys_c1, "c1")
SXCMD_ITEM("c2", Cmd_sys_c2, "c2")
SXCMD_ITEM("s1", Cmd_sys_s1, "s1")
SXCMD_ITEM("s2", Cmd_sys_s2, "s2")
SXCMD_ITEM("s3", Cmd_sys_s3, "s3")
SXCMD_ITEM("s4", Cmd_sys_s4, "s4")
SXCMD_ITEM("x1", Cmd_sys_x1, "x1")
SXCMD_ITEM("x2", Cmd_sys_x2, "x2")
SXCMD_ITEM("yuv", Cmd_sys_yuv, "yuv")
SXCMD_END()

