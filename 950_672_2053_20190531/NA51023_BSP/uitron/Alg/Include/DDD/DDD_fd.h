#ifndef DDD_FD_H
#define DDD_FD_H

#include    <stdio.h>
#include    <string.h>
#include    "SysKer.h"
#include    "time.h"
#include    "GxImage.h"
#include    "Utility.h"
#include    "pll.h"
#include    "DxSys.h"
#include    "FileSysTsk.h"
#include    "ise_lib.h"
#include    "Type.h"
#include    "mIType.h"
#include    "fde_lib.h"

#ifdef FD_MODEL_NAME
#undef FD_MODEL_NAME
#endif
//Define your model name here
#define FD_MODEL_NAME DDDFd

/**
  User should define 'FD_MODEL_NAME' before
  Example:
  suppose user define 'FD_MODEL_NAME' as 'DDDFd'
  @code
  {
      //init
      FD_FUNC(DDDFd)->pfInit(buf, cachebuf, processrate);
      ...
      //process
      FD_FUNC(DDDFd)->pfProcess(input_img);
      FD_FUNC(DDDFd)->pfGetFDResult(input_img, rect);
      ...
  }
  @endcode
*/
//Don't modify them
#define GET_GLOBAL_NAME(name) g_##name
#define FD_EXTERN_STU(X) GET_GLOBAL_NAME(X)
#define FD_FUNC(name) (&FD_EXTERN_STU(name))
#define MAX_FD_RSLT 256

#ifndef FD_SORT_TYPE
typedef enum
{
	FD_SORT_BY_SIZE,
	FD_SORT_BY_SCORE,
	FD_SORT_BY_TOTAL_SCORE
} FD_SORT_TYPE;
#define FD_SORT_TYPE FD_SORT_TYPE
#endif
#ifndef FD_DET_TYPE
typedef enum _FD_DET_TYPE
{
	FD_DET_FDE_ONLY,
	FD_DET_FDE_WITH_NMS,
	FD_DET_WIHOUT_NMS,
	FD_DET_FRONT_ONLY,
	FD_DET_NORM,
} FD_DET_TYPE;
#define FD_DET_TYPE FD_DET_TYPE
#endif
#ifndef FD_PARAM_SETING_FIELDS
#define FD_PARAM_SETING_FIELDS            \
	X(FD_PARAM_MAX_SIZE, INT32, -1) \
	X(FD_PARAM_MIN_SIZE, INT32, 80) \
	X(FD_PARAM_MAX_RSLT, UINT32, 32) \
	X(FD_PARAM_DET_TYPE, FD_DET_TYPE, FD_DET_NORM) \
	X(FD_PARAM_FACTOR, UINT32, 6554) \
	X(FD_PARAM_SCL_NUM, UINT32, 25) \
	X(FD_PARAM_TOTAL_THRE, UINT32, 1) \
	X(FD_PARAM_USE_HISTORY, BOOL, FALSE) \
    X(FD_PARAM_ADJ_CONTRAST, BOOL, FALSE)

typedef enum _FD_PARAM
{
#define X(param, type, def_val) param,
	FD_PARAM_SETING_FIELDS
#undef X
} FD_PARAM;
#endif

#ifndef FD_FUNC_SET
typedef struct _FD_FUNC_SET
{
	VOID(*pfInit)(MEM_RANGE *buf, MEM_RANGE *cachebuf);
    UINT32 (*pfCalcCacheBuffSize)(VOID);
    UINT32 (*pfCalcBuffSize)(VOID);
	BOOL(*pfSetParam)(FD_PARAM type, INT32 value);
	INT32(*pfGetParam)(FD_PARAM type);
	INT32(*pfProcess)(IMG_BUF *input_img);
	INT32(*pfGetFDResult)(FACE *rect, UINT32 uiMaxLen, FD_SORT_TYPE type);
}FD_FUNC_SET;
#define FD_FUNC_SET FD_FUNC_SET
#endif

extern FD_FUNC_SET FD_EXTERN_STU(FD_MODEL_NAME);




#endif // DDD_FD_H
