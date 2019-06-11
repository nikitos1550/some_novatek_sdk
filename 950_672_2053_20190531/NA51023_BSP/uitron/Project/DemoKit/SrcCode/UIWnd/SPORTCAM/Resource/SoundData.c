/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SoundData.c
    @ingroup    mIPRJAPKey

    @brief      Keypad tone, Startup... sound data
                This file contain the PCM (ADPCM) data of keypad tone, Startup...

    @note       Nothing.

    @date       2005/01/23
*/

/** \addtogroup mIPRJAPKey */
//@{
#include "Type.h"
#include "SoundData.h"
#include "GxSound.h"
#include "Debug.h"
#include "UIInfo.h"
#include "Audio.h"
#if (_BOARD_DRAM_SIZE_ == 0x04000000)
#include "SoundData_PowerOn_16K.c"
#include "SoundData_Shutter_16K.c"
#include "SoundData_Key_16K.c"
#if (_ADAS_FUNC_ == ENABLE)
#include "SoundData_LDWS_16K.c"
#include "SoundData_FCW_16K.c"
#include "SoundData_SnG_16K.c"
#endif
#if (_DDD_FUNC_ == ENABLE)
#include "SoundData_DDD1_16K.c"
#include "SoundData_DDD2_16K.c"
#include "SoundData_DDD3_16K.c"
#endif
#else
#include "SoundData_PowerOn_32K.c"
#include "SoundData_Shutter_32K.c"
#include "SoundData_Key_32K.c"
#if (_ADAS_FUNC_ == ENABLE)
#include "SoundData_LDWS_32K.c"
#include "SoundData_FCW_32K.c"
#include "SoundData_SnG_32K.c"
#endif
#if (_DDD_FUNC_ == ENABLE)
#include "SoundData_DDD1_32K.c"
#include "SoundData_DDD2_32K.c"
#include "SoundData_DDD3_32K.c"
#endif
#endif

#if (_BOARD_DRAM_SIZE_ == 0x04000000)
// 16K, mono, note that sound data size must be multiple of 8 for DMA usage!
static const SOUND_DATA gDemo_Sound[DEMOSOUND_SOUND_MAX_CNT] = {
	{ 0 },
	{ uiSoundKey,			(sizeof(uiSoundKey)/8)*8,			AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_KEY_TONE		},
#if(PHOTO_MODE==ENABLE)
	{ uiSoundShutter,		(sizeof(uiSoundShutter)/8)*8,		AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_SHUTTER_TONE	},
#endif
	{ uiSoundPowerOn,		(sizeof(uiSoundPowerOn)/8)*8,		AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_POWERON_TONE	},
#if (_ADAS_FUNC_ == ENABLE)
	{ uiSoundLdws,			(sizeof(uiSoundLdws)/8)*8,			AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_LDWS_TONE		},
	{ uiSoundFcw,			(sizeof(uiSoundFcw)/8)*8,			AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_FCS_TONE		},
	{ uiSoundSnG,			(sizeof(uiSoundSnG)/8)*8,			AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_SNG_TONE		},
#endif  // #if (_ADAS_FUNC_ == ENABLE)
//#NT#2016/07/20#Brain Yen -begin
//#NT#DDD alarm sound
#if (_DDD_FUNC_ == ENABLE)
	{ uiSound_DDDWarning1,	(sizeof(uiSound_DDDWarning1)/8)*8,	AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_DDDWARNING1_TONE	},
	{ uiSound_DDDWarning2,	(sizeof(uiSound_DDDWarning2)/8)*8,	AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_DDDWARNING2_TONE	},
	{ uiSound_DDDWarning3,	(sizeof(uiSound_DDDWarning3)/8)*8,	AUDIO_SR_16000,	FALSE,	DEMOSOUND_SOUND_DDDWARNING3_TONE	},
#endif  //#if (_DDD_FUNC_ == ENABLE)
//#NT#2016/07/20#Brain Yen -end
};
#else
// 32K, mono, note that sound data size must be multiple of 8 for DMA usage!
static const SOUND_DATA gDemo_Sound[DEMOSOUND_SOUND_MAX_CNT] = {
	{ 0 },
	{ uiSoundKey,			(sizeof(uiSoundKey)/8)*8,			AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_KEY_TONE		},
#if(PHOTO_MODE==ENABLE)
	{ uiSoundShutter,		(sizeof(uiSoundShutter)/8)*8,		AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_SHUTTER_TONE	},
#endif
	{ uiSoundPowerOn,		(sizeof(uiSoundPowerOn)/8)*8,		AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_POWERON_TONE	},
#if (_ADAS_FUNC_ == ENABLE)
	{ uiSoundLdws,			(sizeof(uiSoundLdws)/8)*8,			AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_LDWS_TONE		},
	{ uiSoundFcw,			(sizeof(uiSoundFcw)/8)*8,			AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_FCS_TONE		},
	{ uiSoundSnG,			(sizeof(uiSoundSnG)/8)*8,			AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_SNG_TONE		},
#endif  // #if (_ADAS_FUNC_ == ENABLE)
//#NT#2016/07/20#Brain Yen -begin
//#NT#DDD alarm sound
#if (_DDD_FUNC_ == ENABLE)
	{ uiSound_DDDWarning1,	(sizeof(uiSound_DDDWarning1)/8)*8,	AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_DDDWARNING1_TONE	},
	{ uiSound_DDDWarning2,	(sizeof(uiSound_DDDWarning2)/8)*8,	AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_DDDWARNING2_TONE	},
	{ uiSound_DDDWarning3,	(sizeof(uiSound_DDDWarning3)/8)*8,	AUDIO_SR_32000,	FALSE,	DEMOSOUND_SOUND_DDDWARNING3_TONE	},
#endif  //#if (_DDD_FUNC_ == ENABLE)
//#NT#2016/07/20#Brain Yen -end
};
#endif

static volatile BOOL        bKeyToneEn = TRUE;

void UISound_RegTable(void)
{
	#if (AUDIO_FUNC == ENABLE)
	ER retV = E_OK;
	retV = GxSound_SetSoundTable
		   ((UINT32)DEMOSOUND_SOUND_MAX_CNT, (PSOUND_DATA)&gDemo_Sound[0]);
	if (retV != E_OK) {
		debug_err(("Set SoundData Fail:%d\r\n", retV));
	}
	#endif
}

void UISound_EnableKey(BOOL bEn)
{
	bKeyToneEn = bEn;
}

void UISound_Play(UINT32 index)
{
	if (bKeyToneEn == FALSE) {
		return;
	}

	#if (AUDIO_FUNC == ENABLE)
	//#NT#2015/10/16#Do not play sound if previous sound is still playing#KCHong
	//if (UI_GetData(FL_BEEP)== BEEP_ON)
	if (UI_GetData(FL_BEEP) == BEEP_ON && GxSound_IsPlaying() == FALSE) {
#if _TODO
		UINT32 AdasDebugMode = 0;
		if ((AdasDebugMode = ADAS_IsAdasDebug()) != 0) {
			// This is a test code for adas debug mode, mute alarm sound
			GxSound_SetVolume((AdasDebugMode & 0x7f));
		}
#endif

		GxSound_Play(index);
	}
	#endif
}
//@}
