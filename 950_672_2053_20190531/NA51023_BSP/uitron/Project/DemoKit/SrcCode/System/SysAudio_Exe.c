/*
    System Audio Callback

    System Callback for Audio Module.

    @file       SysLens_Audio.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "GxSound.h"
#include "SMediaPlayAPI.h"
#include "Dx.h"
#include "DxType.h"
#include "DxApi.h"
#include "DxSound.h"
#include "Audio.h"
#if(UI_FUNC==ENABLE)
#include "SoundData.h"
#endif

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysAudioExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (AUDIO_FUNC == ENABLE)

void System_SetAudioOutput(void);
void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2);
DX_HANDLE gDevAudObj = 0;

#if (LCD1_FUNC == ENABLE)
extern DX_HANDLE gDevLCDObj;
#endif
#if (TV_FUNC == ENABLE)
extern DX_HANDLE gDevTVObj;
#endif
#if (HDMI_FUNC == ENABLE)
extern DX_HANDLE gDevHDMIObj;
#endif

static AUDIO_SETTING gProjCommDxSndSet = {0};
static AUDIO_DEVICE_OBJ gProjCommDxSndDev = {0};

int SX_TIMER_DET_AIN_ID = -1;
void Audio_DetAudInsert(void);
#if (LINE_INSERT_FUNCTION == ENABLE)
SX_TIMER_ITEM(Audio_DetAudInsert, Audio_DetAudInsert, 25, FALSE)
static UINT32 audInsertMask = LOUT_MASK;//AOUT_DEFAULT_ATTACH_DEVICE; ////510 only LINE
static BOOL gDevAudOut = TRUE; //FALSE:SPK, TRUE:LINE //510 only LINE
#endif
static UINT32 gDevAudSetting = 0;


void System_OnAudioInit(void)
{
	gDevAudObj = Dx_GetObject(DX_CLASS_AUDIO_EXT);
	TM_BOOT_BEGIN("audio", "init");
	//PHASE-1 : Init & Open Drv or DrvExt
	{
		if (gDevAudObj) {
			Dx_Init(gDevAudObj, (void *)&gProjCommDxSndSet, 0, DXSOUND_VER);
			//fix for CID 44938 - begin
			if (Dx_Open(gDevAudObj) != DX_OK) {
				DBG_ERR("DxAudio open fail!\r\n");
				return;
			}
			//fix for CID 44938 - end
			Dx_Control(gDevAudObj, DXSOUND_CAP_SET_DEVINITDATA, (UINT32)&gProjCommDxSndDev, (UINT32)&gProjCommDxSndSet);
			Dx_Control(gDevAudObj, DXSOUND_CAP_SET_CODEC, (UINT32)NULL, AUDIO_CODECSEL_DEFAULT);
			#if (AUDIO_IN_TOTAL_16_LVL == ENABLE)
			Dx_Control(gDevAudObj, DXSOUND_CAP_SET_TOTAL_GAIN_LEVEL, (UINT32)AUDIO_RECG_LEVEL16, 0);
			#endif
			GxSound_Open(gDevAudObj, Sound_CB);

		}
#if(UI_FUNC==ENABLE)
		UISound_RegTable();
#endif
#if (LINE_INSERT_FUNCTION == ENABLE)
		SX_TIMER_DET_AIN_ID = SxTimer_AddItem(&Timer_Audio_DetAudInsert);
		SxTimer_SetFuncActive(SX_TIMER_DET_AIN_ID, TRUE);
#endif
	}
	//PHASE-2 : Init & Open Lib or LibExt
	{
		//config audio Output
		System_SetAudioOutput();
	}
	//    aud_setTotalVolLevel(AUDIO_VOL_LEVEL8);
	TM_BOOT_END("audio", "init");
}

void System_OnAudioExit(void)
{
	//PHASE-2 : Close Lib or LibExt
	{

	}
	//PHASE-1 : Close Drv or DrvExt
	{
		if (gDevAudObj) {
			GxSound_Close();
		}
	}
}

///////////////////////////////////////////////////////////////////////
void Audio_DetAudInsert(void)
{
#if (LINE_INSERT_FUNCTION == ENABLE)
	//DBG_MSG("DetAudio\r\n");
	DX_HANDLE pAudDev1 = Dx_GetObject(DX_CLASS_AUDIO_EXT);
	GxSound_DetInsert((UINT32)pAudDev1, 0);
#endif
	//DBG_MSG("\r\n");
}

///////////////////////////////////////////////////////////////////////

void System_SetAudioOutput(void)
{
#if (LINE_INSERT_FUNCTION == ENABLE)
	//=========================================================================
	// Change Audio Output according to LINE cable inserted or not
	//=========================================================================
	DBG_IND("\r\n");
	if (audInsertMask == SPK_MASK) { //only speaker
		// set audio output device entry for LCD/SPK case
		DBG_DUMP("Audio Out = SPK\r\n");
		gDevAudOut = FALSE;
		GxSound_SetOutDevConfigIdx(0);
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(0);
#endif
	} else if (audInsertMask & LOUT_MASK) { //with lineout
		// set audio output device entry for TV/LINE case
		DBG_DUMP("Audio Out = LINE\r\n");
		gDevAudOut = TRUE;
		GxSound_SetOutDevConfigIdx(1);
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(1);
#endif
	}
#elif (DISPLAY_FUNC == ENABLE)
	//=========================================================================
	// Change Audio Output according to TV/HDMI cable inserted or not
	//=========================================================================
	DX_HANDLE VideoDevObj1;
	DX_HANDLE VideoDevObj2;
	VideoDevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
#if (DISPLAY2_FUNC == ENABLE)
	VideoDevObj2 = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
#else
	VideoDevObj2 = (DX_HANDLE)0;
#endif
#if (LCD1_FUNC == ENABLE)
	if ((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == 0)) { //case 1
		// set audio output device entry for LCD case
		GxSound_SetOutDevConfigIdx(0);
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(0);
#endif
	}
#endif
#if (TV_FUNC == ENABLE)
	if ((VideoDevObj1 == gDevTVObj) && (VideoDevObj2 == 0)) { //case 2
		// set audio output device entry for TV case
		GxSound_SetOutDevConfigIdx(1);
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(1);
#endif
	}
#endif
#if (HDMI_FUNC == ENABLE)
	if ((VideoDevObj1 == gDevHDMIObj) && (VideoDevObj2 == 0)) { //case 3
		// set audio output device entry for HDMI case
		GxSound_SetOutDevConfigIdx(2);
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(2);
#endif
	}
#endif
#if (DISPLAY2_FUNC == ENABLE)
#if ((LCD1_FUNC == ENABLE) && (TV_FUNC == ENABLE) && (DUALDISP_FUNC == ENABLE))
	if ((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == gDevTVObj)) { //case 4
		// set audio output device entry for LCD & TV case
		GxSound_SetOutDevConfigIdx(3);
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(3);
#endif
	}
#endif
#if ((LCD1_FUNC == ENABLE) && (HDMI_FUNC == ENABLE) && (DUALDISP_FUNC == ENABLE))
	if ((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == gDevHDMIObj)) { //case 5
		// set audio output device entry for LCD & HDMI case
		GxSound_SetOutDevConfigIdx(4);
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(4);
#endif
	}
#endif
#endif

#else

	//=========================================================================
	// Change Audio Output to default
	//=========================================================================
		GxSound_SetOutDevConfigIdx(0);
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(0);
#endif

#endif
}

///////////////////////////////////////////////////////////////////////
//Device flow event

INT32 System_OnAudioInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_DUMP("^Y===================================== audio plug [%s]\r\n", "LINE");
#if (LINE_INSERT_FUNCTION == ENABLE)
	audInsertMask |= 0x02; //line insert
	System_SetAudioOutput();
#endif
	return NVTEVT_CONSUME;
}

INT32 System_OnAudioRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_DUMP("^Y===================================== audio unplug [%s]\r\n", "LINE");
#if (LINE_INSERT_FUNCTION == ENABLE)
	audInsertMask &= ~0x02; //line remove
	System_SetAudioOutput();
#endif
	return NVTEVT_CONSUME;
}


INT32 System_OnAudioAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	//  AUDIO: if(boot)->play boot sound
	//  AUDIO: if(boot)->start audio srv
	return NVTEVT_CONSUME;
}

INT32 System_OnAudioDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

INT32 System_OnAudioMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

INT32 System_OnAudioInput(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

INT32 System_OnAudioVol(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

BOOL System_GetAudioOut(void)
{
#if (LINE_INSERT_FUNCTION == ENABLE)
	return gDevAudOut;
#else
	return 0;
#endif
}

BOOL System_GetAudioMode(void)
{
	return 0;
}

BOOL System_GetAudioInput(void)
{
	return 0;
}

UINT32 System_GetAudioVol(void)
{
	return 0;
}

INT32 System_PutAudioData(UINT32 did, UINT32 addr, UINT32 *ptrsize)
{
	return 0;
}

//#NT#2016/09/08#HM Tseng -begin
//#NT#Support audio channel
UINT32 System_GetAudioChannel(void)
{
	return 0;
}
//#NT#2016/09/08#HM Tseng -end

UINT32 System_SwtichAudioSetting(UINT32 setting)
{
#if (AUDIO_SWITCH_SETTING == ENABLE)
	if (gDevAudObj) {
		Dx_Control(gDevAudObj, DXSOUND_CAP_SWITCH_AUD_SETTING, setting, 0);
	}
#endif
	gDevAudSetting = setting;

	return 0;
}

UINT32 System_GetAudioSetting(void)
{
	return gDevAudSetting;
}


#endif

