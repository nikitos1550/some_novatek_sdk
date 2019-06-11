#ifndef _UIIPCAM_H_
#define _UIIPCAM_H_

#include "AppLib.h"
#include "UIAppCommon.h"
#include "NVTEvent.h"
#include "ftg_lib.h"
extern VControl CustomIPCamObjCtrl;


typedef enum {
	NVTEVT_IPCAM_EVT_START      = APPUSER_IPCAM_BASE, ///< Min value = 0x14001000
	//Flow and basic parameter
	NVTEVT_EXE_IPCAM_OPEN         = NVTEVT_IPCAM_EVT_START,
	NVTEVT_EXE_IPCAM_CLOSE,

} CUSTOM_IPCAM_EVENT;





#if(IPCAM_FTG_SNAPSHOT == ENABLE)


typedef enum{

	IPCAM_FTG_FACE_JPEG,
	IPCAM_FTG_FACE_YUV,
	IPCAM_FTG_FULL_JPEG,
	IPCAM_FTG_FULL_YUV,
	IPCAM_FTG_MAX,
	ENUM_DUMMY4WORD(IPCAM_FTG_SNAPSHOT_TYPE)

}IPCAM_FTG_SNAPSHOT_TYPE;

typedef enum{
	IPCAM_FTG_PINGPONG_INIT,
	IPCAM_FTG_PINGPONG_READY,
	IPCAM_FTG_PINGPONG_WRITE,
	IPCAM_FTG_PINGPONG_READ,
	IPCAM_FTG_PINGPONG_MAX,
	ENUM_DUMMY4WORD(IPCAM_FTG_PINGPONG_STATUS)

}IPCAM_FTG_PINGPONG_STATUS;

typedef struct{

	UINT32	uiYUVAddr[MAX_PLANE_NUM];
	UINT32	uiYUVLineOffset[MAX_PLANE_NUM];
	UINT32  uiYUVSize;
	UINT32	uiYUVWidth;
	UINT32	uiYUVHeight;
}IPCAM_FTG_YUV_INFO;

typedef struct{
	char						name[16];
	IPCAM_FTG_PINGPONG_STATUS	eFlag;
	UINT64  					uiSerialNum;
	IPCAM_FTG_YUV_INFO 			sYUVInfo;
	UINT32 						uiFaceNumber;
	FTG_FACE 					sFaceInfo[MAX_FTGNUM];

}IPCAM_FTG_BUF_INFO;


INT32 IPCam_FTG_Snapshot_Init(void);

INT32 IPCam_FTG_SaveFaceInfo(IMG_BUF * YUV_buf,FTG_FACE * Face_info,UINT32 FaceNum);

INT32 IPCam_FTG_GetFaceInfo(IPCAM_FTG_BUF_INFO* ftg_info);

INT32 IPCam_FTG_ReleaseFaceInfo(IPCAM_FTG_BUF_INFO* ftg_info);

INT32 IPCam_FTG_GetSnapshotBuf(IPCAM_FTG_SNAPSHOT_TYPE SnapshotType, void *BufAddr);
#endif



//IPCam CB
extern void IPCam_RegCB(void);

//VdoEnc
extern void IPCamExe_UIDefine_3DNR(UINT32 pathID, UINT32 p3DNRConfig);
extern UINT32 IPCamExe_VdoEnc_ProcCB(UINT32 id, UINT32 state);
extern void IPCamExe_VdoEnc_CB(CHAR* Name, UINT32 event_id, UINT32 value);

//NetRTSP
extern void IPCamExe_RtspIFramReq_CB(UINT32 pathId);

//IPCAM check FPS
#if (IPCAM_RESET_FPS == ENABLE)
extern void IPCam_checkFPS_CB(void);
#endif


#endif //_UIIPCAM_H_
