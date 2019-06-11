#include "SysCommon.h"
#include "alg_MovieExe.h"
#include "mp4log.h"
#include "UIInfo.h"

#if MOVIE_OT_FUNC
#include "NvtOt.h"
#endif

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
// Declare debug log sub-section ID here
// Ex: UINT32 DbgLogTestID = MP4LOG_NULL_ID;
#if (_ADAS_FUNC_ == ENABLE)
UINT32 DbgLogSnGID = MP4LOG_NULL_ID;
UINT32 DbgLogFCLDID = MP4LOG_NULL_ID;
UINT32 DbgLogAutoVPID = MP4LOG_NULL_ID;
#endif  // #if (_ADAS_FUNC_ == ENABLE)

//#NT#2016/11/08#CC Chao -begin
//#NT#Saving log for TSR
#if (_TSR_FUNC_ == ENABLE)
UINT32 DbgLogTsrID = MP4LOG_NULL_ID;
#endif
//#NT#2016/11/08

//#NT#2016/07/20#ML Cui -begin
//#NT#Saving log for DDD
#if (MOVIE_DDD_SAVE_LOG == ENABLE)
UINT32 DbgLogDDDID = MP4LOG_NULL_ID;
#endif
//#NT#2016/7/20

//#NT#2016/08/06#XQ Liu -begin
//#NT#Saving log for TD
#if (MOVIE_TD_SAVE_LOG == ENABLE)
UINT32 DbgLogTDID = MP4LOG_NULL_ID;
#endif
//#NT#2016/08/06#XQ Liu -end
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

//#NT#2016/09/29#KCHong -begin
//#NT#The GPS related variables should not depend on ADAS.
#if (GPS_FUNCTION == ENABLE)
FLOAT g_CurSpeed = 0.0;
BOOL g_GPSLinked = FALSE;
BOOL g_GPSStatus = FALSE;
#endif
//#NT#2016/09/29#KCHong -end

#if (_ADAS_FUNC_ == ENABLE)
static ADAS_APPS_RESULT_INFO g_AdasRltOSD = {0};
static ADAS_APPS_RESULT_INFO g_AdasRltVideo = {0};
#if (_SNG_FUNC_ == ENABLE)
static ADAS_SNG_ROI_INFO g_SnGROIOSD = {0};
static ADAS_SNG_ROI_INFO g_SnGROIVideo = {0};
#endif  // #if (_SNG_FUNC_ == ENABLE)

ADAS_APPS_RESULT_INFO *MovieExe_GetAdasRltOSD(void)
{
	return &g_AdasRltOSD;
}

ADAS_APPS_RESULT_INFO *MovieExe_GetAdasRltVideo(void)
{
	return &g_AdasRltVideo;
}

ADAS_SNG_ROI_INFO *MovieExe_GetSnGROIOSD(void)
{
	ADAS_SNG_ROI_INFO *ptr = NULL;
#if (_SNG_FUNC_ == ENABLE)
	ptr = &g_SnGROIOSD;
#endif  // #if (_SNG_FUNC_ == ENABLE)
	return ptr;
}

ADAS_SNG_ROI_INFO *MovieExe_GetSnGROIVideo(void)
{
	ADAS_SNG_ROI_INFO *ptr = NULL;
#if (_SNG_FUNC_ == ENABLE)
	ptr = &g_SnGROIVideo;
#endif  // #if (_SNG_FUNC_ == ENABLE)
	return ptr;
}
#endif  // #if (_ADAS_FUNC_ == ENABLE)

MOVIE_ALG_INFO gMovie_Alg_Info[] = {
	// _CFG_REC_ID_1
	{
		// path 3
		GX_IMAGE_PIXEL_FMT_YUV420_PACKED,       // format
#if _TSR_FUNC_|| MOVIE_OT_FUNC
		{1920, 1080},                           // image size
#else
		{848,  480},                           // image size
#endif
		{0, 0, 0, 0},                           // window size (N/A)
		FALSE,                                  // preview enable (N/A)
		// path 4
		GX_IMAGE_PIXEL_FMT_Y_ONLY,              // format
#if (_ADAS_FUNC_ == ENABLE)
		{1920, 1080},                           // image size
		{600, 300, 720, 480},                   // window size, x/y/w/h
#else
		{0, 0},                                 // image size
		{0, 0, 0, 0},                           // window size, x/y/w/h
#endif
		FALSE,                                  // preview enable (N/A)
		// REC_ID
		_CFG_REC_ID_1,                          // MOVIE_CFG_REC_ID
	},
	// _CFG_REC_ID_2
	{
		// path 3
		{0},
		// path 4
		{0},
		// REC_ID
		_CFG_REC_ID_2,
	},
};

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
void alg_mp4log_AddSection(void)
{
#if (_ADAS_FUNC_ == ENABLE)
	mp4log_AddSection("SNG ", 3, &DbgLogSnGID);
	mp4log_AddSection("FCLD",10, &DbgLogFCLDID);
	mp4log_AddSection("AUVP", 1, &DbgLogAutoVPID);
#endif  // #if (_ADAS_FUNC_ == ENABLE)

}
#endif



void alg_MovieExe_OnRecStart(void)
{
#if (_ADAS_FUNC_ == ENABLE)
	if ((UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_ON) || (UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_ON)) {
		memset(&g_AdasRltOSD, 0, sizeof(ADAS_APPS_RESULT_INFO));
		memset(&g_AdasRltVideo, 0, sizeof(ADAS_APPS_RESULT_INFO));
		g_AdasRltOSD.LdwsRsltInfo.Failure = LDWS_FAILURE_TRUE;
		g_AdasRltVideo.LdwsRsltInfo.Failure = LDWS_FAILURE_TRUE;
#if (_SNG_FUNC_ == ENABLE)
		memset(&g_SnGROIOSD, 0, sizeof(ADAS_SNG_ROI_INFO));
		memset(&g_SnGROIVideo, 0, sizeof(ADAS_SNG_ROI_INFO));
#endif  // #if (_SNG_FUNC_ == ENABLE)
		ADAS_AppsStart();
	}
#endif  // #if (_ADAS_FUNC_ == ENABLE)

#if MOVIE_OT_FUNC
	OT_START();
#endif //#if(MOVIE_OT_FUNC)

}

void alg_MovieExe_OnRecStop(void)
{
#if (_ADAS_FUNC_ == ENABLE)
	if (ADAS_GetAppsStatus()) {
		ADAS_AppsStop();
	}
#endif  // #if (_ADAS_DEV_ == ENABLE)

#if MOVIE_OT_FUNC
	OT_END();
#endif //#if(MOVIE_OT_FUNC)
}

INT32 alg_MovieExe_OnLDWS(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (_ADAS_FUNC_ == ENABLE)
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	if (uiSelect == MOVIE_LDWS_ON) {
		ADAS_AppsSetLdwsEanble(TRUE);
		ADAS_AppsSetStopDectEanble(TRUE);
		#if (_AUTOVP_FUNC_ == ENABLE)
		ADAS_AppsSetAutoVPEanble(TRUE);
		#endif  // #if (_AUTOVP_FUNC_ == ENABLE)
		#if (_SNG_FUNC_ == ENABLE)
		ADAS_AppsSetSnGEanble(TRUE);
		#endif  // #if (_SNG_FUNC_ == ENABLE)
	} else {
		if (SysGetFlag(FL_MOVIE_FCW) == MOVIE_FCW_ON) {
			// Do nothing since FC relies on LD
		} else {
			// Disable all ADAS function
			ADAS_AppsSetFcwsEanble(FALSE);
			ADAS_AppsSetLdwsEanble(FALSE);
			ADAS_AppsSetStopDectEanble(FALSE);
			#if (_AUTOVP_FUNC_ == ENABLE)
			ADAS_AppsSetAutoVPEanble(FALSE);
			#endif  // #if (_AUTOVP_FUNC_ == ENABLE)
			#if (_SNG_FUNC_ == ENABLE)
			ADAS_AppsSetSnGEanble(FALSE);
			#endif  // #if (_SNG_FUNC_ == ENABLE)
		}
	}
#endif  // #if (_ADAS_FUNC_ == ENABLE)
	return NVTEVT_CONSUME;
}

INT32 alg_MovieExe_OnFCW(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (_ADAS_FUNC_ == ENABLE)
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	if (uiSelect == MOVIE_FCW_ON) {
		ADAS_AppsSetFcwsEanble(TRUE);
		ADAS_AppsSetLdwsEanble(TRUE);
		ADAS_AppsSetStopDectEanble(TRUE);
		#if (_AUTOVP_FUNC_ == ENABLE)
		ADAS_AppsSetAutoVPEanble(TRUE);
		#endif  // #if (_AUTOVP_FUNC_ == ENABLE)
		#if (_SNG_FUNC_ == ENABLE)
		ADAS_AppsSetSnGEanble(TRUE);
		#endif  // #if (_SNG_FUNC_ == ENABLE)
	} else {
		if (SysGetFlag(FL_MOVIE_LDWS) == MOVIE_LDWS_ON) {
			// Only disalbe FC
			ADAS_AppsSetFcwsEanble(FALSE);
		} else {
			// Disable all ADAS function
			ADAS_AppsSetFcwsEanble(FALSE);
			ADAS_AppsSetLdwsEanble(FALSE);
			ADAS_AppsSetStopDectEanble(FALSE);
			#if (_AUTOVP_FUNC_ == ENABLE)
			ADAS_AppsSetAutoVPEanble(FALSE);
			#endif  // #if (_AUTOVP_FUNC_ == ENABLE)
			#if (_SNG_FUNC_ == ENABLE)
			ADAS_AppsSetSnGEanble(FALSE);
			#endif  // #if (_SNG_FUNC_ == ENABLE)
		}
	}
#endif  // #if (_ADAS_FUNC_ == ENABLE)
	return NVTEVT_CONSUME;
}


