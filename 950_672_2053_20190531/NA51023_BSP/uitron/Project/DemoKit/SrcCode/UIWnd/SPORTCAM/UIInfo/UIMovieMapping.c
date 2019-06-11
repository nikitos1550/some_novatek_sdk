//#include "ImageApp_Movie.h"
#include "UIAppMoviePlay.h"
#include "UIMovieInfo.h"
#include "UIPhotoInfo.h" //for IMAGERATIO_XXX
#include "Audio.h"
#include "SysMain.h"
#include "UIMovieMapping.h"
#include "UIFlow.h"
#include "media_def.h"
#include "ImageApp_MovieCommon.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIMovieMapping
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

UINT32 GetWiFiIndex(UINT32 singleIndex);

typedef struct {
	UINT32  uiWidth;
	UINT32  uiHeight;
	UINT32  uiVidFrameRate;
	UINT32  uiTargetBitrate;
	UINT32  uiDispAspectRatio;
	UINT32  uiImageRatio;
} MOVIE_SIZE_PARAM;

typedef struct {
	INT32  aq_enable;
	UINT32 aq_str;
	INT32  sraq_init_aslog2;
	INT32  max_sraq;
	INT32  min_sraq;
	INT32  sraq_const_aslog2;
} MOVIE_AQ_PARAM;

typedef struct {
	UINT32 uiEnable;
	UINT32 uiStaticTime;
	UINT32 uiFrameRate;
	UINT32 uiByteRate;
	UINT32 uiGOP;
	UINT32 uiInitIQp;
	UINT32 uiMinIQp;
	UINT32 uiMaxIQp;
	UINT32 uiInitPQp;
	UINT32 uiMinPQp;
	UINT32 uiMaxPQp;
	INT32  iIPWeight;
	UINT32 uiRowRcEnalbe;
	UINT32 uiRowRcQpRange;
	UINT32 uiRowRcQpStep;
} MOVIE_CBR_PARAM;

typedef struct {
	MOVIE_REC_TYPE Type;
	MOVIE_SIZE_PARAM Front;
	MOVIE_SIZE_PARAM Behind;
	MOVIE_AQ_PARAM AqFrontInfo;
	MOVIE_AQ_PARAM AqBehindInfo;
	MOVIE_CBR_PARAM CbrFrontInfo;
	MOVIE_CBR_PARAM CbrBehindInfo;
} MOVIE_REC_ITEM;

//-------------------------------------------------------------------------------------------------
#if (_BOARD_DRAM_SIZE_ == 0x04000000)
 #define MOVIE_SIZE_WIFI_STREAMING	MOVIE_SIZE_848x480P30_WIFI	// always use 848x480 for WiFi streaming temporarily
#else
#define MOVIE_SIZE_WIFI_STREAMING	MOVIE_SIZE_848x480P30	// always use 848x480 for WiFi streaming temporarily
 #endif

typedef struct {
	MOVIE_SIZE_PARAM SizeInfo;
	MOVIE_AQ_PARAM AqInfo;
	MOVIE_CBR_PARAM CbrInfo;
} MOVIE_SIZE_ITEM;

typedef struct {
	MOVIE_REC_TYPE Type;
	UINT32 SizeIdx[SENSOR_MAX_NUM];			// movie size index for main movie, max 4 sensors
	UINT32 SizeIdxClone[SENSOR_MAX_NUM];	// movie size index for clone movie, max 4 sensors
} MOVIE_SIZE_MAPPING;

// movie size table
static MOVIE_SIZE_ITEM g_MovieSizeTable[] = {

	[MOVIE_SIZE_2880x2160P50] = {
		{2880, 2160, 50, 3800 * 1024, MEDIAREC_DAR_16_9, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 50, 4900 * 1024, 25, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_3840x2160P30] = {
		{3840, 2160, 30, 3900 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 3900 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_2880x2160P24] = {
		{2880, 2160, 24, 2350 * 1024, MEDIAREC_DAR_16_9, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 24, 2350 * 1024, 12, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_2704x2032P60] = {
		{2704, 2032, 60, 3800 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_4_3},
		{1, 2, 36, 6, -6,	0},
		{1, 4, 60, 5200 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_2560x1440P80] = {
		{2560, 1440, 80, 3800 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6,	0},
		{1, 4, 80, 4650 * 1024, 20, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_2560x1440P60] = {
		{2560, 1440, 60, 3500 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6,	0},
		{1, 4, 60, 3500 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_2560x1440P30] = {
		{2560, 1440, 30, 1750 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 1750 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_2304x1296P30] = {
		{2304, 1296, 30, 1400 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 1400 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_1920x1080P120] = {
		{1920, 1080, 120, 3900 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 120, 3900 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_1920x1080P96] = {
		{1920, 1080, 96, 3150 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 96, 3150 * 1024, 12, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_1920x1080P60] = {
		{1920, 1080, 60, 1950 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 60, 1950 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_1920x1080P30] = {
		{1920, 1080, 30, 1024 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 1024 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_1280x720P240] = {
		{1280, 720, 240, 3500 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 240, 3500 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_1280x720P120] = {
		{1280, 720, 120, 1750 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 120, 1750 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_1280x720P60] = {
		{1280, 720, 60, 850 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 60,	850 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_1280x720P30] = {
		{1280, 720, 30, 450 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 450 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_848x480P30] = {
		{848, 480, 30, 200 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 200 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_848x480P30_WIFI] = {
		{848, 480, 30, 100 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 100 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_640x480P240] = {
		{ 640, 480, 240, 1150 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_4_3},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 240, 1150 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_640x480P30] = {
		{ 640, 480, 30, 150 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_4_3},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 150 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_320x240P30] = {
		{ 320, 240, 30, 50 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_4_3},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 50 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_640x360P30] = {
		{ 640, 360, 30, 150 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_16_9},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 150 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_2048x2048P30] = {
		{2048, 2048, 30, 2000 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_1_1},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 2000 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

	[MOVIE_SIZE_480x480P30] = {
		{ 480, 480, 30, 100 * 1024, MEDIAREC_DAR_DEFAULT, IMAGERATIO_1_1},
		{1, 2, 36, 6, -6, 0},
		{1, 4, 30, 100 * 1024, 15, 26, 10, 40, 26, 10, 40, 0, 1, 8, 4},
	},

};

// movie recording table, mapping to movie menu
static MOVIE_SIZE_MAPPING g_MovieRecMappingTable[MOVIE_SIZE_ID_MAX] = {

	[MOVIE_SIZE_FRONT_2880x2160P50] = {
		MOVIE_REC_TYPE_FRONT, 				// TYPE_FRONT means single recording
		{MOVIE_SIZE_2880x2160P50, 0 },	// main movie
		{0, 0}, 						// no clone movie
	},

	[MOVIE_SIZE_FRONT_3840x2160P30] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_3840x2160P30, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_2880x2160P24] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_2880x2160P24, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_2704x2032P60] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_2704x2032P60, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_2560x1440P80] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_2560x1440P80, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_2560x1440P60] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_2560x1440P60, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_2560x1440P30] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_2560x1440P30, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_2304x1296P30] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_2304x1296P30, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_1920x1080P120] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_1920x1080P120, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_1920x1080P96] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_1920x1080P96, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_1920x1080P60] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_1920x1080P60, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_1920x1080P30] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_1920x1080P30, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_1280x720P240] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_1280x720P240, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_1280x720P120] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_1280x720P120, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_1280x720P60] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_1280x720P60, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_1280x720P30] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_1280x720P30, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_848x480P30] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_848x480P30, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_640x480P240] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_640x480P240, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_640x480P30] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_640x480P30, 0},
		{0, 0},
	},

	[MOVIE_SIZE_FRONT_320x240P30] = {
		MOVIE_REC_TYPE_FRONT,
		{MOVIE_SIZE_320x240P30, 0},
		{0, 0},
	},

	[MOVIE_SIZE_DUAL_2560x1440P30_1280x720P30] = {
		MOVIE_REC_TYPE_DUAL,
		{MOVIE_SIZE_2560x1440P30, MOVIE_SIZE_1280x720P30},
		{0, 0},
	},

	[MOVIE_SIZE_DUAL_2304x1296P30_1280x720P30] = {
		MOVIE_REC_TYPE_DUAL,
		{MOVIE_SIZE_2304x1296P30, MOVIE_SIZE_1280x720P30},
		{0, 0},
	},

	[MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30] = {
		MOVIE_REC_TYPE_DUAL,
		{MOVIE_SIZE_1920x1080P30, MOVIE_SIZE_1920x1080P30},
		{0, 0},
	},

	[MOVIE_SIZE_DUAL_1920x1080P30_1280x720P30] = {
		MOVIE_REC_TYPE_DUAL,
		{MOVIE_SIZE_1920x1080P30, MOVIE_SIZE_1280x720P30},
		{0, 0},
	},

	[MOVIE_SIZE_DUAL_1920x1080P30_848x480P30] = {
		MOVIE_REC_TYPE_DUAL,
		{MOVIE_SIZE_1920x1080P30, MOVIE_SIZE_848x480P30},
		{0, 0},
	},

	[MOVIE_SIZE_CLONE_3840x2160P30_848x480P30] = {
		MOVIE_REC_TYPE_CLONE, 				// TYPE_CLONE means single recording + small clone movie
		{MOVIE_SIZE_3840x2160P30, 0}, // main movie
		{MOVIE_SIZE_848x480P30, 0},	// clone movie
	},

	[MOVIE_SIZE_CLONE_1920x1080P30_1920x1080P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1920x1080P30, 0},
		{MOVIE_SIZE_1920x1080P30, 0},
	},

	[MOVIE_SIZE_CLONE_1920x1080P30_1280x720P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1920x1080P30, 0},
		{MOVIE_SIZE_1280x720P30, 0},
	},

	[MOVIE_SIZE_CLONE_2560x1440P60_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_2560x1440P60, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_2560x1440P30_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_2560x1440P30, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_2304x1296P30_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_2304x1296P30, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_1920x1080P120_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1920x1080P120, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_1920x1080P60_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1920x1080P60, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_1920x1080P60_640x360P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1920x1080P60, 0},
		{MOVIE_SIZE_640x360P30, 0},
	},

	[MOVIE_SIZE_CLONE_1920x1080P30_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1920x1080P30, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_2048x2048P30_480x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_2048x2048P30, 0},
		{MOVIE_SIZE_480x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_1280x720P30_1280x720P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1280x720P30, 0},
		{MOVIE_SIZE_1280x720P30, 0},
	},

	[MOVIE_SIZE_CLONE_1280x720P120_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1280x720P120, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_1280x720P60_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1280x720P60, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_1280x720P30_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_1280x720P30, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_848x480P30_848x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_848x480P30, 0},
		{MOVIE_SIZE_848x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_640x480P30_640x480P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_640x480P30, 0},
		{MOVIE_SIZE_640x480P30, 0},
	},

	[MOVIE_SIZE_CLONE_320x240P30_320x240P30] = {
		MOVIE_REC_TYPE_CLONE,
		{MOVIE_SIZE_320x240P30, 0},
		{MOVIE_SIZE_320x240P30, 0},
	},

};

// for main movie
UINT32 MovieMapping_GetRecType(UINT32 uiSizeIdx)
{
	return g_MovieRecMappingTable[uiSizeIdx].Type;
}

UINT32 MovieMapping_GetWidth(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdx[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiWidth;
}

UINT32 MovieMapping_GetHeight(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdx[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiHeight;
}

UINT32 MovieMapping_GetFrameRate(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdx[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiVidFrameRate;
}

UINT32 MovieMapping_GetTargetBitrate(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdx[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiTargetBitrate;
}

UINT32 MovieMapping_GetDispAspectRatio(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdx[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiDispAspectRatio;
}

UINT32 MovieMapping_GetImageRatio(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdx[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiImageRatio;
}

void MovieMapping_GetAqInfo(UINT32 uiSizeIdx, UINT32 uiIplId, UINT32 Value)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdx[uiIplId];
	MOVIE_AQ_PARAM* pAqInfo = (MOVIE_AQ_PARAM*) Value;
	if (pAqInfo) {
		memcpy(pAqInfo, &g_MovieSizeTable[movie_size].AqInfo, sizeof(MOVIE_AQ_PARAM));
	}
}

void MovieMapping_GetCbrInfo(UINT32 uiSizeIdx, UINT32 uiIplId, UINT32 Value)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdx[uiIplId];
	MOVIE_CBR_PARAM* pCbrInfo = (MOVIE_CBR_PARAM*) Value;
	if (pCbrInfo) {
		memcpy(pCbrInfo, &g_MovieSizeTable[movie_size].CbrInfo, sizeof(MOVIE_CBR_PARAM));
	}
}

// for clone movie
UINT32 MovieMapping_GetCloneWidth(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdxClone[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiWidth;
}

UINT32 MovieMapping_GetCloneHeight(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdxClone[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiHeight;
}

UINT32 MovieMapping_GetCloneFrameRate(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdxClone[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiVidFrameRate;
}

UINT32 MovieMapping_GetCloneTargetBitrate(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdxClone[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiTargetBitrate;
}

UINT32 MovieMapping_GetCloneDispAspectRatio(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdxClone[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiDispAspectRatio;
}

UINT32 MovieMapping_GetCloneImageRatio(UINT32 uiSizeIdx, UINT32 uiIplId)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdxClone[uiIplId];
	return g_MovieSizeTable[movie_size].SizeInfo.uiImageRatio;
}

void MovieMapping_GetCloneAqInfo(UINT32 uiSizeIdx, UINT32 uiIplId, UINT32 Value)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdxClone[uiIplId];
	MOVIE_AQ_PARAM* pAqInfo = (MOVIE_AQ_PARAM*) Value;
	if (pAqInfo) {
		memcpy(pAqInfo, &g_MovieSizeTable[movie_size].AqInfo, sizeof(MOVIE_AQ_PARAM));
	}
}

void MovieMapping_GetCloneCbrInfo(UINT32 uiSizeIdx, UINT32 uiIplId, UINT32 Value)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiSizeIdx].SizeIdxClone[uiIplId];
	MOVIE_CBR_PARAM* pCbrInfo = (MOVIE_CBR_PARAM*) Value;
	if (pCbrInfo) {
		memcpy(pCbrInfo, &g_MovieSizeTable[movie_size].CbrInfo, sizeof(MOVIE_CBR_PARAM));
	}
}

// for WiFi/UVC streaming
void MovieMapping_GetStreamInfo(UINT32 uiSizeIdx, UINT32 Value)
{
	MOVIE_STRM_INFO* ptStreamInfo = (MOVIE_STRM_INFO*) Value;

	uiSizeIdx = MOVIE_SIZE_WIFI_STREAMING; // always use 848x480 temporarily

	if (ptStreamInfo) {
		ptStreamInfo->size.w = g_MovieSizeTable[uiSizeIdx].SizeInfo.uiWidth;
		ptStreamInfo->size.h = g_MovieSizeTable[uiSizeIdx].SizeInfo.uiHeight;
		ptStreamInfo->frame_rate = g_MovieSizeTable[uiSizeIdx].SizeInfo.uiVidFrameRate;

		memcpy(&ptStreamInfo->cbr_info, &g_MovieSizeTable[uiSizeIdx].CbrInfo, sizeof(MOVIE_CBR_PARAM));
		ptStreamInfo->max_bit_rate = g_MovieSizeTable[uiSizeIdx].CbrInfo.uiByteRate;
		ptStreamInfo->gop_num = g_MovieSizeTable[uiSizeIdx].CbrInfo.uiGOP;

		memcpy(&ptStreamInfo->aq_info, &g_MovieSizeTable[uiSizeIdx].AqInfo, sizeof(MOVIE_AQ_PARAM));
	}
}
//-------------------------------------------------------------------------------------------------

#if (WIFI_FINALCAM_APP_STYLE == ENABLE || UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE || (YOUKU_SDK == ENABLE))
static MOVIE_SIZE_PARAM g_WiFiMovieAppPreviewSizeTable[WIFI_MOVIE_APP_PREVIEW_SIZE_ID_MAX] = {
{1280, 720, 30, 800 * 1024, MEDIAREC_DAR_DEFAULT }, // 720P, 30fps, 800KB/sec
{ 848, 480, 30, 400 * 1024, MEDIAREC_DAR_DEFAULT }, // WVGA, 30fps, 400KB/sec
{ 640, 480, 30, 400 * 1024, MEDIAREC_DAR_DEFAULT }, // VGA, 30fps, 400KB/sec
{ 640, 360, 30, 400 * 1024, MEDIAREC_DAR_DEFAULT }, // 360P, 30fps, 400KB/sec
{ 320, 240, 30, 300 * 1024, MEDIAREC_DAR_DEFAULT }  // QVGA, 30fps, 300KB/sec
};
#endif

UINT32 GetMovieFrameRate(UINT32 uiIndex)
{
	return MovieMapping_GetFrameRate(uiIndex, 0); // ipl id = 0, temporarily
}

void SetMovieTargetBitrate(UINT32 uiType, UINT32 uiIndex, UINT32 uiBitRate)
{
	UINT32 movie_size = g_MovieRecMappingTable[uiIndex].SizeIdx[uiType];
	g_MovieSizeTable[movie_size].SizeInfo.uiTargetBitrate = uiBitRate;
}

UINT32 GetMovieSizeRatio(UINT32 uiIndex)
{
	return MovieMapping_GetImageRatio(uiIndex, 0); // ipl id = 0, temporarily
}

// 2-path image source
UINT32 GetMovieSizeWidth_2p(UINT32 uiPath, UINT32 uiIndex)
{
	if ((System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) && (uiPath == 1)) {
		// for WiFi streaming (path 1 means WiFi streaming for NovaCam)
		return g_MovieSizeTable[MOVIE_SIZE_WIFI_STREAMING].SizeInfo.uiWidth;
	} else {
		return MovieMapping_GetWidth(uiIndex, uiPath);
	}
}

UINT32 GetMovieSizeHeight_2p(UINT32 uiPath, UINT32 uiIndex)
{
	if ((System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) && (uiPath == 1)) {
		// for WiFi streaming (path 1 means WiFi streaming for NovaCam)
		return g_MovieSizeTable[MOVIE_SIZE_WIFI_STREAMING].SizeInfo.uiHeight;
	} else {
		return MovieMapping_GetHeight(uiIndex, uiPath);
	}
}

UINT32 GetMovieFrameRate_2p(UINT32 uiPath, UINT32 uiIndex)
{
	if ((System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) && (uiPath == 1)) {
		// for WiFi streaming (path 1 means WiFi streaming for NovaCam)
		return g_MovieSizeTable[MOVIE_SIZE_WIFI_STREAMING].SizeInfo.uiVidFrameRate;
	} else {
		return MovieMapping_GetFrameRate(uiIndex, uiPath);
	}
}

UINT32 GetMovieTargetBitrate_2p(UINT32 uiPath, UINT32 uiIndex)
{
	if ((System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) && (uiPath == 1)) {
		// for WiFi streaming (path 1 means WiFi streaming for NovaCam)
		return g_MovieSizeTable[MOVIE_SIZE_WIFI_STREAMING].SizeInfo.uiTargetBitrate;
	} else {
		return MovieMapping_GetTargetBitrate(uiIndex, uiPath);
	}
}

UINT32 GetMovieDispAspectRatio_2p(UINT32 uiPath, UINT32 uiIndex)
{
	if ((System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) && (uiPath == 1)) {
		// for WiFi streaming (path 1 means WiFi streaming for NovaCam)
		return g_MovieSizeTable[MOVIE_SIZE_WIFI_STREAMING].SizeInfo.uiDispAspectRatio;
	} else {
		return MovieMapping_GetDispAspectRatio(uiIndex, uiPath);
	}
}

MOVIE_REC_TYPE GetMovieRecType_2p(UINT32 uiIndex)
{
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
		// always use clone type for WiFi streaming (NovaCam will check clone type for WiFi streaming)
		return MOVIE_REC_TYPE_CLONE;
	} else {
		return MovieMapping_GetRecType(uiIndex);
	}
}

UINT32 GetMovieAudioVolumeValue(UINT32 uiIndex)
{
	UINT32 audVol = 0;
	//map UI setting to HW setting, refering to DrvSound_SetVol()
	if (uiIndex < MOVIE_AUDIO_VOL_SETTING_MAX) {
		audVol = 10 * uiIndex;
	}
	return audVol;
}

UINT32 Get_MovieTimeLapseValue(UINT32 uiIndex)
{
	UINT32 uiTimeLapse = 1000;

	switch (SysGetFlag(FL_MOVIE_TIMELAPSE_REC)) {
	default:
	case MOVIE_TIMELAPSEREC_1SEC:
		uiTimeLapse = 1000;
		break;
	case MOVIE_TIMELAPSEREC_5SEC:
		uiTimeLapse = 5000;
		break;
	case MOVIE_TIMELAPSEREC_10SEC:
		uiTimeLapse = 10000;
		break;
	case MOVIE_TIMELAPSEREC_30SEC:
		uiTimeLapse = 30000;
		break;
	case MOVIE_TIMELAPSEREC_1MIN:
		uiTimeLapse = 60000;
		break;
	case MOVIE_TIMELAPSEREC_5MIN:
		uiTimeLapse = 300000;
		break;
	case MOVIE_TIMELAPSEREC_10MIN:
		uiTimeLapse = 600000;
		break;
	case MOVIE_TIMELAPSEREC_30MIN:
		uiTimeLapse = 1800000;
		break;
	case MOVIE_TIMELAPSEREC_1HOUR:
		uiTimeLapse = 3600000;
		break;
	case MOVIE_TIMELAPSEREC_2HOUR:
		uiTimeLapse = 7200000;
		break;
	case MOVIE_TIMELAPSEREC_3HOUR:
		uiTimeLapse = 10800000;
		break;
	case MOVIE_TIMELAPSEREC_1DAY:
		uiTimeLapse = 86400000;
		break;
	}

	return uiTimeLapse;
}

typedef struct {
	UINT32  SingleIndex;
	UINT32  WiFiIndex;
} MOVIE_SINGLE_WIFI_MAP;

//MOVIE_SIZE_TAG
MOVIE_SINGLE_WIFI_MAP WiFiMapTbl[] = {
	{MOVIE_SIZE_FRONT_2880x2160P50, MOVIE_SIZE_FRONT_2880x2160P50},                             /// UHD P50 (DAR), not support for WiFi. Only match with the menu option (MenuMovie.c)
	{MOVIE_SIZE_FRONT_3840x2160P30, MOVIE_SIZE_CLONE_3840x2160P30_848x480P30},                  /// UHD P30
	{MOVIE_SIZE_FRONT_2880x2160P24, MOVIE_SIZE_FRONT_2880x2160P24},                             /// UHD P24 (DAR), not support for WiFi. Only match with the menu option (MenuMovie.c)
	{MOVIE_SIZE_FRONT_2704x2032P60, MOVIE_SIZE_FRONT_2704x2032P60},                             /// 2.7K P60, not support for WiFi. Only match with the menu option (MenuMovie.c)
	{MOVIE_SIZE_FRONT_2560x1440P80, MOVIE_SIZE_FRONT_2560x1440P80},                             /// QHD P80, not support for WiFi. Only match with the menu option (MenuMovie.c)
	{MOVIE_SIZE_FRONT_2560x1440P60, MOVIE_SIZE_CLONE_2560x1440P60_848x480P30},                  /// QHD P60
	{MOVIE_SIZE_FRONT_2560x1440P30, MOVIE_SIZE_CLONE_2560x1440P30_848x480P30},                  /// QHD P30
	{MOVIE_SIZE_FRONT_2304x1296P30, MOVIE_SIZE_CLONE_2304x1296P30_848x480P30},                  /// 3MHD P30
	{MOVIE_SIZE_FRONT_1920x1080P120, MOVIE_SIZE_CLONE_1920x1080P120_848x480P30},                /// FHD P120
	{MOVIE_SIZE_FRONT_1920x1080P96, MOVIE_SIZE_FRONT_1920x1080P96},                             /// FHD P96, not support for WiFi. Only match with the menu option (MenuMovie.c)
	{MOVIE_SIZE_FRONT_1920x1080P60, MOVIE_SIZE_CLONE_1920x1080P60_848x480P30},                  /// FHD P60
	{MOVIE_SIZE_FRONT_1920x1080P30, MOVIE_SIZE_CLONE_1920x1080P30_848x480P30},                  /// FHD P30
	{MOVIE_SIZE_FRONT_1280x720P240, MOVIE_SIZE_FRONT_1280x720P240},                             /// HD P240, not support for WiFi. Only match with the menu option (MenuMovie.c)
	{MOVIE_SIZE_FRONT_1280x720P120, MOVIE_SIZE_CLONE_1280x720P120_848x480P30},                  /// HD P120
	{MOVIE_SIZE_FRONT_1280x720P60, MOVIE_SIZE_CLONE_1280x720P60_848x480P30},                    /// HD P60
	{MOVIE_SIZE_FRONT_1280x720P30, MOVIE_SIZE_CLONE_1280x720P30_1280x720P30},                   /// HD P30
	{MOVIE_SIZE_FRONT_848x480P30, MOVIE_SIZE_CLONE_848x480P30_848x480P30},                      /// WVGA P30
	{MOVIE_SIZE_FRONT_640x480P240, MOVIE_SIZE_FRONT_640x480P240},                               /// VGA P240, not support for WiFi. Only match with the menu option (MenuMovie.c)
	{MOVIE_SIZE_FRONT_640x480P30, MOVIE_SIZE_CLONE_640x480P30_640x480P30},                      /// VGA P30
	{MOVIE_SIZE_FRONT_320x240P30, MOVIE_SIZE_CLONE_320x240P30_320x240P30},                      /// QVGA P30
	{MOVIE_SIZE_DUAL_2560x1440P30_1280x720P30, MOVIE_SIZE_DUAL_2560x1440P30_1280x720P30 },		/// QHD HD
	{MOVIE_SIZE_DUAL_2304x1296P30_1280x720P30, MOVIE_SIZE_DUAL_2304x1296P30_1280x720P30 },		/// 3MHD HD
	{MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30, MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30},     /// FHD FHD
	{MOVIE_SIZE_DUAL_1920x1080P30_1280x720P30, MOVIE_SIZE_DUAL_1920x1080P30_1280x720P30 },		/// FHD HD
	{MOVIE_SIZE_DUAL_1920x1080P30_848x480P30, MOVIE_SIZE_DUAL_1920x1080P30_848x480P30 },		/// FHD WVGA
	//#NT#2016/09/23#Hideo Lin -begin
	//#NT#Add WiFi streaming settings for small clone movie function
#if (SMALL_CLONE_MOVIE == ENABLE)
	{MOVIE_SIZE_CLONE_1920x1080P30_1920x1080P30, MOVIE_SIZE_CLONE_1920x1080P30_848x480P30 }, // not support
	{MOVIE_SIZE_CLONE_1920x1080P30_1280x720P30, MOVIE_SIZE_CLONE_1920x1080P30_848x480P30 }, // not support
	{MOVIE_SIZE_CLONE_2560x1440P30_848x480P30,  MOVIE_SIZE_CLONE_2560x1440P30_848x480P30 }, // enable
	{MOVIE_SIZE_CLONE_2304x1296P30_848x480P30,  MOVIE_SIZE_CLONE_2304x1296P30_848x480P30 }, // enable
	{MOVIE_SIZE_CLONE_1920x1080P60_848x480P30,  MOVIE_SIZE_CLONE_1920x1080P60_848x480P30 }, // not support
	{MOVIE_SIZE_CLONE_1920x1080P60_640x360P30,  MOVIE_SIZE_CLONE_1920x1080P60_848x480P30 }, // not support
	{MOVIE_SIZE_CLONE_1920x1080P30_848x480P30,  MOVIE_SIZE_CLONE_1920x1080P30_848x480P30 }, // enable
#endif
	//#NT#2016/09/23#Hideo Lin -end
};

UINT32 GetWiFiIndex(UINT32 singleIndex)
{
	UINT32 i = 0 ;

	for (i = 0; i < sizeof(WiFiMapTbl) / sizeof(MOVIE_SINGLE_WIFI_MAP); i++) {
		if (WiFiMapTbl[i].SingleIndex == singleIndex) {
			DBG_IND("%d to %d\r\n", singleIndex, WiFiMapTbl[i].WiFiIndex);
			return WiFiMapTbl[i].WiFiIndex;
		}
	}

	DBG_ERR("index not found %d\r\n", singleIndex);
	return MOVIE_SIZE_ID_MAX;
}

UINT32 CheckWiFiMapTbl(void)
{
	// don't need to check WiFi mapping table as MOVIE_MAPPING_MULTIREC enabled
	return E_OK;
}

UINT32 GetMovieVidEncFmt_2p(UINT32 uiPath)
{
	return 0;
}
