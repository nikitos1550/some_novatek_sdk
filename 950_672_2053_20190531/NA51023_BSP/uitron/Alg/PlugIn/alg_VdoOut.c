#include "SysCommon.h"
#if IPCAM_FD_FUNC
#include "fd_lib.h"
#endif

#if IPCAM_BC_FUNC
#include "bc_lib.h"
#endif

#if IPCAM_FTG_DRAW_VIDEO
#include "ftg_lib.h"
#endif

//#NT#2017/12/26#Yuzhe Bian -begin
//#NT# Face Tracking & Grading
#if ((IPCAM_FD_DRAW_VIDEO == ENABLE)  || (IPCAM_BC_DRAW_VIDEO == ENABLE) || (IPCAM_FTG_DRAW_VIDEO == ENABLE))
static void VdoOut_DrawRect(PIMG_BUF pDstImg, IRECT  *DrawRect, UINT32 drawColor, UINT32 border)
{
	IRECT  LineRect;

	if (border < 2) {
		border = 2;
	}

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y;
	LineRect.w = DrawRect->w;
	LineRect.h = border;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y + DrawRect->h - border;
	LineRect.w = DrawRect->w;
	LineRect.h = border;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y;
	LineRect.w = border;
	LineRect.h = DrawRect->h;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x + DrawRect->w - border;
	LineRect.y = DrawRect->y;
	LineRect.w = border;
	LineRect.h = DrawRect->h;
	GxImg_FillData(pDstImg, &LineRect, drawColor);
}
#endif
//#NT#2017/12/26#Yuzhe Bian -end

#if (IPCAM_FD_DRAW_VIDEO == ENABLE)
static void VdoOut_DrawFD(PIMG_BUF pDstImg){


	UINT32            Fdnumber, i;
	static FD_FACE    FdInfo[MAX_FDNUM];
	IRECT             DrawRect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawColor = COLOR_YUV_WHITE;
	URECT             DispCord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	Fdnumber = FD_GetRsltFace(FdInfo, &DispCord);
	if (Fdnumber > MAX_FDNUM) {
		Fdnumber = MAX_FDNUM;
	}
	if (Fdnumber) {
		for (i = 0; i < Fdnumber; i++) {
			DrawRect.x = FdInfo[i].x;
			DrawRect.y = FdInfo[i].y;
			DrawRect.w = FdInfo[i].w;
			DrawRect.h = FdInfo[i].h;
			//MovieStamp_DrawRectUV(&DrawRect, drawColor, pDstImg);
			VdoOut_DrawRect(pDstImg, &DrawRect, drawColor, border);
		}
	}

}
#endif

//#NT#2017/12/26#Yuzhe Bian -begin
//#NT# Face Tracking & Grading
#if IPCAM_FTG_DRAW_VIDEO
static void VdoOut_DrawFTG(PIMG_BUF pDstImg)
{
    static FTG_TIMESTAMP time_stamp = {0};
	static FTG_FACE	faces[MAX_FTGNUM];
    UINT32			faceNumber, i;
	IRECT           DrawRect;
	UINT32          border = 4; // border can not smaller than 2
	UINT32          drawColor = COLOR_YUV_WHITE;
	URECT           DispCord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	faceNumber = nvt_ftg_get_result(faces, &DispCord, MAX_FTGNUM, (UINT32)&time_stamp, NVT_FTG_RSLT_LOCATION);
	if (faceNumber > MAX_FTGNUM) {
		faceNumber = MAX_FTGNUM;
	}
	if (faceNumber) {
		for (i = 0; i < faceNumber; i++) {
			DrawRect.x = faces[i].x;
			DrawRect.y = faces[i].y;
			DrawRect.w = faces[i].w;
			DrawRect.h = faces[i].h;
			VdoOut_DrawRect(pDstImg, &DrawRect, drawColor, border);
		}
	}

}
#endif
//#NT#2017/12/26#Yuzhe Bian -end

#if (IPCAM_BC_DRAW_VIDEO == ENABLE)
static void VdoOut_DrawBC(PIMG_BUF pDstImg){

	static URECT	  target[1024];
	UINT32            i, number, alarm;
	IRECT             drawrect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawcolor = COLOR_YUV_WHITE;
	URECT             dispcord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	dispcord.x = 0;
	dispcord.y = 0;
	dispcord.w = pDstImg->Width;
	dispcord.h = pDstImg->Height;
	number = bc_gettarget(target, &dispcord, 256);
	//number = bc_getblktarget(target, &dispcord, 1024);
	//number = 0;
	alarm = bc_gettdprms(BC_TD_STATUS);
	if(alarm > 0)
	{
		drawcolor = COLOR_YUV_RED;
	}

	if (number) {
		for (i = 0; i < number; i++) {
			drawrect.x = (INT32)target[i].x;
			drawrect.y = (INT32)target[i].y;
			drawrect.w = (INT32)target[i].w;
			drawrect.h = (INT32)target[i].h;
			VdoOut_DrawRect(pDstImg, &drawrect, drawcolor, border);
		}
	}

}
#endif

void alg_VdoOut_Draw(PIMG_BUF pDstImg)
{
	//#NT#2017/11/21#Charlie Chang -begin
    //#NT# draw FD info
    #if (IPCAM_FD_DRAW_VIDEO == ENABLE)

		VdoOut_DrawFD(pDstImg);

	#endif
	//#NT#2017/11/21#Charlie Chang -end

	#if (IPCAM_BC_DRAW_VIDEO == ENABLE)
		VdoOut_DrawBC(pDstImg);
	#endif

	//#NT#2017/12/26#Yuzhe Bian -begin
	//#NT# Face Tracking & Grading
	#if (IPCAM_FTG_DRAW_VIDEO == ENABLE)

		VdoOut_DrawFTG(pDstImg);

	#endif
	//#NT#2017/12/26#Yuzhe Bian -end
}
