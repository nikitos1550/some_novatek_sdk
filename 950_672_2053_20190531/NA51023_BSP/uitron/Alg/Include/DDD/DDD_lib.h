#ifndef DDD_LIB_H
#define DDD_LIB_H
#include "mIType.h"
#include "Type.h"
#include "GxImage.h"

#define DDD_MAX_SHAPE_LANDMRK         68       ///< max driver drowsiness detection number

#define DDD_MAX_INPUTIMG_WIDTH          1280
#define DDD_MAX_INPUTIMG_HEIGHT         720
#define DDD_INPUTIMG_BUF_SIZE           (DDD_MAX_INPUTIMG_HEIGHT * ALIGN_CEIL_4(DDD_MAX_INPUTIMG_WIDTH))

/**
     @name DDD return status
*/
//@{
#define DDD_STA_OK                  (0)                   ///<  the status is ok
#define DDD_STA_ERROR               (-1)                  ///<  some error occurred
//@}

typedef struct _RECT
{
    INT32 x;
    INT32 y;
    INT32 width;
    INT32 height;

} RECT, QRECT, DDD_RECT;

typedef struct _DDD_SHAPE{
    INT32 nL;//total landmark number
    INT16 pt[DDD_MAX_SHAPE_LANDMRK * 2];//landmark data[x1,y1,x2,y2,...xn,yn]
} DDD_SHAPE;

typedef enum _DDD_SIDE
{
    DDD_SIDE_LEFT,
    DDD_SIDE_RIGHT,
    DDD_SIDE_BOTH
} DDD_SIDE;

typedef enum _DDD_TRACK_MODE
{
    DDD_TRACK_WITH_LK,
    DDD_TRACK_WITH_FACE_VERIFY,
    DDD_TRACK_WITH_BOTH
}DDD_TRACK_MODE;

typedef enum _DDD_TRACK_TYPE
{
	DDD_TRACK_WITHOUT_CONTOUR,
	DDD_TRACK_WITH_CONTOUR,
	DDD_TRACK_MIN_RECT
} DDD_TRACK_TYPE;

/**
     DDD_PARAM_SETTING_FIELDS:
     This field is define the ddd param like threSholder which need be set by user.
     The field defined as these follow:
     X(@Name, @Type, @DefaultValue)
     User can using @Name to set the values through DDD_SetParam
     & get the threshould by DDD_GetParam.
     Example:
     @code
     {
         INT32 leftThre = DDD_GetParam(DDD_LEFT_EYE_CLOS_THRE);
         DDD_SetParam(DDD_LEFT_EYE_CLOS_THRE, 333);
     }
     @endcode

*/
#define DDD_PARAM_SETTING_FIELDS            \
    X(DDD_AUTO_TRACK_MODE, DDD_TRACK_MODE, DDD_TRACK_WITH_FACE_VERIFY) \
    X(DDD_AUTO_TRACK_TYPE, DDD_TRACK_TYPE, DDD_TRACK_WITHOUT_CONTOUR) \
    X(DDD_AUTO_THRE_SET_FRAMES, UINT32, 1000) \
    X(DDD_AUTO_THRE_INIT_FLAG, BOOL, FALSE) \
    X(DDD_BLINK_AUTO_DETECT_SWITCH, BOOL, TRUE) \
    X(DDD_BLINK_AUTO_DETECT_THRE, INT32, 0) \
    X(DDD_FACE_CHECK_FREQ,  INT32, -1)   /*default is 6, 0 do check every frame, <0 never*/ \
    X(DDD_AUTO_FACE_CHECK_THRE, INT32, 0) \
    X(DDD_FLIP_SIDE, DDD_SIDE, DDD_SIDE_BOTH) /*In blink_detect model using SIDE_BOTH is more stable*/\
    X(DDD_INTEN_INHIB_RATIO, INT32, 921) \
    X(DDD_BRIGHT_SPOT_DETECT_SWITCH, BOOL, FALSE)



typedef enum _DDD_PARAM
{
#define X(param, type, def_val) param,
DDD_PARAM_SETTING_FIELDS
#undef X
} DDD_PARAM;


typedef enum _DDD_FACIAL_TYPE
{
    DDD_FACIAL_TYPE_LEFT_EYE,
    DDD_FACIAL_TYPE_RIGHT_EYE,
    DDD_FACIAL_TYPE_MOUTH,
    DDD_FACIAL_TYPE_HEAD,
    //Only used in DDD_GetFacialPts
    DDD_FACIAL_TYPE_OTHERS

} DDD_FACIAL_TYPE;

typedef enum _DDD_EYE_STATE
{
    DDD_EYE_OPENED,
    DDD_EYE_CLOSED,
    DDD_EYE_UNCERTAIN
} DDD_EYE_STATE;

typedef enum _DDD_MOUTH_STATE
{
    DDD_MOUTH_NORMAL,
    DDD_MOUTH_LAUGH,
    DDD_MOUTH_YAWN
} DDD_MOUTH_STATE;

typedef enum _DDD_HEAD_STATE
{
    DDD_HEAD_STATE_NORMAL,
    DDD_HEAD_STATE_NODING,
    DDD_HEAD_STATE_DISTRIBUTE
} DDD_HEAD_STATE;


#define DDD_USER_PARAM_FIELDS            \
    X(DDD_USER_FPS, FLOAT, 30.0) \
    X(DDD_USER_YAWN_RATE, FLOAT, 0.4)       \
    X(DDD_USER_YAWN_FREQ, INT32, 2) \
    X(DDD_USER_YAWN_FREQ_MIN, FLOAT, 1.0) \
    X(DDD_USER_YAWN_FREQ_MAX, FLOAT, 5.0) \
    X(DDD_USER_YAWN_FREQ_LEN, FLOAT, 10.0) \
    X(DDD_USER_YAWN_DURA, FLOAT, 3.0) \
    X(DDD_USER_EYE_CLOS_THRE_LEFT, FLOAT, 0.25) \
    X(DDD_USER_EYE_CLOS_THRE_RIGHT, FLOAT, 0.25)  \
    X(DDD_USER_EYE_ANGLE_LEFT, INT32, 42) \
    X(DDD_USER_EYE_ANGLE_RIGHT, INT32, -22) \
    X(DDD_USER_EYE_ANGLE_UP, INT32, 22) \
    X(DDD_USER_EYE_ANGLE_DOWN, INT32, -22) \
    X(DDD_USER_EYE_FREQ, INT32, 2) \
    X(DDD_USER_EYE_FREQ_LEN, FLOAT, 10.0) \
    X(DDD_USER_EYE_FREQ_MIN, FLOAT, 0.7) \
    X(DDD_USER_EYE_DURA, FLOAT, 1.0) \
    X(DDD_USER_BLINK_FREQ, INT32, 3) \
    X(DDD_USER_BLINK_FREQ_MIN, FLOAT, 0.25) \
    X(DDD_USER_BLINK_FREQ_MAX, FLOAT, 0.70) \
    X(DDD_USER_BLINK_FREQ_LEN, FLOAT, 10.0) \
    X(DDD_USER_NOD_ALGLE, INT32, -10)  \
    X(DDD_USER_NOD_FREQ, INT32, 5) \
    X(DDD_USER_NOD_FREQ_MIN, FLOAT, 0.5) \
    X(DDD_USER_NOD_FREQ_LEN, FLOAT, 30.0) \
    X(DDD_USER_NOD_DURA, FLOAT, 3.0) \
    X(DDD_USER_DIS_ALGLE_LEFT, INT32, 42) \
    X(DDD_USER_DIS_ALGLE_RIGHT, INT32, -20) \
    X(DDD_USER_DIS_FREQ, INT32, 3) \
    X(DDD_USER_DIS_FREQ_MIN, FLOAT, 0.8) \
    X(DDD_USER_DIS_FREQ_LEN, FLOAT, 10.0) \
    X(DDD_USER_DIS_DURA, FLOAT, 3) \
    X(DDD_USER_NOFACE_LEN, FLOAT, 1.5) \


typedef enum _DDD_USER_PARAM
{
#define X(param, type, def_val) param,
DDD_USER_PARAM_FIELDS
#undef X
} DDD_USER_PARAM;


BOOL DDD_SetUserParam(DDD_USER_PARAM type, FLOAT value);
FLOAT DDD_GetUserParam(DDD_USER_PARAM type);
BOOL DDD_LoadUserParam(const CHAR* path);
BOOL DDD_SaveUserParam(const CHAR* path);
VOID DDD_DispUserParamMsg(VOID);



#define SHIFT_BITS(x) (1 << (x))
typedef enum _DDD_UESR_WARN
{
    DDD_UWRN_NORM           = 0,
    DDD_UWRN_NOFACE         = SHIFT_BITS(0),
    DDD_UWRN_NOFACE_DURA    = SHIFT_BITS(1),
    DDD_UWRN_YAWN           = SHIFT_BITS(2),
    DDD_UWRN_YAWN_DURA      = SHIFT_BITS(3),
    DDD_UWRN_EYE            = SHIFT_BITS(4),
    DDD_UWRN_EYE_DURA       = SHIFT_BITS(5),
    DDD_UWRN_BLINK          = SHIFT_BITS(6),
    DDD_UWRN_NOD             = SHIFT_BITS(7),
    DDD_UWRN_NOD_DURA        = SHIFT_BITS(8),
    DDD_UWRN_DIS             = SHIFT_BITS(9),
    DDD_UWRN_DIS_DURA        = SHIFT_BITS(10),
    DDD_UWRN_PERCLOS         = SHIFT_BITS(11)

} DDD_UWRN;
DDD_UWRN DDD_GetUserWarning(VOID);
/**
     Reset one or multiple DDD Warning in calculation.

     @param[in] warnType: could be one multiple user warning.


/**
     Reset one or multiple DDD Warning in calculation.

     @param[in] warnType: could be one multiple user warning.

     Example:
     @code
     {
         // reset one type of warning
         DDD_ResetUserWarning(DDD_UWRN_YAWN_DURA);
         ...
         // reset multiple type of warning
         DDD_ResetUserWarning(DDD_UWRN_YAWN_DURA | DDD_UWRN_NOD_DURA
                            | DDD_UWRN_PERCLOS);
     }
     @endcode

     Note:
        All warning belong to eye will be reset at one call,
        which means that:
     @code
     {
         // reset one type of warning which belong to eye
         DDD_ResetUserWarning(DDD_UWRN_EYE);
         ...
         // is equal to reset all warning for eye
         DDD_ResetUserWarning(DDD_UWRN_EYE | DDD_UWRN_EYE_DURA
                   | DDD_UWRN_BLINK | DDD_UWRN_PERCLOS);
     }
     @endcode
*/
VOID DDD_ResetUserWarning(DDD_UWRN warnType);


typedef enum _DDD_WARN
{
    DDD_WARN_NORM     = 0,
    DDD_WARN_NONEFACE = 1,
    DDD_WARN_PERCLOS  = 1 << 1,
    DDD_WARN_YAWN     = 1 << 2,
    DDD_WARN_DIS      = 1 << 3,
    DDD_WARN_NODE     = 1 << 4,
    DDD_WARN_EYE      = 1 << 5

} DDD_WARN;


typedef INT32 (*DDD_GET_FDRSLT_HANDLE)(const IMG_BUF*, DDD_RECT*);

/**
     Init Driver Drowsiness Detction(DDD).

     @param[in] buf: the work buffer for DDD. the required buffer size can be get by DDD_CalcBuffSize().
     @param[in] cachebuf: the cache work buffer for DDD. the required cache buffer size can be get by DDD_CalcCacheBuffSize().
     @param[in] pfGetFdRslt: The function handle that get a best Face Rectangle with a gray image as input.

     Example:
     @code
     {

        static INT32 Movie_DDDGetFdRslt(IMG_BUF* pImgBuf, RECT* pFdRect)
        {
        #ifndef MAX
        #define MAX(a, b) (((a) > (b)) ? (a) : (b))
        #endif

            INT32 iFaceNum = FD_GetRsltFaceNum();
            if(iFaceNum >= 0)
            {
                static FD_FACE faceRslt[FD_NUM_MAX] = {0};

                FD_GetRsltFace(faceRslt, 0);
                pFdRect->x = faceRslt[0].x * pImgBuf->Width / 640;
                pFdRect->y = faceRslt[0].y * pImgBuf->Height / 360;
                pFdRect->width = MAX(faceRslt[0].w * pImgBuf->Width / 640,
                                     faceRslt[0].h * pImgBuf->Height / 360);
                pFdRect->height = pFdRect->width;
            }

            return iFaceNum;

        }

        MEM_RANGE *buf, MEM_RANGE *cachebuf
        ...

        UINT32 pBinaryMap;
        //load DDD model to pBinaryMap.
        pBinaryMap = DDD_GetBinaryMap(buf);
        ...

        // init the DDD
        DDD_Init(buf, cachebuf, pBinaryMap, Movie_DDDGetFdRslt);
     }
     @endcode
*/
INT32 DDD_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 pBinaryMap,
                      DDD_GET_FDRSLT_HANDLE pfGetFdRslt);


/**
     Install ID flag for DDD.
     It must be called in 'Install_User()' which is in 'SysCfg.c'.
     In DemoKit, it just need to set the MACRO _DDD_FUNC_ to ENABLE
*/
VOID DDD_InstallID(VOID);

/**
     Reset the DDD state.
     This function should be called before restart DDD_Process
        or DDD_FastProcess

     Example:
     @code
     {
         BOOL bEnableDDD, bEnableDDDPre;
         ...
         if(bEnableDDD)
         {
            if(bEnableDDDPre != bEnableDDD)
            {
                DDD_Reset();
                DDD_Lock(FALSE);
            }
            DDD_Process(...);
         }
         else if(bEnableDDDPre != bEnableDDD)
            DDD_Lock(TRUE);
         bEnableDDDPre = bEnableDDD;
     }
     @endcode
*/
VOID DDD_Reset(VOID);
VOID DDD_Process(IMG_BUF* pInputBuf);
VOID DDD_End(MEM_RANGE *buf, MEM_RANGE *cachebuf);
INT32 DDD_UnInit(VOID);

UINT32 DDD_CalcBuffSize(void);
UINT32 DDD_CalcCacheBuffSize(void);


extern DDD_WARN DDD_GetWarning(VOID);


/**
     Lock/Unlock driver drowsiness detction.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the DDD
         DDD_Lock(TRUE);
         // doing something such as capture
         ...
         // end of capture
         // unlock the DDD
         DDD_Lock(FALSE);
     }
     @endcode
*/
extern void DDD_Lock(BOOL bLock);


//add by Morey

//Reset DDD Tracking
VOID DDD_ResetTracking(VOID);

// Auto Reset Thre
VOID DDD_AutoResetThre(VOID);

/**
     Open/Close DDD tracking mode.
     Tracking mode could make the ddd run more stable.

     @param[in] State: TRUE : Open DDD tracking mode
                       FALSE: Close DDD tracking mode

     Example:
     @code
     {
         //Open DDD Tracking Mode
         DDD_SetTrackingModel(TRUE);
         //Close DDD Tracking Mode
         DDD_SetTrackingMode(FALSE);
     }
     @endcode
*/
VOID DDD_SetTrackingMode(BOOL State);
BOOL DDD_GetTrackingMode(VOID);

//Uart Mssage On & off
VOID DDD_SetShowMsgState(BOOL);
BOOL DDD_GetShowMsgState(VOID);


/**
     Set the DDD parameter that defined by DDD_PARAM_SETTING_FIELDS.
     The field defined as these follow:
     X(@Name, @Type, @DefaultValue)

     @param[in] param_type: same as X(@Name)
     @param[in] param_value: the parameter's value,
                        defalutValue is defined by X(@Value)

     Example:
     @code
     {
         DDD_SetParam(DDD_LEFT_EYE_CLOS_THRE, 333);
     }
     @endcode
*/
VOID DDD_SetParam(DDD_PARAM param_type, INT32 param_value);

/**
     Get the DDD parameter that defined by DDD_PARAM_SETTING_FIELDS.
     The field defined as these follow:
     X(@Name, @Type, @DefaultValue)

     @param[in] param_type: same as X(@Name)

     @return  The value of X(@Name), default is X(@DefaultValue)

     Example:
     @code
     {
          INT32 leftThre = DDD_GetParam(DDD_LEFT_EYE_CLOS_THRE);
     }
     @endcode
*/
INT32 DDD_GetParam(DDD_PARAM param_type);

/**
     Load the DDD parameter that defined by DDD_PARAM_SETTING_FIELDS
     from SD card.

     @param[in] path: the file path in SD card to save DDD paramters.

     @return  TRUE: loading success,
              FALSE: loading failed.

     Example:
     @code
     {
          DDD_LoadParam("A:\\DDD_config.txt")
     }
     @endcode
*/
BOOL DDD_LoadParam(const CHAR* path);

/**
     Saving the DDD parameter that defined by DDD_PARAM_SETTING_FIELDS
     to SD card.

     @param[in] path: the file path in SD card to save DDD paramters.

     @return  TRUE: saving success.
              FALSE: saving failed.

     Example:
     @code
     {
          DDD_SaveParam("A:\\DDD_config.txt")
     }
     @endcode
*/
BOOL DDD_SaveParam(const CHAR* path);

typedef enum
{
    DDD_AUTO_PARAM_EYE_OPEN,
    DDD_AUTO_PARAM_EYE_CLOSE,
    DDD_AUTO_PARAM_DIS_LEFT,
    DDD_AUTO_PARAM_DIS_RIGHT

} DDD_AUTO_PARAM;

VOID DDD_SetAutoParam(DDD_AUTO_PARAM param, UINT32 nFrames);

typedef enum
{
    DDD_AUTO_PARAM_STATE_OK,
    DDD_AUTO_PARAM_STATE_ERR,
    DDD_AUTO_PARAM_STATE_IN_SETTING,
    DDD_AUTO_PARAM_STATE_UNSET
} DDD_AUTO_PARAM_STATE;

DDD_AUTO_PARAM_STATE DDD_GetAutoParamState(DDD_AUTO_PARAM param);


/**
     Get the facial state that defined in DDD_FACIAL_TYPE

     @param[in] StateType: the facial state type could be:
                            DDD_FACIAL_TYPE_LEFT_EYE,
                            DDD_FACIAL_TYPE_RIGHT_EYE,
                            DDD_FACIAL_TYPE_MOUTH,
                            DDD_FACIAL_TYPE_HEAD

     @return  The facial state, the facial state values are
              defined  by enum, the relationship between return
              value and StateType is as follows:
                  DDD_FACIAL_TYPE_LEFT_EYE<-->DDD_EYE_STATE
                  DDD_FACIAL_TYPE_RIGHT_EYE<-->DDD_EYE_STATE
                  DDD_FACIAL_TYPE_MOUTH<-->DDD_MOUTH_STATE
                  DDD_FACIAL_TYPE_HEAD<-->DDD_HEAD_STATE

     Example:
     @code
     {
        if(DDD_HEAD_STATE_DISTRIBUTE ==
          DDD_GetFacialState(DDD_FACIAL_TYPE_LEFT_EYE))
        {
            //do something
        }
     }
     @endcode
*/
UINT32 DDD_GetFacialState(DDD_FACIAL_TYPE StateType);


/**
     Get the facial state's points which state that defined
        in DDD_FACIAL_TYPE

     @param[in] Landmark: the pointer of DDD_SHAPE, the shape could be got
                          from DDD_GetLandmark or  DDD_GetRsltLandmark
     @param[in] StateType: the facial state type could be:
                            DDD_FACIAL_TYPE_LEFT_EYE,
                            DDD_FACIAL_TYPE_RIGHT_EYE,
                            DDD_FACIAL_TYPE_MOUTH,
                            DDD_FACIAL_TYPE_HEAD,
                            DDD_FACIAL_TYPE_OTHERS

     @return  The facial state's landmark position from param[in] 'Landmark'


     Example:
     @code
     {
        DDD_SHAPE shape;
        DDD_GetLandmark(&shape);
        if(DDD_HEAD_STATE_DISTRIBUTE ==
          DDD_GetFacialState(DDD_FACIAL_TYPE_LEFT_EYE))
        {
            DDD_SHAPE headShape = DDD_GetFacialPts(&shape,DDD_FACIAL_TYPE_HEAD)
            INT32 i;
            for(i = 0; i < headShape.nL; i++)
            {
                INT16 x = headShape.pt[i << 1];
                INT16 y = headShape.pt[(i << 1) + 1]
                //do something
                  ...
            }
        }
     }
     @endcode
*/
DDD_SHAPE DDD_GetFacialPts(DDD_SHAPE *Landmark, DDD_FACIAL_TYPE PtsType);

//for Drawing
const DDD_GET_FDRSLT_HANDLE  DDD_GetFdHandle(VOID);
const IMG_BUF *DDD_GetInputImgBuf(VOID);
BOOL DDD_GetTrackingRect(DDD_RECT *TrackRect);
//Get Estimated Face Rectangle.
BOOL DDD_GetEstFaceRect(DDD_RECT *FaceRect);
BOOL DDD_GetLandmark(DDD_SHAPE *pShape);
BOOL DDD_GetPoseMatrix(INT32 iPoseRay[9]);
INT32 DDD_GetPoseAngleNode(VOID);
INT32 DDD_GetPoseAngleShake(VOID);
/**
     Clear the result of DDD, then these functions will return nothing before
        do DDD_Process or DDD_FastProcesss.
        DDD_GetTrackingRect
        DDD_GetLandmark
        DDD_GetPoseMatrix
     Call this function if the DDD_Process has paused or stoped.
     DDD_Reset and DDD_Uinit will call this function automatically
*/
VOID DDD_ClearRsltMsg(VOID);

/**
     Get the output landmarks with coordinates.

     This API will return the output landmarks with coordinates.\n

     @param[out] Landmark: the output landmark.

     @return The number of landmarks.

     Example:
     @code
     {
        DDD_SHAPE  LandmarkResult;
        URECT      DddDispCoord={0,0,640,480};
        UINT32     LandmarkNumber;

        LandmarkNumber = DDD_GetRsltLandmark(&LandmarkResult, &DddDispCoord);
     }
     @endcode
*/
UINT32 DDD_GetRsltLandmark(DDD_SHAPE *Landmark, URECT* TargetCoord);
UINT32 DDD_GetRsltPoseRay(INT32* iPoseRay, INT32 scale, URECT* TargetCoord);
UINT32 DDD_GetRsltTrackingRect(URECT* TrackRect, URECT* TargetCoord);
ER DDD_resizeInputImg(IMG_BUF *pInputImg, IMG_BUF *pOutImg);

//These functions is just for debug
UINT32 DDD_GetLog(UINT32 *BufAddr);
VOID DDD_SetAutoTrackReset(BOOL State);
BOOL DDD_GetAutoTrackReset(VOID);
VOID DDD_SetShowProflieState(BOOL flag);
BOOL DDD_GetShowProflieState(VOID);
#endif // DDD_LANDMARK_H
