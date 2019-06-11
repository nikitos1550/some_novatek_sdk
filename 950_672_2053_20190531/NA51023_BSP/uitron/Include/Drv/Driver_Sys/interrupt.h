/**
    Header file for Interrupt module

    This file is the header file that define the API for Interrupt module.

    @file       Interrupt.h
    @ingroup    mIDrvSys_Interrupt
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#if defined(__UITRON)
#include "Type.h"
#else
#include "nvt_type.h"
#endif
#include "cc.h"

// Interrupt number <= 32
//typedef UINT32          INT_PTN;
// Interrupt number > 32
typedef UINT64          INT_PTN;

/**
    @addtogroup mIDrvSys_Interrupt
*/
//@{
#define INT_INTC_ID_DST_VER 0x17021417


typedef enum {
	INT_CONFIG_ID_INT_DESTINATION = 0,              ///< Configured Interrupt exception destination to MIPS1/MIPS2/DSP
	///< @note
	///< Context is :
	///< - @b PINT_ID_DST : Point of destination table

	INT_CONFIG_ID_INT_TO_CPU1,                      ///< Configured specific Interrupt to CPU1
	///< @note
	///< Context is :
	///< - @b INT_ID : Speficif Interrupt ID

	INT_CONFIG_ID_INT_TO_CPU2,                      ///< Configured specific Interrupt to CPU2
	///< @note
	///< Context is :
	///< - @b INT_ID : Speficif Interrupt ID
	ENUM_DUMMY4WORD(INT_CONFIG_ID)
} INT_CONFIG_ID;

/**
    Interrupt module ID

    Interrupt module ID for int_getIRQId() and int_getDummyId().
*/
typedef enum {
	INT_ID_TIMER,
	INT_ID_SIE,
	INT_ID_SIE2,
	INT_ID_DSP,
	INT_ID_DRTC,
	INT_ID_IPE,
	INT_ID_IME,
	INT_ID_DCE,
	INT_ID_IFE,
	INT_ID_IFE2,
	INT_ID_DIS,
	INT_ID_FDE,
	INT_ID_TV,          //0x34
	INT_ID_RHE,
	INT_ID_NOE,
	INT_ID_DAI,
	INT_ID_H264,
	INT_ID_JPEG,
	INT_ID_GRAPHIC,
	INT_ID_GRAPHIC2,    //0x50
	INT_ID_RSA,
	INT_ID_ISE,
	INT_ID_TGE,
	INT_ID_TSE,
	INT_ID_GPIO,
	INT_ID_REMOTE,
	INT_ID_PWM,
	INT_ID_USB,
	INT_ID_HASH,
	INT_ID_NAND,
	INT_ID_SDIO,
	INT_ID_SDIO2,
	INT_ID_SDIO3,
	INT_ID_DMA,
	INT_ID_ETHERNET,
	INT_ID_SPI,
	INT_ID_SPI2,
	INT_ID_SPI3,
	INT_ID_CRYPTO,
	INT_ID_JPEG2,
	INT_ID_SIF,
	INT_ID_I2C,
	INT_ID_I2C2,
	INT_ID_UART,
	INT_ID_UART2,
	INT_ID_UART3,
	INT_ID_UART4,
	INT_ID_ADC,
	INT_ID_IDE,
	INT_ID_NULL_3,      //0xC8 IDE2 removed
	INT_ID_NULL_4,      //0xCC DSI removed
	INT_ID_NULL_5,      //0xD0 MI removed
	INT_ID_NULL_6,      //0xD4 HDMI removed
	INT_ID_NULL_7,      //0xD8 Vx1 removed
	INT_ID_LVDS,        //0xDC
	INT_ID_LVDS2,       //0xE0
	INT_ID_RTC,
	INT_ID_WDT,
	INT_ID_CG,
	INT_ID_CC,          //0xF0
	INT_ID_I2C3,

	INT_ID_MAX,
	ENUM_DUMMY4WORD(INT_ID)
} INT_ID;

/*
    Interrupt module ID

    Indicate which core will configured as destination

    @note For DMA_WRITEPROT_ATTR
*/


typedef union {
	INT_PTN Reg;
	struct {
		//INT0
		UINT32  bInt_ID_TIMER: 1;
		UINT32  bInt_ID_SIE: 1;
		UINT32  bInt_ID_SIE2: 1;
		UINT32  bInt_ID_DSP: 1;

		UINT32  bInt_ID_DRTC: 1;
		UINT32  bInt_ID_IPE: 1;
		UINT32  bInt_ID_IME: 1;
		UINT32  bInt_ID_DCE: 1;

		UINT32  bInt_ID_IFE: 1;
		UINT32  bInt_ID_IFE2: 1;
		UINT32  bInt_ID_DIS: 1;
		UINT32  bInt_ID_FDE: 1;

		UINT32  bInt_ID_TV: 1;
		UINT32  bInt_ID_RHE: 1;
		UINT32  bInt_ID_NOE: 1;
		UINT32  bInt_ID_DAI: 1;


		//INT16
		UINT32  bInt_ID_H264: 1;
		UINT32  bInt_ID_JPEG: 1;
		UINT32  bInt_ID_GRAPHIC: 1;
		UINT32  bInt_ID_GRAPHIC2: 1;

		UINT32  bInt_ID_RSA: 1;
		UINT32  bInt_ID_ISE: 1;
		UINT32  bInt_ID_TGE: 1;
		UINT32  bInt_ID_TSE: 1;

		UINT32  bInt_ID_GPIO: 1;
		UINT32  bInt_ID_REMOTE: 1;
		UINT32  bInt_ID_PWM: 1;
		UINT32  bInt_ID_USB: 1;

		UINT32  bInt_ID_HASH: 1;
		UINT32  bInt_ID_NAND: 1;
		UINT32  bInt_ID_SDIO: 1;
		UINT32  bInt_ID_SDIO2: 1;

		//INT32
		UINT32  bInt_ID_SDIO3: 1;
		UINT32  bInt_ID_DMA: 1;
		UINT32  bInt_ID_ETHERNET: 1;
		UINT32  bInt_ID_SPI: 1;

		UINT32  bInt_ID_SPI2: 1;
		UINT32  bInt_ID_SPI3: 1;
		UINT32  bInt_ID_CRYPTO: 1;
		UINT32  bInt_ID_JPEG2: 1;

		UINT32  bInt_ID_SIF: 1;
		UINT32  bInt_ID_I2C: 1;
		UINT32  bInt_ID_I2C2: 1;
		UINT32  bInt_ID_UART: 1;

		UINT32  bInt_ID_UART2: 1;
		UINT32  bInt_ID_UART3: 1;
		UINT32  bInt_ID_UART4: 1;
		UINT32  bInt_ID_ADC: 1;

		//INT48
		UINT32  bInt_ID_IDE: 1;
		UINT32  bInt_ID_NULL_3: 1;
		UINT32  bInt_ID_NULL_4: 1;
		UINT32  bInt_ID_NULL_5: 1;

		UINT32  bInt_ID_NULL_6: 1;
		UINT32  bInt_ID_NULL_7: 1;
		UINT32  bInt_ID_LVDS: 1;
		UINT32  bInt_ID_LVDS2: 1;

		UINT32  bInt_ID_RTC: 1;
		UINT32  bInt_ID_WDT: 1;
		UINT32  bInt_ID_CG: 1;
		UINT32  bInt_ID_CC: 1;

		UINT32  bInt_ID_I2C3: 1;
		UINT32  bReserved4: 3;
	} Bit;
} INT_ID_DST, *PINT_ID_DST;

typedef struct {
	INT_ID_DST  int_id_dst[CC_CORE_NUM];
} INT_DST, *PINT_DST;


// MIPI CSI share the interrupt ID with LVDS / HiSPi
#define INT_ID_CSI          INT_ID_LVDS
#define INT_ID_CSI2         INT_ID_LVDS2

// Macro to generate bit value from ID
#define INT_ID_TO_BIT(Id)   ((INT_PTN)(1) << (Id))

// check type and ID
STATIC_ASSERT(INT_ID_MAX <= (sizeof(INT_PTN) << 3));

#define INT_ALL_MODULES     ((sizeof(INT_PTN) == 4) ? 0xFFFFFFFFul : 0xFFFFFFFFFFFFFFFFull)

extern INT_PTN  int_getEnable(void);
extern INT_PTN  int_getFlag(void);
extern INT_ID   int_getIRQId(void);
extern INT_ID   int_getDummyId(void);
extern UINT32   int_getLatency(void);
extern void     int_setConfig(INT_CONFIG_ID ConfigID, UINT32 uiConfig);


//@}

#endif
