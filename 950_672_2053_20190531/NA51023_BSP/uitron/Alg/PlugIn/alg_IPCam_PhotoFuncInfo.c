#include "SysCommon.h"
#include "GxImage.h"
#include "UIInfo.h"
#include "NvtUctrlAPI.h"
#include "UIPhotoFuncInfo.h"
#include "UIAppIPCam_Param.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_UserProc.h"
#include "md_module.h"
#include "HwClock.h"
#include "NvtIpcAPI.h"
#if (IPCAM_MD_SAMPLE_660_STYLE == ENABLE)
#include "md_api_660.h"
#endif
#if IPCAM_FD_FUNC
#include "fd_lib.h"
#define IPCAM_FD_FACESCALE	16
#define IPCAM_FD_USE_ODT	DISABLE
extern UINT32 gFD_processId;
static UINT32 gFdWidth  = 0;
static UINT32 gFdHeight = 0;
static IPCAM_FD_INFO *gFD_result;
#endif
//#NT#2017/12/26#Yuzhe Bian -begin
//#NT# Face Tracking & Grading
#if IPCAM_FTG_FUNC
#include "ftg_lib.h"
#include "odt_lib.h"
#include "UIAppIPCam.h"
#include "MovieStamp.h"
#define IPCAM_FTG_FACESCALE	24
#define IPCAM_FTG_USE_ODT   ENABLE
#define IPCAM_FTG_USE_MD    DISABLE
#define FTG_SNAPSHOT_CLARITY ENABLE
#define FTG_SNAPSHOT_DRAW   DISABLE // only for test (default DSIABLE)
extern UINT8 gHowLongtoGetLandmark;
static UINT32 gLandmarkCnt = 3;
static UINT32 gFtgWidth  = 0;
static UINT32 gFtgHeight = 0;
#endif

#if (IPCAM_ODT_FUNC)
#include "odt_lib.h"
#endif

#if IPCAM_PD_FUNC
#define IPCAM_PD_USE_ODT	DISABLE
extern UINT32 gPDImgW;
extern UINT32 gPDImgH;
extern IPCAM_PD_RESULT *gPD_result;
extern UINT32 gPD_processId;
#endif

#if IPCAM_MD_FUNC
#define IPCAM_MD_USE_ODT	DISABLE
extern IPCAM_MD_RESULT *gMD_result;
extern UINT32 gMD_processId;
extern MD_USR_PARAM_INFO gMD_prms;
extern UINT32 gMD_width;
extern UINT32 gMD_height;
#endif

//#NT#2017/12/26#Yuzhe Bian -end
//#NT#2015/09/17#JS Liu - begin
#if _CVHOST_EX_
#include "GxStrg.h"
#include "CvHost_lib.h"
#include "alg_CvHostEx.h"

static BOOL g_CvHostExDone = FALSE;
#endif
//#NT#2015/09/17#JS Liu - end
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppIPCam_PhotoFuncInfo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#ifndef IPCAM_MD_PATH
#define IPCAM_MD_PATH 2
#endif
#if(IPCAM_MD_SAMPLE_660_STYLE == ENABLE)
static BOOL gMDSample660StartFlag = TRUE;
#endif

static BOOL gMDStartFlag = FALSE;
static BOOL gMDStatusFlag = FALSE;
static BOOL gMDDbgFlag = FALSE;
static UINT32 gMdMode = 0;
static UINT64 gMDBlockTimeUS = 0;
static UINT64 gMDFrameTimeUS = 33333;
static MDParam *g_MdParam = NULL;
static UINT64 gMDLastFrameTime = 0;
static UINT64 gMDDiffFrameTimeUS = 0;
static BOOL gBCStartFlag = FALSE;
static BOOL gBCStatusFlag = FALSE;
#if IPCAM_BC_FUNC
static BOOL gBCYUVFlag = FALSE;
#endif
static UINT64 gBCFrameTimeUS = 200000; //200ms
static UINT64 gBCFrameTime 	 = 200000;
static UINT64 gBCLastFrameTime = 0;
static UINT64 gBCDiffFrameTimeUS = 0;
PIMG_BUF gpBCImgBuf = 0;
IPCAM_BC_RESULT *gBC_result;
static BCParam g_BcParam = {0};
static URECT gBC_usr_dim = {0};
IPCAM_MD_RESULT *gMD_result = NULL;
UINT32 gMD_processId = 999;
MD_USR_PARAM_INFO gMD_prms = {0};
UINT32 gMD_width = 1920;
UINT32 gMD_height = 1080;


static UINT8 gAlgFtgDbgLevel = 0;
static UINT8 gAlgBcDbgLevel = 0;

static BOOL cmd_ftg_dbg(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gAlgFtgDbgLevel = value;
	DBG_DUMP("Alg Ftg dbglevel Mode(%u)\r\n", gAlgFtgDbgLevel);
	return TRUE;
}
static BOOL cmd_bc_dbg(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gAlgBcDbgLevel = value;
	DBG_DUMP("Alg Bc dbglevel Mode(%u)\r\n", gAlgBcDbgLevel);
	return TRUE;
}
SXCMD_BEGIN(alg, "alg")
SXCMD_ITEM("ftg %", cmd_ftg_dbg, "set ftg dbg level[0:disable, 1:uctrl notify flow, 2:uctrl notify info]")
SXCMD_ITEM("bc %", cmd_bc_dbg, "set bc dbg level[0:disable, 1:uctrl notify flow, 2:uctrl notify info]")
SXCMD_END()
MDParam * Movie_get_motion_param(void)
{
    return g_MdParam;
}

void Movie_set_motion_param(MDParam *param)
{
	DBG_IND("%s:fps=%d mode=%d md_rect={%d %d %d %d %d} level=%d alarm_th=%d lv_info[0].init_val=%d\n", __func__, g_MdParam->fps,
			g_MdParam->md_mode, g_MdParam->dist_region_nu, g_MdParam->dist_region[0].x, g_MdParam->dist_region[0].y,
			g_MdParam->dist_region[0].width, g_MdParam->dist_region[0].height, g_MdParam->dist_region[0].level,
			g_MdParam->param_info.global_info.alarm_th, g_MdParam->param_info.md1_info.lv_info[0].init_val);
	gMDLastFrameTime = 0;
	gMDDiffFrameTimeUS = 0;
	if (g_MdParam->fps == 0) {
		gMDStartFlag = FALSE;
		mdl_uninit();
	} else {
		gMDFrameTimeUS = 1000000 / g_MdParam->fps;
		DBG_IND("MD period=%llu us\r\n", gMDFrameTimeUS);
		gMDStartFlag = TRUE;
		gMdMode = g_MdParam->md_mode;
		mdl_chg_param((MD_USR_PARAM_INFO *)&g_MdParam->param_info);
		mdl_set_user_lv_info((MD_USR_DISTRICT *)&g_MdParam->dist_region, g_MdParam->dist_region_nu);
		mdl_set_cell_map((UINT32)g_MdParam->param_info.global_info.cell_map, g_MdParam->param_info.global_info.mb_w_num, g_MdParam->param_info.global_info.mb_h_num);
#if IPCAM_BC_FUNC		
		if (g_MdParam->param_info.global_info.md_type == 2){
			g_BcParam.fps = 10; 
			gBCFrameTimeUS = 1000000 / g_BcParam.fps;
			gBCFrameTime = gBCFrameTimeUS;
			gBCStartFlag = TRUE;
		}
#endif		
	}
}


#if (IPCAM_MD_FUNC == ENABLE)
UINT32 IPCam_md_CalBufSize(void)
{
	return mdl_calc_buf_size();
}

UINT32 IPCam_md_CalCacheBufSize(void)
{
	return NVT_UCTRL_RAW_SIZE + sizeof(MDParam);
}

void odt_init_md_param(MD_USR_PARAM_INFO *md_info, UINT32 u_width, UINT32 u_height);
void odt_init_md_param(MD_USR_PARAM_INFO *md_info, UINT32 u_width, UINT32 u_height)
{
	UINT32 i,j;
	MD_USR_PARAM_INFO *default_param = md_info;
	memset(default_param, 0, sizeof(MD_USR_PARAM_INFO));

	// Global parameters
	default_param->GLOBAL_INFO.u_height = u_height;
	default_param->GLOBAL_INFO.u_width = u_width;
	default_param->GLOBAL_INFO.u_mb_height = 32;
	default_param->GLOBAL_INFO.u_mb_width = 32;
	default_param->GLOBAL_INFO.mb_w_num = default_param->GLOBAL_INFO.u_width / default_param->GLOBAL_INFO.u_mb_width;
	default_param->GLOBAL_INFO.mb_h_num = default_param->GLOBAL_INFO.u_height / default_param->GLOBAL_INFO.u_mb_height;
	default_param->GLOBAL_INFO.md_type = 0;
	default_param->GLOBAL_INFO.event_postprocessing = 1;
	default_param->GLOBAL_INFO.md_rst = 0;
	default_param->GLOBAL_INFO.ref_cell_en = 1;
	
	
	// app func 1 parameters
	default_param->GLOBAL_INFO.alarm_th = 20;	
	// app func 2 parameters
	default_param->GLOBAL_INFO.sub_region_num = 4;
	default_param->GLOBAL_INFO.sub_region[0].is_enabled = 1;
	default_param->GLOBAL_INFO.sub_region[0].start_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[0].start_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[0].end_block_x = 3;
	default_param->GLOBAL_INFO.sub_region[0].end_block_y = 3;
	default_param->GLOBAL_INFO.sub_region[0].alarm_th = 10;
	default_param->GLOBAL_INFO.sub_region[1].is_enabled = 0;
	default_param->GLOBAL_INFO.sub_region[1].start_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[1].start_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[1].end_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[1].end_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[1].alarm_th = 10;
	default_param->GLOBAL_INFO.sub_region[2].is_enabled = 1;
	default_param->GLOBAL_INFO.sub_region[2].start_block_x = 4;
	default_param->GLOBAL_INFO.sub_region[2].start_block_y = 4;
	default_param->GLOBAL_INFO.sub_region[2].end_block_x = 7;
	default_param->GLOBAL_INFO.sub_region[2].end_block_y = 7;
	default_param->GLOBAL_INFO.sub_region[2].alarm_th = 10;
	default_param->GLOBAL_INFO.sub_region[3].is_enabled = 0;
	default_param->GLOBAL_INFO.sub_region[3].start_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[3].start_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[3].end_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[3].end_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[3].alarm_th = 10;
	// app func 3-5 parameters
	default_param->GLOBAL_INFO.single_people_size = 60;	
	default_param->GLOBAL_INFO.object_size = 30;
	default_param->GLOBAL_INFO.overlap_th = 10;
	default_param->GLOBAL_INFO.blk1_x = default_param->GLOBAL_INFO.mb_w_num * 3 / 10;
	default_param->GLOBAL_INFO.blk1_y = 0;
	default_param->GLOBAL_INFO.blk2_x = default_param->GLOBAL_INFO.mb_w_num * 7 / 10;
	default_param->GLOBAL_INFO.blk2_y = 0;
	default_param->GLOBAL_INFO.blk3_x = default_param->GLOBAL_INFO.mb_w_num * 7 / 10;
	default_param->GLOBAL_INFO.blk3_y = default_param->GLOBAL_INFO.mb_h_num;
	default_param->GLOBAL_INFO.blk4_x = default_param->GLOBAL_INFO.mb_w_num * 3 / 10;
	default_param->GLOBAL_INFO.blk4_y = default_param->GLOBAL_INFO.mb_h_num;
	// app func 6 parameters
	default_param->GLOBAL_INFO.tamper_th = 50;
	default_param->GLOBAL_INFO.tamper_frm_num = 100;
	// set cell map
	for (i = 0; i < default_param->GLOBAL_INFO.mb_h_num; i++)
		for (j = 0; j < default_param->GLOBAL_INFO.mb_w_num; j++)
			default_param->GLOBAL_INFO.cell_map[i*default_param->GLOBAL_INFO.mb_w_num + j] = (j < default_param->GLOBAL_INFO.mb_w_num/2)?1:0;

	
	// md1 parameters
	default_param->MD1_INFO.tbg = 29490;
	default_param->MD1_INFO.lv_info[0].alpha = 32;
	default_param->MD1_INFO.lv_info[0].tb = 25;
	default_param->MD1_INFO.lv_info[0].sigma = 25;
	default_param->MD1_INFO.lv_info[0].init_val = 0;
	default_param->MD1_INFO.lv_info[0].tg = 9;
	default_param->MD1_INFO.lv_info[0].lum_diff_th = 0;
	default_param->MD1_INFO.lv_info[0].tex_diff_th = 0;
	default_param->MD1_INFO.lv_info[0].tex_th = 0;
	default_param->MD1_INFO.lv_info[0].tex_ratio_th = 127;
	default_param->MD1_INFO.lv_info[1].alpha = 32;
	default_param->MD1_INFO.lv_info[1].tb = 25;
	default_param->MD1_INFO.lv_info[1].sigma = 25;
	default_param->MD1_INFO.lv_info[1].init_val = 0;
	default_param->MD1_INFO.lv_info[1].tg = 9;
	default_param->MD1_INFO.lv_info[1].lum_diff_th = 150;
	default_param->MD1_INFO.lv_info[1].tex_diff_th = 100;
	default_param->MD1_INFO.lv_info[1].tex_th = 50;
	default_param->MD1_INFO.lv_info[1].tex_ratio_th = 100;
	default_param->MD1_INFO.lv_info[2].alpha = 32;
	default_param->MD1_INFO.lv_info[2].tb = 25;
	default_param->MD1_INFO.lv_info[2].sigma = 25;
	default_param->MD1_INFO.lv_info[2].init_val = 0;
	default_param->MD1_INFO.lv_info[2].tg = 9;
	default_param->MD1_INFO.lv_info[2].lum_diff_th = 150;
	default_param->MD1_INFO.lv_info[2].tex_diff_th = 100;
	default_param->MD1_INFO.lv_info[2].tex_th = 50;
	default_param->MD1_INFO.lv_info[2].tex_ratio_th = 100;
	default_param->MD1_INFO.lv_info[3].alpha = 32;
	default_param->MD1_INFO.lv_info[3].tb = 25;
	default_param->MD1_INFO.lv_info[3].sigma = 25;
	default_param->MD1_INFO.lv_info[3].init_val = 0;
	default_param->MD1_INFO.lv_info[3].tg = 9;
	default_param->MD1_INFO.lv_info[3].lum_diff_th = 150;
	default_param->MD1_INFO.lv_info[3].tex_diff_th = 100;
	default_param->MD1_INFO.lv_info[3].tex_th = 50;
	default_param->MD1_INFO.lv_info[3].tex_ratio_th = 100;

	// md2 parameters
	default_param->MD2_INFO.time_period = 10;
	default_param->MD2_INFO.lv_gm_md2_th[0] = 10;
	default_param->MD2_INFO.lv_gm_md2_th[1] = 10;
	default_param->MD2_INFO.lv_gm_md2_th[2] = 10;
	default_param->MD2_INFO.lv_gm_md2_th[3] = 10;
	
	// md3 parameters
	default_param->MD3_INFO.update_prob = 0;
	default_param->MD3_INFO.update_neighbor_en = 1;
	default_param->MD3_INFO.deghost_en = 0;
	default_param->MD3_INFO.th_feature = 10;
	default_param->MD3_INFO.sensi = 100;
}


void IPCam_md_init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (buf->Addr && cachebuf->Addr) {
		DBG_DUMP("--Do mdl_init:0x%x\r\n", buf->Addr);
		mdl_init(buf->Addr);
		memset((void*)cachebuf->Addr, 0 ,NVT_UCTRL_RAW_SIZE+sizeof(MDParam));
		g_MdParam = (MDParam *)(cachebuf->Addr + NVT_UCTRL_RAW_SIZE);
	} else {
		DBG_ERR("Get md buf fail\n");
	}

#if (IPCAM_ODT_FUNC && IPCAM_MD_USE_ODT)
	gMD_width = 1920;
	gMD_height = 1080;
	g_MdParam->param_info.global_info.mb_w_num = 60;
	g_MdParam->param_info.global_info.mb_h_num = 33;
	odt_init_md_param(&gMD_prms, gMD_width, gMD_height);
	//mdl_set_param(&gMD_prms);
	mdl_chg_param((MD_USR_PARAM_INFO *)&gMD_prms);
	gMdMode = 4;
#endif
}

void IPCam_md_process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	clock_t t1, t2;
	static INT32 start_index = -1;
	static clock_t old_time = 0;
	ISF_DATA         isf_data;
	ISF_PORT         *pSrc;
	IMG_BUF *pBuf = NULL;
	HEADER_YUV_MD_INFO *md_info = NULL;
	static UINT32 TimeStamp_sec = 0, TimeStamp_usec = 0;
	UINT32 LastTimeStamp_sec, LastTimeStamp_usec;
	INT32 DiffTimeStamp_sec, DiffTimeStamp_usec;

	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		if (clock() - old_time <= 1000000) { //1secs
			return;
		}
		start_index ++;
		DBG_DUMP("Motion detection start!!\r\n");
	}
	if (gMDStartFlag == FALSE) {
		DBG_IND("MD stop\r\n");
		return;
	}
	if (cachebuf->Addr == 0) {
		DBG_ERR("Get md result buf fail\n");
		return;
	}


#if (IPCAM_ODT_FUNC && IPCAM_MD_USE_ODT)
	static INT32					regist_flag = ODT_STA_ERROR;
	static UINT32					rslt_loc;
	static odt_od_result 			odt_rslts[CUSTOM_MD_MAX_OBJ_NUM];

	static odt_process_buf_param 	odt_buf_param;
	static odt_rslt_inf				buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				buf2;
	static odt_od_result 			buf3[CUSTOM_MD_MAX_OBJ_NUM];

	odt_buf_param.detect_rslt_addr = (UINT32)buf3;
	odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&buf2);
	odt_buf_param.odt_rslt_buffer_addr = (UINT32)buf1;

	if (regist_flag == ODT_STA_ERROR) {
		regist_flag = odt_regist(&odt_buf_param);
		if (regist_flag == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 5, odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 0, odt_buf_param.process_id);
			gMD_processId = odt_buf_param.process_id;
		}
	}
#endif


	gMD_result = (IPCAM_MD_RESULT *)cachebuf->Addr;
	pSrc = ImageUnit_Out(&ISF_UserProc, IPCAM_MD_PATH);
	ImageUnit_Begin(&ISF_UserProc, 0);
	if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 1) != ISF_OK) {
		DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 1 Fail\r\n");
		return;
	}
	ImageUnit_End();
	if (ImageUnit_IsAllowPull(pSrc) == 0) {
		DBG_WRN("ImageUnit_IsAllowPull fail\r\n");
		return;
	}
	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, gMDFrameTimeUS) != ISF_OK) {
		DBG_WRN("ImageUnit_PullData fail\r\n");
		gMD_result->Event = IPCAM_EVENT_MD;
		gMD_result->TimeStamp.tv_sec = TimeStamp_sec;
		gMD_result->TimeStamp.tv_usec = TimeStamp_usec;
		NvtUctrl_NotifyRaw((void *)gMD_result, sizeof(IPCAM_MD_RESULT));
		ImageUnit_Begin(&ISF_UserProc, 0);
		if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 1) != ISF_OK) {
			DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
			return;
		}
		ImageUnit_End();
		return;
	}

	ImageUnit_Begin(&ISF_UserProc, 0);
	if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 1) != ISF_OK) {
		DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
		return;
	}
	ImageUnit_End();

	pBuf = (IMG_BUF *)&isf_data.Desc[0];
	md_info = (HEADER_YUV_MD_INFO *)(pBuf->Metadata.Addr);
	DBG_IND("%s:md_addr=0x%x size=%d PxlAddr=0x%x\r\n", __func__, pBuf->Metadata.Addr, pBuf->Metadata.Size, pBuf->PxlAddr);
	if (!md_info) {
		//DBG_DUMP("^CMD-1 release blk = %08x\r\n", isf_data.hData);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}
	if (gMDLastFrameTime == 0)
		gMDLastFrameTime = isf_data.TimeStamp;

	TimeStamp_sec = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
	TimeStamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
	LastTimeStamp_sec = (gMDLastFrameTime >> 32) & 0xFFFFFFFF;
	LastTimeStamp_usec = gMDLastFrameTime & 0xFFFFFFFF;

	DiffTimeStamp_sec = (long)TimeStamp_sec - (long)LastTimeStamp_sec;
	DiffTimeStamp_usec = (long)TimeStamp_usec - (long)LastTimeStamp_usec;
	gMDDiffFrameTimeUS += DiffTimeStamp_sec * 1000000 + DiffTimeStamp_usec;
	gMDLastFrameTime = isf_data.TimeStamp;

	if (gMDDiffFrameTimeUS > gMDFrameTimeUS) {
		gMDDiffFrameTimeUS -= gMDFrameTimeUS;
#if IPCAM_BC_FUNC
		if (g_MdParam->param_info.global_info.md_type == 2) {
			IMG_BUF foreground = {0};
			foreground.PxlAddr[0] = mdl_get_evt_addr();
			bc_getforeground2(&foreground);	
			mdl_set_bc_info(foreground.Width, foreground.Height);
		}
		else
#endif	
		{
			mdl_set_win_info(md_info);
			mdl_set_evt_addr(md_info->evt_addr);
			if (md_get_evt_rst_v3(md_info->evt_addr, mdl_get_evt_addr(), md_info->win_width_num, md_info->win_y_num, 
				g_MdParam->param_info.global_info.md_type, g_MdParam->param_info.global_info.event_postprocessing, 255, 
				g_MdParam->param_info.global_info.ref_cell_en) < 0) {
				DBG_ERR("Motion event parsing error!\r\n");
				ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
				return;
			}
		}

		#if (IPCAM_ODT_FUNC && IPCAM_MD_USE_ODT)
			rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
		#endif

		t1 = clock();
		mdl_process(gMdMode);
		if (gMdMode == 0) {
			mdl_get_result((MD_USR_RST *) & (gMD_result->md_reslt), 0);
		} else {
			mdl_get_result((MD_USR_RST *) & (gMD_result->md_reslt), 1);
		}
		if (gMDBlockTimeUS) {
			Delay_DelayUs(gMDBlockTimeUS);
			gMDBlockTimeUS = 0;
		}
		t2 = clock();
		//DBG_DUMP("^CMD-2 release blk = %08x\r\n", isf_data.hData);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);


		#if (IPCAM_ODT_FUNC && IPCAM_MD_USE_ODT)
		UINT32 i;
		URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
		mdl_get_result((MD_USR_RST *) & (gMD_result->md_reslt), 1);
		for(i = 1; i <= gMD_result->md_reslt.obj_num; i++)
		{
			odt_rslts[i-1].x = gMD_result->md_reslt.obj[i].start_x * targetcoord.w / gMD_width;
			odt_rslts[i-1].y = gMD_result->md_reslt.obj[i].start_y * targetcoord.h / gMD_height;
			odt_rslts[i-1].w = (gMD_result->md_reslt.obj[i].end_x - gMD_result->md_reslt.obj[i].start_x + 1) * targetcoord.w / gMD_width;
			odt_rslts[i-1].h = (gMD_result->md_reslt.obj[i].end_y - gMD_result->md_reslt.obj[i].start_y + 1) * targetcoord.h / gMD_height;
			odt_rslts[i-1].score = 0;
			odt_rslts[i-1].class = 1004;
		}
		odt_setodrslt(gMD_result->md_reslt.obj_num, odt_rslts, rslt_loc, odt_buf_param.process_id);
		#endif


		if (__DBGLVL__ >= 5 || gMDStatusFlag) {
			DBG_DUMP("gMdMode=%d alarm=%d status=%d process_time=%lu us\r\n", gMdMode,
					gMD_result->md_reslt.motion_alarm, gMD_result->md_reslt.status, (t2 - t1));
		}

		if ((t2 - t1) > (clock_t)mdl_get_time_bound()) {
			gMDFrameTimeUS = 1000 * 1000;
			DBG_WRN("MD ProcessTime=%ld us, adjust FrameTime=%llu us\r\n", (t2 - t1), gMDFrameTimeUS);
		}else {
			if (g_MdParam->fps)
				gMDFrameTimeUS = 1000000 / g_MdParam->fps;
		}
	} else {
		//DBG_DUMP("^CMD-3 release blk = %08x\r\n", isf_data.hData);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}
	gMD_result->Event = IPCAM_EVENT_MD;
	gMD_result->TimeStamp.tv_sec = TimeStamp_sec;
	gMD_result->TimeStamp.tv_usec = TimeStamp_usec;
	
	if (gMDDbgFlag) {
		UINT32 i,j, blk_offset = 0;
		DBG_DUMP("\033[H\033[J");
		DBG_DUMP("[DBG] MD bitmap, ref_en = %d\r\n", g_MdParam->param_info.global_info.ref_cell_en);
		for (i=0; i < g_MdParam->param_info.global_info.mb_h_num; i++) {
			for (j=0; j < g_MdParam->param_info.global_info.mb_w_num; j++) {
				if (gMD_result->md_reslt.evt_map_addr[blk_offset + j])
					DBG_DUMP("1");
					//DBG_DUMP("%d", IPCamMdEvent->md_reslt.evt_map_addr[blk_offset + j]);
				else
					DBG_DUMP(" ");
			}
			DBG_DUMP("\r\n");
			blk_offset += g_MdParam->param_info.global_info.mb_w_num;
		}
	}
	
	
	NvtUctrl_NotifyRaw((void *)gMD_result, sizeof(IPCAM_MD_RESULT));
	return;
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_motion = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"IPCAM_MD",
		IPCam_md_init,   /// < init fp, only execute at first process
		IPCam_md_process,        ///< process fp
		NULL,                   ///< process end fp
		NULL,            ///< exit fp, only execute at photo close
		IPCam_md_CalBufSize,                   ///< get working buffer fp
		IPCam_md_CalCacheBufSize,                   ///< get working cache buffer fp
	},
	NULL                    ///< point next Function Obj, last Function must be set NULL
};
#endif

static BOOL cmd_md_mode(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gMdMode = value;
	DBG_DUMP("MD Mode(%u) FrameTimeUS(%llu), StartFlag(%d), ResultFlag(%d)\r\n", gMdMode, gMDFrameTimeUS, gMDStartFlag, gMDStatusFlag);
	return TRUE;
}

static BOOL cmd_md_block(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gMDBlockTimeUS = (UINT64)value * 1000;
	DBG_DUMP("Set MD BlockTimeUS(%llu)\r\n", gMDBlockTimeUS);
	return TRUE;
}

static BOOL cmd_md_set(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gMDLastFrameTime = 0;
	gMDDiffFrameTimeUS = 0;
	if (value == 0) {
		gMDStartFlag = FALSE;
	} else {
		gMDFrameTimeUS = 1000000 / value;
		gMDStartFlag = TRUE;
	}
	DBG_DUMP("MD Mode(%u) FrameTimeUS(%llu), StartFlag(%d), ResultFlag(%d)\r\n", gMdMode, gMDFrameTimeUS, gMDStartFlag, gMDStatusFlag);
	return TRUE;
}

static BOOL cmd_md_status(CHAR *strCmd)
{
	UINT32 value = 0;
	DBG_DUMP("MD Mode(%u) FrameTimeUS(%llu), StartFlag(%d), ResultFlag(%d)\r\n", gMdMode, gMDFrameTimeUS, gMDStartFlag, gMDStatusFlag);
	sscanf_s(strCmd, "%d", &value);
	if (value == 0) {
		gMDStatusFlag = FALSE;
	} else {
		gMDStatusFlag = TRUE;
	}
	return TRUE;
}

static BOOL cmd_md_dbg(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	if (value == 0) {
		gMDDbgFlag = FALSE;
	} else {
		gMDDbgFlag = TRUE;
	}
	return TRUE;
}

static BOOL cmd_md_setmd1prms(CHAR *strCmd)
{
	UINT32 val0 = 0, val1 = 0, val2 = 0;
	sscanf_s(strCmd, "%d %d %d", &val0, &val1, &val2);
	mdl_set_md1_prms(MD1_LV0_ALPHA, val0);
	mdl_set_md1_prms(MD1_LV0_TB,    val1);
	mdl_set_md1_prms(MD1_LV0_SIGMA, val2);
	mdl_update_hw_prms();
	return TRUE;
}

static BOOL cmd_md_setmd2prms(CHAR *strCmd)
{
	UINT32 val0 = 0, val1 = 0;
	sscanf_s(strCmd, "%d %d", &val0, &val1);
	mdl_set_md2_prms(MD2_TIME_PERIOD,  val0);
	mdl_set_md2_prms(MD2_LV0_GM_TH,    val1);
	mdl_update_hw_prms();
	return TRUE;
}
SXCMD_BEGIN(md, "md")
SXCMD_ITEM("mode %", cmd_md_mode, "set mode, default=0")
SXCMD_ITEM("block %", cmd_md_block, "set md block time (in msec)")
SXCMD_ITEM("set %", cmd_md_set, "set fps, fps=0 to stop")
SXCMD_ITEM("status %", cmd_md_status, "md status, 1=dump")
SXCMD_ITEM("dbg %", cmd_md_dbg, "md dbg app result, 1=dump dbg log")
SXCMD_ITEM("setmd1prms % % %", cmd_md_setmd1prms, "set md1 prm alpha/tb/sigma")
SXCMD_ITEM("setmd2prms % %", cmd_md_setmd2prms, "set md2 prm time_period/GM_TH")
SXCMD_END()



int IPCam_GetFDFaceScale(void)
{
#if IPCAM_FD_FUNC
	return IPCAM_FD_FACESCALE;
#else
	return 0;
#endif
}

UINT32 gFD_processId = 999;

#if IPCAM_FD_FUNC
void IPCam_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (buf->Addr && cachebuf->Addr)
	{
        FD_Config(FD_CFG_MAX_FACE, MAX_FDNUM);
		FD_Config(FD_CFG_FACE_SCALE, IPCAM_FD_FACESCALE);
		FD_Config(FD_CFG_PROCESS_RATE, 3);
		FD_Init(buf, cachebuf);
        gFD_result = (IPCAM_FD_INFO *)((UINT32)(cachebuf->Addr) + FD_CalcCacheBuffSize());
	    memset(gFD_result, 0, sizeof(IPCAM_FD_INFO));
	} else {
		DBG_ERR("Get fd buf fail\n");
	}
}

void IPCam_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	INT32 FD_state = 0;

	double now_time = 0;
	static INT32 start_index = -1;
	static double old_time = 0;

	static UINT64 uiFDLastFrameTime =0;
	UINT32 LastTimeStamp_sec = 0;
	UINT32 LastTimeStamp_usec = 0;
	UINT32 TimeStamp_sec=0;
	UINT32 TimeStamp_usec =0;
	INT32 DiffTimeStamp_sec =0 ;
	INT32 DiffTimeStamp_usec = 0;
	static UINT64 uiFDDiffFrameTimeUS =0;
	UINT32 uiFDTimeOutUS = 33333;

	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		now_time = clock();
		if (now_time - old_time <= 2000000) { ///2secs
			return;
		}
		start_index++;
		DBG_DUMP("FD detection start!!\r\n");
	}

	#if (IPCAM_ODT_FUNC && IPCAM_FD_USE_ODT)
	static INT32					regist_flag = ODT_STA_ERROR;
	static UINT32					rslt_loc;
	static UINT32					num;
	static FD_FACE					rslts[FD_NUM_MAX];
	static odt_od_result 			odt_rslts[FD_NUM_MAX];

	static odt_process_buf_param 	odt_buf_param;
	static odt_rslt_inf				buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				buf2;
	static odt_od_result 			buf3[FD_NUM_MAX];

	odt_buf_param.detect_rslt_addr = (UINT32)buf3;
	odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&buf2);
	odt_buf_param.odt_rslt_buffer_addr = (UINT32)buf1;

	if (regist_flag == ODT_STA_ERROR) {
		regist_flag = odt_regist(&odt_buf_param);
		if (regist_flag == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 15, odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 1, odt_buf_param.process_id);
			odt_config(odt_cfg_scale_for_match, 20, odt_buf_param.process_id);
			gFD_processId = odt_buf_param.process_id;
		}
	}
	#endif

	pUnit = &ISF_UserProc;

	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(ISF_OUT3, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT3);

	if(!ImageUnit_IsAllowPull(pSrc)){
		DBG_WRN("ImageUnit_IsAllowPull fail!\r\n");
		return;
	}

	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK)
	{
		//#NT#2017/10/26#Charlie chang -begin
    	//#NT# flow control
		if (uiFDLastFrameTime == 0){
			uiFDLastFrameTime = isf_data.TimeStamp;
		}

		TimeStamp_sec = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
		TimeStamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
		LastTimeStamp_sec = (uiFDLastFrameTime >> 32) & 0xFFFFFFFF;
		LastTimeStamp_usec = uiFDLastFrameTime & 0xFFFFFFFF;
		DiffTimeStamp_sec = (long)TimeStamp_sec - (long)LastTimeStamp_sec;
		DiffTimeStamp_usec = (long)TimeStamp_usec - (long)LastTimeStamp_usec;
		uiFDDiffFrameTimeUS = DiffTimeStamp_sec * 1000000 + DiffTimeStamp_usec + uiFDDiffFrameTimeUS;

		if (uiFDDiffFrameTimeUS > uiFDTimeOutUS) {

			#if (IPCAM_ODT_FUNC && IPCAM_FD_USE_ODT)
			rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
			#endif

			FD_state = FD_Process((IMG_BUF *)&isf_data.Desc[0]);
			uiFDDiffFrameTimeUS -= uiFDTimeOutUS;
            ImageUnit_GetVdoImgSize(pUnit, pSrc->oPort, &gFdWidth, &gFdHeight);

			#if (IPCAM_ODT_FUNC && IPCAM_FD_USE_ODT)
            if(FD_state != -1) // do FD, -1 mean not do FD in this frame
            {
				UINT32 i;
                static odt_rslt_loc time_stamp_set = {0};
				URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
				num = FD_GetRsltFace(rslts, &targetcoord);
				for(i = 0; i < num; i++)
				{
					odt_rslts[i].x = rslts[i].x;
					odt_rslts[i].y = rslts[i].y;
					odt_rslts[i].w = rslts[i].w;
					odt_rslts[i].h = rslts[i].h;
					odt_rslts[i].score = 0;
					odt_rslts[i].class = 1000;
				}
                time_stamp_set.timestamp_sec  = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
		        time_stamp_set.timestamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
				//odt_setodrslt(num, odt_rslts, rslt_loc, odt_buf_param.process_id);
                odt_setodrslt_timestamp(num, odt_rslts, (UINT32)&time_stamp_set, odt_buf_param.process_id);
            }
			#endif

		}
		uiFDLastFrameTime = isf_data.TimeStamp;
		//#NT#2017/10/26#Charlie Chang -end
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

#if ((!IPCAM_FD_USE_ODT) || (!IPCAM_ODT_FUNC))
    if(FD_state != -1) // do FD, -1 mean not do FD in this frame
    {
		static FD_FACE			FdInfo[MAX_FDNUM];
    	URECT   FdDispCoord = {0, 0, gFdWidth, gFdHeight};
        UINT32  faceNumber = 0, i = 0;

		faceNumber = FD_GetRsltFace(FdInfo, &FdDispCoord);

    	for (i = 0; i < faceNumber; i++) {
    		gFD_result->info[i].x = FdInfo[i].x;
    		gFD_result->info[i].y = FdInfo[i].y;
    		gFD_result->info[i].w = FdInfo[i].w;
    		gFD_result->info[i].h = FdInfo[i].h;
            gFD_result->info[i].id= 0;
        }
        gFD_result->TimeStamp.tv_sec = TimeStamp_sec;
    	gFD_result->TimeStamp.tv_usec = TimeStamp_usec;
        gFD_result->Event = IPCAM_EVENT_FD;
        gFD_result->uiFaceNumber = faceNumber;
		#if 0
    	DBG_DUMP("num:%d sec:%d usec:%d\r\n",gFD_result->uiFaceNumber, gFD_result->TimeStamp.tv_sec, gFD_result->TimeStamp.tv_usec);
    	for (i = 0; i < gFD_result->uiFaceNumber; i++) {
			DBG_DUMP("x:%d y:%d w:%d h:%d\r\n",
	    			gFD_result->info[i].x,
	    			gFD_result->info[i].y,
	    			gFD_result->info[i].w,
	    			gFD_result->info[i].h
			);
		}
		#endif
    	NvtUctrl_NotifyRaw((void *)gFD_result, sizeof(IPCAM_FD_INFO));
    }
#endif
}

UINT32 IPCam_FDCalcBuffSize(void)
{
	FD_Config(FD_CFG_FACE_MAX_SCALE, IPCAM_FD_FACESCALE);
	return FD_CalcBuffSize();
}
UINT32 IPCam_FDCalcCacheBuffSize(void)
{
	return (FD_CalcCacheBuffSize() + sizeof(IPCAM_FD_INFO));
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamfd = {
	{
		.Priority = PHOTO_PRIORITY_M,           ///< function hook process Id.
		.IPLId = IPL_PATH_1,                    ///< IPL Id.
		.TrigEvent = PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		.ProcName = "Photo_IPCAMFDProcess",
		.InitFP= IPCam_FDInit,                  ///< init fp, only execute at first process
		.ProcFP = IPCam_FDProcess,              ///< process fp
		.EndFP = NULL,                          ///< process end fp
		.GetBufFP = IPCam_FDCalcBuffSize,       ///< get working buffer fp
		.GetCacheBufFP = IPCam_FDCalcCacheBuffSize,  ///< get working cache buffer fp
	},
	.pNext = NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif

//#NT#2017/12/26#Yuzhe Bian -begin
//#NT# Face Tracking & Grading
#if IPCAM_FTG_FUNC
#if (IPCAM_FTG_USE_MD == ENABLE && IPCAM_MD_FUNC == ENABLE)
static UINT8* gftg_motion_buf = NULL;
#endif
void IPCam_FTGInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//#NT#2018/06/15#YZ Bian -begin
    //#NT# auto select width and height
    ISF_PORT	*pSrc;
	ISF_UNIT	*pUnit;
    pUnit = &ISF_UserProc;
    ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(FTG_PATH + ISF_OUT_BASE, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
    pSrc = ImageUnit_Out(pUnit, FTG_PATH + ISF_OUT_BASE);
	ImageUnit_GetVdoImgSize(pUnit, pSrc->oPort, &gFtgWidth, &gFtgHeight);
	//#NT#2018/06/15#YZ Bian -end
#if (IPCAM_FTG_USE_MD == ENABLE && IPCAM_MD_FUNC == ENABLE)
	gftg_motion_buf = (UINT8*)(buf->Addr + nvt_ftg_calcbuffsize());
#endif
	nvt_ftg_config(NVT_FTG_FD_CFG_MAX_FACE, MAX_FTGNUM);
	nvt_ftg_config(NVT_FTG_CFG_PROCESS_RATE, 3);
    nvt_ftg_config(NVT_FTG_CFG_MD_THRESHOLD, 12);
    if (IPCAM_FTG_USE_MD == ENABLE && IPCAM_MD_FUNC == ENABLE)
	    nvt_ftg_config(NVT_FTG_FD_SCORE_THRESHOLD, 2);
    else
        nvt_ftg_config(NVT_FTG_FD_SCORE_THRESHOLD, 5);

    //nvt_ftg_config(NVT_FTG_FD_CFG_SUPPORT_POSE, 0x01E1); // delete yaw face

	nvt_ftg_init(buf, cachebuf, gFtgWidth, gFtgHeight);

	URECT roi_rect = {0, 0, IPCAM_MAIN_WIDTH, IPCAM_MAIN_HEIGHT}; // default max roi
	
	nvt_ftg_roi_config(roi_rect);

	nvt_ftg_lock(FALSE);
}

void IPCam_FTGProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	static IPCAM_FTG_RESULT IPCamFTGResult;
    static FTG_TIMESTAMP time_stamp = {0};
	FTG_FACE *pFTGresult = (FTG_FACE*)IPCamFTGResult.info;
	double now_time = 0;
	static INT32 start_index = -1;
	static double old_time = 0;
	UINT32 i;

	if (!odt_getinitstate()) {
		DBG_WRN("nvt_ftg_getinitstate\r\n");
		return;
	}
	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		now_time = clock();
		if (now_time - old_time <= 2000000) { ///2secs
			return;
		}
		start_index++;
		DBG_DUMP("FTG Face tracking start!!\r\n");
	}

	UINT32		ftg_number = 0;
	URECT		disp_cord  = {0, 0, gFtgWidth, gFtgHeight};
	//gLandmarkCnt++;
	gLandmarkCnt = 0;
	if (gLandmarkCnt >= gHowLongtoGetLandmark){
		gLandmarkCnt = 0;
        UINT32 t1 = Perf_GetCurrent();
		ftg_number = nvt_ftg_get_result((FTG_FACE*)IPCamFTGResult.info, &disp_cord, MAX_FTGNUM, (UINT32)&time_stamp, NVT_FTG_RSLT_LOCATION | NVT_FTG_RSLT_CLARITY | NVT_FTG_RSLT_LANDMARK); //
        UINT32 t2 = Perf_GetCurrent();
		if (gAlgFtgDbgLevel == 6)
		    debug_msg("%d\r\n", (t2 - t1)/1000);
        IPCamFTGResult.uiHasLandMark = 1;
	}
	else
	{
		ftg_number = nvt_ftg_get_result((FTG_FACE*)IPCamFTGResult.info, &disp_cord, MAX_FTGNUM, (UINT32)&time_stamp, NVT_FTG_RSLT_LOCATION | NVT_FTG_RSLT_CLARITY_CPU);
		IPCamFTGResult.uiHasLandMark = 0;
	}
    IPCamFTGResult.TimeStamp.tv_sec = time_stamp.timestamp_sec;
    IPCamFTGResult.TimeStamp.tv_usec = time_stamp.timestamp_usec;
	IPCamFTGResult.uiFaceNumber = ftg_number;
	IPCamFTGResult.Event = IPCAM_EVENT_FTG;
	NvtUctrl_NotifyRaw((void *)&IPCamFTGResult, sizeof(IPCAM_FTG_RESULT));

	if (gAlgFtgDbgLevel == 1)
	{
        DBG_DUMP("num:%d sec:%d usec:%d\r\n",IPCamFTGResult.uiFaceNumber, IPCamFTGResult.TimeStamp.tv_sec, IPCamFTGResult.TimeStamp.tv_usec);
		for (i = 0; i < IPCamFTGResult.uiFaceNumber; i++)
		{
			DBG_DUMP("id:%d x:%d y:%d w:%d h:%d pose:%d\r\n", \
				pFTGresult[i].id, pFTGresult[i].x, pFTGresult[i].y, pFTGresult[i].w, pFTGresult[i].h, pFTGresult[i].pose);
		}
	}

	if (gAlgFtgDbgLevel == 2)
	{
		DBG_DUMP("num:%d sec:%d usec:%d\r\n",IPCamFTGResult.uiFaceNumber, IPCamFTGResult.TimeStamp.tv_sec, IPCamFTGResult.TimeStamp.tv_usec);
		for (i = 0; i < IPCamFTGResult.uiFaceNumber; i++)
		{
			DBG_DUMP("id:%d x:%d y:%d w:%d h:%d pose:%d luma:%d clarity:%d contrast:%d lstate:%d lnumber:%d yaw:%d roll:%d pitch:%d lscore:%d\r\n", \
 				pFTGresult[i].id, pFTGresult[i].x, pFTGresult[i].y, pFTGresult[i].w, pFTGresult[i].h, pFTGresult[i].pose,\
 				pFTGresult[i].luma, pFTGresult[i].clarity, pFTGresult[i].contrast, pFTGresult[i].lstate, pFTGresult[i].lnumber,\
				pFTGresult[i].yaw, pFTGresult[i].roll, pFTGresult[i].pitch, pFTGresult[i].lscore);
			#if 1
			INT32  j = 0;
			for(j = 0; j < pFTGresult[i].lnumber; j++)
			{
				DBG_DUMP("%d ",	pFTGresult[i].lpoints[j]);
				if((j+1)%14 == 0)
					DBG_DUMP("\r\n");
			}
			DBG_DUMP("\r\n");
			#endif
		}
	}
}

void IPCam_FTGFDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    static IPCAM_FTG_RESULT IPCamFDResult;
    static FTG_TIMESTAMP time_stamp_set = {0};
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
    double now_time = 0;
    static INT32 start_index = -1;
    static double old_time = 0;
	INT32 FD_state = 0;

    if (old_time == 0) {
            old_time = clock();
    }
    //wait boot device finish
    if (start_index == -1) {
            now_time = clock();
            if (now_time - old_time <= 2000000) { ///2secs
                    return;
            }
            start_index++;
            DBG_DUMP("FTG Face detection start!!\r\n");
    }

#if (IPCAM_FTG_USE_ODT == ENABLE && IPCAM_ODT_FUNC == ENABLE)
    if (!odt_getinitstate())
    {
		DBG_WRN("nvt_ftg_getinitstate\r\n");
		return;
	}
#endif

#if (IPCAM_FTG_USE_MD == ENABLE && IPCAM_MD_FUNC == ENABLE)
	HEADER_YUV_MD_INFO *md_info = NULL;
	IMG_BUF *pBuf = NULL;

	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, IPCAM_MD_PATH);
	if(!ImageUnit_IsAllowPull(pSrc)){
		DBG_WRN("ImageUnit_IsAllowPull fail!\r\n");
		return;
	}
	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK)
	{
		pBuf =  (IMG_BUF *)&isf_data.Desc[0];
		md_info = (HEADER_YUV_MD_INFO *)(pBuf->Metadata.Addr);
		if (md_get_evt_rst_v3(md_info->evt_addr, (UINT32)gftg_motion_buf, IPCAM_MAIN_WIDTH>>5, IPCAM_MAIN_HEIGHT>>5, 0, 4, 255, 0) < 0)
        {
			DBG_ERR("Motion event parsing error!\r\n");
			ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
			return;
		}
		UINT32 count = 0;
		if(count % 100 == 0)
		{
			if (gAlgFtgDbgLevel == 5)
			{
				UINT32 i,j, blk_offset = 0;
				DBG_DUMP("\033[H\033[J");
				DBG_DUMP("[DBG] MD bitmap\r\n");
				for (i=0; i < md_info->win_y_num; i++) {
					for (j=0; j < md_info->win_width_num; j++) {
						if (gftg_motion_buf[blk_offset + j])
							DBG_DUMP("1");
						else
							DBG_DUMP(" ");
					}
					DBG_DUMP("\r\n");
					blk_offset += md_info->win_width_num;
				}
			}
		}
		count++;
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		nvt_ftg_set_mdmask(gftg_motion_buf, IPCAM_MAIN_WIDTH>>5, IPCAM_MAIN_HEIGHT>>5);
	}
#endif
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(FTG_PATH + ISF_OUT_BASE, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, FTG_PATH + ISF_OUT_BASE);

	if(!ImageUnit_IsAllowPull(pSrc)){
		DBG_WRN("ImageUnit_IsAllowPull fail!\r\n");
		return;
	}
    if ( ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK)
	{
		nvt_ftg_setimg((IMG_BUF *)&isf_data.Desc[0]); //input image must same ratio as DIS

        time_stamp_set.timestamp_sec  = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
		time_stamp_set.timestamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;

#if (IPCAM_FTG_USE_ODT == ENABLE && IPCAM_ODT_FUNC == ENABLE)
	    static INT32 				   regist_flag = ODT_STA_ERROR;
	    static UINT32				   ftg_rslt_loc;
	    static UINT32				   ftg_num;
	    static odt_od_result 		   ftg_odt_rslts[MAX_FTGNUM];
	    static odt_process_buf_param   ftg_odt_buf_param;
	    static odt_rslt_inf			   ftg_buf1[ODT_MAX_DISRSLTBUF_NUM];
	    static odt_rslt_inf			   ftg_buf2;
	    static odt_od_result 		   ftg_buf3[MAX_FTGNUM];
	    ftg_odt_buf_param.detect_rslt_addr = (UINT32)ftg_buf3;
	    ftg_odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&ftg_buf2);
	    ftg_odt_buf_param.odt_rslt_buffer_addr = (UINT32)ftg_buf1;

	    if (regist_flag == ODT_STA_ERROR)
	    {
	 	   regist_flag = odt_regist(&ftg_odt_buf_param);//regist fd to odt and assign an id to fd
	 	   if (regist_flag == ODT_STA_ERROR)
	 	   {
	 		   return;
	 	   }
	 	   else
	 	   {
	 	   		nvt_ftg_config(NVT_FTG_CFG_ODT_PROCESS_ID, ftg_odt_buf_param.process_id);
	 	       	nvt_ftg_config(NVT_FTG_CFG_MAX_OBJ_LIFT1, 15);
	 			nvt_ftg_config(NVT_FTG_CFG_MAX_OBJ_LIFT2, 1);
				nvt_ftg_config(NVT_FTG_CFG_SCALE_FOR_MATCH, 15);
                nvt_ftg_config(NVT_FTG_CFG_TRACK_SENSITIVITY, odt_normal);
				if(ODT_PROCESS_NUM_MAX == nvt_ftg_getconfig(NVT_FTG_CFG_ODT_PROCESS_ID))
					regist_flag = ODT_STA_ERROR;
	 	   }
	    }

#endif

		UINT32 t1 = Perf_GetCurrent();
	    FD_state = nvt_ftg_fd_process();
		UINT32 t2 = Perf_GetCurrent();
		if (gAlgFtgDbgLevel == 4)
		{
			if((t2 - t1)/1000 > 3)
				debug_msg("%d\r\n", (t2 - t1)/1000);
		}

	    if(FD_state != -1) // do FD, -1 mean not do FD in this frame
	    {
#if (IPCAM_FTG_USE_ODT == ENABLE && IPCAM_ODT_FUNC == ENABLE)
            //get fd results and pass them to odt
		    URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
		    ftg_num = nvt_ftg_get_fd_result(ftg_odt_rslts, &targetcoord, MAX_FTGNUM);
            ftg_rslt_loc = odt_getsavedisrsltloc_timestamp((UINT32)&time_stamp_set);
		    odt_setodrslt(ftg_num, ftg_odt_rslts, ftg_rslt_loc, ftg_odt_buf_param.process_id);

			static odt_obj	fd_obj[ODT_NUM_MAX];
			UINT32      fd_number = 0, i;
			URECT		disp_cord = {0, 0, gFtgWidth, gFtgHeight};
			UINT32		fd_match_loc = ODT_MAX_DISRSLTBUF_NUM;
            FLOAT       ratio_img = (FLOAT)(gFtgWidth)/gFtgHeight;
            FLOAT       ratio_dis = (FLOAT)(disHw_getCurDisInH())/disHw_getCurDisInV();

            if( (ratio_img - ratio_dis) >= 0.1 || (ratio_img - ratio_dis) <= -0.1 )
                DBG_ERR("input image ratio must same as DIS ratio!\r\n");

			do{
				fd_number = nvt_ftg_get_fd_result_after_match(fd_obj, ftg_odt_buf_param.process_id, &disp_cord, &fd_match_loc);
                if(!odt_getinitstate())
                    break;
			}while(ftg_rslt_loc != fd_match_loc);
			//#NT#2018/06/06#YZ Bian -end
            fd_number = MIN(fd_number, MAX_FTGNUM);
            IPCamFDResult.uiFaceNumber = 0;
            for(i = 0; i < fd_number; i++)
            {
                if(fd_obj[i].type == 0)
                {
                    IPCamFDResult.info[i].x    = fd_obj[i].obj_rect.x;
                    IPCamFDResult.info[i].y    = fd_obj[i].obj_rect.y;
                    IPCamFDResult.info[i].w    = fd_obj[i].obj_rect.w;
                    IPCamFDResult.info[i].h    = fd_obj[i].obj_rect.h;
                    IPCamFDResult.info[i].id   = fd_obj[i].id;
                    IPCamFDResult.info[i].pose = fd_obj[i].class;
                    IPCamFDResult.uiFaceNumber++;
                }
            }
            fd_number = IPCamFDResult.uiFaceNumber;
#else
            static odt_od_result fd_rslts[MAX_FTGNUM];
            URECT targetcoord = {0, 0, gFtgWidth, gFtgHeight};
            UINT32 fd_number = 0, i;

            fd_number = nvt_ftg_get_fd_result(fd_rslts, &targetcoord, MAX_FTGNUM);

            IPCamFDResult.uiFaceNumber = 0;
            for(i = 0; i < fd_number; i++)
            {
                IPCamFDResult.info[i].x    = fd_rslts[i].x;
                IPCamFDResult.info[i].y    = fd_rslts[i].y;
                IPCamFDResult.info[i].w    = fd_rslts[i].w;
                IPCamFDResult.info[i].h    = fd_rslts[i].h;
                IPCamFDResult.info[i].id   = 0;
                IPCamFDResult.info[i].pose = fd_rslts[i].class;
                IPCamFDResult.uiFaceNumber++;
            }
#endif
            //#NT#2018/06/13#YZ Bian -begin
            //#NT#  add clarity for snapshot
            for(i = 0; i < IPCamFDResult.uiFaceNumber; i++)
            {
            	#if (FTG_SNAPSHOT_CLARITY == ENABLE)
                IMG_BUF *pinputimg = (IMG_BUF *)&isf_data.Desc[0];
                URECT  tmp_rect;
                tmp_rect.x = IPCamFDResult.info[i].x;
                tmp_rect.y = IPCamFDResult.info[i].y;
                tmp_rect.w = IPCamFDResult.info[i].w;
                tmp_rect.h = IPCamFDResult.info[i].h;
				CLARITY_PRMS params = {pinputimg->PxlAddr[0], pinputimg->LineOffset[0], pinputimg->Height, tmp_rect, 0, 0, 0};
				nvt_ftg_clarity_process(&params);
				IPCamFDResult.info[i].luma 		= params.luma;
				IPCamFDResult.info[i].clarity 	= params.clarity;
				IPCamFDResult.info[i].contrast 	= params.contrast;
			    #endif
            }
            //#NT#2018/06/13#YZ Bian -end


			if (gAlgFtgDbgLevel == 3)
			{
                debug_msg("num:%d sec:%d usec:%d\r\n", IPCamFDResult.uiFaceNumber, time_stamp_set.timestamp_sec, time_stamp_set.timestamp_usec);
                for(i = 0; i < IPCamFDResult.uiFaceNumber; i++)
                {
    				debug_msg("snapshot id:%d x:%d y:%d w:%d h:%d pose:%d luma:%d clarity:%d contrast:%d\r\n", \
    					IPCamFDResult.info[i].id, IPCamFDResult.info[i].x, IPCamFDResult.info[i].y, IPCamFDResult.info[i].w, IPCamFDResult.info[i].h, \
    					IPCamFDResult.info[i].pose, IPCamFDResult.info[i].luma, IPCamFDResult.info[i].clarity, IPCamFDResult.info[i].contrast);
			    }
            }

			#if (FTG_SNAPSHOT_DRAW == ENABLE)
			////draw rectangles
			if( ((IMG_BUF *)&isf_data.Desc[0]) && IPCamFDResult.uiFaceNumber)
			{
				UINT32	  drawColor = COLOR_YUV_BLUE;
				UINT32	  border = 8;
				IRECT	  DrawRect;
				for (i = 0; i < fd_number; i++) {
					DrawRect.x = IPCamFDResult.info[i].x;
					DrawRect.y = IPCamFDResult.info[i].y;
					DrawRect.w = IPCamFDResult.info[i].w;
					DrawRect.h = IPCamFDResult.info[i].h;
					MovieStamp_DrawRect((IMG_BUF *)&isf_data.Desc[0], &DrawRect, drawColor, border);
				}
			}
			#endif

            #if(IPCAM_FTG_SNAPSHOT == ENABLE)
			FTG_FACE *pFDresult = (FTG_FACE*)IPCamFDResult.info;
			IPCam_FTG_SaveFaceInfo((IMG_BUF *)&isf_data.Desc[0], pFDresult, IPCamFDResult.uiFaceNumber);
    		#endif

            #if(IPCAM_FTG_USE_ODT == DISABLE)
        	IPCamFDResult.Event = IPCAM_EVENT_FTG;
            IPCamFDResult.uiHasLandMark = 0;
            IPCamFDResult.TimeStamp.tv_sec = time_stamp_set.timestamp_sec;
            IPCamFDResult.TimeStamp.tv_usec = time_stamp_set.timestamp_usec;
        	NvtUctrl_NotifyRaw((void *)&IPCamFDResult, sizeof(IPCAM_FTG_RESULT));
            #endif
	    }
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
}



UINT32 IPCam_FTG_CalcBuffSize(void)
{
	nvt_ftg_config(NVT_FTG_FD_CFG_FACE_MAX_SCALE, IPCAM_FTG_FACESCALE);
#if (IPCAM_FTG_USE_MD == ENABLE && IPCAM_MD_FUNC == ENABLE)
	return nvt_ftg_calcbuffsize() + 8192;
#else
	return nvt_ftg_calcbuffsize();
#endif
}


PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_ftg = {
	{
		.Priority = PHOTO_PRIORITY_M,				///< function hook process Id. must be higher than PhotoFuncInfo_ipcam_ftgfd
		.IPLId = IPL_PATH_1,						///< IPL Id.
		.TrigEvent = PHOTO_ISR_IME_FRM_END,			///< isr tigger event
		.ProcName = "IPCam_FTGProcess",
        .InitFP = NULL,                             ///< init
		.ProcFP = IPCam_FTGProcess,					///< process FTG
		.EndFP = NULL,								///< process end FTG
		.GetBufFP = NULL,                			///< get working buffer FTG
		.GetCacheBufFP = NULL,           			///< get working cache buffer FTG
	},
	.pNext = NULL  ///< point next Function Obj, last Function must be set NULL
};

PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_ftgfd = {
	{
		.Priority = PHOTO_PRIORITY_L,               ///< function hook process Id. must be lower than PhotoFuncInfo_ipcam_ftg
		.IPLId = IPL_PATH_1,                        ///< IPL Id.
		.TrigEvent = PHOTO_ISR_IME_FRM_END,         ///< isr tigger event
		.ProcName = "IPCam_FTGFDProcess",
		.InitFP = IPCam_FTGInit,					///< init FTGFD, only execute at first process
		.ProcFP = IPCam_FTGFDProcess,               ///< process FTGFD
		.EndFP = NULL,                              ///< process end FTGFD
		.GetBufFP = IPCam_FTG_CalcBuffSize,			///< get working buffer FTGFD
		.GetCacheBufFP = nvt_ftg_calccachebuffsize,	///< get working cache buffer FTGFD

	},
	.pNext = NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2017/12/26#Yuzhe Bian -end

#if (IPCAM_ODT_FUNC)
void IPCam_ODTInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	odt_init(buf, cachebuf);
}

void IPCam_ODTUninit(void)
{
    odt_uninit();
}
UINT32 IPCam_ODT_CalcBuffSize(void)
{
	return odt_calcbuffsize();
}

UINT32 IPCam_ODT_CalcCacheBuffSize(void)
{
	return odt_calccachebuffsize();
}

void IPCam_ODTODProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	double now_time = 0;
	static INT32 start_index = -1;
	static double old_time = 0;

	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		now_time = clock();
		if (now_time - old_time <= 2000000) { ///2secs
			return;
		}
		start_index++;
		DBG_DUMP("ODTOD start!!\r\n");
	}

	if (!odt_getinitstate()) {
		return;
	}

	odt_od_process();
}

void IPCam_ODTProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	double now_time = 0;
	static INT32 start_index = -1;
	static double old_time = 0;

	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		now_time = clock();
		if (now_time - old_time <= 2000000) { ///2secs
			return;
		}
		start_index++;
		DBG_DUMP("ODT start!!\r\n");
	}

	if (!odt_getinitstate()) {
		return;
	}

	odt_ot_process();

#if (IPCAM_PD_FUNC && IPCAM_PD_USE_ODT)
	if(NULL != gPD_result)
	{
		static odt_obj pd_rslt[ODT_NUM_MAX];
		URECT targetcoord = {0, 0, 1920, 1080};
		UINT32 i;
		#if (IPCAM_PD_DRAW_VIDEO == DISABLE)
		targetcoord.w = gPDImgW;
		targetcoord.h = gPDImgH;
		#endif

		if(gPD_processId < ODT_PROCESS_NUM_MAX) {
			gPD_result->pd_result.people_counts = odt_getrsltobj_for_draw(pd_rslt, gPD_processId, &targetcoord);
		}
		for(i = 0; i < gPD_result->pd_result.people_counts; i++) {
			gPD_result->pd_result.target[i].id = pd_rslt[i].id;
			gPD_result->pd_result.target[i].x = pd_rslt[i].obj_rect.x;
			gPD_result->pd_result.target[i].y = pd_rslt[i].obj_rect.y;
			gPD_result->pd_result.target[i].w = pd_rslt[i].obj_rect.w;
			gPD_result->pd_result.target[i].h = pd_rslt[i].obj_rect.h;
		}
		gPD_result->Event = IPCAM_EVENT_PD;
		NvtUctrl_NotifyRaw((void *)gPD_result, sizeof(IPCAM_PD_RESULT));
	}
#endif

#if (IPCAM_MD_FUNC && IPCAM_MD_USE_ODT)
	if(NULL != gMD_result)
	{
		static odt_obj md_rslt[ODT_NUM_MAX];
		URECT targetcoord = {0, 0, gMD_width, gMD_height};
		UINT32 i;

		if(gMD_processId < ODT_PROCESS_NUM_MAX) {
			gMD_result->md_reslt.obj_num = odt_getrsltobj_for_draw(md_rslt, gMD_processId, &targetcoord);
		}
		for(i = 0; i < gMD_result->md_reslt.obj_num; i++) {
			gMD_result->md_reslt.obj[i].start_x = md_rslt[i].obj_rect.x;
			gMD_result->md_reslt.obj[i].start_y = md_rslt[i].obj_rect.y;
			gMD_result->md_reslt.obj[i].end_x = md_rslt[i].obj_rect.x + md_rslt[i].obj_rect.w - 1;
			gMD_result->md_reslt.obj[i].end_y = md_rslt[i].obj_rect.y + md_rslt[i].obj_rect.h - 1;
		}
		gMD_result->Event = IPCAM_EVENT_MD;
		NvtUctrl_NotifyRaw((void *)gMD_result, sizeof(IPCAM_MD_RESULT));
	}
#endif

#if (IPCAM_BC_FUNC && IPCAM_BC_USE_ODT)
	if(NULL != gBC_result)
	{
		static odt_obj bc_rslt[ODT_NUM_MAX];
		URECT targetcoord = {0, 0, gBC_imgW, gBC_imgH};
		UINT32 i;

		if(gBC_processId < ODT_PROCESS_NUM_MAX) {
			gBC_result->result.targetnum = odt_getrsltobj_for_draw(bc_rslt, gBC_processId, &targetcoord);
		}
		for(i = 0; i < gBC_result->result.targetnum; i++) {
			gBC_result->result.target[i].x = bc_rslt[i].obj_rect.x;
			gBC_result->result.target[i].y = bc_rslt[i].obj_rect.y;
			gBC_result->result.target[i].w = bc_rslt[i].obj_rect.w;
			gBC_result->result.target[i].h = bc_rslt[i].obj_rect.h;
		}
		gBC_result->event = IPCAM_EVENT_BC;
		NvtUctrl_NotifyRaw((void *)gBC_result, sizeof(IPCAM_BC_RESULT));
	}
#endif
#if (IPCAM_FD_FUNC && IPCAM_FD_USE_ODT)
	if(NULL != gFD_result)
	{
		static odt_obj fd_rslt[ODT_NUM_MAX];
		URECT targetcoord = {0, 0, gFdWidth, gFdHeight};
		UINT32 i;
		if(gFD_processId < ODT_PROCESS_NUM_MAX) {
			gFD_result->uiFaceNumber = odt_getrsltobj_for_draw(fd_rslt, gFD_processId, &targetcoord);
		}
		for(i = 0; i < gFD_result->uiFaceNumber; i++) {
            gFD_result->info[i].x = fd_rslt[i].obj_rect.x;
    		gFD_result->info[i].y = fd_rslt[i].obj_rect.y;
    		gFD_result->info[i].w = fd_rslt[i].obj_rect.w;
    		gFD_result->info[i].h = fd_rslt[i].obj_rect.h;
            gFD_result->info[i].id= fd_rslt[i].id;
            gFD_result->TimeStamp.tv_sec = fd_rslt[i].timestamp.timestamp_sec;
            gFD_result->TimeStamp.tv_usec = fd_rslt[i].timestamp.timestamp_usec;
		}
        gFD_result->Event =IPCAM_EVENT_FD;
        #if 0
    	DBG_DUMP("num:%d sec:%d usec:%d\r\n",gFD_result->uiFaceNumber, gFD_result->TimeStamp.tv_sec, gFD_result->TimeStamp.tv_usec);
    	for (i = 0; i < gFD_result->uiFaceNumber; i++) {
    		DBG_DUMP("x:%d y:%d w:%d h:%d id:%d\r\n",
    			gFD_result->info[i].x,
    			gFD_result->info[i].y,
    			gFD_result->info[i].w,
    			gFD_result->info[i].h,
    			gFD_result->info[i].id
    		);
    	}
    	#endif
		NvtUctrl_NotifyRaw((void *)gFD_result, sizeof(IPCAM_FD_INFO));
	}
#endif

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_odt = {
	{
		.Priority = PHOTO_PRIORITY_H,				///< function hook process Id. must be higher than PhotoFuncInfo_ipcam_odtod
		.IPLId = IPL_PATH_1,						///< IPL Id.
		.TrigEvent = PHOTO_ISR_IME_FRM_END,			///< isr tigger event
		.ProcName = "IPCam_ODTProcess",
		.InitFP = IPCam_ODTInit,					///< init, only execute at first process
		.ProcFP = IPCam_ODTProcess,					///< process
		.EndFP = NULL,								///< process end
        .ExitFp = IPCam_ODTUninit,
		.GetBufFP = IPCam_ODT_CalcBuffSize,			///< get working buffer
		.GetCacheBufFP = IPCam_ODT_CalcCacheBuffSize,	///< get working cache buffer
	},
	.pNext = NULL  ///< point next Function Obj, last Function must be set NULL
};

PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_odtod = {
	{
		.Priority = PHOTO_PRIORITY_H,               ///< function hook process Id. must be lower than PhotoFuncInfo_ipcam_odt
		.IPLId = IPL_PATH_1,                        ///< IPL Id.
		.TrigEvent = PHOTO_ISR_IME_FRM_END,         ///< isr tigger event
		.ProcName = "IPCam_ODTODProcess",
		.InitFP = NULL,                    			///< init, only execute at first process
		.ProcFP = IPCam_ODTODProcess,               ///< process
		.EndFP = NULL,                              ///< process end
		.GetBufFP = NULL,                			///< get working buffer
		.GetCacheBufFP = NULL,           			///< get working cache buffer
	},
	.pNext = NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif


#if (IPCAM_DIS_MV_FUNC)
#include "af_api.h"
#include "ipl_header_infor_raw.h"
#include "dis_alg.h"
#include "ipl_cmd.h"
#include "dis_api.h"

void IPCam_DIS_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	dis_initialize(buf, cachebuf);
#if (IPCAM_DIS_COMP_FUNC)
	dis_setDisViewRatio(900);
#else
    dis_setDisViewRatio(1000);
#endif
    DIS_InstallCmd();
    CHKPNT;
}

//#NT#2017/10/02#Harry_Tsai -begin
//#NT rearrange DIS structure
void IPCam_DIS(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //if(dis_time_stamp_ready())
    {
#if (IPCAM_DIS_COMP_FUNC)
    	dis_compensation(buf,cachebuf,FALSE);
#else
        dis_compensation(buf,cachebuf,TRUE);
#endif

#if (IPCAM_ODT_FUNC)
        odt_savedisrslt();
#endif
    }
}
//#NT#2017/10/02#Harry_Tsai -end

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamdis = {
	{
		PHOTO_PRIORITY_H,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		//#NT#2016/05/04#Harry_Tsai -begin
		//#NT# Change to VD
		PHOTO_ISR_IPE_FRM_END,       ///< isr tigger event
		"IPCam_DIS",
		//#NT#2016/05/04#Harry_Tsai -end
		IPCam_DIS_Init,                   ///< init fp, only execute at first process
		IPCam_DIS,           	///< process fp
		NULL,                   ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		dis_GetPrvMaxBuffer,        ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#else
PHOTO_FUNC_INFO PhotoFuncInfo_IPCamdis = {0};
#endif

static BOOL gPDStartFlag = FALSE;
static BOOL gPDStatusFlag = FALSE;
static UINT64 gPDFrameTimeUS = 200000; //200ms
static UINT64 gPDLastFrameTime = 0;
static UINT64 gPDDiffFrameTimeUS = 0;
//for pd_lib
static UINT64 gPDFrameTime 		= 200000; //200ms
static UINT32 gPDMDAlarmTh 		= 36;
static UINT32 gNMSRatio			= 60;
static UINT32 gCOVERNMSRatio	= 55;
static UINT32 gPDClassifyMethod = 0; //0: full-body + half-body, 1: full-body, 2: half-body
static UINT32 gScoreBias 		= 3300; //4800;
static UINT32 gScoreBias_night	= 1600; //4800;
static UINT32 gPDRefMD 			= 10;
static UINT32 gPDSpeedUpMDFlag 	= 0;
static UINT32 gPDNightVision 	= 0;
static UINT32 gPDAlarmStableCountThres = 1;
static UINT32 gPDROIRange[4] 	= {0, 0, IPCAM_MAIN_WIDTH, IPCAM_MAIN_HEIGHT};

PIMG_BUF gpPDImgBuf = 0;
UINT32 gPDImgW = 1920;
UINT32 gPDImgH = 1080;
UINT32 gPD_processId = 999;

IPCAM_PD_RESULT *gPD_result;
void Movie_PdSetFps(UINT32 fps)
{
	gPDLastFrameTime = 0;
	gPDDiffFrameTimeUS = 0;
	if (fps == 0) {
		gPDStartFlag = FALSE;
	} else {
		gPDFrameTimeUS = 1000000 / fps;
		gPDStartFlag = TRUE;
	}
	gPDFrameTime = gPDFrameTimeUS;
}

void Movie_PdSetNightVision(UINT32 NightVision)
{
	if(NightVision > 0)
	{
		DBG_WRN("set night mode!!\r\n");
		gPDNightVision = 1;
		pd_setprms(PD_NIGHT_VISION, gPDNightVision);
	}
	else
	{
		DBG_WRN("set day mode!!\r\n");
		gPDNightVision = 0;
		pd_setprms(PD_NIGHT_VISION, gPDNightVision);
	}
}

void Movie_PdFtgSetROI(URECT roi)
{
#if IPCAM_FTG_FUNC
    nvt_ftg_roi_config(roi);
#endif
#if IPCAM_PD_FUNC
	gPDROIRange[0] = roi.x;
	gPDROIRange[1] = roi.y;
	gPDROIRange[2] = roi.w;
	gPDROIRange[3] = roi.h;
	pd_setprms(PD_ROI_RANGE_X, gPDROIRange[0]);
	pd_setprms(PD_ROI_RANGE_Y, gPDROIRange[1]);
	pd_setprms(PD_ROI_RANGE_W, gPDROIRange[2]);
	pd_setprms(PD_ROI_RANGE_H, gPDROIRange[3]);
#endif
}

UINT32 *Movie_PdFtgGetROI(void)
{
	//URECT roi_rect = {roi[0], roi[1], roi[2], roi[3]}; // default max roi
	return gPDROIRange;
}

PD_RESULT *Movie_PdGetResult(void)
{
	return &(gPD_result->pd_result);
}

#if IPCAM_PD_FUNC
void Photo_PD_Process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	clock_t t1, t2, t3, t4;
	static clock_t old_time = 0;
	static INT32 start_index = -1;

	//for pd_lib param
	UINT32 alarm_sig = 0;
	static UINT32 pd_num = 0;
	static URECT pd_rslts[PD_MAX_NUM];
	
	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		if (clock() - old_time <= 1000000) { ///1secs
			return;
		}
		start_index++;
		pd_setprms(PD_MD_ALARM_TH			, gPDMDAlarmTh);	
		pd_setprms(PD_NMS_RATIO				, gNMSRatio);	
		pd_setprms(PD_COVER_REMOVING_RATIO	, gCOVERNMSRatio);
		pd_setprms(PD_CLASSIFY_METHOD		, gPDClassifyMethod);	
		pd_setprms(PD_SPEEDUP_MD			, gPDSpeedUpMDFlag);	
		pd_setprms(PD_REFINE_MD				, gPDRefMD);	
		pd_setprms(PD_SCORE_BIAS			, gScoreBias);
		pd_setprms(PD_SCORE_BIAS_NIGHT		, gScoreBias_night);
		pd_setprms(PD_NIGHT_VISION			, gPDNightVision);
		pd_setprms(PD_STABLE_COUNT_TH		, gPDAlarmStableCountThres);
		pd_setprms(PD_ROI_RANGE_X			, gPDROIRange[0]);
		pd_setprms(PD_ROI_RANGE_Y			, gPDROIRange[1]);
		pd_setprms(PD_ROI_RANGE_W			, gPDROIRange[2]);
		pd_setprms(PD_ROI_RANGE_H			, gPDROIRange[3]);
		DBG_DUMP("PD detection start!!\r\n");
	}
	if (gPDStartFlag == FALSE) {
		return;
	}
	if (gPD_result == 0) {
		DBG_WRN("Get pd result buf fail\n");
	}

#if (IPCAM_ODT_FUNC == ENABLE && IPCAM_PD_USE_ODT == ENABLE)
	static INT32					regist_flag = ODT_STA_ERROR;
	static UINT32					pd_rslt_loc;
	static odt_od_result 			pd_odt_rslts[PD_MAX_NUM];

	static odt_process_buf_param 	pd_odt_buf_param;
	static odt_rslt_inf				pd_buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				pd_buf2;
	static odt_od_result 			pd_buf3[PD_MAX_NUM];

	pd_odt_buf_param.detect_rslt_addr = (UINT32)pd_buf3;
	pd_odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&pd_buf2);
	pd_odt_buf_param.odt_rslt_buffer_addr = (UINT32)pd_buf1;

	if (regist_flag == ODT_STA_ERROR) {
		regist_flag = odt_regist(&pd_odt_buf_param);//regist pd to odt and assign an id to pd
		if (regist_flag == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 15, pd_odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 1, pd_odt_buf_param.process_id);
			gPD_processId = pd_odt_buf_param.process_id;
		}
	}
#endif

	ISF_DATA isf_data;
	ISF_PORT *pSrc;
	PIMG_BUF pImgBuf;
#if IPCAM_MD_FUNC
	IMG_BUF FGBuf = {0};
#endif
	UINT32 TimeStamp_sec, TimeStamp_usec;
	UINT32 LastTimeStamp_sec, LastTimeStamp_usec;
	INT32 DiffTimeStamp_sec, DiffTimeStamp_usec;

	pSrc = ImageUnit_Out(&ISF_UserProc, ISF_OUT3);
	ImageUnit_Begin(&ISF_UserProc, 0);
	if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 1) != ISF_OK) {
		DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 1 Fail\r\n");
		return;
	}
	ImageUnit_End();
	if (ImageUnit_IsAllowPull(pSrc) == 0) {
		DBG_WRN("ImageUnit_IsAllowPull fail\r\n");
		return;
	}
	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) != ISF_OK) {
		DBG_WRN("ImageUnit_PullData fail\r\n");
		ImageUnit_Begin(&ISF_UserProc, 0);
		if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 0) != ISF_OK) {
			DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
			return;
		}
		ImageUnit_End();
		return;
	}
	ImageUnit_Begin(&ISF_UserProc, 0);
	if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 0) != ISF_OK) {
		DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
		return;
	}
	ImageUnit_End();
	pImgBuf = (IMG_BUF *)&isf_data.Desc[0];
	if (!pImgBuf) {
		//ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}
	if (gPDLastFrameTime == 0)
		gPDLastFrameTime = isf_data.TimeStamp;

	TimeStamp_sec = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
	TimeStamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
	LastTimeStamp_sec = (gPDLastFrameTime >> 32) & 0xFFFFFFFF;
	LastTimeStamp_usec = gPDLastFrameTime & 0xFFFFFFFF;

	DiffTimeStamp_sec = (long)TimeStamp_sec - (long)LastTimeStamp_sec;
	DiffTimeStamp_usec = (long)TimeStamp_usec - (long)LastTimeStamp_usec;
	gPDDiffFrameTimeUS += DiffTimeStamp_sec * 1000000 + DiffTimeStamp_usec;
	gPDLastFrameTime = isf_data.TimeStamp;

	if (gPDDiffFrameTimeUS > gPDFrameTimeUS) {
		gPDDiffFrameTimeUS -= gPDFrameTimeUS;

#if (IPCAM_ODT_FUNC == ENABLE && IPCAM_PD_USE_ODT == ENABLE)
		pd_rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
#endif

		t1 = clock();
		pd_setimg(pImgBuf); // set image buffer
		gPDImgW = pImgBuf->Width;
		gPDImgH = pImgBuf->Height;
#if IPCAM_MD_FUNC
		// set md info
		HEADER_YUV_MD_INFO *md_info = NULL;
		md_info = (HEADER_YUV_MD_INFO *)(pImgBuf->Metadata.Addr);
		FGBuf.PxlAddr[0] = md_info->evt_addr;
		FGBuf.LineOffset[0] = (((HEADER_YUV_MD_INFO *)(pImgBuf->Metadata.Addr))->win_width_num);
		FGBuf.Width = (((HEADER_YUV_MD_INFO *)(pImgBuf->Metadata.Addr))->win_width_num);
		FGBuf.Height = (((HEADER_YUV_MD_INFO *)(pImgBuf->Metadata.Addr))->win_y_num);
		pd_setfg(&FGBuf);
#endif

		t2 = clock();
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		pd_process();
		t3 = clock();
		alarm_sig = 0;

		//get pd results and pass them to odt
#if (IPCAM_ODT_FUNC == ENABLE && IPCAM_PD_USE_ODT == ENABLE)
		UINT32 i;
		URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
		pd_num = pd_gettarget(pd_rslts, &targetcoord, PD_MAX_NUM, &alarm_sig);
		for(i = 0; i < pd_num; i++)
		{
			pd_odt_rslts[i].x = pd_rslts[i].x;
			pd_odt_rslts[i].y = pd_rslts[i].y;
			pd_odt_rslts[i].w = pd_rslts[i].w;
			pd_odt_rslts[i].h = pd_rslts[i].h;
			pd_odt_rslts[i].score = 0;
			pd_odt_rslts[i].class = 1001;
		}
		odt_setodrslt(pd_num, pd_odt_rslts, pd_rslt_loc, pd_odt_buf_param.process_id);
#else
		URECT	rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = IPCAM_MAIN_WIDTH;
		rect.h = IPCAM_MAIN_HEIGHT;
		pd_num = pd_gettarget(pd_rslts, &rect, PD_MAX_NUM, &alarm_sig);
	
		gPD_result->pd_result.people_counts = pd_num;
		UINT32 i = 0;
		for(i = 0; i < gPD_result->pd_result.people_counts; i++) 
		{
			gPD_result->pd_result.target[i].id = 0;
			gPD_result->pd_result.target[i].x  = pd_rslts[i].x;
			gPD_result->pd_result.target[i].y  = pd_rslts[i].y;
			gPD_result->pd_result.target[i].w  = pd_rslts[i].w;
			gPD_result->pd_result.target[i].h  = pd_rslts[i].h;
		}
#endif
		t4 = clock();
		
		DBG_IND("PD SetTime(%d) ProcTime(%d) GetTime(%d) NowTime(%d, %d)\r\n",
				 (t2-t1), (t3-t2), (clock()-t3), LastTimeStamp_sec,LastTimeStamp_usec);

		if ((t2 - t1) > 20000) { //20ms
			gPDFrameTimeUS = 1000 * 1000;
			DBG_WRN("PD ProcessTime=%ld us, adjust FrameTime=%llu us\r\n", (t2 - t1), gPDFrameTimeUS);
		}
		else if(gPDFrameTimeUS >= 1000000) {
			gPDFrameTimeUS = gPDFrameTime;
			DBG_WRN("PD ProcessTime=%ld us, adjust FrameTime=%llu us\r\n", (t2 - t1), gPDFrameTimeUS);
		}
	} else {
		//ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}

	if (__DBGLVL__ >= 5 || gPDStatusFlag) {
		/*UINT32 idx = 0;
		//DBG_DUMP("PD people number(%u)\r\n",gPD_result->pd_result.people_counts);
		for (idx = 0; idx < gPD_result->pd_result.people_counts; idx++) {
			DBG_DUMP("PD idx(%u) rect(%u, %u, %u, %u)\r\n", idx,
					gPD_result->pd_result.target[idx].x, gPD_result->pd_result.target[idx].y,
					gPD_result->pd_result.target[idx].w, gPD_result->pd_result.target[idx].h);
		}*/
		DBG_DUMP("pd_prm: night(%d) clsm(%d) refmd(%d) spdmd(%d) bias(%d, %d) nms(%d,%d) stb(%d) mdth(%d)\r\n", gPDNightVision, gPDClassifyMethod, gPDRefMD, gPDSpeedUpMDFlag, gScoreBias, gScoreBias_night, gNMSRatio, gCOVERNMSRatio, gPDAlarmStableCountThres, gPDMDAlarmTh);
		DBG_DUMP("PD %lluus %dx%d roi(%d,%d,%d,%d) Set(%d) Proc(%d) Get(%d) alarm_sig(%d) (%d)\r\n", gPDFrameTimeUS, pImgBuf->Width, pImgBuf->Height, gPDROIRange[0], gPDROIRange[1], gPDROIRange[2], gPDROIRange[3], t2-t1, t3-t2, t4-t3, alarm_sig, gPD_result->pd_result.people_counts);
	}

	gPD_result->Event = IPCAM_EVENT_PD;
	gPD_result->TimeStamp.tv_sec = TimeStamp_sec;
	gPD_result->TimeStamp.tv_usec = TimeStamp_usec;
	NvtUctrl_NotifyRaw((void *)gPD_result, sizeof(IPCAM_PD_RESULT));
}

void Photo_PD_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (buf->Addr && cachebuf->Addr) {
		gPDStartFlag = FALSE;
		gPDFrameTimeUS = 200000;
		gPDFrameTime = gPDFrameTimeUS;
		gPD_result = (IPCAM_PD_RESULT *)cachebuf->Addr;
		memset(gPD_result, 0, sizeof(IPCAM_PD_RESULT));
		pd_init((UINT32)buf->Addr, 0);
		DBG_DUMP("Photo_PD_Init buf(%#x %d) cachebuf(%#x %d)\r\n",
				buf->Addr, buf->Size, cachebuf->Addr, cachebuf->Size);
	} else {
		DBG_ERR("get PD buf fail\r\n");
	}
}

UINT32 Photo_PD_GetBuf(void)
{
	return pd_calcbuffsize();
}

UINT32 Photo_PD_GetCacheBuf(void)
{
	return sizeof(IPCAM_PD_RESULT);
}

PHOTO_FUNC_INFO PhotoFuncInfo_pd = {{
		PHOTO_PRIORITY_M, 		///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_SIE_VD,       ///< isr tigger event
		"PD", Photo_PD_Init,  	///< init fp, only execute at first process
		Photo_PD_Process,      	///< process fp
		NULL,  					///< process end fp
		NULL,            		///< exit fp, only execute at photo close
		Photo_PD_GetBuf,      	///< get working buffer fp
		Photo_PD_GetCacheBuf,	///< get working cache buffer fp
	},
	NULL ///< point next Function Obj, last Function must be set NULL
};
#endif

static BOOL cmd_pd_set(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gPDLastFrameTime = 0;
	gPDDiffFrameTimeUS = 0;
	if (value == 0) {
		gPDStartFlag = FALSE;
	} else {
		gPDFrameTimeUS = 1000000 / value;
		gPDStartFlag = TRUE;
	}
	gPDFrameTime = gPDFrameTimeUS;
	DBG_DUMP("PD FrameTimeUS(%llu), StartFlag(%d), ResultFlag(%d)\r\n", gPDFrameTimeUS, gPDStartFlag, gPDStatusFlag);
	return TRUE;
}

static BOOL cmd_pd_status(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	if (value == 0) {
		gPDStatusFlag = FALSE;
	} else {
		gPDStatusFlag = TRUE;
	}
	DBG_DUMP("PD FrameTimeUS(%llu), StartFlag(%d), ResultFlag(%d)\r\n", gPDFrameTimeUS, gPDStartFlag, gPDStatusFlag);
	return TRUE;
}

static BOOL cmd_pd_prmmd(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gPDMDAlarmTh = value;
	pd_setprms(PD_MD_ALARM_TH, gPDMDAlarmTh);	
	DBG_DUMP("PD_MD alarm threhold =(%d)\r\n", gPDMDAlarmTh);
	return TRUE;
}

static BOOL cmd_pd_prmnms(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gNMSRatio = value;
	pd_setprms(PD_NMS_RATIO, gNMSRatio);	
	DBG_DUMP("PD NMS Ratio =(%d)\r\n", gNMSRatio);
	return TRUE;
}

static BOOL cmd_pd_prmcovernms(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gCOVERNMSRatio = value;
	pd_setprms(PD_COVER_REMOVING_RATIO, gCOVERNMSRatio);
	DBG_DUMP("PD Cover NMS Ratio =(%d)\r\n", gCOVERNMSRatio);
	return TRUE;
}

static BOOL cmd_pd_prmclsm(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gPDClassifyMethod = value;
	pd_setprms(PD_CLASSIFY_METHOD, gPDClassifyMethod);	
	DBG_DUMP("PD classify method =(%d)\r\n", gPDClassifyMethod);
	return TRUE;
}

static BOOL cmd_pd_spdmd(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gPDSpeedUpMDFlag = value;
	pd_setprms(PD_SPEEDUP_MD, gPDSpeedUpMDFlag);
	if (value == 0) {
		DBG_DUMP("speedup off\r\n");
	} else {
		gPDSpeedUpMDFlag = value;
		DBG_DUMP("speedup on\r\n");
	}	
	return TRUE;
}

static BOOL cmd_pd_prmbias(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gScoreBias = value;
	pd_setprms(PD_SCORE_BIAS, gScoreBias);
	DBG_DUMP("PD Score Bias =(%d)\r\n", gScoreBias);
	return TRUE;
}

static BOOL cmd_pd_prmnightbias(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gScoreBias_night = value;
	pd_setprms(PD_SCORE_BIAS_NIGHT, gScoreBias_night);
	DBG_DUMP("PD Score Bias for night =(%d)\r\n", gScoreBias_night);
	return TRUE;
}

static BOOL cmd_pd_refmd(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gPDRefMD = value;
	pd_setprms(PD_REFINE_MD, gPDRefMD);
	if (value == 0) {
		DBG_DUMP("ref md off\r\n");
	} else {
		gPDRefMD = value;
		DBG_DUMP("ref md on: %d\r\n", gPDRefMD);
	}	
	return TRUE;
}

static BOOL cmd_pd_prmnight(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gPDNightVision = value;
	pd_setprms(PD_NIGHT_VISION, gPDNightVision);
	if (value == 0) {
		DBG_DUMP("night vision off\r\n");
	} else {
		DBG_DUMP("night vision on\r\n");
	}	
	return TRUE;
}

static BOOL cmd_pd_prmstable(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gPDAlarmStableCountThres = value;
	pd_setprms(PD_STABLE_COUNT_TH, gPDAlarmStableCountThres);
	DBG_DUMP("PD Alarm Stable Count Thres =(%d)\r\n", gPDAlarmStableCountThres);        
	return TRUE;
}

static BOOL cmd_pd_prmroi(CHAR *strCmd)
{
	INT32 value1 = 0, value2 = 0, value3 = 0, value4 = 0;
	sscanf_s(strCmd, "%d %d %d %d", &value1, &value2, &value3, &value4);
	if((value1 >= 0) && (value2 >= 0) && (value3 >= 0) && (value4 >= 0))
	{
		gPDROIRange[0] = value1;
		gPDROIRange[1] = value2;
		gPDROIRange[2] = value3;
		gPDROIRange[3] = value4;
		pd_setprms(PD_ROI_RANGE_X, gPDROIRange[0]);
		pd_setprms(PD_ROI_RANGE_Y, gPDROIRange[1]);
		pd_setprms(PD_ROI_RANGE_W, gPDROIRange[2]);
		pd_setprms(PD_ROI_RANGE_H, gPDROIRange[3]);
		DBG_DUMP("PD ROI: x%d, y%d, w%d, h%d\r\n", gPDROIRange[0], gPDROIRange[1], gPDROIRange[2], gPDROIRange[3]);
	}
	else
		DBG_DUMP("PD ROI: set error (%d, %d) - (%d, %d)\r\n", value1, value2, value3, value4);
	return TRUE;
}

static BOOL cmd_fdt_pd_set_roi(CHAR *strCmd)
{
    URECT roi_rect = {0, 0, IPCAM_MAIN_WIDTH, IPCAM_MAIN_HEIGHT}; // default max roi
	sscanf_s(strCmd, "%d %d %d %d", &roi_rect.x, &roi_rect.y, &roi_rect.w, &roi_rect.h);
    DBG_DUMP("FD PD ROI: %s\r\n", strCmd);
	
#if IPCAM_FTG_FUNC
	URECT DispCord = {0, 0, gFtgWidth, gFtgHeight};
    nvt_ftg_roi_config(roi_rect);
    nvt_ftg_roi_getconfig(&roi_rect, &DispCord);
#endif

#if IPCAM_PD_FUNC
	gPDROIRange[0] = roi_rect.x;
	gPDROIRange[1] = roi_rect.y;
	gPDROIRange[2] = roi_rect.w;
	gPDROIRange[3] = roi_rect.h;
	pd_setprms(PD_ROI_RANGE_X			, gPDROIRange[0]);
	pd_setprms(PD_ROI_RANGE_Y			, gPDROIRange[1]);
	pd_setprms(PD_ROI_RANGE_W			, gPDROIRange[2]);
	pd_setprms(PD_ROI_RANGE_H			, gPDROIRange[3]);
    DBG_DUMP("Set FD PD ROI: %d %d %d %d\r\n", roi_rect);
#endif
    return TRUE;
}

SXCMD_BEGIN(pd, "pd")
SXCMD_ITEM("set %", cmd_pd_set, "set fps, fps=0 to stop")
SXCMD_ITEM("status %", cmd_pd_status, "pd status, 1=dump")
SXCMD_ITEM("prmmd %", cmd_pd_prmmd, "md alarm th")
SXCMD_ITEM("prmnms %", cmd_pd_prmnms, "pd nms ratio")
SXCMD_ITEM("prmcovernms %", cmd_pd_prmcovernms, "pd cover nms ratio")
SXCMD_ITEM("prmclsm %", cmd_pd_prmclsm, "classify method")
SXCMD_ITEM("spdmd %", cmd_pd_spdmd, "Speedup by md evt, 0=without md evt")
SXCMD_ITEM("prmbias %", cmd_pd_prmbias, "pd score bias")
SXCMD_ITEM("prmnightbias %", cmd_pd_prmnightbias, "pd score bias for night")
SXCMD_ITEM("refmd %", cmd_pd_refmd, "ref md evt, 0=without md evt")
SXCMD_ITEM("prmnight %", cmd_pd_prmnight, "pd night vision")
SXCMD_ITEM("prmstable %", cmd_pd_prmstable, "pd stable")
SXCMD_ITEM("prmroi % % % %", cmd_pd_prmroi, "pd roi setting")
SXCMD_ITEM("fdtroi % % % %", cmd_fdt_pd_set_roi, "set pd & fdt input roi")
SXCMD_END()


UINT32 gBC_imgW = 1920;
UINT32 gBC_imgH = 1080;
UINT32 gBC_processId = 999;

void Movie_SetBcParam(BCParam *param)
{
	memcpy(&g_BcParam, param, sizeof(BCParam));
	gBCLastFrameTime = 0;
	gBCDiffFrameTimeUS = 0;
	DBG_DUMP("Bc param_info:fps=%d bc_mode=%d bc_deghost_en=%d bc_update_prob=%d\r\n", g_BcParam.fps, g_BcParam.bc_mode,
		g_BcParam.param_info.bc_deghost_en, g_BcParam.param_info.bc_update_prob);
	if (g_BcParam.fps == 0) {
		gBCStartFlag = FALSE;
	} else {
		bc_setprms(BC_RESET,              g_BcParam.param_info.bc_reset);
		bc_setprms(BC_RSLT_NEW,           g_BcParam.param_info.bc_rslt_new);
		bc_setprms(BC_UPDATE_PROB,        g_BcParam.param_info.bc_update_prob);
		bc_setprms(BC_UPDATE_NEIGHBOR_EN, g_BcParam.param_info.bc_update_neighbor_en);
		bc_setprms(BC_DEGHOST_EN,         g_BcParam.param_info.bc_deghost_en);
		bc_setprms(BC_MAX_FG_FRM_NUM,     g_BcParam.param_info.bc_max_fg_frm_num);
		bc_setprms(BC_MIN_OBJ_AREA_PCT,   g_BcParam.param_info.bc_min_obj_area_pct);
		bc_setprms(BC_TH_CROSS_FRM_NUM,   g_BcParam.param_info.bc_th_cross_frm_num);
		bc_setprms(BC_TH_FEATURE,         g_BcParam.param_info.bc_th_feature);
		bc_setprms(BC_SENSI,              g_BcParam.param_info.bc_sensi);
		gBC_usr_dim.x = 0;
		gBC_usr_dim.y = 0;
		gBC_usr_dim.w = g_BcParam.param_info.bc_width;
		gBC_usr_dim.h = g_BcParam.param_info.bc_height;
		gBCFrameTimeUS = 1000000 / g_BcParam.fps;
		gBCFrameTime = gBCFrameTimeUS;
		gBCStartFlag = TRUE;
	}
}

BC_RESULT *Movie_BcGetResult(void)
{
	if(gBC_result == NULL){
		return NULL;
	}
	return &(gBC_result->result);
}

#if IPCAM_BC_FUNC

void IPCam_BCYUVEnable(BOOL enable){

	gBCYUVFlag = enable;
}

void IPCam_BCLock(BOOL enable){
	gBCStartFlag = enable;
}

static INT32 IPCam_BCGetSource(void){


    if(gBCYUVFlag  == TRUE){
		////find YUV path , if there is no YUV path return -1
		INT32 YUVPath = UIAppIPCam_get_YUVPath();
		IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
		if(YUVPath != -1 && pStreamInfo[YUVPath].enable == IPCAM_STATE_ENABLE){
			return YUVPath;
		}
		else{
			///YUV path not set can not run BC
			if(YUVPath  >= 0){
				DBG_WRN("YUV path:%d  enable:%d can not run BC with YUV\r\n",YUVPath,pStreamInfo[YUVPath].enable);
			}
			else{
				DBG_WRN("YUV path:%d  can not run BC with YUV\r\n",YUVPath);
			}
			return -1;
		}
	}
	else{

		return IPCAM_MD_PATH;
	}


	return -1;
}

void IPCam_BC_Process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


ISF_OUT   sISFOutMap[IPCAM_PATH_MAX_ID * 2] = {
	       ISF_OUT1, ISF_OUT2,ISF_OUT3, ISF_OUT4,
		   ISF_OUT5, ISF_OUT6,ISF_OUT7, ISF_OUT8,
		   ISF_OUT9, ISF_OUT10,ISF_OUT11, ISF_OUT12,
		   ISF_OUT13, ISF_OUT14,ISF_OUT15, ISF_OUT16,
#if defined(_BSP_NA51000_)
		   ISF_OUT17, ISF_OUT18,ISF_OUT19, ISF_OUT20,
		   ISF_OUT21, ISF_OUT22,ISF_OUT23, ISF_OUT24,
	       ISF_OUT25, ISF_OUT26,ISF_OUT27, ISF_OUT28,
		   ISF_OUT29, ISF_OUT14,ISF_OUT31, ISF_OUT32
#endif
};
	clock_t t1, t2, t3;
	static clock_t old_time = 0;
	static INT32 start_index = -1;

	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		if (clock() - old_time <= 4000000) { ///1secs
			return;
		}
		start_index++;
		DBG_DUMP("BC start!!\r\n");
	}
	if (gBCStartFlag == FALSE && gBCYUVFlag == FALSE) {
		return;
	}

	#if (IPCAM_ODT_FUNC && IPCAM_BC_USE_ODT)
	static INT32					regist_flag = ODT_STA_ERROR;
	static UINT32					rslt_loc;
	static UINT32					num;
	static URECT					rslts[32];
	static odt_od_result 			odt_rslts[32];

	static odt_process_buf_param 	odt_buf_param;
	static odt_rslt_inf				buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				buf2;
	static odt_od_result 			buf3[32];

	odt_buf_param.detect_rslt_addr = (UINT32)buf3;
	odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&buf2);
	odt_buf_param.odt_rslt_buffer_addr = (UINT32)buf1;

	if (regist_flag == ODT_STA_ERROR) {
		regist_flag = odt_regist(&odt_buf_param);
		if (regist_flag == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 5, odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 0, odt_buf_param.process_id);
			gBC_processId = odt_buf_param.process_id;
		}
	}
	#endif

	ISF_DATA isf_data;
	ISF_PORT *pSrc;
	PIMG_BUF pImgBuf;
	UINT32 TimeStamp_sec, TimeStamp_usec;
	UINT32 LastTimeStamp_sec, LastTimeStamp_usec;
	INT32 DiffTimeStamp_sec, DiffTimeStamp_usec;
	IMG_BUF foreground = {0};
	foreground.PxlAddr[0] = (UINT32)(gBC_result->result.evt_map_addr);

	INT32 SourcePath = IPCam_BCGetSource();

	if(SourcePath < 0){
		return;
	}
	pSrc = ImageUnit_Out(&ISF_UserProc, sISFOutMap[SourcePath]);
	ImageUnit_Begin(&ISF_UserProc, 0);
		if (ImageUnit_SetParam(sISFOutMap[SourcePath], USERPROC_PARAM_ALLOW_PULL_IMM, 1) != ISF_OK) {
			DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 1 Fail\r\n");
			return;
		}
		if(gBCYUVFlag == TRUE){
			ImageUnit_SetParam(sISFOutMap[SourcePath], USERPROC_PARAM_BYPASS_IMM, FALSE);
		}
	ImageUnit_End();
	if (ImageUnit_IsAllowPull(pSrc) == 0) {
		DBG_WRN("ImageUnit_IsAllowPull fail\r\n");
		return;
	}
	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) != ISF_OK) {
		DBG_WRN("ImageUnit_PullData fail\r\n");
		ImageUnit_Begin(&ISF_UserProc, 0);
		if (ImageUnit_SetParam(sISFOutMap[SourcePath], USERPROC_PARAM_ALLOW_PULL_IMM, 0) != ISF_OK) {
			DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
			return;
		}
		ImageUnit_End();
		return;
	}

	pImgBuf = (IMG_BUF *)&isf_data.Desc[0];
	if (!pImgBuf) {

		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}
	if (gBCLastFrameTime == 0)
		gBCLastFrameTime = isf_data.TimeStamp;

	TimeStamp_sec = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
	TimeStamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
	LastTimeStamp_sec = (gBCLastFrameTime >> 32) & 0xFFFFFFFF;
	LastTimeStamp_usec = gBCLastFrameTime & 0xFFFFFFFF;

	DiffTimeStamp_sec = (long)TimeStamp_sec - (long)LastTimeStamp_sec;
	DiffTimeStamp_usec = (long)TimeStamp_usec - (long)LastTimeStamp_usec;
	gBCDiffFrameTimeUS += DiffTimeStamp_sec * 1000000 + DiffTimeStamp_usec;
	gBCLastFrameTime = isf_data.TimeStamp;
	if (gBCDiffFrameTimeUS > gBCFrameTimeUS) {
		gBCDiffFrameTimeUS -= gBCFrameTimeUS;

		#if (IPCAM_ODT_FUNC && IPCAM_BC_USE_ODT)
		rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
		#endif

		t1 = clock();
		bc_setimg(pImgBuf); // set image buffer
		if (gBC_usr_dim.w == 0 || gBC_usr_dim.h == 0)
		{
			gBC_imgW = pImgBuf->Width;
			gBC_imgH = pImgBuf->Height;
		}
		else
		{
			gBC_imgW = gBC_usr_dim.w;
			gBC_imgH = gBC_usr_dim.h;
		}
		t2 = clock();

		bc_process();
		bc_getforeground2(&foreground);
		t3 = clock();

		#if (IPCAM_ODT_FUNC && IPCAM_BC_USE_ODT)
		UINT32 i;
		URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
		num = bc_gettarget(rslts, &targetcoord, 32);
		for(i = 0; i < num; i++)
		{
			odt_rslts[i].x = rslts[i].x;
			odt_rslts[i].y = rslts[i].y;
			odt_rslts[i].w = rslts[i].w;
			odt_rslts[i].h = rslts[i].h;
			odt_rslts[i].score = 0;
			odt_rslts[i].class = 1008;
		}
		odt_setodrslt(num, odt_rslts, rslt_loc, odt_buf_param.process_id);
		#endif

		gBC_result->result.evt_width = foreground.Width;
		gBC_result->result.evt_height = foreground.Height;
		/*UINT32 i,j;
		DBG_DUMP("evt: %d %d\r\n", gBC_result->result.evt_height, gBC_result->result.evt_width);

		for (i=0;i<1;i++) {
			for (j=0; j<gBC_result->result.evt_width; j++) {
				DBG_DUMP("%d ", gBC_result->result.evt_map_addr[i*gBC_result->result.evt_width+j]);
			}
			DBG_DUMP("\r\n");
		}*/
		gBC_result->result.targetnum = 0;
		if (g_BcParam.bc_mode == 1) {
			if (gBC_usr_dim.w == 0 || gBC_usr_dim.h == 0)
				gBC_result->result.targetnum = bc_gettarget((URECT*)gBC_result->result.target, NULL, BC_MAX_NUM);
			else
				gBC_result->result.targetnum = bc_gettarget((URECT*)gBC_result->result.target, &gBC_usr_dim, BC_MAX_NUM);
			//DBG_DUMP("target number = %d\r\n", gBC_result->result.targetnum);
		}
		if ((t2 - t1) > 20000) { //20ms
			gBCFrameTimeUS = 2000 * 1000;
			DBG_ERR("BC ProcessTime=%ld us, adjust FrameTime=%llu us\r\n", (t2 - t1), gBCFrameTimeUS);
		}
		else if(gBCFrameTimeUS >= 2000 * 1000) {
			gBCFrameTimeUS = gBCFrameTime;
			DBG_ERR("BC ProcessTime=%ld us, adjust FrameTime=%llu us\r\n", (t2 - t1), gBCFrameTimeUS);
		}
	} else {
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}

	if (__DBGLVL__ >= 5 || gBCStatusFlag) {
		UINT32 idx = 0;
		DBG_DUMP("BC target number(%u)\r\n",gBC_result->result.targetnum);
		for (idx = 0; idx < gBC_result->result.targetnum; idx++) {
			DBG_DUMP("BC idx(%u) rect(%u, %u, %u, %u)\r\n", idx,
					gBC_result->result.target[idx].x, gBC_result->result.target[idx].y,
					gBC_result->result.target[idx].w, gBC_result->result.target[idx].h);
		}

		UINT32 pct = bc_gettdprms(BC_TAMPER_PCT_CUR);
		UINT32 frmnum = bc_gettdprms(BC_TAMPER_FRM_NUM_CUR);
		UINT32 status = bc_gettdprms(BC_TD_STATUS);
		debug_msg("td: pct = %d, frmnum = %d, status = %d\r\n", pct, frmnum, status);
	}
	gBC_result->event = IPCAM_EVENT_BC;
	gBC_result->TimeStamp.tv_sec = TimeStamp_sec;
	gBC_result->TimeStamp.tv_usec = TimeStamp_usec;

	if (gAlgBcDbgLevel == 1)
		DBG_DUMP("[%s]:targetnum=%d evt_map_data:(0x%x 0x%x 0x%x 0x%x) evt_dim(%d %d)\r\n", __func__, gBC_result->result.targetnum,
		gBC_result->result.evt_map_addr[0], gBC_result->result.evt_map_addr[1], gBC_result->result.evt_map_addr[2],
		gBC_result->result.evt_map_addr[3], gBC_result->result.evt_width, gBC_result->result.evt_height);


	if(gBCYUVFlag == TRUE){
		/////merge BC data with YUV
		ISF_DATA   isf_new_data;
		UINT32 buff_addr,dst_w, dst_h;
		IMG_BUF  dst_img;
		UINT32 TmpAddr =0;
		dst_w = pImgBuf->Width;
		dst_h = pImgBuf->Height;
		UINT32 img_buf_size = dst_w*dst_h*3/2 + sizeof(IPCAM_BC_RESULT);////hard code for YUV 420
		UINT32 offset = dst_w*dst_h*3/2;
		buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);
		GxImg_InitBuf(&dst_img, dst_w, dst_h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, offset);
		GxImg_ScaleData(pImgBuf, REGION_MATCH_IMG, &dst_img, REGION_MATCH_IMG);
		memcpy(&dst_img, pImgBuf, sizeof(IMG_BUF));

		TmpAddr = dst_img.PxlAddr[0];

		TmpAddr = TmpAddr +offset;
		memcpy((UINT32 *)TmpAddr, gBC_result, sizeof(IPCAM_BC_RESULT));
		memcpy(&isf_new_data.Desc[0], &dst_img, sizeof(IMG_BUF));

		isf_new_data.TimeStamp = isf_data.TimeStamp;




		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_new_data, 0);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_new_data);

	}



	NvtUctrl_NotifyRaw((void *)gBC_result, sizeof(IPCAM_BC_RESULT));

	ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
}

void IPCam_BC_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	gBCStartFlag = FALSE;
	gBCFrameTimeUS = 200000;
	bc_init((UINT32)buf->Addr, (UINT32)cachebuf->Addr);
	gBC_result = (IPCAM_BC_RESULT *)((UINT32)cachebuf->Addr + bc_getcachebufsize());
	memset(gBC_result, 0, sizeof(IPCAM_BC_RESULT));
	DBG_DUMP("IPCam_BC_Init buf(%#x %d) cachebuf(%#x %d)\r\n",
			buf->Addr, buf->Size, cachebuf->Addr, cachebuf->Size);
	DBG_DUMP("BC rst buf(%#x %d)\r\n",
			(UINT32)gBC_result, sizeof(IPCAM_BC_RESULT));
	g_BcParam.fps = 10; 
	gBCFrameTimeUS = 1000000 / g_BcParam.fps;
	gBCStartFlag = TRUE;
	gBCFrameTime = gBCFrameTimeUS;
}

UINT32 IPCam_BC_GetBufSize(void)
{
	return bc_getbufsize();
}

UINT32 IPCam_BC_GetCacheBufSize(void)
{
	return bc_getcachebufsize() + sizeof(IPCAM_BC_RESULT);
}

PHOTO_FUNC_INFO PhotoFuncInfo_bc = {{
		PHOTO_PRIORITY_M, 		///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_SIE_VD,       ///< isr tigger event
		"BC", IPCam_BC_Init,  	///< init fp, only execute at first process
		IPCam_BC_Process,      	///< process fp
		NULL,  					///< process end fp
		NULL,            		///< exit fp, only execute at photo close
		IPCam_BC_GetBufSize,      	///< get working buffer fp
		IPCam_BC_GetCacheBufSize,	///< get working cache buffer fp
	},
	NULL ///< point next Function Obj, last Function must be set NULL
};
#endif

static BOOL cmd_bc_set(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gBCLastFrameTime = 0;
	gBCDiffFrameTimeUS = 0;
	if (value == 0) {
		gBCStartFlag = FALSE;
	} else {
		gBCFrameTimeUS = 1000000 / value;
		gBCFrameTime = gBCFrameTimeUS;
		gBCStartFlag = TRUE;
	}
	DBG_DUMP("BC FrameTimeUS(%llu), StartFlag(%d), ResultFlag(%d)\r\n", gBCFrameTimeUS, gBCStartFlag, gBCStatusFlag);
	return TRUE;
}

static BOOL cmd_bc_status(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	if (value == 0) {
		gBCStatusFlag = FALSE;
	} else {
		gBCStatusFlag = TRUE;
	}
	DBG_DUMP("BC FrameTimeUS(%llu), StartFlag(%d), ResultFlag(%d)\r\n", gBCFrameTimeUS, gBCStartFlag, gBCStatusFlag);
	return TRUE;
}
SXCMD_BEGIN(bc, "bc")
SXCMD_ITEM("set %", cmd_bc_set, "set fps, fps=0 to stop")
SXCMD_ITEM("status %", cmd_bc_status, "bc status, 1=dump")
SXCMD_END()
static BOOL gVQAStartFlag = FALSE;
static BOOL gVQAStatusFlag = FALSE;
static UINT64 gVQAFrameTimeUS = 33333;
static UINT64 gVQAFrameTime   = 33333;
static UINT64 gVQALastFrameTime = 0;
static UINT64 gVQADiffFrameTimeUS = 0;
static UINT64 gVQABlockTimeUS = 0;
vqa_res_t gVQA_res;
void Movie_VqaSetFps(UINT32 fps)
{
	gVQALastFrameTime = 0;
	gVQADiffFrameTimeUS = 0;
	if (fps == 0) {
		gVQAStartFlag = FALSE;
	} else {
		gVQAFrameTimeUS = 1000000 / fps;
		gVQAFrameTime = gVQAFrameTimeUS;
		gVQAStartFlag = TRUE;
	}
}

vqa_res_t Movie_VqaGetResult(void)
{
	return gVQA_res;
}

#if IPCAM_VQA_FUNC
BOOL Photo_VQA_param_init(vqa_param_t *vqa_param)
{
	// init VQA default param
	//enable
	vqa_param->enable_param.en_too_light = 1;
	vqa_param->enable_param.en_too_dark = 1;
	vqa_param->enable_param.en_blur = 1;
	vqa_param->enable_param.en_auto_adj_param = 1;
	vqa_param->enable_param.en_ref_md = 1;
	//global
	vqa_param->global_param.height = 480;
	vqa_param->global_param.width = 640;
	vqa_param->global_param.mb_y_size = 40;
	vqa_param->global_param.mb_x_size = 32;
	vqa_param->global_param.auto_adj_period = 5;
	vqa_param->global_param.g_alarm_frame_num = 20;
	//overexposure
	vqa_param->light_param.too_light_strength_th = 215;
	vqa_param->light_param.too_light_cover_th = 15;
	vqa_param->light_param.too_light_alarm_times = 5;
	//underexposure
	vqa_param->light_param.too_dark_strength_th = 50;
	vqa_param->light_param.too_dark_cover_th = 75;
	vqa_param->light_param.too_dark_alarm_times = 5;
	//blur
	vqa_param->contrast_param.blur_strength = 8;
	vqa_param->contrast_param.blur_cover_th = 85;
	vqa_param->contrast_param.blur_alarm_times = 5;
	vqa_set_param(0, vqa_param);
	return TRUE;
}
void Photo_VQA_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	vqa_param_t vqa_param;
	memset(&vqa_param, 0, sizeof(vqa_param_t));
	if (buf->Addr) {
		gVQAStartFlag = FALSE;
		gVQAFrameTimeUS = 33333;
		gVQAFrameTime = gVQAFrameTimeUS;
		memset(&gVQA_res, 0, sizeof(vqa_res_t));
		vqa_init(0, (UINT32)buf->Addr);
		Photo_VQA_param_init(&vqa_param);
		DBG_DUMP("Photo_VQA_Init buf(%#x %d)\r\n", buf->Addr, buf->Size);
	} else {
		DBG_ERR("Get vqa buf fail\n");
	}

}

void Photo_VQA_Process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	static IPCAM_VQA_RESULT IPCamVQAResult = {0};
	clock_t t1, t2;
	static clock_t old_time = 0;
	static INT32 start_index = -1;

	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		if (clock() - old_time <= 1000000) { ///1secs
			return;
		}
		start_index++;
		DBG_DUMP("VQA detection start!!\r\n");
	}
	if (gVQAStartFlag == FALSE) {
		return;
	}

	ISF_DATA isf_data;
	ISF_PORT *pSrc;
	PIMG_BUF pImgBuf;
	UINT8 *y_map_ptr = 0;
	UINT32 sc_hw_evt = 0;
	INT32 ret;
	HEADER_YUV_MD_INFO *md_info;
	static UINT32 TimeStamp_sec = 0, TimeStamp_usec = 0;
	UINT32 LastTimeStamp_sec, LastTimeStamp_usec;
	INT32 DiffTimeStamp_sec, DiffTimeStamp_usec;

	pSrc = ImageUnit_Out(&ISF_UserProc, IPCAM_MD_PATH);
	ImageUnit_Begin(&ISF_UserProc, 0);
	if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 1) != ISF_OK) {
		DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 1 Fail\r\n");
		return;
	}
	ImageUnit_End();
	if (ImageUnit_IsAllowPull(pSrc) == 0) {
		DBG_WRN("ImageUnit_IsAllowPull fail\r\n");
		return;
	}
	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, gVQAFrameTimeUS) != ISF_OK) {
		DBG_WRN("ImageUnit_PullData fail\r\n");
		IPCamVQAResult.Event = IPCAM_EVENT_VQA;
		IPCamVQAResult.TimeStamp.tv_sec = TimeStamp_sec;
		IPCamVQAResult.TimeStamp.tv_usec = TimeStamp_usec;
		NvtUctrl_NotifyRaw((void *)&IPCamVQAResult, sizeof(IPCAM_VQA_RESULT));
		ImageUnit_Begin(&ISF_UserProc, 0);
		if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 0) != ISF_OK) {
			DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
			return;
		}
		ImageUnit_End();
		return;
	}
	ImageUnit_Begin(&ISF_UserProc, 0);
	if (ImageUnit_SetParam(IPCAM_MD_PATH, USERPROC_PARAM_ALLOW_PULL_IMM, 0) != ISF_OK) {
		DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
		return;
	}
	ImageUnit_End();
	pImgBuf = (IMG_BUF *)&isf_data.Desc[0];
	md_info = (HEADER_YUV_MD_INFO *)(pImgBuf->Metadata.Addr);
	if (!md_info) {
		//DBG_DUMP("^CVQA-1 release blk = %08x\r\n", isf_data.hData);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}
	if (gVQALastFrameTime == 0)
		gVQALastFrameTime = isf_data.TimeStamp;

	TimeStamp_sec = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
	TimeStamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
	LastTimeStamp_sec = (gVQALastFrameTime >> 32) & 0xFFFFFFFF;
	LastTimeStamp_usec = gVQALastFrameTime & 0xFFFFFFFF;

	DiffTimeStamp_sec = (long)TimeStamp_sec - (long)LastTimeStamp_sec;
	DiffTimeStamp_usec = (long)TimeStamp_usec - (long)LastTimeStamp_usec;
	gVQADiffFrameTimeUS += DiffTimeStamp_sec * 1000000 + DiffTimeStamp_usec;
	gVQALastFrameTime = isf_data.TimeStamp;

	if (gVQADiffFrameTimeUS > gVQAFrameTimeUS) {
		gVQADiffFrameTimeUS -= gVQAFrameTimeUS;
		DBG_IND("sc_hw_evt(%d) wh(%d, %d)\r\n", md_info->scence_change_evt, md_info->win_width_num, md_info->win_y_num);
		y_map_ptr = (UINT8 *)pImgBuf->PxlAddr[0];
		sc_hw_evt = md_info->scence_change_evt;
		t1 = clock();
		ret = vqa_run(0, sc_hw_evt, y_map_ptr, pImgBuf->Width, pImgBuf->Height, md_info->evt_addr, &gVQA_res);
		if (gVQABlockTimeUS) {
			Delay_DelayUs(gVQABlockTimeUS);
			gVQABlockTimeUS = 0;
		}
		t2 = clock();
		DBG_IND("y_map_ptr(%p) evt_map(%p) ret(%d)\r\n", y_map_ptr, md_info->evt_addr, ret);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);

		if ((t2 - t1) > 20000) {
			gVQAFrameTimeUS = 1000*1000;
			DBG_WRN("VQA ProcessTime=%ld us, adjust FrameTime=%llu us\r\n", (t2 - t1), gVQAFrameTimeUS);
		}
		else if(gVQAFrameTimeUS >= 1000 * 1000) {
			gVQAFrameTimeUS = gVQAFrameTime;
			DBG_WRN("VQA ProcessTime=%ld us, adjust FrameTime=%llu us\r\n", (t2 - t1), gVQAFrameTimeUS);
		}
	} else {
		//DBG_DUMP("^CVQA-3 release blk = %08x\r\n", isf_data.hData);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}

	if (__DBGLVL__ >= 5 || gVQAStatusFlag)
		DBG_DUMP("vqa_res light(%d) dark(%d) blur(%d)\r\n",
			 gVQA_res.res_too_light, gVQA_res.res_too_dark,
			 gVQA_res.res_blur);

	IPCamVQAResult.Event = IPCAM_EVENT_VQA;
	IPCamVQAResult.TimeStamp.tv_sec = TimeStamp_sec;
	IPCamVQAResult.TimeStamp.tv_usec = TimeStamp_usec;
	IPCamVQAResult.TooLight = gVQA_res.res_too_light;
	IPCamVQAResult.TooDark = gVQA_res.res_too_dark;
	IPCamVQAResult.Blur = gVQA_res.res_blur;
	NvtUctrl_NotifyRaw((void *)&IPCamVQAResult, sizeof(IPCAM_VQA_RESULT));
}

UINT32 Photo_VQA_GetBuf(void)
{
	return vqa_calc_buf_size();
}

PHOTO_FUNC_INFO PhotoFuncInfo_vqa = {{
		PHOTO_PRIORITY_M, ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_SIE_VD,       ///< isr tigger event
		"VQA", Photo_VQA_Init, ///< init fp, only execute at first process
		Photo_VQA_Process,     ///< process fp
		NULL,  			///< process end fp
		NULL,            ///< exit fp, only execute at photo close
		Photo_VQA_GetBuf,      ///< get working buffer fp
		NULL,    		///< get working cache buffer fp
	},
	NULL ///< point next Function Obj, last Function must be set NULL
};
#endif

static BOOL cmd_vqa_case(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	if (value == 1) {
		gVQA_res.res_too_light = !gVQA_res.res_too_light;
	} else if (value == 2) {
		gVQA_res.res_too_dark = !gVQA_res.res_too_dark;
	} else if (value == 3) {
		gVQA_res.res_blur = !gVQA_res.res_blur;
	} else if (value == 0) {
		gVQAStartFlag = !gVQAStartFlag;
	}
	return TRUE;
}

static BOOL cmd_vqa_block(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gVQABlockTimeUS = (UINT64)value * 1000;
	DBG_DUMP("Set VQA BlockTimeUS(%llu)\r\n", gVQABlockTimeUS);
	return TRUE;
}

static BOOL cmd_vqa_set(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gVQALastFrameTime = 0;
	gVQADiffFrameTimeUS = 0;
	if (value == 0) {
		gVQAStartFlag = FALSE;
	} else {
		gVQAFrameTimeUS = 1000000 / value;
		gVQAFrameTime = gVQAFrameTimeUS;
		gVQAStartFlag = TRUE;
	}
	DBG_DUMP("VQA FrameTimeUS(%llu), StartFlag(%d) ResultFlag(%d)\r\n", gVQAFrameTimeUS, gVQAStartFlag, gVQAStatusFlag);
	return TRUE;
}

static BOOL cmd_vqa_status(CHAR *strCmd)
{
	UINT32 value = 0;
	DBG_DUMP("VQA FrameTimeUS(%llu), StartFlag(%d) ResultFlag(%d)\r\n", gVQAFrameTimeUS, gVQAStartFlag, gVQAStatusFlag);
	sscanf_s(strCmd, "%d", &value);
	if (value == 0) {
		gVQAStatusFlag = FALSE;
	} else {
		gVQAStatusFlag = TRUE;
	}
	return TRUE;
}

SXCMD_BEGIN(vqa, "vqa")
SXCMD_ITEM("case %", cmd_vqa_case, "send test result to linux [1:light, 2:dark, 3:blur]")
SXCMD_ITEM("block %", cmd_vqa_block, "set md block time (in msec)")
SXCMD_ITEM("set %", cmd_vqa_set, "set fps, fps=0 to stop")
SXCMD_ITEM("status %", cmd_vqa_status, "vqa status, 1=dump")
SXCMD_END()

void Photo_InstallCmd(void)
{
	SxCmd_AddTable(md);
	SxCmd_AddTable(vqa);
	SxCmd_AddTable(pd);
	SxCmd_AddTable(bc);
	SxCmd_AddTable(alg);
}

#if (IPCAM_MD_SAMPLE_660_STYLE == ENABLE)

typedef struct _IPCAM_MD_SAMPLE_660_INFO{

    BOOL set_no_trigger;


}IPCAM_MD_SAMPLE_660_INFO;

void IPCam_MD_Sample_660_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

   IPCAM_MD_SAMPLE_660_INFO *motion_info = (IPCAM_MD_SAMPLE_660_INFO *)buf;
    motion_info->set_no_trigger=0;

}

UINT32 IPCam_MD_Sample_660_BufSize(void){

	return sizeof(IPCAM_MD_SAMPLE_660_INFO);

}

static UINT32 EVENT_motion_get_clock_sec(void){

	UINT64 tmp_clock=0;

	tmp_clock = HwClock_GetLongCounter();

	return ((tmp_clock >> 32) & 0xFFFFFFFF);
}

void Movie_MD660Lock(BOOL enable){

    gMDSample660StartFlag = enable;
}

void IPCam_MD_Sample_660_Process(MEM_RANGE *buf, MEM_RANGE *cachebuf){


    INT32 id=0;
    BOOL trigger=FALSE;
    UINT32 now_time = 0;
    UINT32 time_out=1;
    static INT32 start_index = -1;
    static UINT32 old_time = 0;


	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	IMG_BUF *pBuf=NULL;


	IPCAM_MD_SAMPLE_660_INFO *motion_info = (IPCAM_MD_SAMPLE_660_INFO *)buf;
	IPCAM_MD_SAMPLE_660_STATUS motion_status ={0};
	motion_status.Event = IPCAM_EVENT_MD_SAMPLE_660;
    if(old_time == 0){
        old_time = EVENT_motion_get_clock_sec();
    }
    //wait boot device finish
    if(start_index ==-1){
        now_time = EVENT_motion_get_clock_sec();
        if(now_time - old_time <= 5){ ///5secs
            return;
        }

        start_index ++;
        DBG_DUMP("Motion sample detection start!!\r\n");
    }

    if(gMDSample660StartFlag == FALSE){
        if(motion_info->set_no_trigger ==1){
            DBG_DUMP("motion sample stop trigger\r\n");
			motion_status.bTrigger = FALSE;
			NvtUctrl_NotifyRaw((void *)&motion_status, sizeof(motion_status));
            //NvtUctrl_Notify(MOTION1_NOTRIGGER);
            motion_info->set_no_trigger = 0;
		}
        return;
	}
////

	pUnit = &ISF_UserProc;

	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT3, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
		//ImageUnit_SetParam(ISF_OUT3, USERPROC_PARAM_BYPASS_IMM, 0);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT3);

	if(!ImageUnit_IsAllowPull(pSrc)){
		DBG_WRN("ImageUnit_IsAllowPull fail!\r\n");
		return;
	}

	if ( ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		pBuf =  (IMG_BUF *)&isf_data.Desc[0];

   		trigger = MD_Process(id,pBuf);
    	now_time = EVENT_motion_get_clock_sec();
    	if(trigger == TRUE){

        	if(motion_info->set_no_trigger == 1){
            	DBG_IND("motion still trigger\r\n");
			}
        	else
        	{

            	DBG_DUMP("motion sample trigger\r\n");
				motion_status.bTrigger = TRUE;
				NvtUctrl_NotifyRaw((void *)&motion_status, sizeof(motion_status));
            	motion_info->set_no_trigger =1;

        	}
        	old_time = now_time;

    	}
    	else{
        	if(((now_time - old_time) >time_out) && (motion_info->set_no_trigger==1))
        	{

            	DBG_DUMP("motion sample stop trigger\r\n");
				motion_status.bTrigger = FALSE;
				NvtUctrl_NotifyRaw((void *)&motion_status, sizeof(motion_status));
            	motion_info->set_no_trigger = 0;

        	}
    	}
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
}



PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_md_sample_660 = {
	{
		.Priority = PHOTO_PRIORITY_M,           ///< function hook process Id.
		.IPLId = IPL_PATH_1,                    ///< IPL Id.
		.TrigEvent = PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		.ProcName = "IPCam_MDSample660",
		.InitFP = IPCam_MD_Sample_660_Init,         ///< init fp, only execute at first process
		.ProcFP = IPCam_MD_Sample_660_Process,      ///< process fp
		.EndFP = NULL,                          	///< process end fp
		.GetBufFP = IPCam_MD_Sample_660_BufSize,  	///< get working buffer fp
		.GetCacheBufFP = NULL,  					///< get working cache buffer fp
	},
	.pNext = NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif

//#NT#2015/09/17#JS Liu - begin
#if _CVHOST_EX_
void IPCam_CvHostExProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (g_CvHostExDone != TRUE) {
		if (GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 0) {
			return;
		}

		debug_msg("\n\r************************\n");
		debug_msg("\r*  Run CvHost Example  *\n");
		debug_msg("\r************************\n");
#if _CVHOST_EX_ENDSP_
		SxCmd_DoCommand("dsp open");    // ASSUME DSP is opened already!!
#endif

		CvHostEx_Opening(buf);
		CvHostEx_Sobel3x3(buf);
		CvHostEx_Gaussian3x3(buf);

		g_CvHostExDone = TRUE;

#if _CVHOST_EX_ENDSP_
		SxCmd_DoCommand("dsp close");
#endif
		debug_msg("\r** CvHost Example End **\n");
	}
}

UINT32 IPCam_CvHostExCalcBufSize(void)
{
	UINT32 ImgRowsMax = 1080;
	UINT32 ImgColsMax = 1920;
	UINT32 ImgSizeMax = ImgRowsMax * ImgColsMax;
	UINT32 CvHostBufSize = CvHost_GetWorkMemSize();

	return ImgSizeMax * 3 + CvHostBufSize;  // input gray, output gray and CvHost working buffer
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_cvhost = {
	{
		.Priority = PHOTO_PRIORITY_M,           ///< function hook process Id.
		.IPLId = IPL_PATH_1,                    ///< IPL Id.
		.TrigEvent = PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		.ProcName = "IPCam_CvHostExProcess",    ///< process name
		.InitFP = NULL,                         ///< init fp, only execute at first process
		.ProcFP = IPCam_CvHostExProcess,        ///< process fp
		.EndFP = NULL,                          ///< process end fp
		.GetBufFP = IPCam_CvHostExCalcBufSize,  ///< get working buffer fp
		.GetCacheBufFP = NULL,                  ///< get working cache buffer fp
	},
	.pNext = NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2015/09/17#JS Liu - end

void IPCam_InitPhotoFunc(void)
{
	PHOTO_FUNC_INFO *phead = NULL;
	//
	// Setup IPL information.
	//
	phead = &PhotoFuncInfo_dummy;
	phead->pNext = NULL; //mark end

#if (IPCAM_DIS_MV_FUNC == ENABLE)
	{
		// DIS function
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamdis;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if IPCAM_MD_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcam_motion;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;   //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL;//mark end
	}
#endif

#if IPCAM_VQA_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_vqa;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if IPCAM_PD_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_pd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if IPCAM_BC_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_bc;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if IPCAM_FD_FUNC
	{

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

	}
	//IP cam will unlock FD by CPU_CORE2
	FD_Lock(FALSE);
#endif
//#NT#2017/12/26#Yuzhe Bian -begin
//#NT# Face Tracking & Grading
#if IPCAM_FTG_FUNC
    #if(IPCAM_FTG_USE_ODT == ENABLE)
	{

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcam_ftg;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

	}
    #endif
	{

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcam_ftgfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

	}
#endif
//#NT#2017/12/26#Yuzhe Bian -end

#if (IPCAM_ODT_FUNC)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcam_odt;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;	 //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcam_odtod;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;	 //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if (IPCAM_MD_SAMPLE_660_STYLE == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcam_md_sample_660;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;   //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL;//mark end
	}
#endif

#if _CVHOST_EX_
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcam_cvhost;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

	UIPhotoFunc_Open(phead);
}
