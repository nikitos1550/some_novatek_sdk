#include <stdio.h>
#include "Debug.h"
#include "PrjCfg.h"
#include "UIAppPhoto_Param.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppPhoto_Param
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#if (SENSOR_CAPS_COUNT == 1)
static UIAPP_PHOTO_SENSOR_INFO g_UIAppPhotoSensorInfo[UIAPP_PHOTO_SENSOR_ID_MAX] = {
	//sensor ID,         IPL fps, IPL size   , IPL ratio, flip type, IME 3DNR , video format[ime_out],                                            video compress, crop enable[ime_out]
	{UIAPP_PHOTO_SENSOR_ID_1, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
};

static PHOTO_DISP_INFO g_UIAppPhotoDispConfig[UIAPP_PHOTO_DISP_ID_MAX] = {
//enable           multi_view_type  disp_id       vid_in           w     h      ratio     rotate dir  is_merge_3dnr_path    vid_out
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_1,PHOTO_VID_IN_1,  0,  0  ,  4 ,  3  ,  0,          0,                    PHOTO_VID_OUT_1},
};

static PHOTO_STRM_INFO g_UIAppPhotoStreamConfig[UIAPP_PHOTO_STRM_ID_MAX] = {
//enable            multi_view_type  strm_id       vid_in           w     h   ratio   fps   max_w  max_h  max_bitrate      target_bitrate      codec             strm_type
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_1,PHOTO_VID_IN_1,  864,  480  ,4 , 3, 30  , 864,   480,  1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_H264, PHOTO_STRM_TYPE_RTSP},
	//{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_1,PHOTO_VID_IN_1,  864,  480  ,4 , 3, 30  , 864,   480,  1024*1024,  1024*1024, PHOTO_CODEC_MJPG, PHOTO_STRM_TYPE_HTTP},
};
#elif (SENSOR_CAPS_COUNT == 2)
static UIAPP_PHOTO_SENSOR_INFO g_UIAppPhotoSensorInfo[UIAPP_PHOTO_SENSOR_ID_MAX] = {
	//sensor ID,         IPL fps,   IPL size   ,IPL ratio, flip type, IME 3DNR , video format[ime_out],                                            video compress, crop enable[ime_out]
	{UIAPP_PHOTO_SENSOR_ID_1, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
	{UIAPP_PHOTO_SENSOR_ID_2, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
};

static PHOTO_DISP_INFO g_UIAppPhotoDispConfig[UIAPP_PHOTO_DISP_ID_MAX] = {
//enable     multi_view_type            disp_id       vid_in           w     h   ratio rotate dir    is_merge_3dnr_path     vid_out
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_1,PHOTO_VID_IN_1,  0,  0  ,  4 ,  3  ,  0,          0,                    PHOTO_VID_OUT_1},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_2,PHOTO_VID_IN_2,  0,  0  ,  4 ,  3  ,  0,          0,                    PHOTO_VID_OUT_1},
};
static PHOTO_STRM_INFO g_UIAppPhotoStreamConfig[UIAPP_PHOTO_STRM_ID_MAX] = {
//enable       multi_view_type  strm_id       vid_in           w     h   ratio   fps         max_w  max_h  max_bitrate      target_bitrate      codec             strm_type
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_1,PHOTO_VID_IN_1,  864,  480  ,4 , 3, 30 , 864,   480  , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_H264, PHOTO_STRM_TYPE_RTSP},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_2,PHOTO_VID_IN_2,  864,  480  ,4 , 3, 30 , 864,   480  , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_MJPG, PHOTO_STRM_TYPE_HTTP},
};
#elif(SENSOR_CAPS_COUNT == 3)
static UIAPP_PHOTO_SENSOR_INFO g_UIAppPhotoSensorInfo[UIAPP_PHOTO_SENSOR_ID_MAX] = {
	//sensor ID,         IPL fps,   IPL size   ,IPL ratio, flip type, IME 3DNR , video format[ime_out],                                            video compress, crop enable[ime_out]
	{UIAPP_PHOTO_SENSOR_ID_1, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
	{UIAPP_PHOTO_SENSOR_ID_2, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
	{UIAPP_PHOTO_SENSOR_ID_3, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
};

static PHOTO_DISP_INFO g_UIAppPhotoDispConfig[UIAPP_PHOTO_DISP_ID_MAX] = {
//enable          multi_view_type  disp_id       vid_in           w     h      ratio     rotate dir    is_merge_3dnr_path     vid_out
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_1,PHOTO_VID_IN_1,  0,  0  ,  4 ,  3  ,    0,          0,                    PHOTO_VID_OUT_1},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_2,PHOTO_VID_IN_2,  0,  0  ,  4 ,  3  ,    0,          0,                    PHOTO_VID_OUT_1},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_3,PHOTO_VID_IN_3,  0,  0  ,  4 ,  3  ,    0,          0,                    PHOTO_VID_OUT_1},
};

static PHOTO_STRM_INFO g_UIAppPhotoStreamConfig[UIAPP_PHOTO_STRM_ID_MAX] = {
//enable         multi_view_type  strm_id       vid_in           w     h   ratio   fps        max_w  max_h   max_bitrate      target_bitrate      codec             strm_type
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_1,PHOTO_VID_IN_1,  864,  480  ,4 , 3, 30  , 864,   480 , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_H264, PHOTO_STRM_TYPE_RTSP},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_2,PHOTO_VID_IN_2,  864,  480  ,4 , 3, 30  , 864,   480 , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_MJPG, PHOTO_STRM_TYPE_HTTP},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_3,PHOTO_VID_IN_3,  864,  480  ,4 , 3, 30  , 864,   480 , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_MJPG, PHOTO_STRM_TYPE_HTTP},
};
#else
static UIAPP_PHOTO_SENSOR_INFO g_UIAppPhotoSensorInfo[UIAPP_PHOTO_SENSOR_ID_MAX] = {
	//sensor ID,         IPL fps,   IPL size   ,IPL ratio, flip type, IME 3DNR , video format[ime_out],                                            video compress, crop enable[ime_out]
	{UIAPP_PHOTO_SENSOR_ID_1, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
	{UIAPP_PHOTO_SENSOR_ID_2, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
	{UIAPP_PHOTO_SENSOR_ID_3, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
	{UIAPP_PHOTO_SENSOR_ID_4, 30, {1920, 1080}, {4, 3}, FLIP_NONE, FALSE,  {PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_420, PHOTO_FMT_Y, PHOTO_FMT_422}, FALSE, {FALSE,FALSE,FALSE,FALSE,FALSE}},
};

static PHOTO_DISP_INFO g_UIAppPhotoDispConfig[UIAPP_PHOTO_DISP_ID_MAX] = {
//enable          multi_view_type  disp_id       vid_in           w     h      ratio     rotate dir    is_merge_3dnr_path     vid_out
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_1,PHOTO_VID_IN_1,  0,  0  ,  4 ,  3  ,    0,          0,                    PHOTO_VID_OUT_1},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_2,PHOTO_VID_IN_2,  0,  0  ,  4 ,  3  ,    0,          0,                    PHOTO_VID_OUT_1},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_3,PHOTO_VID_IN_3,  0,  0  ,  4 ,  3  ,    0,          0,                    PHOTO_VID_OUT_1},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_DISP_ID_4,PHOTO_VID_IN_4,  0,  0  ,  4 ,  3  ,    0,          0,                    PHOTO_VID_OUT_1},
};

static PHOTO_STRM_INFO g_UIAppPhotoStreamConfig[UIAPP_PHOTO_STRM_ID_MAX] = {
//enable         multi_view_type  strm_id       vid_in           w     h   ratio   fps        max_w  max_h   max_bitrate      target_bitrate      codec             strm_type
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_1,PHOTO_VID_IN_1,  864,  480  ,4 , 3, 30  , 864,   480 , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_H264, PHOTO_STRM_TYPE_RTSP},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_2,PHOTO_VID_IN_2,  864,  480  ,4 , 3, 30  , 864,   480 , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_MJPG, PHOTO_STRM_TYPE_HTTP},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_3,PHOTO_VID_IN_3,  864,  480  ,4 , 3, 30  , 864,   480 , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_MJPG, PHOTO_STRM_TYPE_HTTP},
	{TRUE, PHOTO_MULTI_VIEW_SINGLE, PHOTO_STRM_ID_4,PHOTO_VID_IN_4,  864,  480  ,4 , 3, 30  , 864,   480 , 1 * 200 * 1024,  1 * 200 * 1024, PHOTO_CODEC_MJPG, PHOTO_STRM_TYPE_HTTP},
};
#endif

UIAPP_PHOTO_SENSOR_INFO *UIAppPhoto_get_SensorInfo(UIAPP_PHOTO_SENSOR_ID  sensor_id)
{
	if (sensor_id > UIAPP_PHOTO_SENSOR_ID_MAX)
	{
		DBG_ERR("sensor_id = %d\r\n",sensor_id);
		return NULL;
	}
	return &g_UIAppPhotoSensorInfo[sensor_id];
}

PHOTO_DISP_INFO *UIAppPhoto_get_DispConfig(UIAPP_PHOTO_DISP_ID  disp_id)
{
	if (disp_id > UIAPP_PHOTO_DISP_ID_MAX)
	{
		DBG_ERR("disp_id = %d\r\n",disp_id);
		return NULL;
	}
	return &g_UIAppPhotoDispConfig[disp_id];
}

PHOTO_STRM_INFO *UIAppPhoto_get_StreamConfig(UIAPP_PHOTO_STRM_ID  strm_id)
{
	if (strm_id > UIAPP_PHOTO_STRM_ID_MAX)
	{
		DBG_ERR("strm_id = %d\r\n",strm_id);
		return NULL;
	}
	return &g_UIAppPhotoStreamConfig[strm_id];
}



