/**
    Face detection library.

    @file       fd_lib.h
    @ingroup    mILibFD

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _FD_LIB_H
#define _FD_LIB_H

#include    "GxImage.h"
#include    "fde_lib.h"
//#include    "NvtFd.h"

/**
    @addtogroup mILibFD
*/
#define __660_CODE__     DISABLE
/**
     @name FD return status
*/
//@{
#define FD_STA_OK       (0)     ///<  the status is ok
#define FD_STA_ERROR    (-1)    ///<  some error occurred
//@}

#define FD_NUM_MAX      (36)    ///< max face detection number
#if __660_CODE__
#define FDE_FW_FACESZ   (32)   // FD HW min size
#else
#define FDE_FW_FACESZ   (30)   // FD HW min size
#endif


#define MIN(x, y)   ( (x)<(y)?(x):(y) )
#define MAX(x, y)   ( (x)>(y)?(x):(y) )
#define CLIP(x,xmin,xmax)   MAX((xmin), MIN((x),(xmax)))
#define DIST(x,y)           ( ((x) > (y))?((x) -(y)):((y) - (x)) )

/**
     @name Face Orientation of supported
*/
//@{
typedef enum
{
    FD_SUPPORT_ORIENT_0   = 0x0001,                   ///<  support face orient degree 0
    FD_SUPPORT_ORIENT_90  = 0x0002,                   ///<  support face orient degree 90
    FD_SUPPORT_ORIENT_270 = 0x0004,                   ///<  support face orient degree 270
    FD_SUPPORT_ORIENT_180 = 0x0008,                   ///<  support face orient degree 180
    FD_SUPPORT_ORIENT_ALL = 0x000F,                   ///<  support all face orient degree
    ENUM_DUMMY4WORD(FD_SUPPORT_ORIENT)
} FD_SUPPORT_ORIENT;
//@}

/**
     Adaboost type. *** add by YZ 20170927 ***

*/
typedef enum
{
    FD_METHOD1_V1 = 0,		///< using method1 and support front roll+/-30 yaw+-90
    FD_METHOD1_V2 = 3,      ///< using method1 and support front sunglasses hat yaw+-90
    FD_METHOD2_V1 = 4,      ///< using method2 and support front yaw+/-45 yaw+-90
    FD_METHOD2_V2 = 5,      ///< using method2 and support front yaw+/-45 yaw+-90, pitchup pitchdown roll+/-30
    ENUM_DUMMY4WORD(FD_CLASSIFY_METHOD)
} FD_CLASSIFY_METHOD;

/**
     method2 speed, for adaptive fde target

*/
typedef enum
{
    FD_SPEED_AUTO = 0,	    ///< using auto FDE threshold for method2
    FD_SPEED_SLOW,	        ///< using losse FDE threshold for method2
    FD_SPEED_NORM,          ///< using normal FDE threshold for method2
    FD_SPEED_FAST,			///< using hard FDE threshold for method2
    ENUM_DUMMY4WORD(FD_METHOD_SPEED)
} FD_METHOD_SPEED;


/**
     sort result face type

*/
typedef enum
{
    FD_SORT_BYSIZE = 0,         ///< sort result face by score, only for method2
    FD_SORT_BYSCORE,	        ///< sort result face by size
    ENUM_DUMMY4WORD(FD_SORT_METHOD)
} FD_SORT_METHOD;

/**
     Config ID.

*/
typedef enum
{
	FD_CFG_MAX_FACE,            ///< config maximum face number
	FD_CFG_FACE_MAX_SCALE,      ///< config max scale, for save memory, FD_CFG_FACE_SCALE must < FD_CFG_FACE_MAX_SCALE;
	FD_CFG_FACE_SCALE,          ///< config the minimal face to detect releative to the maximum of image width and weight
	FD_CFG_PROCESS_RATE,        ///< config the process rate of frame, set 3 means 3 frames detect FD 1 time.
	FD_CFG_CLASSIFY_METHOD,		///< config the type of classifier
    FD_CFG_SUPPORT_ORIENT,      ///< config support orient, it will auto rotate image orient
    FD_CFG_SCORE_THRESHOLD,     ///< config face score threshold, only method2 support
    FD_CFG_METHOD2_SPEED,       ///< config FD2 speed, if demokit plz using FD_SPEED_AUTO, only method2 support
    FD_CFG_SUPPORT_POSE,        ///< config FD2 support poses, only method2 support
    FD_CFG_RESULT_SORT,         ///< config FD result sort method
    FD_CFG_MD_FILTER_TH,        ///< config MD filter FDE proposal threshold
	ENUM_DUMMY4WORD(FD_CFG)
} FD_CFG;


typedef struct _FD_FACE {
	UINT16  x;               ///< result start x
	UINT16  y;               ///< result start y
	UINT16  w;               ///< result width
	UINT16  h;               ///< result height
	UINT16  score;			 ///< result score
	UINT8   trgIdx;          ///< trigger index
} FD_FACE, *P_FD_FACE;

/**
     Face detection result info.

*/
typedef struct _FDMSG_INFO {
	UINT32      FaceDetectNum;                   ///<Detected total face number
	FD_FACE     FaceDetectRect[FD_NUM_MAX];      ///<Detected face rectangle
} FDMSG_INFO, *P_FDMSG_INFO;

/**
     Face detection callback message.

*/
typedef enum {
	FD_CBMSG_DETEND = 0,        ///< user will receive this notification when face detection process is end.
	FD_CBMSG_MAX_CNT,           ///< total callback message count
	ENUM_DUMMY4WORD(FD_CBMSG)
} FD_CBMSG;

/**
    FD callback funcion proto type
*/
typedef void (*FD_CBMSG_FP)(FD_CBMSG uiMsgID, void *Data);

/**
     Face detection input image ratio, only for 660 compile

*/
typedef enum _FD_IMAGERATIO
{
    FD_IMAGERATIO_4_3 = 0,      ///< input image ratio is 4:3
    FD_IMAGERATIO_3_2,          ///< input image ratio is 3:2
    FD_IMAGERATIO_16_9,         ///< input image ratio is 16:9
    FD_IMAGERATIO_1_1,          ///< input image ratio is 1:1
    FD_IMAGERATIO_MAX_CNT,      ///< total image ratio count
    ENUM_DUMMY4WORD(FD_IMAGERATIO)
}FD_IMAGERATIO;


/**
     Config some settings of FD.

     @param[in] config_id: configure ID.
     @param[in] value: setting value.

     Example:
     @code
     {
         FD_Config(FD_CFG_MAX_FACE, 10);
     }
     @endcode
*/
extern void FD_Config(FD_CFG config_id, UINT32 value);


/**
     Get some configurations of FD.

     @param[in] config_id: configure ID.

     @return the setting value.

     Example:
     @code
     {
         UINT32 value;
         value  = FD_GetConfig(FD_CFG_SCALE_ENG);
     }
     @endcode
*/
extern UINT32 FD_GetConfig(FD_CFG config_id);
/**
     Init Face detection function.

     This function is used to initialize face detection buffer and set some initial setting.

     @param[in] buf: the work buffer for face detection process. the required buffer size can be get by FD_CalcBuffSize().
     @param[in] cachebuf: the cache work buffer for face detection process. the required cache buffer size can be get by FD_CalcCacheBuffSize().
     @param[in] maxFace: the maximum number of detected faces.
     @param[in] faceScale: the value defining the minimal face to detect releative to the maximum of image width and weight.
     @param[in] ProcessRate: the process rate of FD_Process(), if set value 3, means calling the function FD_Process() 3 times will run the face detection once.
     @return
         - @b FD_STA_OK:       Success.
         - @b FD_STA_ERROR:    Some error occurred.

     Example:
     @code
     {
         MEM_RANGE buf, cachebuf;

         buf.Addr = pbuf;
         buf.Size = FD_CalcBuffSize();
         cachebuf.Addr = pcachebuf;
         cachebuf.Size = FD_CalcCacheBuffSize();

         // Init FD buffer
         FD_Init(buf,cachebuf);
     }
     @endcode


*/
extern INT32 FD_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf);

/**
     UnInit face detection function.

     The API will reset the face detection result number and release buffer.
     @return
         - @b FD_STA_OK:       Success.
         - @b FD_STA_ERROR:    Some error occurred.
*/
extern INT32 FD_UnInit(void);
/**
     Process the face detection function.

     This API will scale the input image to temp buffer by image ratio.\n
     The temp image buffer will be the input image of HW face detection engine FDE.\n
     After FDE process, if returned any candidate faces, each face will be verified by Arcsoft FD lib.\n
     If Arcsoft FD lib return TRUE, then we treat this candidate face as a real face, and the face number count add 1.\n
     This API also uses the face tracking to enhance the stability of face detection.

     @note The process function will really run the face detection when FD is unlocked. It will just returned when FD is locked.\n
           User can lock/unlock the FD function by calling FD_Lock().\n

     @param[in] pInputImg: the face detection input image.
     @param[in] imgRatio: the input image w/h ratio.

     @return
         - @b >=0:       The number of detected faces
         - @b <0:        Some error occurred.

     Example:
     @code
     {
        IMG_BUF          InputImg;
        IPL_IME_BUF_ADDR CurInfo;
        UINT32           srcW,srcH,PxlFmt;
        UINT32           PxlAddr[3];
        UINT32           LineOff[3];
        INT32            FaceNumber;

        CurInfo.Id = IPL_ID_1;
        IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);
        if (CurInfo.ImeP2.type == IPL_IMG_Y_PACK_UV422)
        {
            PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
        }
        else
        {
            PxlFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        }
        srcW = CurInfo.ImeP2.Ch[0].Width;
        srcH = CurInfo.ImeP2.Ch[0].Height;
        LineOff[0] = CurInfo.ImeP2.Ch[0].LineOfs;
        LineOff[1] = CurInfo.ImeP2.Ch[1].LineOfs;
        LineOff[2] = CurInfo.ImeP2.Ch[2].LineOfs;
        PxlAddr[0] = CurInfo.ImeP2.PixelAddr[0];
        PxlAddr[1] = CurInfo.ImeP2.PixelAddr[1];
        PxlAddr[2] = CurInfo.ImeP2.PixelAddr[2];
        GxImg_InitBufEx(&InputImg, srcW, srcH, PxlFmt, LineOff, PxlAddr);
        FaceNumber = FD_Process(&InputImg);
     }
     @endcode
*/
extern INT32    FD_Process(IMG_BUF *pInputImg);

/**
     Calculate face detection required buffer size.

     The working buffer is used for scaling temp buffer and HW engine FDE output data.

     @return the required buffer size.
*/
extern UINT32   FD_CalcBuffSize(void);

/**
     Calculate face detection required cache buffer size.

     The cache buffer size is for arcsoft lib using.

     @return the required cache buffer size.
*/
extern UINT32   FD_CalcCacheBuffSize(void);

/**
     Lock/Unlock face detction.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the FD
         FD_Lock(TRUE);
         // doing something such as capture
         ...
         // end of capture
         // unlock the FD
         FD_Lock(FALSE);
     }
     @endcode
*/
extern void     FD_Lock(BOOL bLock);

/**
     Register callback function.

     @note User must register callback funcion then can receive the callback message notification when detected faces.

     @param[in] CB: the callback function pointer.

     Example:
     @code
     {
        void Photo_FdCB(FD_CBMSG uiMsgID, void *Data)
        {
            switch (uiMsgID)
            {
                case FD_CBMSG_DETEND:
                   DBG_IND("===FDEND===\r\n");
                   Ux_PostEvent(NVTEVT_ALGMSG_FDEND,1, NVTEVT_ALGMSG_FDEND);
                   break;
                default:
                   break;
            }
        }
        FD_RegisterCB(Photo_FdCB);
     }
     @endcode
*/
extern void     FD_RegisterCB(FD_CBMSG_FP CB);

/**
     Clear the face number.

     This API will clear the result face number of face detection.

*/
extern void     FD_ClrRsltFaceNum(void);

/**
     Get the face number.

     This API will return the result face number of face detection.

     @return The number of detected faces.
*/
extern UINT32   FD_GetRsltFaceNum(void);


/**
     Get the result faces of different coordinate.

     This API will return the result faces by different coordinate.\n

     @param[out] Face: the result face info.
     @param[in] TargetCoord: the target window coordinate.

     @return The number of detected faces.

     Example:
     @code
     {
        FD_FACE    FdInfo[FD_NUM_MAX];
        URECT      FdDispCoord={0,0,640,480};
        UINT32     FaceNumber;

        FaceNumber = FD_GetRsltFace(FdInfo, &FdDispCoord);
     }
     @endcode
*/
extern UINT32   FD_GetRsltFace(FD_FACE *Faces, URECT *TargetCoord);

/**
     Get the face detection result flag.

     Get the face detection result flag.
     If the face detection result has been updated, this flag will be set true.
     When API FD_GetRsltFace() is called, the flag is auto cleared.

     @return The face detection result flag.
*/
extern BOOL     FD_GetRsltFlag(void);



extern UINT32   fd_getImageBuffSize(VOID);

extern VOID     FD_RoiConfig(URECT roi);
extern VOID     FD_GetRoiConfig(URECT *proi, URECT* targetcoord);

extern VOID     fd_set_mdmask(UINT8 *pmask, UINT32 w_num, UINT32 h_num, UINT32 mask_w, UINT32 mask_h);

#endif
