/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_Play.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef PLAY_H
#define PLAY_H

//#include "GxLib.h"

#include "ImageStream.h"
#include "ImageUnit_Play.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_AudOut.h"
#include "ImageUnit_UserProc.h"
#include "ImageUnit_ImgTrans.h"

/**
    @addtogroup mIImageApp
*/
//@{

enum {
	PLAY_CONFIG_DUAL_DISP  = 0xF000F000,
	PLAY_CONFIG_DISP_INFO,
};

typedef enum {
	PLAY_DISP_ID_MIN = 0,
	PLAY_DISP_ID_1 = PLAY_DISP_ID_MIN,
	PLAY_DISP_ID_2,
	PLAY_DISP_ID_3,
	PLAY_DISP_ID_4,
	PLAY_DISP_ID_MAX,
	ENUM_DUMMY4WORD(PLAY_DISP_ID)
} PLAY_DISP_ID;

typedef struct {
	BOOL                 enable;
	PLAY_DISP_ID         disp_id;
	UINT32               width;
	UINT32               height;
	UINT32               width_ratio;
	UINT32               height_ratio;
	UINT32               rotate_dir;
} PLAY_DISP_INFO;

extern void ImageApp_Play_Config(UINT32 config_id, UINT32 value);

extern void ImageApp_Play_Open(void);
extern void ImageApp_Play_Close(void);

//@}
#endif//PLAY_H
