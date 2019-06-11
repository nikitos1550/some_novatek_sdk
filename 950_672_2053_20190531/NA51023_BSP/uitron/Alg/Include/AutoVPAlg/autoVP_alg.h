/**
    AutoVP detection library.

    @file       autoVP_alg.h
    @ingroup    mILibAutoVP_Nvt

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _AUTO_VP_ALG_H
#define _AUTO_VP_ALG_H

/**
    @addtogroup mILibAutoVP_Nvt
*/

/**
    AutoVP failure info
*/
//@{
#define AUTO_VP_FAIL_ROI        1       ///<  AVP out of ROI
#define AUTO_VP_FAIL_AVPS       2       ///<  Too much failure AVP
#define AUTO_VP_FAIL_FEW_AVP    4       ///<  Few AVP detected
//@}

#define AVP_MAX_CAR_LINES       4
#define AVP_MAX_CAR_LINES_ROI   (AVP_MAX_CAR_LINES<<1)

typedef struct _AUTOVP_ROI
{
    UINT32 left;
    UINT32 top;
    UINT32 right;
    UINT32 bottom;
}AUTOVP_ROI;

typedef struct _AUTOVP_IMAGE
{
    UINT32 addr;
    UINT32 width;
    UINT32 height;
    UINT32 lofs;
}AUTOVP_IMAGE;

typedef struct _AUTOVP_POINT
{
    INT32 x;
    INT32 y;
}AUTOVP_POINT;

typedef struct _AUTOVP_LINE
{
    AUTOVP_POINT sP;
    AUTOVP_POINT eP;
}AUTOVP_LINE;

typedef struct _AUTOVP_RECT
{
    INT32 x;
    INT32 y;
    UINT32 width;
    UINT32 height;
}AUTOVP_RECT;

/**
                 uiVPSrcWidth
 -------------------------------------------------
 - - (uiVPRoiSx, uiVPRoiSy)                      -
 -   -                                           -
 -     ------------uiVPRoiWidth-----------       -
 -     -                                 -       -
 -     -   (iLx1,iLy1)*    * (iRx1,iRy1) -       -
 -     -                          uiVPRoiHeight  - uiVPSrcHeight
 -     -                                 -       -
 -     -(iLx2,iLy2)*           * (iRx2,iRy2)-    -
 -     ----------------------------------        -
 -                                               -
 -                                               -
 -------------------------------------------------
*/
/**
     AutoVP detection input structure.
*/
//@{
typedef struct _AUTO_VP_SRCIMG_INFO
{
    UINT32 uiVPSrcBufAddr;      ///<  Auto VP input buffer address with ROI IME image
    UINT32 uiVPSrcWidth;        ///<  width of Auto VP input image with ROI IME image
    UINT32 uiVPSrcHeight;       ///<  height of Auto VP input image with ROI IME image
    UINT32 uiVPSrcLineofs;      ///<  lineofset of Auto VP input image with ROI IME image
    UINT32 uiFrameWidth;        ///<  width of IME image
    UINT32 uiFrameHeight;       ///<  height of IME image
    UINT32 uiRoiWidth;          ///<  width of ROI region of IME image
    UINT32 uiRoiHeight;         ///<  height of ROI region of IME image
    UINT32 uiRoiSx;             ///<  start x coordinate of ROI region of IME image
    UINT32 uiRoiSy;             ///<  start y coordinate of ROI region of IME image
    UINT32 uiScaleUp;           ///<  IME image scale up
    UINT32 uiScaleDown;         ///<  IME image scale down
    UINT32 uiScaleRsl;          ///<  IME image scale resolution
} AUTO_VP_SRCIMG_INFO;
//@}

//@{
typedef struct _AUTO_VP_SRCDATA_INFO
{
    AUTO_VP_SRCIMG_INFO AutoVPSrcImgInfo;
    UINT32 uiWorkBufAddr;
    UINT32 uiCacheBufAddr;
} AUTO_VP_SRCDATA_INFO;
//@}

//@{
typedef struct _AUTO_VP_RESULT_INFO
{
    AUTOVP_LINE             CandiLines[AVP_MAX_CAR_LINES_ROI];
    UINT32                  CandiFlag[AVP_MAX_CAR_LINES_ROI];       /// flag: 1~4
    AUTOVP_LINE             ChosedLine[AVP_MAX_CAR_LINES];
    UINT32                  FailureStatus;
    AUTOVP_POINT            uiVPNew;
    UINT32                  ISETime;
    UINT32                  GraphicTime;
} AUTO_VP_RESULT_INFO;
//@}

typedef struct _AUTO_VP_DEBUG_INFO
{
    BOOL isECosEnable;
    BOOL isLinuxEnable;
    BOOL isCPU2Open;
} AUTO_VP_DEBUG_INFO;

/**
    initial AutoVP parameters with default value.
*/
extern void AutoVP_InitDefParms(void);

/**
    initial AutoVP Lib.
*/
extern void AutoVP_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf);

/**
    uninitial AutoVP Lib.
*/
extern void AutoVP_Uninit(void);

/**
    AutoVP Detection
*/
extern UINT32 AutoVP_Detection(AUTO_VP_SRCIMG_INFO Src_Img_info, UINT32 InputUsableMemAddr, UINT32 InputUsableCacheMemAddr, AUTO_VP_RESULT_INFO *Auto_VP_Info, AUTO_VP_DEBUG_INFO *Auto_VP_Dbg_Info);

/**
    get AutoVP initial flag
*/
extern UINT32 AutoVP_GetInitFlag(void);

/**
    Get AutoVP results info
*/
extern AUTO_VP_RESULT_INFO ADAS_GetAutoVPRstInfo(void);

/**
    Get AutoVP source info
*/
extern AUTO_VP_SRCIMG_INFO ADAS_GetAutoVPSrcInfo(void);

/**
    Get AutoVP debug info
*/
extern AUTO_VP_DEBUG_INFO ADAS_GetAutoVPDbgInfo(void);

/**
    Get AutoVP ROI
*/
extern AUTOVP_ROI GetAutoVPROI(void);

/**
    Set AutoVP ROI with default value
*/
extern void SetAutoVPROIDefault(void);

/**
    Set AutoVP ROI
*/
extern void SetAutoVPROI(AUTOVP_ROI roi);

/**
    Get AutoVP candidate number
*/
extern UINT32 GetAutoVPCandiNum(void);

/**
    Set AutoVP candidate number with default value
*/
extern void SetAutoVPCandiNumDefault(void);

/**
    Set AutoVP candidate number
*/
extern void SetAutoVPCandiNum(UINT32 num);

#endif

