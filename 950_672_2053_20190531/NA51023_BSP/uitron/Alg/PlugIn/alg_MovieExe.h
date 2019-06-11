#ifndef _ALG_MOVIEEXE_H
#define _ALG_MOVIEEXE_H

#include "ImageApp_MovieCommon.h"

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
// Extern debug log sub-section ID here
// Ex: extern UINT32 DbgLogTestID;
#if (_ADAS_FUNC_ == ENABLE)
extern UINT32 DbgLogSnGID;
extern UINT32 DbgLogFCLDID;
extern UINT32 DbgLogAutoVPID;
#endif
#if (_TSR_FUNC_ == ENABLE)
extern UINT32 DbgLogTsrID;
#endif
#if (MOVIE_DDD_SAVE_LOG == ENABLE)
extern UINT32 DbgLogDDDID;
#endif
#if (MOVIE_TD_SAVE_LOG == ENABLE)
extern UINT32 DbgLogTDID;
#endif
#endif

#if (_ADAS_FUNC_ == ENABLE)
#include "adas_Apps.h"

extern ADAS_APPS_RESULT_INFO *MovieExe_GetAdasRltOSD(void);
extern ADAS_APPS_RESULT_INFO *MovieExe_GetAdasRltVideo(void);
extern ADAS_SNG_ROI_INFO *MovieExe_GetSnGROIOSD(void);
extern ADAS_SNG_ROI_INFO *MovieExe_GetSnGROIVideo(void);
#endif  // #if (_ADAS_FUNC_ == ENABLE)

extern MOVIE_ALG_INFO gMovie_Alg_Info[];
extern void alg_mp4log_AddSection(void);
extern void alg_MovieExe_OnRecStart(void);
extern void alg_MovieExe_OnRecStop(void);
extern INT32 alg_MovieExe_OnLDWS(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 alg_MovieExe_OnFCW(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
#endif
