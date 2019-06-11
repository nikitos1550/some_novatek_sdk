/**
    Header file for ADC module.

    This file is the header file that define the APIs and data types for ADC module.
    The user can reference this section for the detail description of the each driver API usage
    and also the parameter descriptions and its limitations.
    The overall combinational usage flow is introduced in the application note document,
    and the user must reference to the application note for the driver usage flow.

    @file       adc.h
    @ingroup    mIDrvIO_ADC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _ADC_H
#define _ADC_H

#include "Type.h"


/**
    @addtogroup mIDrvIO_ADC
*/
//@{

/**
    Definitions for ADC configuration ID.

    Definitions for ADC configuration ID.
    These definitions are used to configure the general settings such as adc analog block operating frequency and sample periods.

    @note Used in adc_setConfig() / adc_getConfig().
*/
typedef enum {
	ADC_CONFIG_ID_OCLK_FREQ,        ///< Set the ADC analog block operating frequency. This value allowable range is from 250000 to 2000000 in current design.
	ADC_CONFIG_ID_SAMPLE_AVERAGE,   ///< Set ADC Sample average times. Use "ADC_SAMPLEAVG" as input paramters.
	ADC_CONFIG_ID_EXT_SAMPLE_CNT,   ///< Set ADC channel external HOLD time. Valid range from 0x0~0xF. This setting would enlarge the hold time of adc each channel.

	ENUM_DUMMY4WORD(ADC_CONFIG_ID)
} ADC_CONFIG_ID;


/**
    ADC sampling average times every sequence

    Definition ADC sampling average times every output sample. By setting this value to ADC_SAMPAVG_2 or ADC_SAMPAVG_4, the
    ADC output rate would be slower because the controller would sample each ADC channel 2 or 4 times and output the average value automatically.

    @note Used in adc_setConfig(ADC_CONFIG_ID_SAMPLE_AVERAGE, ADC_SAMPAVG_1/2/4).
*/
typedef enum {
	ADC_SAMPAVG_1,  ///< Sample one time and output the value.
	ADC_SAMPAVG_2,  ///< Sample two times and output the average value.
	ADC_SAMPAVG_4   ///< Sample four times and output the average value.
} ADC_SAMPLEAVG;


/**
    ADC channel ID

    Definition for ADC channel identification. These definitions are used to specify the dedicated channel to access.

    @note Used in adc_open() / adc_close() / adc_readVoltage() / adc_setChConfig() / ...
*/
typedef enum {
	ADC_CHANNEL_0,          ///< ADC channel 0
	ADC_CHANNEL_1,          ///< ADC channel 1
	ADC_CHANNEL_2,          ///< ADC channel 2
	ADC_CHANNEL_3,          ///< ADC channel 3

	ADC_TOTAL_CH,   // Total ADC channel number
	ENUM_DUMMY4WORD(ADC_CHANNEL)
} ADC_CHANNEL;


/**
    ADC Channel Configuration ID

    These definitions are used to configure each ADC channel operating settings such as ISR callback handler, sample frequency, interrupt enanle,
    sample mode, and also the value trigger settings. Please refer to the application note for the functional introductions.

    @note Used in adc_setChConfig(ADC_CHANNEL_X, ADC_CH_CONFIG_ID_SAMPLE_MODE) / adc_getChConfig(ADC_CHANNEL_X, ADC_CH_CONFIG_ID_SAMPLE_MODE).
*/
typedef enum {
	ADC_CH_CONFIG_ID_SAMPLE_MODE,           ///< Set ADC Channel sample mode. Use ADC_CH_SAMPLEMODE_ONESHOT or ADC_CH_SAMPLEMODE_CONTINUOUS.
	ADC_CH_CONFIG_ID_SAMPLE_FREQ,           ///< Set ADC Channel sample frequency. Unit in Hertz.
	ADC_CH_CONFIG_ID_INTEN,                 ///< Set ADC Channel interrupt enable/disable.

	ADC_CH_CONFIG_ID_VALUETRIG_VOL_LOW,     ///< Set ADC Channel value trigger LOW voltage in mV. Valid Range from 0 to 3300(mV).
	///< The user must notice that the NT96650's ADC is 10bits design and the valid resolution is about 8bits,
	///< so the value trigger function is designed for 256 levels only(8bits) and aligned to MSB 8bits of the ADC value.
	///< So this assigned trigger voltage would be slightly different about 0~10mV if this value is read out form adc_getChConfig().
	///< This is because the ADC trigger value LSB 2 bits is forced to 0x0 in the ADC design.
	ADC_CH_CONFIG_ID_VALUETRIG_VOL_HIGH,    ///< Set ADC Channel value trigger HIGH voltage in mV. Valid Range from 0 to 3300.
	///< The user must notice that the NT96650's ADC is 10bits design and the valid resolution is about 8bits,
	///< so the value trigger function is designed for 256 levels only(8bits) and aligned to MSB 8bits of the ADC value.
	///< So this assigned trigger voltage would be slightly different about 0~10mV if this value is read out form adc_getChConfig().
	///< This is because the ADC trigger value LSB 2 bits is forced to 0x0 in the ADC design.
	ADC_CH_CONFIG_ID_VALUETRIG_THD_LOW,     ///< Set ADC Channel value trigger LOW threshold value. Valid Range from 0 to 1023.
	///< The user must notice that the NT96650's ADC is 10bits design and the valid resolution is about 8bits,
	///< so the value trigger function is designed for 256 levels only(8bits) and aligned to MSB 8bits of the ADC value.
	///< So this assigned trigger voltage would be slightly different about 0~2 if this value is read out form adc_getChConfig().
	///< This is because the ADC trigger value LSB 2 bits is forced to 0x0 in the ADC design.
	ADC_CH_CONFIG_ID_VALUETRIG_THD_HIGH,    ///< Set ADC Channel value trigger HIGH threshold value. Valid Range from 0 to 1023.
	///< The user must notice that the NT96650's ADC is 10bits design and the valid resolution is about 8bits,
	///< so the value trigger function is designed for 256 levels only(8bits) and aligned to MSB 8bits of the ADC value.
	///< So this assigned trigger voltage would be slightly different about 0~2 if this value is read out form adc_getChConfig().
	///< This is because the ADC trigger value LSB 2 bits is forced to 0x0 in the ADC design.
	ADC_CH_CONFIG_ID_VALUETRIG_RANGE,       ///< Set ADC Channel value trigger range condition. Please use ADC_VALTRIG_RANGE as input parameter.
	ADC_CH_CONFIG_ID_VALUETRIG_MODE,        ///< Set ADC Channel value trigger mode condition. Please use ADC_VALTRIG_MODE as input parameter.
	ADC_CH_CONFIG_ID_VALUETRIG_EN,          ///< Set ADC Channel value trigger function enable/ disable.
	ADC_CH_CONFIG_ID_VALUETRIG_INTEN,       ///< Set ADC Channel value trigger interrupt enable / disable.

	ENUM_DUMMY4WORD(ADC_CH_CONFIG_ID)
} ADC_CH_CONFIG_ID;


/**
    ADC Channel Sample Mode

    Specify the specified ADC Channel is in OneShot or Continuously Sample Mode.

    @note Used in adc_setChConfig(ADC_CHANNEL_X, ADC_CH_CONFIG_ID_SAMPLE_MODE)
*/
typedef enum {
	ADC_CH_SAMPLEMODE_ONESHOT,      ///< Set ADC Channel to OneShot trigger mode.
	ADC_CH_SAMPLEMODE_CONTINUOUS,   ///< Set ADC Channel to Continuously trigger mode.

	ENUM_DUMMY4WORD(ADC_CH_SAMPLEMODE)
} ADC_CH_SAMPLEMODE;


/**
    Definitions for setup value trigger interrupt's value trigger range.

    ADC controller's value trigger interrupt can select to issue interrupt when the sample value is within specified range.

    @note Used in adc_setChConfig(ADC_CHANNEL_X, ADC_CH_CONFIG_ID_VALUETRIG_RANGE).
*/
typedef enum {
	ADC_VALTRIG_RANGE_IN,   ///< The value trigger interrupt is issued when sample value is inside the specified range.
	ADC_VALTRIG_RANGE_OUT,  ///< The value trigger interrupt is issued when sample value is outside the specified range.

	ENUM_DUMMY4WORD(ADC_VALTRIG_RANGE)
} ADC_VALTRIG_RANGE;


/**
    Definition for setup the value trigger interrupt is edge trigger or level trigger interrupt.

    @note Used in adc_setChConfig(ADC_CHANNEL_X, ADC_CH_CONFIG_ID_VALUETRIG_MODE).
*/
typedef enum {
	ADC_VALTRIG_MODE_LVL,   ///< Set the value trigger interrupt issue condition to level trigger.
	ADC_VALTRIG_MODE_EDGE,  ///< Set the value trigger interrupt issue condition to edge trigger.

	ENUM_DUMMY4WORD(ADC_VALTRIG_MODE)
} ADC_VALTRIG_MODE;


/**
    The ADC interrupt identification IDs.

    These definitions can be used in the adc_waitInterrupt() to specify the waited interrupt events.
*/
typedef enum {
	ADC_INTERRUPT_READY_CH0 = 0x00000001,       ///< ADC Channel 0 Data Ready
	ADC_INTERRUPT_READY_CH1 = 0x00000002,       ///< ADC Channel 1 Data Ready
	ADC_INTERRUPT_READY_CH2 = 0x00000004,       ///< ADC Channel 2 Data Ready
	ADC_INTERRUPT_READY_CH3 = 0x00000008,       ///< ADC Channel 3 Data Ready

	ADC_INTERRUPT_VALTRIG_1 = 0x00010000,       ///< ADC Channel 1 Value Trigger Hit
	ADC_INTERRUPT_VALTRIG_2 = 0x00100000,       ///< ADC Channel 2 Value Trigger Hit
	ADC_INTERRUPT_VALTRIG_3 = 0x01000000,       ///< ADC Channel 6 Value Trigger Hit

	ENUM_DUMMY4WORD(ADC_INTERRUPT)
} ADC_INTERRUPT;


/**
    Configuration ID for ADC wakeup CPU event condition.

    After CPU entering power down mode, the ADC can wakeup the CPU by the specified conditions.

    @note Used in the adc_setWakeupConfig().
*/
typedef enum {
	ADC_WAKEUP_CONFIG_ENABLE,   ///< Set ADC Wakeup CPU source enable. Parameter uses "Bit-Wise OR" of the ADC_WAKEUP_SRC.
	ADC_WAKEUP_CONFIG_DISABLE,  ///< Set ADC Wakeup CPU source disable. Parameter uses "Bit-Wise OR" of the ADC_WAKEUP_SRC.


	ENUM_DUMMY4WORD(ADC_WAKEUP_CONFIG)
} ADC_WAKEUP_CONFIG;


/**
    Wakeup Source Options of the ADC module

    @note Used in the adc_setWakeupConfig(ADC_WAKEUP_CONFIG_ENABLE / ADC_WAKEUP_CONFIG_DISABLE).
*/
typedef enum {
	ADC_WAKEUP_SRC_ADCCH_1      = 0x00010000,   ///< Set ADC Wakeup CPU condition as ADC_CHANNEL_1 Value Trigger Interrupt
	ADC_WAKEUP_SRC_ADCCH_2      = 0x00100000,   ///< Set ADC Wakeup CPU condition as ADC_CHANNEL_2 Value Trigger Interrupt
	ADC_WAKEUP_SRC_ADCCH_3      = 0x01000000,   ///< Set ADC Wakeup CPU condition as ADC_CHANNEL_3 Value Trigger Interrupt

	ADC_WAKEUP_SRC_MASK         = 0x01110000,
	ENUM_DUMMY4WORD(ADC_WAKEUP_SRC)
} ADC_WAKEUP_SRC;




//
// Export ADC APIs
//
extern ER               adc_open(ADC_CHANNEL Channel);
extern ER               adc_close(ADC_CHANNEL Channel);
extern BOOL             adc_isOpened(ADC_CHANNEL Channel);

extern ER               adc_setEnable(BOOL bEn);
extern BOOL             adc_getEnable(void);

extern void             adc_setConfig(ADC_CONFIG_ID CfgID, UINT32 uiCfgValue);
extern UINT32           adc_getConfig(ADC_CONFIG_ID CfgID);

extern void             adc_setChConfig(ADC_CHANNEL Channel, ADC_CH_CONFIG_ID CfgID, UINT32 uiCfgValue);
extern UINT32           adc_getChConfig(ADC_CHANNEL Channel, ADC_CH_CONFIG_ID CfgID);

extern void             adc_triggerOneShot(ADC_CHANNEL Channel);

UINT32 extern           adc_readData(ADC_CHANNEL Channel);
extern BOOL             adc_isDataReady(ADC_CHANNEL Channel);
extern void             adc_clearDataReady(ADC_CHANNEL Channel);
extern UINT32           adc_readVoltage(ADC_CHANNEL Channel);

extern void             adc_setCaliOffset(UINT32 Offset, UINT32 CalVDDADC);
extern ER               adc_setWakeupConfig(ADC_WAKEUP_CONFIG CfgID, UINT32 uiParam);

extern void             adc_setTvCalibEn(BOOL bEn);

extern ADC_INTERRUPT    adc_waitInterrupt(ADC_INTERRUPT WaitedFlag);


#if _FPGA_EMULATION_
#define ADC_SRCCLOCK                1600000
#else
#define ADC_SRCCLOCK               16000000
#endif

#endif
//@}
