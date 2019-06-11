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
#define VOLDET_KEY_ADC_RANGE               (20)
#define VOLDET_KEY_ADC_LVL0			(0)
#define VOLDET_KEY_ADC_LVL1			(120)
#define VOLDET_KEY_ADC_LVL2			(250)
#define VOLDET_KEY_ADC_LVL3			(460)
#define VOLDET_KEY_ADC_LVL4			(420)
#define VOLDET_KEY_ADC_LVL5			(360)
#define VOLDET_KEY_ADC_LVL6			(150)
#define VOLDET_KEY_ADC_LVL7			(150)
#define VOLDET_KEY_ADC_LVL8			(150)

#define VOLDET_KEY_LVL_UNKNOWN		0xFFFFFFFF
#define VOLDET_KEY_LVL_SHUTTER2         0
#define VOLDET_KEY_LVL_UP			1
#define VOLDET_KEY_LVL_DOWN			2
#define VOLDET_KEY_LVL_MENU			3
#define VOLDET_KEY_LVL_MODE			4
#define VOLDET_KEY_LVL_ENTER			5
#define VOLDET_KEY_LVL_RIGHT			6
#define VOLDET_KEY_LVL_LEFT			7


#define KEYSCAN_PWROFF_INIT_STATE       0
#define KEYSCAN_PWROFF_RELEASE_STATE    1
#define KEYSCAN_PWROFF_PRESS_STATE      2

#define KEYSCAN_PWRKEY_UNKNOWN          0xFFFFFFFF
#define KEYSCAN_PWRKEY_RELEASED         0
#define KEYSCAN_PWRKEY_PRESSED          1
#endif

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
/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetKey1Level(void)
{
	static UINT32   uiRetKey1Lvl;
	UINT32          uiKey1ADC, uiCurKey2Lvl;

	uiKey1ADC = VolDet_GetKey1ADC();
	DBG_IND("uiKey1ADC %d \r\n", uiKey1ADC);
	//if(uiKey1ADC<1000)
	//debug_msg("uiKey1ADC %d \r\n", uiKey1ADC);

    if (uiKey1ADC < 1024)//VOLDET_KEY_ADC_TH
    {
        if(uiKey1ADC<=(VOLDET_KEY_ADC_LVL0+VOLDET_KEY_ADC_RANGE))
    	{
			//CHKPNT;
			uiCurKey2Lvl = VOLDET_KEY_LVL_UP;
		}
        else if((uiKey1ADC>=VOLDET_KEY_ADC_LVL1)&&
    		(uiKey1ADC<=(VOLDET_KEY_ADC_LVL1+VOLDET_KEY_ADC_RANGE)))
        {
			//CHKPNT;
			uiCurKey2Lvl = VOLDET_KEY_LVL_MENU;
        }
		else if((uiKey1ADC>=(VOLDET_KEY_ADC_LVL2-VOLDET_KEY_ADC_RANGE))&&
    		(uiKey1ADC<=(VOLDET_KEY_ADC_LVL2+VOLDET_KEY_ADC_RANGE)))
        {
			// CHKPNT;
			uiCurKey2Lvl = VOLDET_KEY_LVL_DOWN;
        }
		else if((uiKey1ADC>=(VOLDET_KEY_ADC_LVL3-VOLDET_KEY_ADC_RANGE))&&
    		(uiKey1ADC<=(VOLDET_KEY_ADC_LVL3+VOLDET_KEY_ADC_RANGE)))
        {
			uiCurKey2Lvl = VOLDET_KEY_LVL_SHUTTER2;
        }
		else if((uiKey1ADC>=(VOLDET_KEY_ADC_LVL4-VOLDET_KEY_ADC_RANGE))&&
    		(uiKey1ADC<=(VOLDET_KEY_ADC_LVL4+VOLDET_KEY_ADC_RANGE)))
        {
			//CHKPNT;
			uiCurKey2Lvl = VOLDET_KEY_LVL_MODE;
        }
		else if((uiKey1ADC>=(VOLDET_KEY_ADC_LVL5-VOLDET_KEY_ADC_RANGE))&&
    		(uiKey1ADC<=(VOLDET_KEY_ADC_LVL5+VOLDET_KEY_ADC_RANGE)))
        {
			uiCurKey2Lvl = VOLDET_KEY_LVL_ENTER;
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
UINT32 uiKeyCode_key_custome = 0;
UINT32 DrvKey_DetNormalKey(void)
{
	UINT32 uiKeyCode = 0;

#if (ADC_KEY == ENABLE)
	UINT32 uiKey1Lvl = VolDet_GetKey1Level();
	switch (uiKey1Lvl) {
	case VOLDET_KEY_LVL_UNKNOWN:
	default:
		break;
	case VOLDET_KEY_LVL_MODE:
		//CHKPNT;
		uiKeyCode |= FLGKEY_MODE;
		break;
	case VOLDET_KEY_LVL_DOWN:
		//CHKPNT;
		uiKeyCode |= FLGKEY_DOWN;
		break;
	case VOLDET_KEY_LVL_SHUTTER2:
		uiKeyCode |= FLGKEY_SHUTTER2;
		break;
	case VOLDET_KEY_LVL_UP:
		//CHKPNT;
		uiKeyCode |= FLGKEY_UP;
		break;
	case VOLDET_KEY_LVL_MENU:
		//CHKPNT;
		uiKeyCode |= FLGKEY_MENU;
		break;
	case VOLDET_KEY_LVL_ENTER:
		uiKeyCode |= FLGKEY_ENTER;
		break;
	}
#endif


#if (GPIO_KEY == ENABLE)
	if (gpio_getPin(GPIO_KEY_LEFT)) {
		uiKeyCode |= FLGKEY_LEFT;
	}
	if (gpio_getPin(GPIO_KEY_SHUTTER1)) {
		uiKeyCode |= FLGKEY_SHUTTER1;
	}
	if (gpio_getPin(GPIO_KEY_SHUTTER2)) {
		uiKeyCode |= FLGKEY_SHUTTER2;
	}
	if (gpio_getPin(GPIO_KEY_ZOOMIN)) {
		uiKeyCode |= FLGKEY_ZOOMIN;
	}
	if (gpio_getPin(GPIO_KEY_ZOOMOUT)) {
		uiKeyCode |= FLGKEY_ZOOMOUT;
	}
	if (gpio_getPin(GPIO_KEY_ENTER)) {
		uiKeyCode |= FLGKEY_ENTER;
	}
#endif
          if (uiKeyCode_key_custome)
         {
     	    //debug_msg(">>>>>>>%d\r\n",uiKeyCode_key_custome);

             uiKeyCode_key_custome--;
         }

	/*
	    //detect if power-on by press playback key
	    if (!HwPower_GetPowerKey(POWER_ID_PSW2))
	    {
	        uiKeyCode |= FLGKEY_PLAYBACK;
	    }
	*/
	DBG_IND("KEY=%08x\r\n", uiKeyCode);

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
#define KEYSCAN_POWEROFF_COUNT  15

UINT32 DrvKey_DetPowerKey(void)
#if 1
{
    UINT32 uiKeyCode = 0;
    static UINT32 uiPowerKey    = KEYSCAN_PWRKEY_UNKNOWN;
    static UINT32 uiPWRState    = KEYSCAN_PWROFF_INIT_STATE;
    #if 1
    static UINT32 uiPowerKeyPressCount = 0;
    #endif
//	uiKeyCode_key_custome=0;
    if (rtc_getPWRStatus())
    {
        rtc_resetShutdownTimer();

        if (uiPowerKey == KEYSCAN_PWRKEY_PRESSED)
        {
        	uiPowerKeyPressCount++;
            if (uiPWRState == KEYSCAN_PWROFF_RELEASE_STATE)
            {
                // uiPowerKeyPressCount++;
                if (uiPowerKeyPressCount > KEYSCAN_POWEROFF_COUNT)
                {
                    uiPWRState = KEYSCAN_PWROFF_PRESS_STATE;
                }
            }
            if (uiPWRState == KEYSCAN_PWROFF_PRESS_STATE)
            {
                uiKeyCode = FLGKEY_KEY_POWER;
            }
        }
        else
        {
            uiPowerKey = KEYSCAN_PWRKEY_PRESSED;
        }

		#if 0
		if (uiPowerKeyPressCount > KEYSCAN_POWEROFF_COUNT+8)
		{
			if (uiPowerKey == KEYSCAN_PWRKEY_RELEASED)
			{
				if (uiPWRState == KEYSCAN_PWROFF_INIT_STATE)
				{
					uiPWRState = KEYSCAN_PWROFF_RELEASE_STATE;
				}
			}
			else
			{
				uiPowerKey = KEYSCAN_PWRKEY_RELEASED;
			}
		}
		#endif
		/*
		else if ((uiPowerKeyPressCount>0)&&(uiPowerKeyPressCount<KEYSCAN_POWEROFF_COUNT))
		{
			uiKeyCode_key_custome ++;
			//uiKeyCode_key_custome = 1;
		}*/
    }
    else
    {
        if (uiPowerKey == KEYSCAN_PWRKEY_RELEASED)
        {
            if (uiPWRState == KEYSCAN_PWROFF_INIT_STATE)
            {
                uiPWRState = KEYSCAN_PWROFF_RELEASE_STATE;
            }
            if ((uiPowerKeyPressCount>0)&&(uiPowerKeyPressCount<KEYSCAN_POWEROFF_COUNT))
            {
				//debug_msg("ssuiKeyCode_key_custome = %d\r\n",uiKeyCode_key_custome);
                uiKeyCode_key_custome = 3;
		        uiPowerKeyPressCount=0;
            }
            if(uiPowerKeyPressCount>KEYSCAN_POWEROFF_COUNT+8)
		        uiPowerKeyPressCount=0;
        }
        else
        {
	        uiPowerKey = KEYSCAN_PWRKEY_RELEASED;
        }
    }
    return uiKeyCode;
}
#else
{
	UINT32 uiKeyCode = 0;

	if (HwPower_GetPowerKey(POWER_ID_PSW1)) {
		uiKeyCode = FLGKEY_KEY_POWER;
		// Reset shutdown timer
		HwPower_SetPowerKey(POWER_ID_PSW1, 0xf0);
	}
	return uiKeyCode;
}
#endif
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
