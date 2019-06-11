/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2015.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: DIS Library Module                                                  *
* Description:                                                             *
* Author: Connie Yu                                                         *
****************************************************************************/

/**
    DIS lib

    Sample module detailed description.

    @file       dis_alg_fw.h
    @ingroup    mILibDIS
    @note       DIS FW process.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

/** \addtogroup DISLib */
//@{

#ifndef _DIS_ALG_FW_H_
#define _DIS_ALG_FW_H_

#include "dis_lib.h"


//#NT#2018/09/07# -begin
//#NT#Support time stamp
#define	DIS_TSTAMP_BUF_SIZE		5
#define	DIS_MV_BUF_SIZE			5

typedef struct _DIS_TIME_STAMP {
	UINT32 frame_count;
	UINT64 time_stamp;
} DIS_TIME_STAMP;

typedef struct _DIS_MOV_VEC {
	BOOL   mv_ready;
	UINT32 frame_count;
	MOTION_INFOR mv[MVNUMMAX];
} DIS_MOV_VEC;
//#NT#2018/09/07# -end


#define DISLIB_VECTOR_NORM      12      ///< output vector norm factor(2^12)
#define DIS_ABS(x)                  ((x)>=0?(x):(-(x)))

/**
    DIS Motion Vector.

    Structure of directional motion vector elements.
*/
//@{
typedef struct _DISALG_MOTION_VECTOR
{
    INT32 iX; ///< x component
    INT32 iY; ///< y component
} DISALG_MOTION_VECTOR;
//@}

/**
    DIS ALG Entire Configuration

    Structure of DIS ALG parameters when one wants to configure this module.
*/
//@{
typedef struct _DISALG_PARAM_STRUCT
{
    UINT32 InSizeH;                     ///< input horizontal pixels
    UINT32 InSizeV;                     ///< input vertical pixels
    UINT32 InLineOfs;                   ///< input line offset
    UINT32 InAdd0;                      ///< input starting address 0
    UINT32 InAdd1;                      ///< input starting address 1
    UINT32 InAdd2;                      ///< input starting address 2
    UINT32 frame_cnt;                   ///< frame count
} DISALG_PARAM;
//@}

/**
    DIS ALG result Vector information

    Structure of DIS ALG result vector information.
*/
//@{
typedef struct _DISALG_VECTOR_STRUCT
{
    DISALG_MOTION_VECTOR vector;///< result vector
    UINT32 frame_cnt;           ///< frame count
    UINT32 score;               ///< 1: useable, 0: not sure
} DISALG_VECTOR;
//@}

/**
    DIS Global Motion Information

    Structure of DIS module parameters when calculating global motion.
*/
//@{
typedef struct DIS_GM_STRUCT
{
    MOTION_VECTOR     *pImv;    ///< integral motion vector
    COMPENSATION_INFO *pComp;   ///< compensation information
    DIS_MVSCORE_LEVEL ScoreLv;  ///< Level of MV score screening
    DIS_STICKY_LEVEL  StickyLv; ///< Stickiness level of compensation
    MOTION_INFOR *pMV;          ///< Motion verctors
} DIS_GM;
//@}



/**
    DIS Global Motion Vector in ROI

    Structure of input coordinate and size.
*/
//@{
typedef struct _DIS_ROIGMV_IN_STRUCT
{
    UINT8 uiStartX;              ///< input x coordinate, 0~100%
    UINT8 uiStartY;              ///< input y coordinate, 0~100%
    UINT8 uiSizeX;               ///< input width, 0~100%
    UINT8 uiSizeY;               ///< input height, 0~100%
} DIS_ROIGMV_IN;
//@}

/**
    DIS Global Motion Vector in ROI

    Structure of output global motion vector.
*/
//@{
typedef struct _DIS_ROIGMV_OUT_STRUCT
{
    DISALG_MOTION_VECTOR vector; ///< output result vector
    BOOL  bValidVec;             ///< if the result vector is valid
} DIS_ROIGMV_OUT;
//@}


extern void disFw_initialize(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void disFw_process(MEM_RANGE *buf, MEM_RANGE *cachebuf, MOTION_INFOR *pMV);
extern void disFw_end(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void disFw_setDisInfor(DISALG_PARAM *disInfo);
extern UINT32 disFw_GetPrvMaxBuffer(void);
extern void disFw_accumUpdateProcess(MOTION_INFOR *pMV);
extern UINT16 dis_getDisViewRatio(void);
extern void dis_setDisViewRatio(UINT16 ratio);
extern ER dis_getFrameCorrVec(DISALG_VECTOR *Corr, UINT32 frame_cnt);
extern ER dis_getFrameCorrVecPxl(DISALG_VECTOR *Corr, UINT32 frame_cnt);
extern VOID dis_getOisDetMotVec(DIS_SGM *pIV);
extern UINT32 dis_accessOisOnOff(DIS_ACCESS_TYPE AccType, UINT32 uiIsOisOn);
extern UINT32 dis_setOisDetMotVec(INT32 iX, INT32 iY);
extern DIS_ROIGMV_OUT disFw_getRoiMotVec(DIS_ROIGMV_IN *pRoiIn, MOTION_INFOR *MV);

//#NT#2018/09/07# -begin
//#NT#Support time stamp
extern void dis_dump_tstamp_buf(void);
//#NT#2018/09/07# -end


#endif

