#ifndef _ALG_MOVIE_PHOTOFUNCINFO_H
#define _ALG_MOVIE_PHOTOFUNCINFO_H

#include "photo_task.h"
#include "md_lib.h"

typedef struct {
	UINT32 fps;
	UINT32 md_mode;
	UINT32 dist_region_nu;
	MD_USR_DISTRICT dist_region[MD_MAX_SUB_REGION_NUM];
	MD_USR_PARAM_INFO param_info;
	UINT32	reserved[6];
} MOVIE_MDParam;


#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
void Movie_BCLock(BOOL bLock);
void Movie_BCProcessRateSet(UINT32 ProcessRate);
UINT32 Movie_BCProcessRateGet(void);
#endif


extern PHOTO_FUNC_INFO PhotoFuncInfo_dis;
extern void Movie_FTGLock(BOOL bLock);
extern BOOL Movie_FTGGetLockSts(void);
extern UINT32 Movie_md_GetResult(void);
extern void alg_Movie_InitPhotoFunc(PHOTO_FUNC_INFO *phead);

#endif

