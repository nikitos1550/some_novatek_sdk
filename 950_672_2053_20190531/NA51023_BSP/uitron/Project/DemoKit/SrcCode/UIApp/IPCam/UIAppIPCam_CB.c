////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppCommon.h"
#include "ImageApp_IPCam.h"
#include "UIAppIPCam.h"
#include "UIAppIPCam_Param.h"
#include "iq_api.h"
#include "videosprite.h"
#include "ae_task.h"
#include "NMediaRecVdoEnc.h"
#include "MovieStamp.h"
#if (defined(_BSP_NA51000_) && (IPCAM_DIS_COMP_FUNC == ENABLE))
#include "dis_alg_fw.h"
#endif

#define THIS_DBGLVL        2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppIPCamCB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

extern BOOL bEnableAE;
extern BOOL bStableAE;
extern BOOL bEnableVdo;
extern BOOL bShowPreview;
static BOOL bFirstIPL = TRUE;
static BOOL bFirstENC = TRUE;
#define YUV_DRAW 	DISABLE
//IMAGEPIPE_PARAM_IPL_CB
#include "ipl_cb_msg.h"
#if (IPCAM_RESET_FPS == ENABLE)
static ISF_OUT   g_ISFOutMap[IPCAM_PATH_MAX_ID * 2] = {     ISF_OUT1, ISF_OUT2,
															ISF_OUT3, ISF_OUT4,
															ISF_OUT5, ISF_OUT6,
															ISF_OUT7, ISF_OUT8,
															ISF_OUT9, ISF_OUT10,
															ISF_OUT11, ISF_OUT12,
															ISF_OUT13, ISF_OUT14,
															ISF_OUT15, ISF_OUT16,
#if defined(_BSP_NA51000_)
															ISF_OUT17, ISF_OUT18,
															ISF_OUT19, ISF_OUT20,
															ISF_OUT21, ISF_OUT22,
															ISF_OUT23, ISF_OUT24,
															ISF_OUT25, ISF_OUT26,
															ISF_OUT27, ISF_OUT28,
															ISF_OUT29, ISF_OUT14,
															ISF_OUT31, ISF_OUT32
#endif
													  };

#endif

static UINT32 IPCamExe_IPLProcCB(UINT32 id, UINT32 state)
{
	switch (state) {
	default:
		if (bFirstIPL) {
			TM_BOOT_BEGIN("ImagePipe", "start");
		}
		break;
	}
	return  0;
}

static void IPCamExe_IPLCB(IPL_PROC_ID Id, IPL_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {
	case IPL_CBMSG_PREVIEW:
		DBG_IND("===IPL_CBMSG_PREVIEW===\r\n");
		if (bFirstIPL) {
			TM_BOOT_END("ImagePipe", "start");
			bFirstIPL = FALSE;
		}
		if(!bEnableAE)
			bEnableAE = ae_isopen(AE_ID_1);
		break;
	default:
		break;
	}
}

//AE_CBMsgInit
#include "ae_cb_msg.h"
static void IPCamExe_AE_CB(AE_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {

	case AE_CBMSG_PREVIEWSTABLE:
		DBG_IND("===AE_CBMSG_PREVIEWSTABLE===\r\n");
		bStableAE = TRUE;
		#if (DISPLAY_FUNC == ENABLE)
		if(bEnableVdo) {
			if((DX_HANDLE)GxVideo_GetDevice(DOUT1) && !bShowPreview) {
				bShowPreview = TRUE;
				Display_ShowPreview(); //show preview immediately (AE should be last one)
				DBG_DUMP("^YShow Preview! (IPL > Video VSync > AE stable)\r\n");
			}
		}
		#endif
		break;

	default:
		break;
	}
}

//VDOENC_PARAM_3DNR_CB
void IPCamExe_UIDefine_3DNR(UINT32 pathID, UINT32 p3DNRConfig)
{
    IPCAM_SENSOR sensor_id;
    IPCAM_STREAM_INFO *pStreamInfo = (IPCAM_STREAM_INFO *)UIAppIPCam_get_streamInfo();
    sensor_id = pStreamInfo[pathID].sensor;
    iq_get_tnr_config(IPL_PATH(sensor_id), p3DNRConfig);
}

UINT32 IPCamExe_VdoEnc_ProcCB(UINT32 id, UINT32 state)
{
	switch (state) {
	default:
		if (bFirstENC) {
			TM_BOOT_BEGIN("VdoEnc", "start");
		}
		break;
	}
	return  0;
}

//VDOENC_PARAM_EVENT_CB
void IPCamExe_VdoEnc_CB(CHAR* Name, UINT32 event_id, UINT32 value)
{
	MP_VDOENC_YUV_SRC* p_yuv_src = (MP_VDOENC_YUV_SRC*)value;

	if(!p_yuv_src)
		return;

	if (strcmp(Name, "VdoEnc") == 0)
	{
		switch (event_id)
		{
			case NMI_VDOENC_EVENT_STAMP_CB:
				{

				videosprite_draw_btn_ext(p_yuv_src->uiVidPathID,
							p_yuv_src->uiYAddr,
							p_yuv_src->uiCbAddr,
							p_yuv_src->uiCrAddr,
							p_yuv_src->uiYLineOffset,
							p_yuv_src->uiWidth,
							p_yuv_src->uiHeight);

				#if (STAMP_FUNC==ENABLE)
				MovieStamp_CopyData(p_yuv_src->uiVidPathID,
					p_yuv_src->uiYAddr,
					p_yuv_src->uiCbAddr,
					p_yuv_src->uiCrAddr,
					p_yuv_src->uiYLineOffset,
					p_yuv_src->uiWidth);
				#endif
				}
			break;

			case NMI_VDOENC_EVENT_BS_CB:
				if (bFirstENC) {
					TM_BOOT_END("VdoEnc", "start");
					bFirstENC = FALSE;
				}
			break;

			default:
			break;
		}
	}
}

//NETRTSP_PARAM_IFRAME_REQ_CB
#if(FORCE_UNPLUG_RTSP != ENABLE)
void IPCamExe_RtspIFramReq_CB(UINT32 pathId)
{
	ImageUnit_Begin(&ISF_VdoEnc, 0);
		ImageUnit_SetParam(pathId, VDOENC_PARAM_RESET_IFRAME, 1);
	ImageUnit_End();
	return;
}
#endif

#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
#include "dis_alg_fw.h"
static UINT32 IPCamExe_ViewTracking_CB(UINT32 id, UINT32 frame_cnt, INT32* pOffx, INT32* pOffy)
{
	DISALG_VECTOR track_ret;
	dis_getFrameCorrVec(&track_ret, frame_cnt);
	//DBG_DUMP("tracking score=%d\r\n", track_ret.score);
	if(pOffx)pOffx[0] = track_ret.vector.iX;
	if(pOffy)pOffy[0] = track_ret.vector.iY;
	return track_ret.score;
}
#endif

void IPCam_RegCB(void)
{
	if (System_GetEnableSensor() == (SENSOR_2)) {
		ImageUnit_Begin(ISF_IPL(1), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_PROC_CB, (UINT32)IPCamExe_IPLProcCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_IPL_CB, (UINT32)IPCamExe_IPLCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOOSD_CB,  (UINT32)videosprite_draw_ps_image);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOMASK_CB, (UINT32)videosprite_draw_ps_mask);
		#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIEWTRACKING_CB, (UINT32)IPCamExe_ViewTracking_CB);
		#endif
		#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		ImageUnit_SetParam(ISF_OUT1, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT2, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT3, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT4, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT5, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		#endif
		#if defined(_BSP_NA51000_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		dis_setDisViewRatio(1100);
		#endif
		ImageUnit_End();
		AE_CBMsgInit(IPL_PATH(1), IPCamExe_AE_CB);
	} else {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_PROC_CB, (UINT32)IPCamExe_IPLProcCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_IPL_CB, (UINT32)IPCamExe_IPLCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOOSD_CB,  (UINT32)videosprite_draw_ps_image);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOMASK_CB, (UINT32)videosprite_draw_ps_mask);
		#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIEWTRACKING_CB, (UINT32)IPCamExe_ViewTracking_CB);
		#endif
		#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		ImageUnit_SetParam(ISF_OUT1, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT2, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT3, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT4, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT5, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		#endif
		#if defined(_BSP_NA51000_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		dis_setDisViewRatio(1100);
		#endif
		ImageUnit_End();
		AE_CBMsgInit(IPL_PATH(0), IPCamExe_AE_CB);
	}
}

#if (IPCAM_RESET_FPS == ENABLE)
static UINT32 uiCheckFPS[IPCAM_PATH_MAX_ID] ={0};
//force to update dynamic FPS
void IPCam_dirtyFPS(UINT32 path_index)
{
	uiCheckFPS[path_index] = 0;
}
//check and update dynamic FPS
void IPCam_checkFPS_CB(void){


	UINT32 path_index=0;
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UINT32 uiNowFPS[IPCAM_PATH_MAX_ID] = {0};
	UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pFixQPConfig = UIAppIPCam_get_FixQPInfo();

	UINT32 tmpFPS =0;

	UINT32 uiCodecFPS =0;

	for (path_index = 0 ; path_index < IPCAM_PATH_MAX_ID ; path_index++){
		IPCAM_SENSOR sensor_index = pStreamInfo[path_index].sensor;

		if(!pStreamInfo[path_index].enable)
			continue; //ignore disabled path

		#if (IPCAM_DISP_ON_OFF != 0)
		if(path_index == IPCAM_DISP_PATH_NO)
			continue; //ignore display path
		#endif

		tmpFPS = ImageUnit_GetParam(ISF_IPL(sensor_index) ,ISF_IN1, IMAGEPIPE_PARAM_LIVE_INFPS_IMM);
		uiNowFPS[path_index] =  SEN_FPS_FMT_CONV2_REAL(tmpFPS) / 1000;

		if(uiCheckFPS[path_index] != uiNowFPS[path_index]){

			if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_CBR){
				uiCodecFPS = pCBRConfig[path_index].cbr_info.uiFrameRate;
			}
			else if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_VBR){
				uiCodecFPS = pVBRConfig[path_index].vbr_info.uiFrameRate;
			}
			else if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_FIXQP){
				uiCodecFPS = pFixQPConfig[path_index].sFixQPInfo.uiFrameRate;
			}
			else if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_EVBR){
				uiCodecFPS = pEVBRConfig[path_index].evbr_info.uiFrameRate;
			}
			else if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_NONE){
				uiCodecFPS = pBRCNoneConfig[path_index].uiFrameRate;
			}
			else{
				DBG_ERR("brc type error:%d path:%d\r\n",pEncodeConfig[path_index].eBrcType,path_index);
				continue;
			}

			ImageUnit_Begin(&ISF_VdoEnc, 0);
				////set fps 1/1, for IPL trigger mode
				if (pStreamInfo[path_index].trig == IPCAM_TRIGGER_DEST) {
					ImageUnit_SetParam(g_ISFOutMap[path_index] , VDOENC_PARAM_FPS_IMM, ISF_VDO_FRC(uiCodecFPS, uiNowFPS[path_index]));
				} else {
					// ....
				}
			ImageUnit_End();

			ImageUnit_Begin(&ISF_ImgTrans, 0);
				if (pStreamInfo[path_index].trig == IPCAM_TRIGGER_DEST) {
					ImageUnit_SetParam(g_ISFOutMap[path_index] , IMGTRANS_PARAM_FPS_IMM, ISF_VDO_FRC(1, 1));
				} else {
					ImageUnit_SetParam(g_ISFOutMap[path_index] , IMGTRANS_PARAM_FPS_IMM, ISF_VDO_FRC(uiCodecFPS, uiNowFPS[path_index]));
				}
			ImageUnit_End();


		}

		uiCheckFPS[path_index] = uiNowFPS[path_index];
	}


}

#endif

#if(IPCAM_FTG_SNAPSHOT == ENABLE)
NVTMPP_VB_POOL  		g_FTG_PINGPIONG_Pool[2] = {0};
IPCAM_FTG_BUF_INFO	g_sFTGBufInfo[2]={0};
UINT64 g_uiFTGSerialNum =0;
UINT32 FTG_SNAPSHOT_SEM_ID = 0;
UINT32 g_FaceBufAddr[MAX_FTGNUM]={0};
UINT32 g_FaceYUVBufAddr = 0;
UINT32 g_FullBufAddr = 0;
UINT32 g_FullYUVBufAddr = 0;
NVTMPP_VB_POOL	g_FTG_FaceJpeg_Pool={0};
NVTMPP_VB_POOL	g_FTG_FaceYUV_Pool={0};
NVTMPP_VB_POOL	g_FTG_FullJpeg_Pool={0};
NVTMPP_VB_POOL	g_FTG_FullYUV_Pool={0};
void FTG_SANSPSHOT_InstallID(void){
	OS_CONFIG_SEMPHORE(FTG_SNAPSHOT_SEM_ID, 0, 1, 1);
}


INT32 IPCam_FTG_GetSnapshotBuf(IPCAM_FTG_SNAPSHOT_TYPE SnapshotType, void *BufAddr){


	switch(SnapshotType){

		case IPCAM_FTG_FACE_JPEG:

			*((UINT32 *)BufAddr) = g_FaceBufAddr[0];
			*(((UINT32 *)BufAddr)+1) = g_FaceBufAddr[1];
			*(((UINT32 *)BufAddr)+2) = g_FaceBufAddr[2];
			*(((UINT32 *)BufAddr)+3) = g_FaceBufAddr[3];
			*(((UINT32 *)BufAddr)+4) = g_FaceBufAddr[4];
			*(((UINT32 *)BufAddr)+5) = g_FaceBufAddr[5];
			*(((UINT32 *)BufAddr)+6) = g_FaceBufAddr[6];
			*(((UINT32 *)BufAddr)+7) = g_FaceBufAddr[7];
			*(((UINT32 *)BufAddr)+8) = g_FaceBufAddr[8];
			*(((UINT32 *)BufAddr)+9) = g_FaceBufAddr[9];

			//charlie test
			#if 0
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[0]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[1]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[2]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[3]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[4]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[5]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[6]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[7]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[8]);
			DBG_WRN("charlie %x\r\n",g_FaceBufAddr[9]);
			#endif
			//end charlie test
			break;
		case IPCAM_FTG_FULL_JPEG:

			*(UINT32 *)BufAddr = g_FullBufAddr;
			break;
		case IPCAM_FTG_FULL_YUV:
			*(UINT32 *)BufAddr = g_FullYUVBufAddr;
			break;
		case IPCAM_FTG_FACE_YUV:
			*(UINT32 *)BufAddr = g_FaceYUVBufAddr;
			break;
		default:
			DBG_ERR("error type:%d\r\n",SnapshotType);
			return -1;

	}
	return 0;
}

INT32 IPCam_FTG_Snapshot_Buf_Create(void){


	//UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
	//UINT32 uiWidth = pEncodeConfig[FTG_PATH].resolution.w;
	//UINT32 uiHeight = pEncodeConfig[FTG_PATH].resolution.h;

	UINT32 uiFaceBufSize = IPCAM_FTG_FACE_MAX_BUF_SIZE;
	UINT32 uiFullBufSize = IPCAM_FTG_FULL_MAX_BUF_SIZE;


	NVTMPP_VB_BLK  blk = 0;
	int i=0;

	//////create face buf for tmp YUV
	UINT32 uiTmpFaceYUVSize = IPCAM_FTG_FULL_YUV_MAX_BUF_SIZE;
	g_FTG_FaceYUV_Pool = nvtmpp_vb_create_pool("FTG_FaceYUV", uiTmpFaceYUVSize, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == g_FTG_FaceYUV_Pool) {
		DBG_ERR("create g_FTG_SnapshotFace_Pool err\r\n");
		return -1;
	}

	blk = nvtmpp_vb_get_block( 0,g_FTG_FaceYUV_Pool,uiTmpFaceYUVSize,NVTMPP_DDR_1);
	if( blk != NVTMPP_VB_INVALID_BLK){
		g_FaceYUVBufAddr = nvtmpp_vb_block2addr(blk);
	}
	else{
		DBG_ERR("pingpong buf%d error\r\n",i);
		return -1;
	}


	/////create full buf for jpeg
	g_FTG_FullJpeg_Pool = nvtmpp_vb_create_pool("FTG_FullJpeg", uiFullBufSize, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == g_FTG_FullJpeg_Pool) {
		DBG_ERR("create g_FTG_FullJpeg_Pool err\r\n");
		return -1;
	}

	blk = nvtmpp_vb_get_block( 0,g_FTG_FullJpeg_Pool,uiFullBufSize,NVTMPP_DDR_1);
	if( blk != NVTMPP_VB_INVALID_BLK){
		g_FullBufAddr = nvtmpp_vb_block2addr(blk);
	}
	else{
		DBG_ERR("g_FTG_FullJpeg_Pool  error\r\n");
		return -1;
	}





	//////create face buf for jpeg
	g_FTG_FaceJpeg_Pool = nvtmpp_vb_create_pool("FTG_FaceJpeg", uiFaceBufSize*MAX_FTGNUM, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == g_FTG_FaceJpeg_Pool) {
		DBG_ERR("create g_FTG_SnapshotFace_Pool err\r\n");
		return -1;
	}

	blk = nvtmpp_vb_get_block( 0,g_FTG_FaceJpeg_Pool,uiFaceBufSize*MAX_FTGNUM, NVTMPP_DDR_1);
	if( blk != NVTMPP_VB_INVALID_BLK){
		g_FaceBufAddr[0] = nvtmpp_vb_block2addr(blk);
	}
	else{
		DBG_ERR("pingpong buf%d error\r\n",i);
		return -1;
	}

	for(i=1; i<MAX_FTGNUM ;i++){
		g_FaceBufAddr[i] = g_FaceBufAddr[i-1]+uiFaceBufSize;

	}


	/////create full buf for YUV
	UINT32 uiFullYUVBufSize = IPCAM_FTG_FULL_YUV_MAX_BUF_SIZE;// 2048 for IPL some header

	g_FTG_FullYUV_Pool = nvtmpp_vb_create_pool("FTG_FullYUV", uiFullYUVBufSize, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == g_FTG_FullYUV_Pool) {
		DBG_ERR("create g_FTG_FullYUV_Pool err\r\n");
		return -1;
	}

	blk = nvtmpp_vb_get_block( 0,g_FTG_FullYUV_Pool,uiFullYUVBufSize,NVTMPP_DDR_1);
	if( blk != NVTMPP_VB_INVALID_BLK){
		g_FullYUVBufAddr = nvtmpp_vb_block2addr(blk);
	}
	else{
		DBG_ERR("pingpong buf%d error\r\n",i);
		return -1;
	}



	return 0;

}


INT32 IPCam_FTG_PINGPONG_Buf_Create(void){


	UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
	UINT32 uiWidth = pEncodeConfig[FTG_PATH].resolution.w;
	UINT32 uiHeight = pEncodeConfig[FTG_PATH].resolution.h;


	UINT32 uiBufSize = ALIGN_CEIL_16(uiWidth) * ALIGN_CEIL_16(uiHeight) * 3/2 + 2048;// 2048 for IPL some header

	NVTMPP_VB_BLK  blk = 0;
	int i=0;
	char pool_name[32]={0};
	for(i=0; i<2;i++){
		sprintf(pool_name,"IPCAM_FTG_Snapshot%d",i);
		g_FTG_PINGPIONG_Pool[i] = nvtmpp_vb_create_pool(pool_name, uiBufSize, 1, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_POOL == g_FTG_PINGPIONG_Pool[i]) {
			DBG_ERR("create g_FTG_PINGPIONG_Pool%d err\r\n",i);
			return -1;
		}

		blk = nvtmpp_vb_get_block( 0,g_FTG_PINGPIONG_Pool[i],uiBufSize,NVTMPP_DDR_1);
		if( blk != NVTMPP_VB_INVALID_BLK){
			g_sFTGBufInfo[i].sYUVInfo.uiYUVAddr[0] = nvtmpp_vb_block2addr(blk);
		}
		else{
			DBG_ERR("pingpong buf%d error\r\n",i);
			return -1;
		}
	}
	DBG_IND("w:%d h:%d uiBufSize:%d\r\n",uiWidth,uiHeight,uiBufSize);
	DBG_IND("buf1:%x buf2:%x\r\n", g_sFTGBufInfo[0].sYUVInfo.uiYUVAddr[0], g_sFTGBufInfo[1].sYUVInfo.uiYUVAddr[0]);


	return 0;
}

INT32 IPCam_FTG_Snapshot_Init(void){

	INT32 ret=0;

	ret = IPCam_FTG_PINGPONG_Buf_Create();
	if(ret < 0){
		DBG_ERR("IPCam_FTG_PINGPONG_Buf_Create fail\r\n");
		return ret;
	}

	ret = IPCam_FTG_Snapshot_Buf_Create();
	if(ret < 0){
		DBG_ERR("IPCam_FTG_Snapshot_Buf_Create fail\r\n");
		return ret;
	}
	sprintf(g_sFTGBufInfo[0].name,"FTG_PPBuf1");
	sprintf(g_sFTGBufInfo[1].name,"FTG_PPBuf2");

	g_sFTGBufInfo[0].eFlag = IPCAM_FTG_PINGPONG_INIT;
	g_sFTGBufInfo[1].eFlag = IPCAM_FTG_PINGPONG_INIT;

	FTG_SANSPSHOT_InstallID();


	return ret;

}

INT32 IPCam_FTG_ReleaseFaceInfo(IPCAM_FTG_BUF_INFO* ftg_info){

#if 1
	if(strcmp(ftg_info->name,g_sFTGBufInfo[0].name)==0){
		g_sFTGBufInfo[0].eFlag = IPCAM_FTG_PINGPONG_READY;
	}
	else{
		g_sFTGBufInfo[1].eFlag = IPCAM_FTG_PINGPONG_READY;
	}
#endif
	return 0;
}

INT32 IPCam_FTG_GetFaceInfo(IPCAM_FTG_BUF_INFO* ftg_info){

	//check pingpong buf and set flag
	static IPCAM_FTG_BUF_INFO *pFTGBufInfo = NULL;
	wai_sem(FTG_SNAPSHOT_SEM_ID);
	if(g_sFTGBufInfo[0].uiSerialNum <= g_sFTGBufInfo[1].uiSerialNum){
		if(g_sFTGBufInfo[0].eFlag == IPCAM_FTG_PINGPONG_READY){
			pFTGBufInfo = &g_sFTGBufInfo[0];
		}
		else{
			if(g_sFTGBufInfo[1].eFlag == IPCAM_FTG_PINGPONG_READY){
				pFTGBufInfo = &g_sFTGBufInfo[1];
			}
			else{
				DBG_ERR("Flag error buf1 flag:%d  buf2 flag:%d\r\n",g_sFTGBufInfo[0].eFlag,g_sFTGBufInfo[1].eFlag);
			}
		}
	}
	else{
		if(g_sFTGBufInfo[1].eFlag == IPCAM_FTG_PINGPONG_READY){
			pFTGBufInfo = &g_sFTGBufInfo[1];
		}
		else{
			if(g_sFTGBufInfo[0].eFlag == IPCAM_FTG_PINGPONG_READY){
				pFTGBufInfo = &g_sFTGBufInfo[0];
			}
			else{
				DBG_ERR("Flag error buf1 flag:%d  buf2 flag:%d!\r\n",g_sFTGBufInfo[0].eFlag,g_sFTGBufInfo[1].eFlag);
			}
		}
	}
	pFTGBufInfo->eFlag = IPCAM_FTG_PINGPONG_READ;
	sig_sem(FTG_SNAPSHOT_SEM_ID);

	ret = IPCam_FTG_GetSnapshotBuf(IPCAM_FTG_FULL_YUV, (void *)ftg_info->sYUVInfo.uiYUVAddr);
	if(ret < 0){
		DBG_ERR("IPCam_FTG_GetSnapshotBuf fail\r\n");
		return ret;

	}
#if 0
	char path[64]={0};
	FST_FILE     filehdl = NULL;

		sprintf(path,"A:\\raw\\yuv_data1.yuv");
		DBG_DUMP("path:%s\r\n",path);
		filehdl = FileSys_OpenFile(path, FST_CREATE_ALWAYS | FST_OPEN_WRITE);
		if (filehdl) {
			char *tmpBuf = (char *)pFTGBufInfo->sYUVInfo.uiYUVAddr[0];
			FileSys_WriteFile(filehdl, (UINT8 *)tmpBuf,&YSize, 0, NULL);
			tmpBuf = (char *)pFTGBufInfo->sYUVInfo.uiYUVAddr[1];
			FileSys_WriteFile(filehdl, (UINT8 *)tmpBuf,&UVSize, 0, NULL);

			FileSys_CloseFile(filehdl);
		} else {
			DBG_ERR("file open error!!\r\n");
		}
#endif
	UINT32 YSize = pFTGBufInfo->sYUVInfo.uiYUVLineOffset[0]*pFTGBufInfo->sYUVInfo.uiYUVHeight;
	UINT32 UVSize = pFTGBufInfo->sYUVInfo.uiYUVLineOffset[1]*pFTGBufInfo->sYUVInfo.uiYUVHeight/2;
	hwmem_open();
	hwmem_memcpy(ftg_info->sYUVInfo.uiYUVAddr[0], pFTGBufInfo->sYUVInfo.uiYUVAddr[0], YSize);
	ftg_info->sYUVInfo.uiYUVAddr[1] = ftg_info->sYUVInfo.uiYUVAddr[0] + YSize;
	hwmem_memcpy(ftg_info->sYUVInfo.uiYUVAddr[1], pFTGBufInfo->sYUVInfo.uiYUVAddr[1], UVSize);
	hwmem_close();
	ftg_info->sYUVInfo.uiYUVLineOffset[0] = pFTGBufInfo->sYUVInfo.uiYUVLineOffset[0];
	ftg_info->sYUVInfo.uiYUVLineOffset[1] = pFTGBufInfo->sYUVInfo.uiYUVLineOffset[1];
	ftg_info->sYUVInfo.uiYUVLineOffset[2] = pFTGBufInfo->sYUVInfo.uiYUVLineOffset[2];
	ftg_info->sYUVInfo.uiYUVWidth = pFTGBufInfo->sYUVInfo.uiYUVWidth;
	ftg_info->sYUVInfo.uiYUVHeight = pFTGBufInfo->sYUVInfo.uiYUVHeight;
	ftg_info->sYUVInfo.uiYUVSize = pFTGBufInfo->sYUVInfo.uiYUVSize; 
	ftg_info->uiFaceNumber = pFTGBufInfo->uiFaceNumber;
	ftg_info->uiSerialNum = pFTGBufInfo->uiSerialNum;
	hwmem_open();
	hwmem_memcpy((UINT32)ftg_info->sFaceInfo,(UINT32)pFTGBufInfo->sFaceInfo, sizeof(FTG_FACE)*pFTGBufInfo->uiFaceNumber);
	hwmem_close();
    strcpy(ftg_info->name, pFTGBufInfo->name);
#if 0
		sprintf(path,"A:\\raw\\yuv_data2.yuv");
		DBG_DUMP("path:%s\r\n",path);
		filehdl = FileSys_OpenFile(path, FST_CREATE_ALWAYS | FST_OPEN_WRITE);
		if (filehdl) {
			char *tmpBuf = (char *)ftg_info->sYUVInfo.uiYUVAddr[0];
			FileSys_WriteFile(filehdl, (UINT8 *)tmpBuf,&YSize, 0, NULL);
			tmpBuf = (char *)ftg_info->sYUVInfo.uiYUVAddr[1];
			FileSys_WriteFile(filehdl, (UINT8 *)tmpBuf,&UVSize, 0, NULL);

			FileSys_CloseFile(filehdl);
		} else {
			DBG_ERR("file open error!!\r\n");
		}
#endif
		//charlie test
		//static int test=0;
		#if 0
		char *tmpBuf = (char *)ftg_info->sYUVInfo.uiYUVAddr[0];
		DBG_DUMP("get:save %x %x %x\r\n",*tmpBuf, *(tmpBuf+1),*(tmpBuf+2));
		char *tmpBuf1 = (char *)ftg_info->sYUVInfo.uiYUVAddr[1];
		DBG_DUMP("get:save %x %x %x\r\n",*tmpBuf1, *(tmpBuf1+1),*(tmpBuf1+2));
		char *tmpBuf2 = (char *)ftg_info->sYUVInfo.uiYUVAddr[2];
		DBG_DUMP("get:save %x %x %x\r\n",*tmpBuf2, *(tmpBuf2+1),*(tmpBuf2+2));
		#endif
		//end charlie test

	pFTGBufInfo->eFlag = IPCAM_FTG_PINGPONG_READY;

	return 0;

}


INT32 IPCam_FTG_SaveFaceInfo(IMG_BUF * YUV_buf,FTG_FACE * Face_info,UINT32 FaceNum){

	//check pingpong buf and set flag
	static IPCAM_FTG_BUF_INFO *pFTGBufInfo = NULL;
	wai_sem(FTG_SNAPSHOT_SEM_ID);
	if(g_sFTGBufInfo[0].uiSerialNum <= g_sFTGBufInfo[1].uiSerialNum){
		if(g_sFTGBufInfo[0].eFlag != IPCAM_FTG_PINGPONG_READ){
			pFTGBufInfo = &g_sFTGBufInfo[0];
		}
		else{
			if(g_sFTGBufInfo[1].eFlag != IPCAM_FTG_PINGPONG_READ){
				pFTGBufInfo = &g_sFTGBufInfo[1];
			}
			else{
				DBG_ERR("Flag error buf1 flag:%d  buf2 flag:%d\r\n",g_sFTGBufInfo[0].eFlag,g_sFTGBufInfo[1].eFlag);
			}
		}
	}
	else{
		if(g_sFTGBufInfo[1].eFlag != IPCAM_FTG_PINGPONG_READ){
			pFTGBufInfo = &g_sFTGBufInfo[1];
		}
		else{
			if(g_sFTGBufInfo[0].eFlag != IPCAM_FTG_PINGPONG_READ){
				pFTGBufInfo = &g_sFTGBufInfo[0];
			}
			else{
				DBG_ERR("Flag error buf1 flag:%d  buf2 flag:%d!\r\n",g_sFTGBufInfo[0].eFlag,g_sFTGBufInfo[1].eFlag);
			}
		}
	}
	pFTGBufInfo->eFlag = IPCAM_FTG_PINGPONG_WRITE;
	sig_sem(FTG_SNAPSHOT_SEM_ID);



	#if (YUV_DRAW == ENABLE)
		////draw rectangle
		if(YUV_buf && Face_info){

			UINT32    drawColor = COLOR_YUV_ORANGE;
			UINT32    border = 6;
			IRECT             DrawRect;
			UINT32 i=0;
			if (FaceNum) {
				for (i = 0; i < FaceNum; i++) {
					DrawRect.x = Face_info[i].x;
					DrawRect.y = Face_info[i].y;
					DrawRect.w = Face_info[i].w;
					DrawRect.h = Face_info[i].h;
					//MovieStamp_DrawRectUV(&DrawRect, drawColor, pDstImg);
					MovieStamp_DrawRect(YUV_buf, &DrawRect, drawColor, border);
				}
			}
		}

	#endif


	//charlie test
	//DBG_DUMP("name:%s %llu\r\n",pFTGBufInfo->name,pFTGBufInfo->uiSerialNum);
	//end charlie test
	///save YUV buf
	if(YUV_buf){
		UINT32 YSize = YUV_buf->LineOffset[0]*YUV_buf->Height;
		UINT32 UVSize = YUV_buf->LineOffset[1]*YUV_buf->Height;

		hwmem_open();
		hwmem_memcpy(pFTGBufInfo->sYUVInfo.uiYUVAddr[0], YUV_buf->PxlAddr[0], YSize);
		pFTGBufInfo->sYUVInfo.uiYUVAddr[1] = pFTGBufInfo->sYUVInfo.uiYUVAddr[0] + YSize;
		hwmem_memcpy(pFTGBufInfo->sYUVInfo.uiYUVAddr[1], YUV_buf->PxlAddr[1], UVSize/2);
		hwmem_close();
		//hwmem_open();
		//hwmem_memcpy(pFTGBufInfo->sYUVInfo.uiYUVAddr[1], YUV_buf->PxlAddr[1], UVSize);
		//hwmem_close();
		pFTGBufInfo->sYUVInfo.uiYUVLineOffset[0] = YUV_buf->LineOffset[0];
		pFTGBufInfo->sYUVInfo.uiYUVLineOffset[1] = YUV_buf->LineOffset[1];
		pFTGBufInfo->sYUVInfo.uiYUVLineOffset[2] = YUV_buf->LineOffset[2];
		pFTGBufInfo->sYUVInfo.uiYUVWidth = YUV_buf->Width;
		pFTGBufInfo->sYUVInfo.uiYUVHeight = YUV_buf->Height;
		pFTGBufInfo->sYUVInfo.uiYUVSize = YUV_buf->LineOffset[0]*YUV_buf->Height*3/2; //hard code for YUV 420
		//charlie test
		#if 0
		//static int test=0;
		char *tmpBuf = (char *)YUV_buf->PxlAddr[0];
		char *tmpBuf1= (char *)YUV_buf->PxlAddr[1];
		char *tmpBuf2= (char *)YUV_buf->PxlAddr[2];
		DBG_DUMP("=== %x %x %x size:%d %x %x %x\r\n",YUV_buf->PxlAddr[0],YUV_buf->PxlAddr[1],YUV_buf->PxlAddr[2],YSize,
			pFTGBufInfo->sYUVInfo.uiYUVAddr[0],pFTGBufInfo->sYUVInfo.uiYUVAddr[1],pFTGBufInfo->sYUVInfo.uiYUVAddr[2]);
		DBG_DUMP("save %x %x %x  %x %x %x %x %x %x\r\n",
			*tmpBuf, *(tmpBuf+1),*(tmpBuf+2),*(tmpBuf1),*(tmpBuf1+1),*(tmpBuf1+2),
			*(tmpBuf2),*(tmpBuf2+1),*(tmpBuf2+2));

		char *tmpBuf3 = (char *)pFTGBufInfo->sYUVInfo.uiYUVAddr[0];
		char *tmpBuf4= (char *)pFTGBufInfo->sYUVInfo.uiYUVAddr[1];
		char *tmpBuf5= (char *)pFTGBufInfo->sYUVInfo.uiYUVAddr[2];

		DBG_DUMP("save1 %x %x %x  %x %x %x %x %x %x\r\n",
			*tmpBuf3, *(tmpBuf3+1),*(tmpBuf3+2),*(tmpBuf4),*(tmpBuf4+1),*(tmpBuf4+2),
			*(tmpBuf5),*(tmpBuf5+1),*(tmpBuf5+2));
		#endif
		//end charlie test

	}

	///charlie test
	#if 0
	FST_FILE     filehdl = NULL;
	char path[64]={0};
	static UINT32 count=0;
	if(count==0){
		 DBG_ERR("file system set\r\n");
		 FsLinux_InstallID();
		if (FST_STA_OK != FileSys_Init(FileSys_GetOPS_uITRON())) {
			DBG_ERR("FileSys_Init failed\r\n");
		}
	}
	if((count%100)==0){

		sprintf(path,"A:\\raw\\yuv_data%d.yuv",count);
		DBG_DUMP("path:%s\r\n",path);
		filehdl = FileSys_OpenFile(path, FST_CREATE_ALWAYS | FST_OPEN_WRITE);
		if (filehdl) {
			char *tmpBuf = (char *)pFTGBufInfo->sYUVInfo.uiYUVAddr[0];
			UINT32 tmpSize = pFTGBufInfo->sYUVInfo.uiYUVSize;
			FileSys_WriteFile(filehdl, (UINT8 *)tmpBuf,&tmpSize, 0, NULL);

			FileSys_CloseFile(filehdl);

		} else {
			DBG_ERR("file open error!!\r\n");
		}

	}
	count++;
	#endif
	//end charlie test

	///save Face Info
	if(Face_info){
		hwmem_open();
		hwmem_memcpy((UINT32)&pFTGBufInfo->sFaceInfo,(UINT32)Face_info,FaceNum*sizeof(FTG_FACE));
		hwmem_close();
		pFTGBufInfo->uiSerialNum =g_uiFTGSerialNum;
		g_uiFTGSerialNum++;
		pFTGBufInfo->uiFaceNumber = FaceNum;

		//charlie test
		#if 0
		int i=0;
		DBG_DUMP("num:%d\r\n",pFTGBufInfo->uiFaceNumber);
		for(i=0;i<(int)pFTGBufInfo->uiFaceNumber;i++){
			DBG_DUMP("i:%d x:%d y:%d w:%d h:%d id:%d p:%d l:%d c:%d c:%d\r\n",i,
				pFTGBufInfo->sFaceInfo[i].x,
				pFTGBufInfo->sFaceInfo[i].y,
				pFTGBufInfo->sFaceInfo[i].w,
				pFTGBufInfo->sFaceInfo[i].h,
				pFTGBufInfo->sFaceInfo[i].id,
				pFTGBufInfo->sFaceInfo[i].pose,
				pFTGBufInfo->sFaceInfo[i].luma,
				pFTGBufInfo->sFaceInfo[i].clarity,
				pFTGBufInfo->sFaceInfo[i].contrast

				);

		}
		#endif
		//end charlie test
		pFTGBufInfo->eFlag = IPCAM_FTG_PINGPONG_READY;
	}




	return 0;
}

#endif

