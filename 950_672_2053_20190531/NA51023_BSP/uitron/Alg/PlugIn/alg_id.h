#ifndef _ALG_ID_H
#define _ALG_ID_H
#include "Type.h"
//#include "ImageApp_Movie.h"
#include "ImageApp_MovieCommon.h"


extern void alg_InstallID(void) _SECTION(".kercfg_text");
extern UINT32 _SECTION(".kercfg_data") AlgPullIsfDataSemiID[MOVIE_ALG_IPL_MAX];

#endif