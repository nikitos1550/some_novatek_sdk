#include "SysCommon.h"
#include "GxImage.h"
#include "UIInfo.h"
#include "ImageUnit_UserProc.h"
#include "UIPhotoFuncInfo.h"

#if _FD_FUNC_
#include "fd_lib.h"
#include "UIPhotoMapping.h"
#include "ImageUnit_ImagePipe.h"
#include "UIPhotoInfo.h"
#define PHOTO_FD_FACESCALE	16
#endif


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppPhoto_PhotoFuncInfo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if _FD_FUNC_

void Photo_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    FD_Config(FD_CFG_MAX_FACE, MAX_FDNUM);
	FD_Config(FD_CFG_FACE_SCALE, PHOTO_FD_FACESCALE);
	FD_Config(FD_CFG_PROCESS_RATE, 3);
	FD_Init(buf, cachebuf);
}

void Photo_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if 0
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;

	ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));

	myDualCam = Photo_getDualCamSetting();

	//if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL(0);
	} else {
		pUnit = ISF_IPL(1);
	}
	ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT2), (ISF_DATA *)&InputImg, 0);
	//GxImg_DumpBuf("A:\\test.raw",&InputImg,GX_IMAGE_DUMP_ALL);
	FD_Process(&InputImg);
#else

	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	UINT32           ImageRatioIdx;
	UINT32		output_port=(ISF_OUT1 + PHOTO_STRM_ID_1);
	ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(output_port, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, output_port);
	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		#if 0
		{
			static UINT32 isdump = 0;
			PIMG_BUF pImgBuf = (IMG_BUF *) &isf_data.Desc[0];
			if (!isdump)
			{
				DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				GxImg_DumpBuf("A:\\fd.yuv", pImgBuf, GX_IMAGE_DUMP_ALL);
				isdump = 1;
			}
		}
		#endif
		FD_Process((IMG_BUF *)&isf_data.Desc[0]);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
#endif
}

UINT32 Photo_FDCalcBuffSize(void)
{
	FD_Config(FD_CFG_FACE_MAX_SCALE, PHOTO_FD_FACESCALE);
	return FD_CalcBuffSize();
}

PHOTO_FUNC_INFO PhotoFuncInfo_fd = {
	{
		PHOTO_PRIORITY_M,       ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Photo_FDProcess",
		Photo_FDInit,           ///< init fp, only execute at first process
		Photo_FDProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		Photo_FDCalcBuffSize,   ///< get working buffer fp
		FD_CalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif


#if STEREO_FUNC

static UINT32 Photo_StereoCalcBuffSize(void)
{
	return 0;
}


static UINT32 Photo_StereoCalcCacheBuffSize(void)
{
	return 0;
}



static void Photo_StereoInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

}

void Photo_StereoProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	ISF_DATA         isf_data1,isf_data2;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc1,*pSrc2;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(ISF_OUT5, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_SetParam(ISF_OUT6, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc1 = ImageUnit_Out(pUnit, ISF_OUT5);
	pSrc2 = ImageUnit_Out(pUnit, ISF_OUT6);
	if (ImageUnit_IsAllowPull(pSrc1) && ImageUnit_IsAllowPull(pSrc2)){
		if (ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data1, 0) != ISF_OK) {
			return;
		}
		if (ImageUnit_PullData(pSrc2, (ISF_DATA *)&isf_data2, 0) != ISF_OK) {
			ImageUnit_ReleaseData(&isf_data1);
			return;
		}


#if 1
		{
			static UINT32 isdump = 0;
			static UINT32 frame_count = 0;
			if (!isdump && frame_count == 60)
			{
				PIMG_BUF pImgBuf = (IMG_BUF *) &isf_data1.Desc[0];

				DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				GxImg_DumpBuf("A:\\stereo1.yuv", pImgBuf, GX_IMAGE_DUMP_ALL);

				pImgBuf = (IMG_BUF *) &isf_data2.Desc[0];
				DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				GxImg_DumpBuf("A:\\stereo2.yuv", pImgBuf, GX_IMAGE_DUMP_ALL);

				isdump = 1;
			}
			frame_count++;
		}
#endif
        // process stereo here


        // process end , release buffer
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data1);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data2);
	}
}


PHOTO_FUNC_INFO PhotoFuncInfo_Stereo = {
	{
		PHOTO_PRIORITY_H,       ///< function hook process Id.
		IPL_PATH_2,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Photo_StereoProcess",
		Photo_StereoInit,           ///< init fp, only execute at first process
		Photo_StereoProcess,        ///< process fp
		NULL,                      ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		Photo_StereoCalcBuffSize,        ///< get working buffer fp
		Photo_StereoCalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif

void alg_Photo_InitPhotoFunc(PHOTO_FUNC_INFO *phead)
{
    if (1) {
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_dis;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#if _FD_FUNC_
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_fd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();

		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_fd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_fd.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, FD IPLID = %d\r\n", myDualCam, PhotoFuncInfo_fd.Info.IPLId);
	}
#endif
#if STEREO_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_Stereo;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL;  //mark end
	}
#endif
}

