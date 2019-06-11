/**
    Face Detection and Tracking library.

    @file       nvt_ftg_lib.h
    @ingroup    FTG lib

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef NVTODT_LIB_H
#define NVTODT_LIB_H

#include "dis_lib.h"
#include "dis_alg_fw.h"
#include "dis_alg_hw.h"
#include "odt_lib.h"

#define NVT_FTG_CLARITY_FUNC        DISABLE
#define NVT_FTG_LANDMARK_FUNC		DISABLE		///< open or close face landmark calculating function

#define NVT_FTG_NUM_MAX					(36)
#define NVT_FTG_MAX_LANDMARK			(68)       ///< max landmark detection number

#define MIN(x, y)   ( (x)<(y)?(x):(y) )
#define MAX(x, y)   ( (x)>(y)?(x):(y) )
#define CLIP(x,xmin,xmax)   MAX((xmin), MIN((x),(xmax)))
#define DIST(x,y)           ( ((x) > (y))?((x) -(y)):((y) - (x)) )
#define CLAMP(x, a, b)		MAX(MIN(x, b), a)

/**
     @name NvtODT return status
*/
//@{
#define NVT_FTG_STA_OK                  (0)                   ///<  the status is ok
#define NVT_FTG_STA_ERROR               (-1)                  ///<  some error occurred
//@}

typedef struct _FTG_FACE
{
	// FTG location
    UINT32  x;								///< result x
    UINT32  y;								///< result y
    UINT32  w;								///< result w
    UINT32  h;								///< result h
    UINT32  id;								///< result id
    UINT32  pose;							///< result pose: 0 front face, 1 left side face, 2 right side face, 3 error pose;
    // FTG clarity
    UINT16  luma;							///< result face average luma range [0, 255]
	UINT16  clarity;						///< result face average clarity range [0, 255]
	UINT16  contrast;						///< result face average contrast range [0, 255]
	// FTG landmark
	INT32   lstate;							///< if landmark success is 1, failed is -1;
	INT32   yaw;							///< result face yaw angle
	INT32	pitch;							///< result face pitch angle
	INT32	roll;							///< result face roll angle
	INT32	lscore;							///< result face landmark score range [-1024 1024]
	INT32   lnumber;						///< result face landmark number
	INT16	lpoints[NVT_FTG_MAX_LANDMARK*2];///< result face landmark data[x1,y1,x2,y2,...xn,yn]
} FTG_FACE;


typedef struct _FTG_TIMESTAMP
{
	UINT32 timestamp_sec;
	UINT32 timestamp_usec;
} FTG_TIMESTAMP;

/**
     Face results type.

*/
typedef enum _FTG_RESULT_TYPE
{
	NVT_FTG_RSLT_LOCATION		= 0x0000,			///< ftg results only with face location and input image.
	NVT_FTG_RSLT_CLARITY		= 0x0001,			///< ftg results with face location, input image, clarity and luma.
	NVT_FTG_RSLT_LANDMARK		= 0x0002,			///< ftg results with face location, input image, clarity, luma, angle and landmark score.
	NVT_FTG_RSLT_CLARITY_CPU	= 0x0004,			///< ftg results with face location, input image, clarity, luma, angle and landmark score.
    ENUM_DUMMY4WORD(NVT_FTG_RESULT_TYPE)
} NVT_FTG_RESULT_TYPE;

/**
     Face landmark result type.

*/
typedef enum _FTG_LANDMARK_TYPE
{
    NVT_FTG_LANDMARK_FAILED     = -1,           ///< ftg landmark failed.
    NVT_FTG_LANDMARK_5POINT		= 0,			///< ftg 5 point landmark.
	NVT_FTG_LANDMARK_68POINT	= 1,			///< ftg 68 point landmark.
    ENUM_DUMMY4WORD(FTG_LANDMARK_TYPE)
} FTG_LANDMARK_TYPE;

/**
     Face pose type. (by human subject)

*/
typedef enum _FTG_POSE_TYPE
{
    NVT_FTG_FRONT_FACE      = 0,            ///< ftg pose : frontal face.
    NVT_FTG_LEFT_FACE       = 1,			///< ftg pose : left profile face.
	NVT_FTG_RIGHT_FACE      = 2,			///< ftg pose : right profile face.
    ENUM_DUMMY4WORD(FTG_POSE_TYPE)
} FTG_POSE_TYPE;

/**
     support fd2 pose type

*/
typedef enum
{
    FTG_FD_SUPPORT_FRONT        = 0x0001,           ///<  support front face detection
    FTG_FD_SUPPORT_YAW45_LEFT   = 0x0002,           ///<  support yaw45 left face detection
    FTG_FD_SUPPORT_YAW45_RIGHT  = 0x0004,           ///<  support yaw45 right face detection
    FTG_FD_SUPPORT_YAW90_LEFT   = 0x0008,           ///<  support yaw90 left face detection
    FTG_FD_SUPPORT_YAW90_RIGHT  = 0x0010,           ///<  support yaw90 right face detection
    FTG_FD_SUPPORT_PITCH20_DOWN = 0x0020,           ///<  support pitchdown face detection
    FTG_FD_SUPPORT_PITCH20_UP   = 0x0040,           ///<  support pitchup face detection
    FTG_FD_SUPPORT_ROLL30_LEFT  = 0x0080,           ///<  support roll30 left face detection
    FTG_FD_SUPPORT_ROLL30_RIGHT = 0x0100,           ///<  support roll30 right face detection
    FTG_FD_SUPPORT_ALL_POSE     = 0x01FF,           ///<  support all face detection
    ENUM_DUMMY4WORD(FTG_FD2_SUPPORT_POSE)
} FTG_FD2_SUPPORT_POSE;

/**
     Config ID.

*/
typedef enum
{
	// ****** FD ******
	NVT_FTG_FD_CFG_MAX_FACE,        ///< config maximum face number
    NVT_FTG_FD_CFG_FACE_MAX_SCALE,  ///< config max scale, for save memory, FD_CFG_FACE_SCALE must < FD_CFG_FACE_MAX_SCALE;
    NVT_FTG_FD_CFG_FACE_SCALE,      ///< config the minimal face to detect releative to the maximum of image width and weight
	NVT_FTG_FD_CLASSIFIER_TYPE, 	///< default value is 4 (FD methold2 support angel: front, +/-yaw45, +/-yaw90), 5 ( plus support angle: +/-roll30, pitchup, pitchdown)
	NVT_FTG_CFG_PROCESS_RATE,       ///< config od the process rate of frame, set 3 means 3 frames detect OD 1 time.
	NVT_FTG_FD_SCORE_THRESHOLD,     ///< only using when FD methold 2 (NVT_FTG_FD_CLASSIFIER_TYPE = 4 / 5); range [0-10] default = 5;
    NVT_FTG_CFG_MD_THRESHOLD,       ///< config md mask filter fd proposals overlap threshold, e.g. set 32(defalut) means overlap iou = 32/256 = 0.125 ;
    NVT_FTG_FD_CFG_SUPPORT_POSE,    ///< config fd support pose type, default all pose type as FTG_FD2_SUPPORT_POSE;
    // ****** ODT ******
	NVT_FTG_CFG_ODT_PROCESS_ID,		///< config odt prcess id
	NVT_FTG_CFG_MAX_OBJ_LIFT1,		///< config max obj life OT
	NVT_FTG_CFG_MAX_OBJ_LIFT2,		///< config max obj life OD
	NVT_FTG_CFG_OUTOBJ_CONTEXT,		///< config output obj context range, can be set to 0~100, e.g. set 15 means output obj box with 1.5 times the original size and context range.
	NVT_FTG_CFG_SCALE_FOR_MATCH,	///< config scale factor for area match
	NVT_FTG_CFG_TRACK_SENSITIVITY,  ///< config tracking sensitivity
	NVT_FTG_CFG_EXPAND_TIMES,       ///< config tracking obj context, will keep src size after tracking
	// ****** FTG ******
    NVT_FTG_LANDMARK_THRESHOLD,		///< landmark score threshold range [-1024 1024]
    NVT_FTG_FD_RESULT_CONTEXT,      ///< config FD result context range, not keep src size after tracking
    NVT_FTG_SNAPSHOT_CONTEXT,		///< config snapshot context range, can be set to 0~100, e.g. set 15 means output obj box with 1.5 times the original size and context range.
	ENUM_DUMMY4WORD(NVT_FTG_CFG)
} NVT_FTG_CFG;


/**
     Config some settings of FTG.

     @param[in] config_id: configure ID.
     @param[in] value: setting value.

     Example:
     @code
     {
         nvt_ftg_config(NVT_FTG_CFG_PROCESS_RATE, 3);
     }
     @endcode
*/
extern void nvt_ftg_config(NVT_FTG_CFG config_id, INT32 value);


/**
     Get some configurations of FTG.

     @param[in] config_id: configure ID.

     @return the setting value.

     Example:
     @code
     {
         INT32 value;
         value  = nvt_ftg_getconfig(NVT_FTG_CFG_PROCESS_RATE);
     }
     @endcode
*/
extern INT32 nvt_ftg_getconfig(NVT_FTG_CFG config_id);
/**
     Init ODT function.

     This function is used to initialize FTG buffer and set some initial setting.

     @param[in] buf: the work buffer for FTG process. the required buffer size can be get by odt_calcbuffsize().
     @param[in] cachebuf: the cache work buffer for FTG process. the required cache buffer size can be get by odt_calccachebuffsize().
     @return
         - @b NVT_FTG_STA_OK:       Success.
         - @b NVT_FTG_STA_ERROR:    Some error occurred.

     Example:
     @code
     {
         MEM_RANGE buf, cachebuf;

         buf.Addr = pbuf;
         buf.Size = nvt_ftg_calcbuffsize();
         cachebuf.Addr = pcachebuf;
         cachebuf.Size = nvt_ftg_calccachebuffsize();

         // Init ODT buffer
         nvt_ftg_init(buf,cachebuf);
     }
     @endcode


*/
extern INT32 nvt_ftg_init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 width, UINT32 height);

/**
     Reset ODT function.

     The API will reset the FTG result number and params.
     @return
         - @b NVT_FTG_STA_OK:       Success.
         - @b NVT_FTG_STA_ERROR:    Some error occurred.
*/
extern INT32 nvt_ftg_reset(void);


/**
     UnInit ODT function.

     The API will reset the FTG result number and release buffer.
     @return
         - @b NVT_FTG_STA_OK:       Success.
         - @b NVT_FTG_STA_ERROR:    Some error occurred.
*/
extern INT32 nvt_ftg_uninit(void);


/**
     Init FTG required image.

     Set the image copy for face detection & face tracking.

     @return viod.
*/
extern VOID nvt_ftg_setimg(IMG_BUF *pinputimg);

extern VOID nvt_ftg_set_mdmask(UINT8* pmask, UINT32 w_num, UINT32 h_num);

/**
     Process the FTG function.

     This API include face detection and face tracking

     If Novatek FD lib return TRUE, then we treat this candidate obj as a real obj, and give it a id number.\n

     face tracking will tracking the reference real face in present frame use DIS process.\n
     If DIS process return TRUE, then we treat this obj and the reference real obj having same id number.\n


     @return
         - @b >=0:       The number of objs
         - @b <0:        Some error occurred.

     Example:
     @code
     {
        IMG_BUF          InputImg;

		ftg_fd_setimg(pinputimg);
        nvt_ftg_fd_process();
     }
     @endcode
*/
extern INT32 nvt_ftg_fd_process(VOID);
extern INT32 nvt_ftg_ot_process(VOID);

/**
     Calculate FTG required buffer size.

     The working buffer is used for obj detection and tracking.

     @return the required buffer size.
*/
extern UINT32   nvt_ftg_calcbuffsize(void);

/**
     Calculate FTG required cache buffer size.

     The working buffer is used for obj detection and tracking.

     @return the required cache buffer size.
*/
extern UINT32   nvt_ftg_calccachebuffsize(void);


/**
     Lock/Unlock FTG.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the FTG
         nvt_ftg_lock(TRUE);
         // doing something such as capture
         ...
         // end of capture
         // unlock the FTG
         nvt_ftg_lock(FALSE);
     }
     @endcode
*/
extern void     nvt_ftg_lock(BOOL block);

/**
     Get the result objs of FTG.

     This API will return the result objs by different coordinate.\n

     @param[out] Obj: the result obj info.
     @param[in] TargetCoord: the target window coordinate.

     @return The number of objs in present frame.

     Example:
     @code
     {
        FTG_FACE   FTGInfo[NVT_FTG_NUM_MAX];
        URECT      FTGDispCoord={0,0,640,480};
        UINT32     face_number;
        UINT32     max_number = 10;

        face_number = nvt_ftg_get_result(ODTInfo, &ODTDispCoord, max_number, UINT32 time_addr, NVT_FTG_RSLT_LOCATION | NVT_FTG_RSLT_CLARITY);
     }
     @endcode
*/
extern UINT32	nvt_ftg_get_result(FTG_FACE *faces, URECT* targetcoord, UINT32 max_number, UINT32 time_addr, UINT32 result_type);

/**
     Map obj IDs to continuous IDs.

     @param[in] obj: FTG obj detection results.
     @param[in] objnumber: FTG obj number.

     @return void.

     Example:
     @code
     {
        FTG_FACE   FTGInfo[NVT_FTG_NUM_MAX];
        URECT      FTGDispCoord={0,0,640,480};
        UINT32     face_number;
        UINT32     max_number = 10;

        face_number = nvt_ftg_get_result(ODTInfo, &ODTDispCoord, max_number, UINT32 time_addr, NVT_FTG_RSLT_LOCATION);
		nvt_ftg_idmap(ODTInfo, face_number);
     }
     @endcode

*/
extern void		nvt_ftg_idmap(FTG_FACE* faces, UINT32 face_number);

/**
     set roi in the frame.

     @param[in] roi: the roi rect.
     @param[in] width: width of frame.
	 @param[in] height: height of frame.

     @return void.

     Example:
     @code
     {
        URECT      roi={100, 100, 640, 480};
        UINT32     width = image.width;
        UINT32     height = image.height;

       nvt_ftg_roi_config(roi, width, height);
     }
     @endcode

*/
extern void		nvt_ftg_roi_config(URECT roi);
extern void		nvt_ftg_roi_getconfig(URECT *proi, URECT* targetcoord);

typedef struct _ftg_fd_face
{
    UINT16  x;               ///< result start x
    UINT16  y;               ///< result start y
    UINT16  w;               ///< result width
    UINT16  h;               ///< result height
    UINT8   trgidx;           ///< trigger index
} ftg_fd_face, *p_ftg_fd_face;

/**
     Get the result faces of different coordinate.

     This API will return the result faces by different coordinate.\n

     @param[out] Face: the result face info.
     @param[in] TargetCoord: the target window coordinate.

     @return The number of detected faces.

     Example:
     @code
     {
        ftg_fd_face    FdInfo[FD_NUM_MAX];
        URECT      FdDispCoord={0,0,640,480};
        UINT32     FaceNumber;

        FaceNumber = ftg_fd_getrsltface(FdInfo, &FdDispCoord);
     }
     @endcode
*/
extern UINT32	nvt_ftg_get_fd_result(odt_od_result *odt_face, URECT* targetcoord, UINT32 max_num);
extern UINT32	nvt_ftg_get_fd_result_after_match(odt_obj *obj, UINT32 process_id, URECT* targetcoord, UINT32 *p_loc);

/**
     Save one DIS motion vecter result.
*/
extern BOOL		nvt_ftg_getinitstate(void);
extern INT32	nvt_ftg_imgsequence_process(IMG_BUF* pinputimg, UINT32 edgeimgaddr);

extern VOID		ftg_enablepstore(BOOL enable);
extern INT32	ftg_savelandmarktopstore(MEM_RANGE *buf);

typedef struct
{
    UINT32 img_addr;		//in
    UINT32 width;			//in
    UINT32 height;			//in
    URECT  rect;			//in
    UINT16 luma;			//out
    UINT16 contrast;		//out
    UINT16 clarity;			//out

}CLARITY_PRMS;

extern VOID		nvt_ftg_clarity_process(CLARITY_PRMS *pparams);

#endif
