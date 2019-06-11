/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg_Default.h
    @ingroup

    @brief

    @note       This config genernate default config where isn't set in PrjCfg_MODEL.h
*/

#ifndef _PRJCFG_DEFAULT_H_
#define _PRJCFG_DEFAULT_H_

#ifndef GPROF_FUNC
#define GPROF_FUNC                      DISABLE
#endif

#ifndef STEREO_FUNC
#define STEREO_FUNC                     DISABLE
#endif

#ifndef IPCAM_AUDIO_IN_FUNC
#define IPCAM_AUDIO_IN_FUNC             DISABLE
#endif

#ifndef IPCAM_AUDIO_OUT_FUNC
#define IPCAM_AUDIO_OUT_FUNC            DISABLE
#endif

#ifndef IPCAM_ANR_FUNC
#define IPCAM_ANR_FUNC  				DISABLE
#endif

#ifndef IPCAM_AUDFILT_FUNC
#define IPCAM_AUDFILT_FUNC              DISABLE
#endif

#ifndef IPCAM_AUDDOA_FUNC
#define IPCAM_AUDDOA_FUNC               DISABLE
#endif

#ifndef IPCAM_AUDSRC_FUNC
#define IPCAM_AUDSRC_FUNC               DISABLE
#endif

#ifndef AUDIO_FUNC_PCM
#define AUDIO_FUNC_PCM               	ENABLE
#endif

#ifndef AUDIO_FUNC_AAC
#define AUDIO_FUNC_AAC                  DISABLE
#endif

#ifndef AUDIO_FUNC_PPCM
#define AUDIO_FUNC_PPCM               	DISABLE
#endif

#ifndef AUDIO_FUNC_ULAW
#define AUDIO_FUNC_ULAW                 DISABLE
#endif

#ifndef AUDIO_FUNC_ALAW
#define AUDIO_FUNC_ALAW                 DISABLE
#endif

#ifndef AUDIO_IN_TOTAL_16_LVL
#define AUDIO_IN_TOTAL_16_LVL           DISABLE
#endif

#ifndef AUDIO_SWITCH_SETTING
#define AUDIO_SWITCH_SETTING            DISABLE
#endif

#ifndef IPCAM_FD_DRAW_VIDEO
#define IPCAM_FD_DRAW_VIDEO  			DISABLE
#endif

#ifndef IPCAM_PD_FUNC
#define IPCAM_PD_FUNC					DISABLE
#endif

#ifndef IPCAM_PD_DRAW_VIDEO
#define IPCAM_PD_DRAW_VIDEO  			DISABLE
#endif

#ifndef IPCAM_BC_FUNC
#define IPCAM_BC_FUNC					DISABLE
#endif

#ifndef IPCAM_BC_DRAW_VIDEO
#define IPCAM_BC_DRAW_VIDEO  			DISABLE
#endif

#ifndef IPCAM_FTG_DRAW_VIDEO
#define IPCAM_FTG_DRAW_VIDEO        	DISABLE
#endif

#ifndef IPCAM_FTG_FUNC
#define IPCAM_FTG_FUNC              	DISABLE
#endif

#ifndef IPCAM_FTG_ROI_DRAW
#define IPCAM_FTG_ROI_DRAW				DISABLE
#endif

#ifndef IPCAM_ODT_FUNC
#define IPCAM_ODT_FUNC              	DISABLE
#endif

#ifndef IPCAM_ODT_DRAW_VIDEO
#define IPCAM_ODT_DRAW_VIDEO        	DISABLE
#endif

#ifndef MOVIE_ODT_FUNC
#define MOVIE_ODT_FUNC              	DISABLE
#endif

#ifndef MOVIE_ODT_DRAW_VIDEO
#define MOVIE_ODT_DRAW_VIDEO        	DISABLE
#endif

#ifndef TV_POWER_SAVE_MODE
#define TV_POWER_SAVE_MODE  			DISABLE
#endif

#ifndef DISPLAY1_DEFAULT_DISABLE
#define DISPLAY1_DEFAULT_DISABLE  		DISABLE
#endif



#ifndef IPCAM_RESET_FPS
#define IPCAM_RESET_FPS  				DISABLE
#endif


#ifndef IPCAM_MD_SAMPLE_660_STYLE
#define IPCAM_MD_SAMPLE_660_STYLE  		DISABLE
#endif

#ifndef IPCAM_WDR_FUNC
#define IPCAM_WDR_FUNC  				DISABLE
#endif

#ifndef IPCAM_SHDR_FUNC
#define IPCAM_SHDR_FUNC  				DISABLE
#endif

#ifndef MOVIE_STRM_PATH_USERPROC
#define MOVIE_STRM_PATH_USERPROC		DISABLE
#endif

#ifndef MOVIE_MODE_D2D_FUNC
#define MOVIE_MODE_D2D_FUNC				DISABLE
#endif

#ifndef IPCAM_DEFOG_FUNC
#define IPCAM_DEFOG_FUNC				DISABLE
#endif

#ifndef VIDEOSPRITE_MOSAIC
#define VIDEOSPRITE_MOSAIC              DISABLE
#endif

#ifndef VIDEOSPRITE_STAMP_FUNC
#define VIDEOSPRITE_STAMP_FUNC 			DISABLE
#endif

#ifndef PTY_FUNC
#define PTY_FUNC 						DISABLE
#endif


#ifndef IPCAM_LCE_FUNC
#define IPCAM_LCE_FUNC 					DISABLE
#endif

#ifndef IPCAM_VA_FUNC
#define IPCAM_VA_FUNC 					DISABLE
#endif

#ifndef VDOENC_MULTI_TEMPORARY_LAYER
#define VDOENC_MULTI_TEMPORARY_LAYER 	ENABLE
#endif

#ifndef YUV_TV_RANGE
#define YUV_TV_RANGE DISABLE
#endif

#ifndef VIDEO_FUNC_MJPG
#define VIDEO_FUNC_MJPG               	ENABLE
#endif

#ifndef VIDEO_FUNC_H264
#define VIDEO_FUNC_H264               	ENABLE
#endif

#ifndef VIDEO_FUNC_H265
#define VIDEO_FUNC_H265                 ENABLE
#endif

#ifndef SENSOR_GP_FUNC
#define SENSOR_GP_FUNC                  DISABLE
#endif

#ifndef STAMP_PRIVATE_POOL
#define STAMP_PRIVATE_POOL				ENABLE
#endif

#ifndef LINUX_NVT_DISP
#define LINUX_NVT_DISP					DISABLE
#endif

#ifndef IPCAM_FTG_SNAPSHOT
#define IPCAM_FTG_SNAPSHOT				DISABLE
#endif

#ifndef NMEDIAPLAY_FUNC
#define NMEDIAPLAY_FUNC                 ENABLE
#endif

#ifndef FTG_PATH
#define FTG_PATH						2
#endif

#ifndef MAX_FTGNUM
#define MAX_FTGNUM						10
#endif


#ifndef MOVIE_MAPPING_MULTIREC
#if (IPCAM_FUNC == ENABLE)
#define MOVIE_MAPPING_MULTIREC			DISABLE
#else
#define MOVIE_MAPPING_MULTIREC			ENABLE
#endif
#endif

#ifndef STOP_REC_BK
#define STOP_REC_BK						DISABLE
#endif

#ifndef FS_SWITCH_STRG_FUNC
#define FS_SWITCH_STRG_FUNC 			DISABLE
#endif
#if (FS_SWITCH_STRG_FUNC && FS_MULTI_STRG_FUNC)
#error "FS_SWITCH_STRG_FUNC and FS_MULTI_STRG_FUNC cannot be both enabled."
#endif
#if (FS_SWITCH_STRG_FUNC && SDHOTPLUG_FUNCTION)
#error "FS_SWITCH_STRG_FUNC and SDHOTPLUG_FUNCTION cannot be both enabled."
#endif

#ifndef POWERON_FAST_BOOT_WITH_MSG
#define POWERON_FAST_BOOT_WITH_MSG		ENABLE
#endif

#ifndef POWERON_FAST_SENSOR
#define POWERON_FAST_SENSOR				DISABLE
#endif

#ifndef IPCAM_PARAM_INI
#define IPCAM_PARAM_INI					DISABLE
#endif

#ifndef IME_TMNR_FUNC
#define IME_TMNR_FUNC					DISABLE
#endif

#ifndef POWER_KEY_LONG_PRESS
#define POWER_KEY_LONG_PRESS			DISABLE
#endif

#ifndef MOVIE_UVAC_FUNC
#define MOVIE_UVAC_FUNC					DISABLE
#endif

#ifndef UVC_MULTIMEDIA_FUNC
#define UVC_MULTIMEDIA_FUNC				DISABLE
#endif

#ifndef PHOTO_MODE_CAP_YUV420_FUNC
#define PHOTO_MODE_CAP_YUV420_FUNC				DISABLE
#endif

#ifndef MOVIE_MULTISTAMP_FUNC
#define MOVIE_MULTISTAMP_FUNC				DISABLE
#endif

#endif //_PRJCFG_DEFAULT_H_
