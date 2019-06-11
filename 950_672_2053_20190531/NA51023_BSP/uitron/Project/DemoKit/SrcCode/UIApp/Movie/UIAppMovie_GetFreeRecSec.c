////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppMovie.h"
#include "UIMovieMapping.h"
#include "FileSysTsk.h"
//#include "ImageApp_Movie.h"
#include "ImageApp_MovieMulti.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovie
#define __DBGLVL__          6 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
extern BOOL     gb3DNROut;
#endif
//#NT#2016/09/20#Bob Huang -end

UINT32 MovieExe_GetMaxRecSec(void)
{
	UINT32 sec = 0;
	sec = Movie_GetFreeSec();
	DBG_DUMP("%s(): tmp max sec %d\r\n",__func__, sec);

	exam_msg("movie preview\r\n"); // AUTO_TEST

	return sec ;
}

UINT32 MovieExe_GetFreeRecSec(void)
{
	UINT32 sec = 0;

	sec = Movie_GetFreeSec();
	DBG_IND("tmp free sec %d\r\n", sec);
	return sec ;
}

