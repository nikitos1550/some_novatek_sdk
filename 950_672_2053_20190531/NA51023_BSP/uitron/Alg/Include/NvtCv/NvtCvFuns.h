/**
    NVT CV library functions.

    Definitions of NVT CV library functions.

    @file       NvtCvFuns.h
    @ingroup    NvtCv
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef _NVTCVFUNS_H
#define _NVTCVFUNS_H

#include "Type.h"
#include "NvtIpcAPI.h"


#define NVTCV_FUN_BASE 10000

typedef enum
{
    // NOTE: DO NOT INSERT NEW CV FUNCTION ENUM INTO THE ENUM LIST.
    //       ADD NEW CV FUNCTION ENUM AT THE END. (BEFORE NVTCV_FUN_MAX)

    NVTCV_TEST = 0,
    NVTCV_LANDMARK,
    NVTCV_LK_TRACKING,
    NVTCV_BC,
    NVTCV_TSR,
    NVTCV_OT,
    NVTCV_DISDSP,

    //............................................
    // NvtCv_CevaCv functions

    NVTCV_SRAND,
    NVTCV_GENERATE_BRIEF_DESCRIPTOR,
    NVTCV_PATCH_MOMENT,
    NVTCV_FIND_HOMOGRAPHY,
    NVTCV_ESTIMATE_RIGID_TRANSFORM,
    NVTCV_THRESHOLD_OTSU,
    NVTCV_MATRIX_MULT,
    NVTCV_TRANSPOSE,
    NVTCV_HOUGH_LINE,

    NVTCV_CALC_CIRCLE_POINTS,
    NVTCV_HOUGH_TRANSFORM,

    NVTCV_INIT_KALMAN_FILTER_1D,
    NVTCV_INIT_KALMAN_FILTER_2D,
    NVTCV_KALMAN_FILTER_UPDATE_1D,
    NVTCV_KALMAN_FILTER_UPDATE_2D,

    //............................................
    // NvtCv functions

    NVTCV_INIT_ROI_FILTER = NVTCV_FUN_BASE,
    NVTCV_ROI_FILTER,
    NVTCV_ROTATE,
    NVTCV_CLARITY,
    NVTCV_HOG,
    NVTCV_SVM,
    NVTCV_RESIZE,
    NVTCV_WARPAFFINE,
    NVTCV_TEMPLATEMATCH,
    NVTCV_MATRIX_DET,
    NVTCV_MATRIX_INV,

    // ADD NEW CV FUNCTION ENUM HERE (BEFORE NVTCV_FUN_MAX)
    NVTCV_PD,
    //............................................
    NVTCV_FUN_MAX,
    ENUM_DUMMY4WORD(NVTCV_FUN)
} NVTCV_FUN;


////////////////////////////////////////////////////////////////////////////////
typedef struct _NVTCV_TESTFUN_PRMS
{
    UINT32 addrSrcImg;
    UINT32 addrDstImg;
    UINT32 width;
    UINT32 height;
} NVTCV_TESTFUN_PRMS;


////////////////////////////////////////////////////////////////////////////////
typedef struct _NVTCV_SRAND_PRMS
{
    UINT32 uiSeed;
} NVTCV_SRAND_PRMS;


////////////////////////////////////////////////////////////////////////////////
//Brief Descriptor types
#define NVTCV_DESCRIPTOR_BITS     256
#define NVTCV_DESCRIPTOR_SIZE     (NVTCV_DESCRIPTOR_BITS>>5)
typedef UINT32 NVTCV_ORB_bits[NVTCV_DESCRIPTOR_SIZE];
#define NVTCV_DESCRIPTOR_WINDOW_S 40        //patch descriptor stride
#define NVTCV_DESCRIPTOR_WINDOW_W 40        //patch descriptor width
#define NVTCV_DESCRIPTOR_WINDOW_H 40        //patch descriptor height
#define NVTCV_DESCRIPTOR_WINDOW_R 19        //patch descriptor radius

typedef struct
{
    INT16 x;    ///< x-coordinate
    INT16 y;    ///< y-coordinate
} S16Point;

typedef struct _NVTCV_GENERATE_BRIEF_DESCRIPTOR_PRMS
{
    UINT32 puiPatch;
    UINT32 pDesc;               ///< output
    UINT32 uiCountDesc;
     INT16 iOrientationIdx;
    UINT16 uiPatchStride;
    UINT16 uiOffX;
    UINT16 uiOffY;
} NVTCV_GENERATE_BRIEF_DESCRIPTOR_PRMS;


////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    NVTCV_PATCHMOMENT_17X17_U8,
    NVTCV_PATCHMOMENT_31X31_U8
} NVTCV_PATCHMOMENT_OPT;

typedef struct _NVTCV_PATCHMOMENT_PRMS
{
    UINT32 puiSrcCen;   ///< patch center
     INT32 iSrcStride;
    UINT32 piMx;        ///< output
    UINT32 piMy;        ///< output
    UINT8  uiOption;    ///< option in NVTCV_PATCHMOMENT_OPT
} NVTCV_PATCHMOMENT_PRMS;


////////////////////////////////////////////////////////////////////////////////
#define NVTCV_HOMOGRAPHY_NUM_MODEL_PRMS 9

typedef struct _NVTCV_FIND_HOMOGRAPHY_PRMS
{
    UINT32 piSrcX;
    UINT32 piSrcY;
    UINT32 piDstX;
    UINT32 piDstY;
    UINT32 uiNumPoints;
    UINT32 uiMethod;
     INT16 iReprojThr;
    UINT32 uiMaxIter;
    UINT32 puiOutMask;      ///< output
    UINT32 pfModel;         ///< output
    UINT32 uiNumInliers;    ///< output
} NVTCV_FIND_HOMOGRAPHY_PRMS;


////////////////////////////////////////////////////////////////////////////////
#define NVTCV_RIGIDTRANSFORM_NUM_MODEL_PRMS 6

typedef struct _NVTCV_ESTIMATE_RIGID_TRANSFORM_PRMS
{
    UINT32 piSrcX;
    UINT32 piSrcY;
    UINT32 piDstX;
    UINT32 piDstY;
    UINT32 uiNumPoints;
    UINT32 puiInliersMask;  ///< output
    UINT32 uiNumInliers;    ///< output
    UINT32 pfOutParams;     ///< output
     INT32 iMethod;
    UINT32 uiSuccessValue;  ///< output
} NVTCV_ESTIMATE_RIGID_TRANSFORM_PRMS;


////////////////////////////////////////////////////////////////////////////////
#define NVTCV_OTSU_BIN_NUM 256

typedef struct _NVTCV_THRSH_OTSU_PRMS
{
    UINT32 puiHist;         ///< starting address of input histogram (256 bins)
    UINT32 piOtsuThresh;    ///< output (deprecated)
     INT32 iOtsuThresh;     ///< output
} NVTCV_THRSH_OTSU_PRMS;


////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    NVTCV_MATRIXMULT_U8_S16,    ///< input: UINT8; output: INT16
    NVTCV_MATRIXMULT_U8_S32,    ///< input: UINT8; output: INT32
    NVTCV_MATRIXMULT_S16_S16,   ///< input: INT16; output: INT16
    NVTCV_MATRIXMULT_S16_S32    ///< input: INT16; output: INT32
} NVTCV_MATRIXMULT_OPT;

typedef struct _NVTCV_MATRIXMULT_PRMS
{
    UINT32 piSrc1;
    UINT32 piSrc2;
    UINT32 piDst;           ///< output
     INT32 iSrc1Stride;
     INT32 iSrc2Stride;
     INT32 iDstStride;
    UINT32 uiN;
    UINT32 uiM;
    UINT32 uiL;
     INT32 iRnd;            ///< only for NVTCV_MATRIXMULT_U8_S16, NVTCV_MATRIXMULT_S16_S16, NVTCV_MATRIXMULT_S16_S32
    UINT32 uiShiftRight;    ///< only for NVTCV_MATRIXMULT_U8_S16, NVTCV_MATRIXMULT_S16_S16, NVTCV_MATRIXMULT_S16_S32
    UINT8  uiOption;        ///< option in NVTCV_MATRIXMULT_OPT
} NVTCV_MATRIXMULT_PRMS;


////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    NVTCV_TRANSPOSE_NxM_S8,         ///< input/output: INT8
    NVTCV_TRANSPOSE_NxM_S16,        ///< input/output: INT16
    NVTCV_TRANSPOSE_4Nx16M_S8,      ///< input/output: INT8
    NVTCV_TRANSPOSE_4Nx16M_S16,     ///< input/output: INT16
    NVTCV_TRANSPOSE_4x4_S8,         ///< input/output: INT8
    NVTCV_TRANSPOSE_4x4_S16         ///< input/output: INT16
} NVTCV_TRANSPOSE_OPT;

typedef struct _NVTCV_TRANSPOSE_PRMS
{
    UINT32 piSrc;
    UINT32 piDst;           ///< output
    UINT32 uiSrcStride;
    UINT32 uiDstStride;
    UINT32 uiWidth;
    UINT32 uiHeight;
    UINT8  uiOption;        ///< option in NVTCV_TRANSPOSE_OPT
} NVTCV_TRANSPOSE_PRMS;


////////////////////////////////////////////////////////////////////////////////
typedef struct _NVTCV_HOUGH_LINE_PRMS
{
    UINT32 pPoints;
     INT32 iDr;
     INT32 iRMax;
     INT32 iSinTeta;
     INT32 iCosTeta;
    UINT32 puiDst;          ///< output
    UINT32 uiM;
} NVTCV_HOUGH_LINE_PRMS;


////////////////////////////////////////////////////////////////////////////////
#define NVTCV_HOUGHTRANSFORM_OFFSET_SIZE 720

typedef struct _NVTCV_CALC_CIRCLE_POINTS_PRMS
{
    UINT32 piCircle;        ///< output
     INT16 iRadiusMin;
     INT16 iRadiusMax;
     INT16 iStride;
    UINT32 piSinTeta720;
    UINT32 piCosTeta720;
    UINT16 uiFixedPrec;
    UINT16 uiFixedStep;
     INT32 iNumPoints;      ///< output
} NVTCV_CALC_CIRCLE_POINTS_PRMS;

typedef struct _NVTCV_HOUGH_TRANSFORM_PRMS
{
    UINT32 puiIn;
     INT16 iInStride;
    UINT32 puiOut;          ///< output
     INT16 iOutStride;
    UINT32 piOffsets;
     INT16 iNumOffsets;
     INT16 iGrayScaleThr;
    UINT32 uiN;
    UINT32 uiM;
} NVTCV_HOUGH_TRANSFORM_PRMS;


////////////////////////////////////////////////////////////////////////////////
typedef struct _NVTCV_KALMAN_FILTER_STATE_1D
{
    FLOAT fX;           ///< state x
    FLOAT fCX;          ///< x correction (output)
    FLOAT fP;           ///< error post-covariance
    FLOAT fQ;           ///< process noise variance
    FLOAT fR;           ///< measurement noise variance
    FLOAT fCXMin;       ///< correction constraint - lower bound
    FLOAT fCXMax;       ///< correction constraint - upper bound
    FLOAT fErrAcum;     ///< accumulate the error, due to the constraint

    FLOAT fReserve1;    ///< reserve
    FLOAT fReserve2;    ///< reserve
} NVTCV_KALMAN_FILTER_STATE_1D;     ///< 1-D Kalman filter state

typedef struct _NVTCV_KALMAN_FILTER_STATE_2D
{
    FLOAT fX;           ///< state - x(t)
    FLOAT fV;           ///< state - dx(t)/dt
    FLOAT fCX;          ///< x correction (output)
    FLOAT fP11;         ///< error post-covariance P(1,1)
    FLOAT fP12;         ///< error post-covariance P(1,2)
    FLOAT fP21;         ///< error post-covariance P(2,1)
    FLOAT fP22;         ///< error post-covariance P(2,2)

    FLOAT fQ;           ///< process noise variance - assuming that only dx(t)/dt is directly corrupted by additive noise
    FLOAT fR;           ///< measurement noise variance (only accumulated x(t) is measured)

    FLOAT fReserve1[3]; ///< reserve
    FLOAT fReserve2;    ///< reserve
    FLOAT fReserve3;    ///< reserve
} NVTCV_KALMAN_FILTER_STATE_2D;     ///< 2-D Kalman filter state

typedef struct _NVTCV_INIT_KALMAN_FILTER_1D_PRMS
{
    UINT32 pState;      ///< output
    FLOAT fX;
    FLOAT fP;
    FLOAT fQ;
    FLOAT fR;
    FLOAT fCXMin;
    FLOAT fCXMax;
    FLOAT fErrAcum;
} NVTCV_INIT_KALMAN_FILTER_1D_PRMS;

typedef struct _NVTCV_INIT_KALMAN_FILTER_2D_PRMS
{
    UINT32 pState;      ///< output
    FLOAT fX;
    FLOAT fV;
    FLOAT fP;
    FLOAT fQ;
    FLOAT fR;
    FLOAT fCXMin;
    FLOAT fCXMax;
    FLOAT fCVMin;
    FLOAT fCVMax;
} NVTCV_INIT_KALMAN_FILTER_2D_PRMS;

typedef struct _NVTCV_KALMAN_FILTER_UPDATE_1D_PRMS
{
    UINT32 pState;      ///< output
    FLOAT fZ;
} NVTCV_KALMAN_FILTER_UPDATE_1D_PRMS;

typedef struct _NVTCV_KALMAN_FILTER_UPDATE_2D_PRMS
{
    UINT32 pState;      ///< output
    FLOAT fZ;
} NVTCV_KALMAN_FILTER_UPDATE_2D_PRMS;


////////////////////////////////////////////////////////////////////////////////
typedef struct _NVTCV_INIT_ROI_FILTER_PRMS
{
    UINT32 uiNumLevels;         ///< number of filter strength levels
    UINT32 puiQuantTables;      ///< address of quantization tables
} NVTCV_INIT_ROI_FILTER_PRMS;

typedef struct _NVTCV_ROI_FILTER_PRMS
{
    UINT32 puiSrcY;             ///< Y plane of input source image
    UINT32 puiSrcUV;            ///< UV plane of input source image
    UINT32 puiDstY;             ///< Y plane of output destination image
    UINT32 puiDstUV;            ///< UV plane of output destination image
     INT32 iSrcYLineOffset;     ///< line offset (stride) of source image      (Y plane)
     INT32 iSrcUVLineOffset;    ///< line offset (stride) of source image      (UV plane)
     INT32 iDstYLineOffset;     ///< line offset (stride) of destination image (Y plane)
     INT32 iDstUVLineOffset;    ///< line offset (stride) of destination image (UV plane)
    UINT32 uiWidth;             ///< width of source image
    UINT32 uiHeight;            ///< height of source image
     INT32 iRoiX;               ///< x coordinate of ROI (Y plane)
     INT32 iRoiY;               ///< y coordinate of ROI (Y plane)
    UINT32 uiRoiWidth;          ///< width of ROI        (Y plane)
    UINT32 uiRoiHeight;         ///< height of ROI       (Y plane)
    UINT32 uiLevel;             ///< level of filter strength
    UINT8  uiDegree;            ///< rotation degrees in NVTCV_ROTATE_DEGREE
    UINT32 uiFormat;            ///< pixel format in IPL_YUV_IMG_TYPE
    UINT8  uiDrawRoi;           ///< whether drawing the ROI or not (0: No; 1: Yes)
} NVTCV_ROI_FILTER_PRMS;


////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    NVTCV_ROTATE_0,             ///< no rotation
    NVTCV_ROTATE_CW_90,         ///< 90 degree clockwise rotation
    NVTCV_ROTATE_CCW_90         ///< 90 degree counterclockwise rotation
} NVTCV_ROTATE_DEGREE;

typedef enum
{
    NVTCV_ROTATE_16Nx16M        ///< rows (N): multiple of 16; columns (M): multiple of 16
} NVTCV_ROTATE_OPT;

typedef struct _NVTCV_ROTATE_PRMS
{
    UINT32 puiSrcY;             ///< Y plane of input source image
    UINT32 puiSrcUV;            ///< UV plane of input source image
    UINT32 puiDstY;             ///< Y plane of output destination image
    UINT32 puiDstUV;            ///< UV plane of output destination image
     INT32 iSrcYLineOffset;     ///< line offset (stride) of source image      (Y plane)
     INT32 iSrcUVLineOffset;    ///< line offset (stride) of source image      (UV plane)
     INT32 iDstYLineOffset;     ///< line offset (stride) of destination image (Y plane)
     INT32 iDstUVLineOffset;    ///< line offset (stride) of destination image (UV plane)
    UINT32 uiWidth;             ///< width of source image
    UINT32 uiHeight;            ///< height of source image
    UINT8  uiDegree;            ///< rotation degrees in NVTCV_ROTATE_DEGREE
    UINT8  uiOption;            ///< option in NVTCV_ROTATE_OPT
} NVTCV_ROTATE_PRMS;


////////////////////////////////////////////////////////////////////////////////
#endif  //_NVTCVFUNS_H
