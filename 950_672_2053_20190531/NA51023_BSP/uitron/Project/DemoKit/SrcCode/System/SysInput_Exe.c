/*
    System Input Callback

    System Callback for Input Module.

    @file       SysInput_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"
#include "GxInput.h"
#include "GxPower.h"
#if (IPCAM_FUNC != ENABLE)
#include "SoundData.h"
#endif
#if (GSENSOR_FUNCTION == ENABLE)
#include "GSensor.h"
#endif
#include "UIFlowMovieFuncs.h"
#include "MovieInterface.h"

#include "UIFlowMovieIcons.h"
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysInputExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#include "SysInput_API.h"
#include "UIFlowWndWiFiMovie.h"
#include "ImageApp_MovieCommon.h"
#include "ImageApp_MovieMulti.h"
#include "gpio.h"
#include "GxSound.h"
//#NT#Refine code for continue key
#define    BURSTKEY_DEBOUNCE     800//ms
#define    BURSTKEY_INTERVAL     200//ms
#define TOUCH_TIMER_CNT            1//20ms
#define    DOUBLECLICK_INTERVAL     500//ms

int SX_TIMER_DET_KEY_ID = -1;
int SX_TIMER_DET_TOUCH_ID = -1;
int SX_TIMER_DET_PWR_ID = -1;
int SX_TIMER_DET_MODE_ID = -1;
int SX_TIMER_AUTO_INPUT_ID = -1;
int SX_TIMER_DET_GSENSOR_ID = -1;
int SX_TIMER_ONESEC_ID = -1;

void UI_DetPwrKey(void);
void UI_DetNormalKey(void);
void UI_DetStatusKey(void);
void UI_DetCustom1Key(void);
void UI_DetGsensor(void);// 2018.12.11
void Timer_DoOneSec(void);//van 2018.12.11

extern BOOL ACIsPlug;
BOOL IsWifiOpen = FALSE;
extern void FlowMovie_IconDrawLock(VControl *pCtrl,BOOL bDraw);


#if (POWERKEY_FUNCTION == ENABLE)
SX_TIMER_ITEM(Input_DetPKey, UI_DetPwrKey, 5, FALSE)
#endif
#if (NORMALKEY_FUNCTION == ENABLE)
SX_TIMER_ITEM(Input_DetNKey, UI_DetNormalKey, 1, FALSE)
#endif
#if (STATUSKEY_FUNCTION == ENABLE)
SX_TIMER_ITEM(Input_DetSKey, UI_DetStatusKey, 7, FALSE)
#endif

#if defined(_TOUCH_ON_)
SX_TIMER_ITEM(Input_DetTP, GxTouch_DetTP, TOUCH_TIMER_CNT, FALSE)
#endif
#if (GSENSOR_FUNCTION == ENABLE)
SX_TIMER_ITEM(Input_DetC1Key, UI_DetGsensor, 5, FALSE)
#endif
SX_TIMER_ITEM(Do_OneSec, Timer_DoOneSec, 50, FALSE)//50*20=1000ms=1S
static BOOL         m_uiAnyKeyUnlockEn        = FALSE;

#if defined(_TOUCH_ON_)
static TOUCH_OBJ g_TouchTable[] = {
	{TP_GESTURE_PRESS,         NVTEVT_PRESS,        DEMOSOUND_SOUND_KEY_TONE},
	{TP_GESTURE_MOVE,          NVTEVT_MOVE,         0},
	{TP_GESTURE_HOLD,          NVTEVT_HOLD,         0},
	{TP_GESTURE_RELEASE,       NVTEVT_RELEASE,      DEMOSOUND_SOUND_KEY_TONE},
	{TP_GESTURE_CLICK,         NVTEVT_CLICK,        0},
	{TP_GESTURE_SLIDE_LEFT,    NVTEVT_SLIDE_LEFT,   0},
	{TP_GESTURE_SLIDE_RIGHT,   NVTEVT_SLIDE_RIGHT,  0},
	{TP_GESTURE_SLIDE_UP,      NVTEVT_SLIDE_UP,     0},
	{TP_GESTURE_SLIDE_DOWN,    NVTEVT_SLIDE_DOWN,   0}
};
#endif

#if defined(_KEY_METHOD_4KEY_)
static KEY_OBJ g_KeyTable[] = {
#if defined(_UI_STYLE_CARDV_)
	//POWER KEY
	{FLGKEY_KEY_POWER,    KEY_PRESS,       NVTEVT_KEY_POWER_REL,    0,                      0},
	//NORMAL KEY
	{FLGKEY_MENU,         KEY_PRESS,       NVTEVT_KEY_MENU,         NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_MODE,         KEY_RELEASE,     NVTEVT_KEY_MODE,         NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_UP,           KEY_PRESS,       NVTEVT_KEY_UP,           NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_UP,           KEY_CONTINUE,    NVTEVT_KEY_UP,           NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_UP,           KEY_RELEASE,     NVTEVT_KEY_UP,           NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_DOWN,         KEY_PRESS,       NVTEVT_KEY_DOWN,         NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_DOWN,         KEY_CONTINUE,    NVTEVT_KEY_DOWN,         NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_DOWN,         KEY_RELEASE,     NVTEVT_KEY_DOWN,         NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_SHUTTER2,     KEY_PRESS,       NVTEVT_KEY_SHUTTER2,     NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_SHUTTER2,     KEY_RELEASE,     NVTEVT_KEY_SHUTTER2,     NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_ENTER,        KEY_PRESS,       NVTEVT_KEY_ENTER,        NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_ENTER,        KEY_RELEASE,     NVTEVT_KEY_ENTER,        NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_CUSTOM1,      KEY_PRESS,       NVTEVT_KEY_CUSTOM1,      NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_NONE},
	{FLGKEY_CUSTOM1,      KEY_RELEASE,     NVTEVT_KEY_CUSTOM1,      NVTEVT_KEY_RELEASE,     0},
#else// _UI_STYLE_SPORTCAM_
	//POWER KEY
	#if (POWER_KEY_LONG_PRESS == ENABLE)
	{FLGKEY_KEY_POWER,    KEY_PRESS,       NVTEVT_KEY_POWER,        0,                      0},
	{FLGKEY_KEY_POWER,    KEY_CONTINUE,    NVTEVT_KEY_POWER_CONT,   0,                      0},
	#endif
	{FLGKEY_KEY_POWER,    KEY_RELEASE,     NVTEVT_KEY_POWER_REL,    0,                      0},
	//NORMAL KEY
	{FLGKEY_UP,           KEY_PRESS,       NVTEVT_KEY_PREV,           NVTEVT_KEY_PRESS,     DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_UP,           KEY_CONTINUE,    NVTEVT_KEY_PREV,           NVTEVT_KEY_CONTINUE,     0},
	{FLGKEY_UP,           KEY_RELEASE,     NVTEVT_KEY_PREV,           NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_DOWN,         KEY_PRESS,       NVTEVT_KEY_DOWN,         NVTEVT_KEY_PRESS,     DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_DOWN,         KEY_CONTINUE,    NVTEVT_KEY_DOWN,         NVTEVT_KEY_CONTINUE,     0},
	{FLGKEY_DOWN,         KEY_RELEASE,     NVTEVT_KEY_DOWN,         NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_LEFT,         KEY_PRESS,       NVTEVT_KEY_MOVIE,         NVTEVT_KEY_PRESS,     DEMOSOUND_SOUND_KEY_TONE},
	//{FLGKEY_LEFT,         KEY_CONTINUE,    NVTEVT_KEY_PREV,         NVTEVT_KEY_CONTINUE,     0},
	//{FLGKEY_LEFT,         KEY_RELEASE,     NVTEVT_KEY_PREV,         NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_SHUTTER2,     KEY_PRESS,       NVTEVT_KEY_SHUTTER2,     NVTEVT_KEY_PRESS,     DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_SHUTTER2,     KEY_RELEASE,     NVTEVT_KEY_SHUTTER2,     NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_RIGHT,        KEY_PRESS,       NVTEVT_KEY_RIGHT,        NVTEVT_KEY_PRESS,     DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_RIGHT,        KEY_CONTINUE,    NVTEVT_KEY_RIGHT,        NVTEVT_KEY_CONTINUE,     0},
	{FLGKEY_RIGHT,        KEY_RELEASE,     NVTEVT_KEY_RIGHT,        NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_CUSTOM1,      KEY_PRESS,       NVTEVT_KEY_CUSTOM1,      NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_NONE},
	{FLGKEY_CUSTOM1,      KEY_RELEASE,     NVTEVT_KEY_CUSTOM1,      NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_MENU,         KEY_PRESS,       NVTEVT_KEY_MENU,         NVTEVT_KEY_PRESS,     DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_ENTER,         KEY_PRESS,       NVTEVT_KEY_ENTER,         NVTEVT_KEY_PRESS,     DEMOSOUND_SOUND_KEY_TONE},


#endif
};
#else
//_KEY_METHOD_2KEY_ (for SPORTCAM only)
static KEY_OBJ g_KeyTable[] = {
	//POWER KEY
	{FLGKEY_KEY_POWER,    KEY_RELEASE,     NVTEVT_KEY_POWER_REL,    0,                      0},
	//NORMAL KEY
	{FLGKEY_ENTER,        KEY_PRESS,       NVTEVT_KEY_ENTER,        NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_MENU,         KEY_PRESS,       NVTEVT_KEY_MENU,         NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_MODE,         KEY_PRESS,       NVTEVT_KEY_MODE,         NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_PLAYBACK,     KEY_PRESS,       NVTEVT_KEY_PLAYBACK,     NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_UP,           KEY_PRESS,       NVTEVT_KEY_UP,           NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_UP,           KEY_CONTINUE,    NVTEVT_KEY_UP,           NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_UP,           KEY_RELEASE,     NVTEVT_KEY_UP,           NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_DOWN,         KEY_PRESS,       NVTEVT_KEY_DOWN,         NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_DOWN,         KEY_CONTINUE,    NVTEVT_KEY_DOWN,         NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_DOWN,         KEY_RELEASE,     NVTEVT_KEY_DOWN,         NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_LEFT,         KEY_PRESS,       NVTEVT_KEY_SELECT,       NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_LEFT,         KEY_CONTINUE,    NVTEVT_KEY_SELECT,       NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_LEFT,         KEY_RELEASE,     NVTEVT_KEY_SELECT,       NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_RIGHT,        KEY_PRESS,       NVTEVT_KEY_NEXT,         NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_RIGHT,        KEY_CONTINUE,    NVTEVT_KEY_NEXT,         NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_RIGHT,        KEY_RELEASE,     NVTEVT_KEY_NEXT,         NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_DOWN,         KEY_PRESS,       NVTEVT_KEY_DOWN,         NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_DOWN,         KEY_CONTINUE,    NVTEVT_KEY_DOWN,         NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_DOWN,         KEY_RELEASE,     NVTEVT_KEY_DOWN,         NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_ZOOMIN,       KEY_PRESS,       NVTEVT_KEY_ZOOMIN,       NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_ZOOMIN,       KEY_CONTINUE,    NVTEVT_KEY_ZOOMIN,       NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_ZOOMIN,       KEY_RELEASE,     NVTEVT_KEY_ZOOMIN,       NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_ZOOMOUT,      KEY_PRESS,       NVTEVT_KEY_ZOOMOUT,      NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_ZOOMOUT,      KEY_CONTINUE,    NVTEVT_KEY_ZOOMOUT,      NVTEVT_KEY_CONTINUE,    0},
	{FLGKEY_ZOOMOUT,      KEY_RELEASE,     NVTEVT_KEY_ZOOMOUT,      NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_SHUTTER2,     KEY_PRESS,       NVTEVT_KEY_SHUTTER2,     NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_SHUTTER2,     KEY_RELEASE,     NVTEVT_KEY_SHUTTER2,     NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_SHUTTER1,     KEY_PRESS,       NVTEVT_KEY_SHUTTER1,     NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_KEY_TONE},
	{FLGKEY_SHUTTER1,     KEY_RELEASE,     NVTEVT_KEY_SHUTTER1,     NVTEVT_KEY_RELEASE,     0},
	{FLGKEY_CUSTOM1,      KEY_PRESS,       NVTEVT_KEY_CUSTOM1,      NVTEVT_KEY_PRESS,       DEMOSOUND_SOUND_NONE},
	{FLGKEY_CUSTOM1,      KEY_RELEASE,     NVTEVT_KEY_CUSTOM1,      NVTEVT_KEY_RELEASE,     0},
};
#endif
void KeySoundCB(UINT32 uiSoundID)
{
	if (uiSoundID) {
#if(UI_FUNC==ENABLE)
		UISound_Play(uiSoundID);
#endif
	}
}

UINT32 Input_GroupStatus2Event(UINT32 status)
{
	UINT32 i = 0;
	for (i = 0; i < sizeof(g_KeyTable) / sizeof(KEY_OBJ); i++) {
		if ((g_KeyTable[i].uiKeyFlag == STATUS_KEY_GROUP1) && (g_KeyTable[i].status == status)) {
			return g_KeyTable[i].uiKeyEvent;
		}
	}
	return 0;
}

/**
  convert GPIO key to UI key event
  [InputCB internal API]

  @param UINT32 keys: Input key code detected from GPIO mapping
  @return UINT32: NVTEVT
**/
#if 0
static UINT32 Input_Key2Evt(UINT32 keys)
{
	if (keys & FLGKEY_MOVIE) {
		return NVTEVT_KEY_MOVIE;
	}
	if (keys & FLGKEY_I) {
		return NVTEVT_KEY_I;
	}
	if (keys & FLGKEY_MODE) {
		return NVTEVT_KEY_MODE;
	}
	if (keys & FLGKEY_PLAYBACK) {
		return NVTEVT_KEY_PLAYBACK;
	}
	if (keys & FLGKEY_MENU) {
		return NVTEVT_KEY_MENU;
	}
	if (keys & FLGKEY_FACEDETECT) {
		return NVTEVT_KEY_FACEDETECT;
	}
	if (keys & FLGKEY_DEL) {
		return NVTEVT_KEY_DEL;
	}
	if (keys & FLGKEY_LEFT) {
		return NVTEVT_KEY_LEFT;
	}
	if (keys & FLGKEY_RIGHT) {
		return NVTEVT_KEY_RIGHT;
	}
	if (keys & FLGKEY_ENTER) {
		return NVTEVT_KEY_ENTER;
	}
	if (keys & FLGKEY_SHUTTER1) {
		return NVTEVT_KEY_SHUTTER1;
	}
	if (keys & FLGKEY_SHUTTER2) {
		return NVTEVT_KEY_SHUTTER2;
	}
	if (keys & FLGKEY_ZOOMOUT) {
		return NVTEVT_KEY_ZOOMOUT;
	}
	if (keys & FLGKEY_ZOOMIN) {
		return NVTEVT_KEY_ZOOMIN;
	}
	if (keys & FLGKEY_UP) {
		return NVTEVT_KEY_UP;
	}
	if (keys & FLGKEY_DOWN) {
		return NVTEVT_KEY_DOWN;
	} else {
		return NVTEVT_NULL;
	}
}
#endif
//just for backward compatible
void Input_SetKeyMask(KEY_STATUS uiMode, UINT32 uiMask)
{
	SysMan_SetKeyMask(uiMode, uiMask);
}
UINT32 Input_GetKeyMask(KEY_STATUS uiMode)
{
	return SysMan_GetKeyMask(uiMode);
}
void Input_SetKeySoundMask(KEY_STATUS uiMode, UINT32 uiMask)
{
	SysMan_SetKeySoundMask(uiMode, uiMask);
}
UINT32 Input_GetKeySoundMask(KEY_STATUS uiMode)
{
	return SysMan_GetKeySoundMask(uiMode);
}

/**
    reset all mask,usually in new winodw onOpen
*/
void Input_ResetMask(void)
{
	// Set key mask as default
	SysMan_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
	SysMan_SetKeyMask(KEY_RELEASE, FLGKEY_KEY_MASK_DEFAULT);
	SysMan_SetKeyMask(KEY_CONTINUE, FLGKEY_KEY_MASK_NULL);
#if defined(_TOUCH_ON_)
	SysMan_SetTouchMask(TOUCH_MASK_DEFAULT);
#endif
	SysMan_SetKeySoundMask(KEY_PRESS, FLGKEY_SOUND_MASK_DEFAULT);
	SysMan_SetKeySoundMask(KEY_RELEASE, FLGKEY_KEY_MASK_NULL);
	SysMan_SetKeySoundMask(KEY_CONTINUE, FLGKEY_SOUND_MASK_DEFAULT);
}
/**
    for some case,press any key unlock all,and post NVTEVT_KEY_PRESS
    ex:Timelapse or smile detect
*/
void Input_SetAnyKeyUnlock(BOOL en)
{
	m_uiAnyKeyUnlockEn = en;
}


UINT32 Input_Key2Mode(UINT32 keys)
{
#if (STATUSKEY_FUNCTION == ENABLE)
	switch (keys) {
	case NVTEVT_KEY_STATUS1:
		return DSC_MODE_MOVIE;
	case NVTEVT_KEY_STATUS2:
		return DSC_MODE_PHOTO_MANUAL;
	case NVTEVT_KEY_STATUS3:
		return DSC_MODE_PHOTO_MANUAL;
	case NVTEVT_KEY_STATUS4:
		return DSC_MODE_PHOTO_MANUAL;
	case NVTEVT_KEY_STATUS5:
		return DSC_MODE_PHOTO_MANUAL;
	case NVTEVT_KEY_STATUS6:
		return DSC_MODE_PHOTO_SCENE;
	case NVTEVT_KEY_STATUS7:
		return DSC_MODE_PHOTO_MANUAL;
	case NVTEVT_KEY_STATUS8:
		return DSC_MODE_PHOTO_MANUAL;
	case NVTEVT_KEY_STATUS9:
		return DSC_MODE_PHOTO_MANUAL;
	case NVTEVT_KEY_STATUS10:
		return DSC_MODE_PHOTO_MANUAL;
	default:
		return DSC_MODE_PHOTO_AUTO;
	}
#else
	return 0;
#endif
}
extern SX_CMD_ENTRY key[];
void System_OnInputInit(void)
{
	//PHASE-1 : Init & Open Drv or DrvExt
	{
		GxKey_RegCB(Key_CB);         //Register CB function of GxInput
#if defined(_TOUCH_ON_)
		GxTouch_RegCB(Touch_CB);     //Register CB function of GxInput
#endif
		GxKey_Init();
		SxCmd_AddTable(key);
	}
	//PHASE-2 : Init & Open Lib or LibExt
	{
		//1.砞﹚init
		//2.砞﹚CB,
		GxKey_SetContDebounce(BURSTKEY_DEBOUNCE / SxTimer_GetData(SXTIMER_TIMER_BASE));
		GxKey_SetRepeatInterval(BURSTKEY_INTERVAL / SxTimer_GetData(SXTIMER_TIMER_BASE));
		SysMan_RegKeySoundCB(KeySoundCB);
		SysMan_RegKeyTable(g_KeyTable, sizeof(g_KeyTable) / sizeof(KEY_OBJ));
		//3.爹SxJob狝叭 ---------> System Job
		//4.爹SxTimer狝叭 ---------> Detect Job

#if (POWERKEY_FUNCTION == ENABLE)
		SX_TIMER_DET_PWR_ID = SxTimer_AddItem(&Timer_Input_DetPKey);
#endif
#if (NORMALKEY_FUNCTION == ENABLE)
		SX_TIMER_DET_KEY_ID = SxTimer_AddItem(&Timer_Input_DetNKey);
#endif
#if (STATUSKEY_FUNCTION == ENABLE)
		SX_TIMER_DET_MODE_ID = SxTimer_AddItem(&Timer_Input_DetSKey);
#endif
#if (GSENSOR_FUNCTION == ENABLE)
		SX_TIMER_DET_GSENSOR_ID = SxTimer_AddItem(&Timer_Input_DetC1Key);
#endif
#if defined(_TOUCH_ON_)
		SX_TIMER_DET_TOUCH_ID = SxTimer_AddItem(&Timer_Input_DetTP);
#endif
		SX_TIMER_ONESEC_ID = SxTimer_AddItem(&Timer_Do_OneSec);

#if (STATUSKEY_FUNCTION == ENABLE)
		GxKey_DetStatusKey();
#endif

#if (POWERKEY_FUNCTION == ENABLE)
		SxTimer_SetFuncActive(SX_TIMER_DET_PWR_ID, TRUE);
#endif
#if (NORMALKEY_FUNCTION == ENABLE)
		SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID, TRUE);
#endif
#if (STATUSKEY_FUNCTION == ENABLE)
		SxTimer_SetFuncActive(SX_TIMER_DET_MODE_ID, TRUE);
#endif
#if (GSENSOR_FUNCTION == ENABLE)
		SxTimer_SetFuncActive(SX_TIMER_DET_GSENSOR_ID, TRUE);
#endif

		SxTimer_SetFuncActive(SX_TIMER_ONESEC_ID, TRUE);
#if defined(_TOUCH_ON_)
		GxTouch_Init();
		GxTouch_SetCtrl(GXTCH_DOUBLE_CLICK_INTERVAL,
						DOUBLECLICK_INTERVAL / TOUCH_TIMER_CNT / SxTimer_GetData(SXTIMER_TIMER_BASE));
		SysMan_RegTouchSoundCB(KeySoundCB);
		SysMan_RegTouchTable(g_TouchTable, sizeof(g_TouchTable) / sizeof(TOUCH_OBJ));
		SxTimer_SetFuncActive(SX_TIMER_DET_TOUCH_ID, TRUE);
#endif
	}
}

void System_OnInputExit(void)
{
	//PHASE-2 : Close Lib or LibExt
	{
	}
	//PHASE-1 : Close Drv or DrvExt
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
UINT32 ACC_ShuntDownCnt = 0;
extern UINT8 ADAS_Turn_Cnt;
extern UINT8 Take_PIC_Cnt;
extern BOOL OpenSSID;
UINT8 Wifi_ssid_key = 0;
extern BOOL LINKOK_OPEN_SSID;
extern UINT8 Audio_COUNT;
extern BOOL OneKeyFormatAndReset;
extern BOOL PWRON_SRC_PWR_KEY;
extern BOOL PWRON_SRC_PWR_GSENSOR;
BOOL DOForOne = FALSE;
UINT16 TimeOut2ResetSystem = 0;
extern UINT32 VolDet_GetLOW_VOLUTE_ADC(void);
UINT32 OneSec_Count = 0;

void Timer_DoOneSec(void)		//van 2016.07.21
{
    OneSec_Count++;
	Audio_COUNT++;
	//ACC detected  //if ACC is unplug, after 1 min will be shutdown any way!!!!!
	//debug_msg("***************ACIsPlug = %d  ACC_ShuntDownCnt = %d\r\n", ACIsPlug,ACC_ShuntDownCnt);
	if(ACIsPlug == FALSE)
	{
        ++ACC_ShuntDownCnt;
		debug_msg("ACC_ShuntDownCnt = %d\r\n",ACC_ShuntDownCnt);

        if(ACC_ShuntDownCnt == 300 && PWRON_SRC_PWR_KEY == FALSE && PWRON_SRC_PWR_GSENSOR ==FALSE)
        {
	      Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
          debug_msg("****** f ACC is unplug, after 3S will be shutdown any way!!!!\r\n");
        }

        if(PWRON_SRC_PWR_KEY == TRUE && Audio_COUNT >= 180)//shutdown after 3min
        {
            Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
            debug_msg("****** shutdown after 3min for PWRON KEY\r\n");
        }

        if( PWRON_SRC_PWR_GSENSOR == TRUE && ACC_ShuntDownCnt == 64)
        {
            Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
            debug_msg("****** shutdown after 1min for PWRON GSENSOR\r\n");
        }

	}
	else
		ACC_ShuntDownCnt = 0;
	//===============================


	//Watch Dog config
	#if(_WATCH_DOG_FUNC_ == ENABLE)
    static BOOL wdtinit = FALSE;
    if(wdtinit==FALSE)
    {
		wdtinit = TRUE;
		WDT_Init();
	}
	#endif

	#if (_WATCH_DOG_FUNC_ == ENABLE)
	WDT_Trigger(WDT_MOVIE_MODE);
	WDT_SensorVDSignal_Check(10);
	#endif
#if 0
	//ADAS press count, do nothing in 12s
	if(ADAS_Turn_Cnt%13 != 0)
	{
		ADAS_Turn_Cnt++;
	}
	else
		ADAS_Turn_Cnt = 0;
	//take pic press count,do nothing in 2s
	if(Take_PIC_Cnt%2 != 0)
	{
		Take_PIC_Cnt++;
	}
	else
		Take_PIC_Cnt = 0;
	if(OpenSSID == TRUE || LINKOK_OPEN_SSID == TRUE)
  	{
  		Wifi_ssid_key++;
  		if(Wifi_ssid_key>=8)
		{
			//debug_err(("========:Wifi_ssid_key %d===========",Wifi_ssid_key));
			UxStatic_SetData(&UIMenuWndWiFiModuleLink_StaticTXT_SSIDCtrl,STATIC_VALUE,STRID_NULL);
			UxStatic_SetData(&UIMenuWndWiFiModuleLink_StaticTXT_KeyCtrl,STATIC_VALUE,STRID_NULL);
			Ux_Redraw();
			Wifi_ssid_key =0;
			OpenSSID = FALSE;
			LINKOK_OPEN_SSID = FALSE;
		}
  	}
#endif
	#if 0
	 if(BI_TimeCount < 10 && PWROFF_PRESS == FALSE)
  {
	if(UI_GetData(FL_BEEP)== BEEP_ON && UI_GetData(FL_CardStatus) == CARD_REMOVED)
	{
		if(BiBi_NCARD_Flag==0)
		{
			BiBI_Audio_CRTL_ON();
			BiBi_NCARD_Flag=1;
			BI_TimeCount++;
		}
		else
		{
			BiBI_Audio_CRTL_OFF();
			BiBi_NCARD_Flag=0;
		}
	}
	else
	{
		if (UI_GetData(FL_BEEP)== BEEP_ON&&System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE&&ACIsPlug == TRUE)
		{
			if((gMovData.State == MOV_ST_REC)||(UIFlowWndWiFiMovie_GetStatus() ==WIFI_MOV_ST_RECORD))
		{
		BiBI_Audio_CRTL_OFF();
		BiBi_Flag=0;
		BIBI_Delay = 0;
		}
		else
		{
			if((BIBI_Delay++) < 4)
				return;
			if(BiBi_Flag==0&&Audio_COUNT>2)//15sec  =30
			{
				BiBI_Audio_CRTL_ON();
				BiBi_Flag=1;
				Audio_COUNT=0;
				BI_TimeCount++;
			}
			else
			{
				BiBI_Audio_CRTL_OFF();
				BiBi_Flag=0;
			}
		}
	  }
		else
		{
			BiBI_Audio_CRTL_OFF();
			BiBi_Flag=0;
		}
	}
  }
  else
  {
  	BiBI_Audio_CRTL_OFF();
	BiBi_Flag=0;
	if(PWR_PRESS_Cnt++>8)
		PWROFF_PRESS = FALSE;
	BIBI_Delay = 0;

  }
  #endif
	if(OneKeyFormatAndReset == TRUE)
	{
		//强行恢复默认值
		if(DOForOne == FALSE)
		{
			debug_msg("on force to format and reset system!!!!!!!!!\r\n");
			FlowMovie_StopRec();
			//UINet_WifiResetSSIDPASSPHRASE();
			Reset_MenuInfo();
			Save_MenuInfo();
            if (System_GetState(SYS_STATE_CARD)  != CARD_REMOVED)
			{
				BKG_PostEvent(NVTEVT_BKW_FORMAT);
				Ux_OpenWindow(&UIFlowWndWaitMomentCtrl,2,UIFlowWndWaitMoment_StatusTXT_Msg_STRID_PLEASE_WAIT,FLOWWRNMSG_TIMER_5SEC);
				BKG_PostEvent(NVTEVT_BKW_FORMAT_CARD);
			}
			DOForOne = TRUE;
			debug_msg("system wait to reboot!!!!!!!!!\r\n");
		}

		if(++TimeOut2ResetSystem > 5)
        {
            GxSystem_SWResetNOW();
        }
	}
  //debug_err(("========BI_TimeCount: %d===========",BI_TimeCount));
}
void UI_DetPwrKey(void)
{
    static UINT16 Battery_Low = 0;
    static BOOL Battery_Low_Shutdown = FALSE;

	if (!UI_IsForceLock()) {
		GxKey_DetPwrKey();
	}
    //debug_msg("111Timer do one Sec...VolDet_GetLOW_VOLUTE_ADC=%d\r\n",VolDet_GetLOW_VOLUTE_ADC());
    if(VolDet_GetLOW_VOLUTE_ADC()<100 && OneSec_Count>= 10)
    {
        Battery_Low++;
        if(Battery_Low >= 5 && ACIsPlug == FALSE && Battery_Low_Shutdown == FALSE)//1S li
        {
           Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);//电容版本检测长电断开关机
           Battery_Low_Shutdown =TRUE;
           debug_msg("******Battery_Low shutdown \r\n");
           //debug_msg("111Timer do one Sec...VolDet_GetLOW_VOLUTE_ADC=%d\r\n",VolDet_GetLOW_VOLUTE_ADC());
        }
    }
    else
    Battery_Low = 0;

}

void UI_DetNormalKey(void)
{
	static UINT32 keyDetectCount = 0;
	if (!UI_IsForceLock()) {
		GxKey_DetNormalKey();

		if (keyDetectCount < 4) {
			keyDetectCount++;
		}

		if (keyDetectCount == 2) {
			//recover the key detection after system boot up
			GxKey_SetFirstKeyInvalid(KEY_PRESS, 0);
		}
	}
}

void UI_DetStatusKey(void)
{
//check mode key
	if ((!UI_IsForceLock()) && (!UI_IsForceLockStatus())) {
		GxKey_DetStatusKey();
	}
}


#if (GSENSOR_FUNCTION == ENABLE)

BOOL IsLockFile = FALSE;
BOOL GSENSOR_TRIGGER=FALSE;
BOOL GSENSOR_TRIGGER_LOCK=FALSE;
UINT32 GSENSOR_TRIGGER_COUNT=0;
extern VControl UIMenuWndWiFiModuleLinkOK_Status_LOCKCtrl;
extern VControl UIMenuWndWiFiModuleLink_Status_LOCKCtrl;
extern VControl UIFlowWndMovie_Status_LOCKCtrl;//Status type

void UI_DetGsensor(void)	///van 2016.07.21
{
    BOOL   bGSensorStatus=FALSE;
    Gsensor_Data GS_Data = {0};
	static BOOL LockFirstFlie = FALSE;
	//static BOOL TMT_FLAG =FALSE;

	if(gMovData.State ==  MOV_ST_MENU) //fan
		return;

    bGSensorStatus = GSensor_GetStatus(&GS_Data);
//debug_msg("SYS_STATE_CURRMODE=%d,bGSensorStatus=%d,	 gMovData.State = %d,		UIFlowWndWiFiMovie_GetStatus() = %d 		+++\r\n",System_GetState(SYS_STATE_CURRMODE) ,bGSensorStatus, gMovData.State, WiFiCmd_GetStatus());
    if (bGSensorStatus == TRUE && (System_GetState(SYS_STATE_CARD)  != CARD_REMOVED))
	{
		//debug_msg("gMovData.State = %d,		UIFlowWndWiFiMovie_GetStatus() = %d 		+++\r\n", gMovData.State, WiFiCmd_GetStatus());
		//if detected gsensor crash, add the code here    van.2016.07.21
		if((System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE )&& ((gMovData.State ==  MOV_ST_REC) || (WiFiCmd_GetStatus()  == WIFI_MOV_ST_RECORD)))
		{
            if (WiFiCmd_GetStatus()== WIFI_MOV_ST_LVIEW&&(GSENSOR_TRIGGER==FALSE))
		 	{
		 	//Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_REC, 2, 1, 1);
			//Delay_DelayMs(2500);
			Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);

			debug_err(("====WIFI WIFI_MOV_ST_LVIEW*****************************88\r\n"));//20181222-wifi view gsensor star rec
		 	}
		  else
			{
			debug_msg("Only lock the file !!!!!\r\n");
			//MovRec_SetCrash();
            ImageApp_MovieMulti_SetCrash(_CFG_REC_ID_1,TRUE);
			FlowMovie_IconDrawLock(&UIFlowWndMovie_Status_LOCKCtrl, TRUE);
			FlowMovie_IconDrawLock(&UIMenuWndWiFiModuleLink_Status_LOCKCtrl, TRUE);
            FlowMovie_IconDrawLock(&UIMenuWndWiFiModuleLinkOK_Status_LOCKCtrl, TRUE);

            }
		}
		else if((System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE )&&((gMovData.State == MOV_ST_VIEW)||(  WiFiCmd_GetStatus()==WIFI_MOV_ST_LVIEW)))
		{
		 if (WiFiCmd_GetStatus()== WIFI_MOV_ST_LVIEW&&(GSENSOR_TRIGGER==FALSE))
		 	{
		 	//Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_REC, 2, 1, 1);
			//Delay_DelayMs(2500);

			Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
			debug_err(("====WIFI GSNEOSR_REC*************************88\r\n"));
		 	}
		  else
			{
			Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
			debug_msg("Start to REC and lock the file !!!!!\r\n");
			ImageApp_MovieMulti_SetCrash(_CFG_REC_ID_1,TRUE);

			FlowMovie_IconDrawLock(&UIFlowWndMovie_Status_LOCKCtrl, TRUE);
			FlowMovie_IconDrawLock(&UIMenuWndWiFiModuleLink_Status_LOCKCtrl, TRUE);
            FlowMovie_IconDrawLock(&UIMenuWndWiFiModuleLinkOK_Status_LOCKCtrl, TRUE);

			}
			GSENSOR_TRIGGER=TRUE;
			GSENSOR_TRIGGER_LOCK=TRUE;
		}
		debug_msg("Lock the file!!!!!!!\r\n");
	}


	//gsensor open, auto REC and lock the file

	if(GxSystem_GetPowerOnSource() == GX_PWRON_SRC_PWR_VBAT&& LockFirstFlie == FALSE && ACIsPlug == FALSE)
	{

		if(gMovData.State !=  MOV_ST_REC && (System_GetState(SYS_STATE_CARD)  != CARD_REMOVED) && IsWifiOpen == TRUE)
		{

		debug_msg("Boot triggered by gsensor!!!!!!!!\r\n");
			//Ux_PostEvent(NVTEVT_KEY_SHUTTER2 , 1, NVTEVT_KEY_PRESS);
            ImageApp_MovieMulti_SetCrash(_CFG_REC_ID_1,TRUE);
			FlowMovie_IconDrawLock(&UIFlowWndMovie_Status_LOCKCtrl, TRUE);
			FlowMovie_IconDrawLock(&UIMenuWndWiFiModuleLink_Status_LOCKCtrl, TRUE);
            FlowMovie_IconDrawLock(&UIMenuWndWiFiModuleLinkOK_Status_LOCKCtrl, TRUE);

			LockFirstFlie = TRUE;
		}
	}
		if((System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE &&GSENSOR_TRIGGER==TRUE&&gMovData.State == MOV_ST_REC)||( WiFiCmd_GetStatus() ==WIFI_MOV_ST_RECORD&&GSENSOR_TRIGGER==TRUE ))
		{
		  GSENSOR_TRIGGER_COUNT++;
		  if(GSENSOR_TRIGGER_COUNT>3&&GSENSOR_TRIGGER_LOCK==TRUE)
		  	{
		  	 GSENSOR_TRIGGER=FALSE;
		  	   GSENSOR_TRIGGER_COUNT=0;
			  	GSENSOR_TRIGGER_LOCK=FALSE;
		       FlowMovie_IconDrawLock(&UIFlowWndMovie_Status_LOCKCtrl,TRUE);
				FlowMovie_IconDrawLock(&UIMenuWndWiFiModuleLink_Status_LOCKCtrl,TRUE);
                FlowMovie_IconDrawLock(&UIMenuWndWiFiModuleLinkOK_Status_LOCKCtrl, TRUE);

            ImageApp_MovieMulti_SetCrash(_CFG_REC_ID_1,TRUE);
		    }

		}
	//倒车检测
	#if 0//(_SENSORLIB2_ != _SENSORLIB2_OFF_)

	if(GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_DUAL)
	{
		if(gpio_getPin(C_GPIO_12) == 0 && TMT_FLAG ==FALSE)
		{
			Ux_SendEvent(0,NVTEVT_EXE_DUALCAM, 1, DUALCAM_BEHIND);
			TMT_FLAG = TRUE;
		}
		else if(gpio_getPin(C_GPIO_12) == 1 && TMT_FLAG ==TRUE)
		{
			Ux_SendEvent(0,NVTEVT_EXE_DUALCAM, 1, DUALCAM_BOTH);
			TMT_FLAG = FALSE;
		}
	}

	#endif

}

void UI_DetCustom1Key(void)
{
	BOOL   bGSensorStatus = FALSE;
	Gsensor_Data GS_Data = {0};

	bGSensorStatus = GSensor_GetStatus(&GS_Data);
	if (bGSensorStatus == TRUE) {
		Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, NVTEVT_KEY_PRESS);
	}
	//DBGD(bGSensorStatus);
}
#endif

/////////////////////////////////////////////////////////////////////////////
extern void System_ResetDetCloseLenCount(void);
extern void System_ResetPowerSaveCount(void);
BOOL g_bConsumeStatus = FALSE;

INT32 System_UserKeyFilter(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 key = evt;
	#if (POWER_KEY_LONG_PRESS == ENABLE)
	static UINT32 power_key_press_count = 0;
	#endif

	if (IN_RANGE_EVENT(key, NVTEVT_KEY_STATUS_START, NVTEVT_KEY_STATUS_END)) { //Status class
		System_ResetDetCloseLenCount();
		System_ResetPowerSaveCount();
#if (STATUSKEY_FUNCTION == ENABLE)
		DBG_IND("^Bgroup key event=0x%x\r\n", key);

		if (g_bConsumeStatus) {
			g_bConsumeStatus = 0;
		} else {
			UINT32 uiDscMode = Input_Key2Mode(key);
			DBG_IND("^YDscMode=0x%x\r\n", uiDscMode);
			//FlowMode_OnKeyMode(uiDscMode);
			UI_Switch_DscMode(uiDscMode, DSCMODE_SWITCH_FORCE, DSCMODE_SWITCHDIR_DONT_CARE);
		}
#endif
		return NVTEVT_CONSUME;
	} else if (IN_RANGE_EVENT(key, NVTEVT_KEY_BUTTON_START, NVTEVT_KEY_BUTTON_END)) { //Button class
		if (IN_RANGE_EVENT(key, NVTEVT_KEY_PRESS_START, NVTEVT_KEY_PRESS_END)) { //Press key
			if (key == NVTEVT_KEY_POWER) {
				return NVTEVT_CONSUME;
			}

			System_ResetDetCloseLenCount();
			if (GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) > 1) { // drop key if sleep level > 1
				//NOTE! do not set GXPWR_CTRL_SLEEP_RESET before get GXPWR_CTRL_SLEEP_LEVEL,
				//  because GXPWR_CTRL_SLEEP_LEVEL status maybe alter by sleep wakeup flow~
				System_ResetPowerSaveCount();
				return NVTEVT_CONSUME;
			} else {
				System_ResetPowerSaveCount();
				if (m_uiAnyKeyUnlockEn) {
					Ux_PostEvent(NVTEVT_KEY_PRESS_START, 1, key);
					return NVTEVT_CONSUME;
				}
				return NVTEVT_PASS;
			}
		} else if (IN_RANGE_EVENT(key, NVTEVT_KEY_CONTINUE_START, NVTEVT_KEY_CONTINUE_END)) { //Contine key
			if (key == NVTEVT_KEY_POWER_CONT) {
				#if (POWER_KEY_LONG_PRESS == ENABLE)
				power_key_press_count++;
				if (power_key_press_count > 10) { // about 1.2 sec
					System_PowerOff(SYS_POWEROFF_NORMAL);
				}
				#endif
				return NVTEVT_CONSUME;
			}

			System_ResetDetCloseLenCount();
			System_ResetPowerSaveCount();
			return NVTEVT_PASS;
		} else if (IN_RANGE_EVENT(key, NVTEVT_KEY_RELEASE_START, NVTEVT_KEY_RELEASE_END)) { //Release key
			if (key == NVTEVT_KEY_POWER_REL) {
				System_ResetDetCloseLenCount();
				if (GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) > 1) { // drop key if sleep level > 1
					//NOTE! do not set GXPWR_CTRL_SLEEP_RESET before get GXPWR_CTRL_SLEEP_LEVEL,
					//  because GXPWR_CTRL_SLEEP_LEVEL status maybe alter by sleep wakeup flow~
					System_ResetPowerSaveCount();
				} else {
					//#NT#2016/03/07#KCHong -begin
					//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
					MovieTLLPR_Process(TIMELAPSE_FROM_PWRKEY);
#endif
					//#NT#2016/03/07#KCHong -end
					#if (POWER_KEY_LONG_PRESS == ENABLE)
					power_key_press_count = 0;
					#else
					System_PowerOff(SYS_POWEROFF_NORMAL);
					#endif
				}
				return NVTEVT_CONSUME;
			}

			System_ResetDetCloseLenCount();
			System_ResetPowerSaveCount();
			return NVTEVT_PASS;
		}
	}
	return NVTEVT_PASS;
}

#if defined(_TOUCH_ON_)
INT32 System_UserTouchFilter(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray)
{
	INT32 Ret = NVTEVT_PASS;
	//DBG_DUMP("^Bevt=%d, point=(%d, %d)\r\n", evt - NVTEVT_PRESS, paramArray[0], paramArray[1]);
	if (evt >= NVTEVT_PRESS && evt <= NVTEVT_SLIDE_DOWN) {
		if (GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) > 1) { // drop key if sleep level > 1
			//NOTE! do not set GXPWR_CTRL_SLEEP_RESET before get GXPWR_CTRL_SLEEP_LEVEL,
			//  because GXPWR_CTRL_SLEEP_LEVEL status maybe alter by sleep wakeup flow~
			System_ResetPowerSaveCount();
			return NVTEVT_CONSUME;
		} else {
			System_ResetPowerSaveCount();
			return NVTEVT_PASS;
		}
	}
	return Ret;
}
#endif
