/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       DetKey.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan key, modedial
                Scan key, modedial

    @note       Nothing.

    @date       2017/05/02
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxInput.h"
#include "Debug.h"
#include "KeyDef.h"
#include "rtc.h"
#include "HwPower.h"
#include "Delay.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ADC related
////////////////////////////////////////////////////////////////////////////////
#if (ADC_KEY == ENABLE)
#define VOLDET_KEY_ADC_RANGE            20
#define VOLDET_KEY_ADC_LVL0             0		//DEL
#define VOLDET_KEY_ADC_LVL1             119   	//UP
#define VOLDET_KEY_ADC_LVL2             690    	//
#define VOLDET_KEY_ADC_LVL3             400   	//
#define VOLDET_KEY_ADC_LVL4             590  	//
#define VOLDET_KEY_ADC_LVL5             (557-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_TH               500   //

#define VOLDET_KEY_LVL_UNKNOWN           0xFFFFFFFF

#define VOLDET_KEY_LVL_ENTER             0
#define VOLDET_KEY_LVL_MENU              1
#define VOLDET_KEY_LVL_SHUTTER2          2
#define VOLDET_KEY_LVL_UP                3
#define VOLDET_KEY_LVL_DOWN              4
#define VOLDET_KEY_LVL_PLAYBACK          5
#endif

UINT8 Audio_COUNT=0;
UINT8 BI_TimeCount=0;

UINT32  uikeyCountUp=0;
UINT32  uikeyCountMenu=0;

BOOL KEY_ISPRESS_ACTIVE=FALSE;
UINT32 MODE_KEY_PRESS_RELEASE=0;
UINT32 MODE_KEY_PRESS=0;
 UINT32 VolDet_GetLOW_VOLUTE_ADC(void);

#if (ADC_KEY == ENABLE)
static UINT32 VolDet_GetKey1ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
	UINT32 uiADCValue;

	uiADCValue = adc_readData(ADC_CH_VOLDET_KEY1);
	// One-Shot Mode, trigger one-shot
	adc_triggerOneShot(ADC_CH_VOLDET_KEY1);

	return uiADCValue;
#else
	return adc_readData(ADC_CH_VOLDET_KEY1);
#endif
}

 UINT32 VolDet_GetLOW_VOLUTE_ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
	UINT32 uiADCValue;

	uiADCValue = adc_readData(ADC_CH_VOLDET_LOW_VOLUTE);
	// One-Shot Mode, trigger one-shot
	adc_triggerOneShot(ADC_CH_VOLDET_LOW_VOLUTE);

	return uiADCValue;
#else
	return adc_readData(ADC_CH_VOLDET_LOW_VOLUTE);
#endif

}
/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetKey1Level(void)
{
	static UINT32   uiRetKey1Lvl;
	UINT32          uiKey1ADC=0,uiCurKey2Lvl=0;

    uiKey1ADC = VolDet_GetKey1ADC();
	  // debug_msg("===============uiKey1ADC %d ====================\r\n", uiKey1ADC);

    if (uiKey1ADC <411)
	{
		Audio_COUNT=0;
	    BI_TimeCount=0;
      // debug_msg("===============uiKey1ADC %d ====================\r\n", uiKey1ADC);
	 }
    if (uiKey1ADC <411)
    {
        if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL0) && (uiKey1ADC < VOLDET_KEY_ADC_LVL0 + VOLDET_KEY_ADC_RANGE))
        {
        	//debug_msg("========VOLDET_KEY_LVL_MENU=======uiKey1ADC %d ====================\r\n", uiKey1ADC);
            uiCurKey2Lvl = VOLDET_KEY_LVL_MENU;
        }
        else if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL4-VOLDET_KEY_ADC_RANGE)&& (uiKey1ADC < VOLDET_KEY_ADC_LVL4 + VOLDET_KEY_ADC_RANGE))
        {
        	//debug_msg("======== VOLDET_KEY_LVL_ENTER=======uiKey1ADC %d ====================\r\n", uiKey1ADC);
            //uiCurKey2Lvl = VOLDET_KEY_LVL_ENTER;
        }
        else if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL3-VOLDET_KEY_ADC_RANGE)&& (uiKey1ADC < VOLDET_KEY_ADC_LVL3 + VOLDET_KEY_ADC_RANGE))
        {
       		//debug_msg("=========VOLDET_KEY_LVL_DOWN======uiKey1ADC %d ====================\r\n", uiKey1ADC);
            //uiCurKey2Lvl = VOLDET_KEY_LVL_DOWN;
        }
        else if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL2-VOLDET_KEY_ADC_RANGE)&& (uiKey1ADC < VOLDET_KEY_ADC_LVL2 + VOLDET_KEY_ADC_RANGE))
        {
        	//debug_msg("=========VOLDET_KEY_LVL_PLAYBACK======uiKey1ADC %d ====================\r\n", uiKey1ADC);
            //uiCurKey2Lvl = VOLDET_KEY_LVL_PLAYBACK;
        }
        else if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL1-VOLDET_KEY_ADC_RANGE)&& (uiKey1ADC < VOLDET_KEY_ADC_LVL1 + VOLDET_KEY_ADC_RANGE))
        {
        	//debug_msg("==========VOLDET_KEY_LVL_UP=====uiKey1ADC %d ====================\r\n", uiKey1ADC);
            uiCurKey2Lvl = VOLDET_KEY_LVL_UP;
        }
        else
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
        }
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

	uiRetKey1Lvl = uiCurKey2Lvl;

	return uiRetKey1Lvl;
}

/**
  Detect Mode Switch state.

  Detect Mode Switch state.

  @param void
  @return UINT32 Mode Switch state (DSC Mode)
*/
#endif

////////////////////////////////////////////////////////////////////////////////
// GPIO related

//static BOOL g_bIsShutter2Pressed = FALSE;

/**
  Delay between toggle GPIO pin of input/output

  Delay between toggle GPIO pin of input/output

  @param void
  @return void
*/
static void DrvKey_DetKeyDelay(void)
{
	gpio_readData(0);
	gpio_readData(0);
	gpio_readData(0);
	gpio_readData(0);
}

void DrvKey_Init(void)
{
}
/**
  Detect normal key is pressed or not.

  Detect normal key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
UINT32  uikeyCountMenuLong=0;
BOOL OneKeyFormatAndReset = FALSE;
UINT32 DrvKey_DetNormalKey(void)
{
    UINT32 uiKeyCode = 0;

	#if (ADC_KEY == ENABLE)
	if (VolDet_GetKey1Level() == VOLDET_KEY_LVL_MENU)
	{
		uikeyCountMenu++;
        uikeyCountMenuLong++;//10s = 525   15S = 774   20S = 1027

//CHKPNT;
//debug_msg("uikeyCountMenu====%d====",uikeyCountMenu);
		MODE_KEY_PRESS_RELEASE=4;
		if(uikeyCountMenu>30)
		uiKeyCode |= FLGKEY_MENU;
	 if(uikeyCountMenuLong > 525)
	  	OneKeyFormatAndReset = TRUE;

	}
	else
	{
		if(uikeyCountMenu>=2)
		{
		  if(uikeyCountMenu<=30)
		  {
		  	uiKeyCode |= FLGKEY_DOWN;
		  }
		  else
		  {
		    uiKeyCode |= FLGKEY_MENU;
		  }
		}
	}

    if (VolDet_GetKey1Level() == VOLDET_KEY_LVL_UP)
    {
		uikeyCountUp++;
		MODE_KEY_PRESS_RELEASE=4;
		if(uikeyCountUp>30)
			uiKeyCode |= FLGKEY_PLAYBACK;
    }
    else
	{
		if(uikeyCountUp>=5)
		{
			if(uikeyCountUp<=30)
			{
				uiKeyCode |= FLGKEY_UP;
			}
			else
			{
				uiKeyCode |= FLGKEY_PLAYBACK;
			}
		}
	}
    if (VolDet_GetKey1Level() == VOLDET_KEY_LVL_DOWN)
    {

    }

    if (VolDet_GetKey1Level() == VOLDET_KEY_LVL_ENTER)
    {

    }
    if (VolDet_GetKey1Level() == VOLDET_KEY_LVL_PLAYBACK)
    {

    }

	if(--MODE_KEY_PRESS_RELEASE==0)
	{
		MODE_KEY_PRESS_RELEASE=0;
		MODE_KEY_PRESS=0;
        uikeyCountMenuLong=0;
		uikeyCountMenu=0;//FU 2014.1.2
		uikeyCountUp=0;
        //debug_msg("===1212121212121212121121212uikeyCountMenu====%d====\r\n",uikeyCountMenu);
	}
#endif
	/*
	    //detect if power-on by press playback key
	    if (!HwPower_GetPowerKey(POWER_ID_PSW2))
	    {
	        uiKeyCode |= FLGKEY_PLAYBACK;
	    }
	*/
	DBG_IND("KEY=%08x\r\n", uiKeyCode);
	if (rtc_getPWRStatus())
	{
		BI_TimeCount=0;
		uiKeyCode |= FLGKEY_ENTER;//DISPLAY MODE
	}

	DrvKey_DetKeyDelay();
	return uiKeyCode;
}
/**
  Detect power key is pressed or not.

  Detect power key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
	BOOL PWROFF_PRESS = FALSE;

UINT32 DrvKey_DetPowerKey(void)
{
    UINT32 uiKeyCode = 0;
	static	UINT32 uiKeyCount = 0;

	if(HwPower_GetPowerKey(POWER_ID_PSW1))
	{
		if(  uiKeyCount  >10 )
		{

			PWROFF_PRESS = TRUE;
			uiKeyCode = FLGKEY_KEY_POWER;
			// Reset shutdown timer
			HwPower_SetPowerKey(POWER_ID_PSW1, 0xf0);
		}
		else
			uiKeyCount++;
	}
	else
	{
		if( uiKeyCount >0  &&  uiKeyCount <10 ) 	//power key release to change Night Led
		{
			uiKeyCount= 0;
		}
	}


    return uiKeyCode;
}
UINT32 DrvKey_DetStatusKey(DX_STATUS_KEY_GROUP KeyGroup)
{
	UINT32 uiReturn = STATUS_KEY_LVL_UNKNOWN;
	switch (KeyGroup) {
	case DX_STATUS_KEY_GROUP1:
		break;

	case DX_STATUS_KEY_GROUP2:
		break;

	case DX_STATUS_KEY_GROUP3:
		break;

	case DX_STATUS_KEY_GROUP4:
		break;

	case DX_STATUS_KEY_GROUP5:
		break;

	default:
		DBG_ERR("[StatusKey]no this attribute");
		break;
	}
	return uiReturn;
}
