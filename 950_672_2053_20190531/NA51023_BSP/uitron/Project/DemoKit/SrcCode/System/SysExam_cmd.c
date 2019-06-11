//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysCfg.h"
#include "SxCmd.h"
#include "UIInfo.h"
#include "MsdcNvtCb_Exam.h"

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysExamCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (UI_FUNC == ENABLE)
static BOOL cmd_recsize(CHAR* strCmd)
{
	if (strncmp(strCmd,"MOVIE_SIZE_FRONT_3840x2160P30",strlen("MOVIE_SIZE_FRONT_3840x2160P30")) == 0) {
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_3840x2160P30);
	} else if (strncmp(strCmd,"MOVIE_SIZE_FRONT_2560x1440P60",strlen("MOVIE_SIZE_FRONT_2560x1440P60")) == 0) {
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_2560x1440P60);
	} else if (strncmp(strCmd,"MOVIE_SIZE_FRONT_1280x720P60",strlen("MOVIE_SIZE_FRONT_1280x720P60")) == 0) {
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_1280x720P60);
	} else {
		exam_msg("unknown size");
	}
	return TRUE;
}
#endif
SXCMD_BEGIN(autotest, "auto test command")
#if (UI_FUNC == ENABLE)
SXCMD_ITEM("recsize %", cmd_recsize, "set rec size")
#endif
SXCMD_END()
