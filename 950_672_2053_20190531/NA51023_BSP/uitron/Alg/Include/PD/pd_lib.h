#ifndef _PD_LIB_H_
#define _PD_LIB_H_

#include "stdio.h"
#include "Type.h"
#include "GxImage.h"

typedef enum
{
	PD_MD_ALARM_TH,
	PD_NMS_RATIO,
	PD_COVER_REMOVING_RATIO,
	PD_CLASSIFY_METHOD,
	PD_SPEEDUP_MD,
	PD_SCORE_BIAS,
	PD_SCORE_BIAS_NIGHT,
	PD_REFINE_MD,
	PD_NIGHT_VISION,
	PD_STABLE_COUNT_TH,
	PD_ROI_RANGE_X,
	PD_ROI_RANGE_Y,
	PD_ROI_RANGE_W,
	PD_ROI_RANGE_H,
    ENUM_DUMMY4WORD(PD_USR_PRMS)
}PD_USR_PRMS;


VOID 	pd_init(UINT32 buf, UINT32 cachebuf);
VOID 	pd_setimg(IMG_BUF *img);
VOID 	pd_setfg(IMG_BUF *fg);
VOID 	pd_process(VOID);
UINT32 	pd_calccachebuffsize(VOID);
UINT32 	pd_calcbuffsize(VOID);
VOID 	pd_setprms(PD_USR_PRMS name, INT32 val);
UINT32 	pd_getprms(PD_USR_PRMS name);
UINT32 	pd_gettarget(URECT* target, URECT* targetcoord, UINT32 maxnum, UINT32 *alarm_sig);

#endif