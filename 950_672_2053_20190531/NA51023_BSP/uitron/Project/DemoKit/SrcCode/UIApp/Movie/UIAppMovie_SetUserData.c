////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppMovie.h"
#include "MovieUdtaVendor.h"
#if _TODO
#include "IQS_Utility.h"
#endif
//#include "ImageApp_Movie.h"
#include "ImageApp_MovieMulti.h"
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovie
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern GPSDATA    gpsdata;

void Movie_SetUserData(void)
{
	MEM_RANGE udata_buf = {0};

	if (MovieUdta_MakeVendorUserData(&(udata_buf.Addr), &(udata_buf.Size))) {
		ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_USER_DATA, (UINT32)&udata_buf);
	}

	MEM_RANGE log_buf;
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	log_buf.Size = mp4log_GetTotalLogSize() * 1024;
	log_buf.Size = log_buf.Size ? (log_buf.Size - 16) : (1024 - 16);
	if (mp4log_ToggleBuf(&(log_buf.Addr), NULL) == E_OK) {
		// since all file share same gps data, only config once
		ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_GPS_DATA, (UINT32)&log_buf);
	} else {
		DBG_ERR("No GPS buffer!!!");
	}
#else   // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	log_buf.Addr = &gpsdata;
	log_buf.Size = sizeof(GPSDATA);
	// since all file share same gps data, only config once
	ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_GPS_DATA, (UINT32)&log_buf);
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}

