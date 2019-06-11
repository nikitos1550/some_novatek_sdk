/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       IOCfg.c
    @ingroup    mIPRJAPCommonIO

    @brief      IO config module
                This file is the IO config module

    @note       Nothing.

    @date       2005/12/24
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#include "Type.h"
#include "DrvExt.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "Debug.h"
#include "IOCfg.h"
#include "Utility.h"
#include "pll.h"
#include "IOInit.h"
#include "IntDirCfg.h"
//#include "timer.h"
#if defined(_MCU_ENABLE_)
#include "MCUCtrl.h"
#endif

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (USE_VIO == ENABLE)
UINT32 Virtual_IO[VIO_MAX_ID] = {0};
UINT32 vio_getpin(UINT32 id)
{
	if (id >= VIO_MAX_ID) {
		return 0;
	}
	return Virtual_IO[id];
}
void vio_setpin(UINT32 id, UINT32 v)
{
	if (id >= VIO_MAX_ID) {
		return;
	}
	Virtual_IO[id] = v;
}
#endif

/**
  Do GPIO initialization

  Initialize input/output pins, and pin status

  @param void
  @return void
*/
void IO_InitGPIO(void)
{
	UINT32 iSValue;

	MODELEXT_HEADER *header;
	GPIO_INIT_OBJ *uiGPIOMapInitTab;

	uiGPIOMapInitTab = (GPIO_INIT_OBJ *)Dx_GetModelExtCfg(MODELEXT_TYPE_GPIO_INFO, &header);
	if (!uiGPIOMapInitTab || !header) {
		DBG_FATAL("Modelext: iocfg is null\n");
		return;
	}

	UINT32 totalGpioInitCount = header->number;

	DBG_IND("GPIO START\r\n");
	//--------------------------------------------------------------------
	// Open GPIO driver
	//--------------------------------------------------------------------
	gpio_open();
	for (iSValue = 0 ; iSValue < totalGpioInitCount ; iSValue++) {
		if (uiGPIOMapInitTab[iSValue].GpioDir == GPIO_DIR_INPUT) {
			gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_INPUT);
			pad_setPullUpDown(uiGPIOMapInitTab[iSValue].PadPin, uiGPIOMapInitTab[iSValue].PadDir);
		} else {
			gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_OUTPUT);
			if (uiGPIOMapInitTab[iSValue].PadDir == GPIO_SET_OUTPUT_HI) {
				gpio_setPin(uiGPIOMapInitTab[iSValue].GpioPin);
			} else {
				gpio_clearPin(uiGPIOMapInitTab[iSValue].GpioPin);
			}
		}
	}

	//--------------------------------------------------------------------
	// Use Non-Used PWM to be Delay Timer
	//--------------------------------------------------------------------
#if defined(PWMID_TIMER)
	Delay_setPwmChannels(PWMID_TIMER);
#endif

	DBG_IND("GPIO END\r\n");

}
void LED_RED_TurnOn(void)
{
		gpio_setPin(GPIO_RED_LED_CTL);
}
void LED_RED_TurnOff(void)
{
    gpio_clearPin(GPIO_RED_LED_CTL);

}
void LED_BLUE_TurnOn(void)
{
    gpio_setPin(GPIO_BLUE_LED_CTL);

}
void LED_BLUE_TurnOff(void)
{
		gpio_clearPin(GPIO_BLUE_LED_CTL);
}
void SPK_TurnON(void)
{
	gpio_setPin(GPIO_SPK_EN);
}
void SPK_TurnOff(void)
{
	gpio_clearPin(GPIO_SPK_EN);
}

/**
  Initialize voltage detection

  Initialize voltage detection for battery and flash

  @param void
  @return void
*/

void IO_InitADC(void)
{
#if 0
	if (adc_open(ADC_CH_VOLDET_BATTERY) != E_OK) {
		DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
		return;
	}

	//650 Range is 250K Hz ~ 2M Hz
	adc_setConfig(ADC_CONFIG_ID_OCLK_FREQ, 250000); //250K Hz

	//battery voltage detection
	adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
	adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
	adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_INTEN, FALSE);
#endif

#if (ADC_KEY == ENABLE)
	if (adc_open(ADC_CH_VOLDET_KEY1) != E_OK) {
		DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
		return;
	}
	adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
	adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
	adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_INTEN, FALSE);
    	if (adc_open(ADC_CH_VOLDET_LOW_VOLUTE) != E_OK) {
		DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
		return;
	}
	adc_setChConfig(ADC_CH_VOLDET_LOW_VOLUTE, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
	adc_setChConfig(ADC_CH_VOLDET_LOW_VOLUTE, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
	adc_setChConfig(ADC_CH_VOLDET_LOW_VOLUTE, ADC_CH_CONFIG_ID_INTEN, FALSE);

#endif

	// Enable adc control logic
	adc_setEnable(TRUE);

	//???
	Delay_DelayMs(15); //wait ADC stable  //for pwr on speed up
}

void Dx_InitIO(void)  // Config IO for external device
{
	#if defined(_CLOCK_RTC_)
	debug_msg("open RTC!\r\n");
	#elif defined(_CLOCK_DRTC_)
	debug_msg("open DRTC!\r\n");
	#else
	#warning Unknown CLOCK?
	#warning Please assign your "CLOCK" in include "ModelConfig.txt"
	#error (see above)
	#endif

	// when cpu2 is linux , the pinmux and interrupt destination is setted by uboot
#if !defined(_CPU2_LINUX_)
	IO_InitIntDir();    //initial interrupt destination
#endif
	IO_InitPinmux();    //initial PINMUX config
	IO_InitGPIO();      //initial GPIO pin status
	IO_InitADC();       //initial ADC pin status
#if defined(_MCU_ENABLE_)
	MCUCtrl_Open();
#endif
}

void Dx_UninitIO(void)  // Config IO for external device
{
}



