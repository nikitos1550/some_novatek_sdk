#ifndef __FEAT_LIB_H__
#define __FEAT_LIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <Type.h>
#include <Debug.h>
#include "FileSysTsk.h"
#include "GxImage.h"
#include "NvtCv_lib.h"

#define DSP_RSLT_MEM_EPS	256

//for hog-----------------------------------------
typedef enum
{
	HOG_CELL_SIZE4 = 4,
	HOG_CELL_SIZE8 = 8,
	HOG_CELL_SIZE16 = 16,
	ENUM_DUMMY4WORD(HOG_CELL_SIZE)
}HOG_CELL_SIZE;

typedef enum
{
	HOG_BIN_SIZE4 = 4,
	HOG_BIN_SIZE5,
	HOG_BIN_SIZE6,
	HOG_BIN_SIZE7,
	HOG_BIN_SIZE8,
	HOG_BIN_SIZE9,
	ENUM_DUMMY4WORD(HOG_BIN_SIZE)
}HOG_BIN_SIZE;

typedef struct
{
	HOG_CELL_SIZE cellsize;
	UINT32 celloffset;
	HOG_BIN_SIZE binsize;
	UINT32 isfhog;
}HOG_INIT_PRMS;

typedef struct
{
	UINT8 *img;
	URECT *roi;
	UINT16 *rslt;
	UINT32 height;
	UINT32 width;
	UINT32 lineoffset;
	UINT32 roinum;
	UINT32 normth;
}HOG_PROC_PRMS;

//for resize-------------------------------------------
typedef struct
{
	UINT8 *imgin;
	UINT8 *imgout;
	UINT32 lineoffsetin;
	UINT32 heightin;
	UINT32 widthin;
	UINT32 heightout;
	UINT32 widthout;
}RESIZE_PROC_PRMS;

//for svm--------------------------------------------
typedef enum
{
	SVM_LINEAR_KERNEL,
	SVM_POLY_KERNEL,
	SVM_INTER_KERNEL,
	SVM_RBF_KERNEL,
	SVM_SIGMOID_KERNEL,
	ENUM_DUMMY4WORD(SVM_KERNEL)
}SVM_KERNEL;

typedef struct
{
    INT16 *feat;
	UINT32 *nsv;
    INT16 *svs;
	INT16 *coef;
	INT32 *rhos;
	INT32 *rslt;
	UINT32 flen;
	UINT32 nfeat;
	UINT32 nclass;
	SVM_KERNEL kernel;
	UINT32 command;
    INT32 gamma;
	INT32 coef0;
	INT32 degree;	
}SVM_PROC_PRMS;

//for warpaffine------------------------------------------
typedef struct
{
	UINT8 *imgin;
	UINT8 *imgout;
	INT32 *map;
	UINT32 lineoffsetin;
	UINT32 heightin;
	UINT32 widthin;
	UINT32 canvas;
}WARPAFFINE_PROC_PRMS;

//for templatematch---------------------------------------
typedef enum
{
	TM_SQDIFF,
	TM_SQDIFF_NORMED,
	TM_CCORR,
	TM_CCORR_NORMED,
	TM_CCOEFF,
	TM_CCOEFF_NORMED,
	ENUM_DUMMY4WORD(TM_METHOD)
}TM_METHOD;

typedef struct
{
	UINT8 *img;
	UINT8 *tmpl;
	INT32 *rslt;
	UPOINT *bestloc;
	UINT32 lineoffset;
	UINT32 height;
	UINT32 width;
	UINT32 tmplofs;
	UINT32 tmplh;
	UINT32 tmplw;
	UINT32 rsltofs;
	TM_METHOD method;
}TEMPLATEMATCH_PROC_PRMS;


//for matrix(det, inv...)
typedef struct
{
	FLOAT *data;
	FLOAT *rslt;
	UINT32 n;
}MATRIX_DET_PROC_PRMS;

typedef struct
{
	FLOAT *data;
	FLOAT *rslt;
	UINT32 n;
}MATRIX_INV_PROC_PRMS;


UINT32 nvtcv_feat_calcbufsize(VOID);

UINT32 hog_getbufsize(UINT32 imgh, UINT32 imgw);
VOID hog_init(HOG_INIT_PRMS *prms, UINT32 buf);
VOID hog_process(HOG_PROC_PRMS *prms);

VOID resize_init(UINT32 buf);
VOID resize_process(RESIZE_PROC_PRMS *prms);

VOID svm_init(UINT32 buf);
VOID svm_process(SVM_PROC_PRMS *prms);

VOID warpaffine_getmap(INT32 *map, FLOAT degree, FLOAT scale, UINT32 height, UINT32 width);
USIZE warpaffine_getdstsize(INT32 *map, UINT32 height, UINT32 width, UINT32 canvas);
VOID warpaffine_init(UINT32 buf);
VOID warpaffine_process(WARPAFFINE_PROC_PRMS *prms);

VOID templatematch_init(UINT32 buf);
VOID templatematch_process(TEMPLATEMATCH_PROC_PRMS *prms);

VOID matrix_det_init(UINT32 buf);
VOID matrix_det_process(MATRIX_DET_PROC_PRMS *prms);

VOID matrix_inv_init(UINT32 buf);
VOID matrix_inv_process(MATRIX_INV_PROC_PRMS *prms);


#endif

