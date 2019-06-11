/**
    PLL Configuration module header

    PLL Configuration module header file.

    @file       pll.h
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PLL_H
#define _PLL_H

#include "Type.h"

/**
    @addtogroup mIHALSysCG
*/
//@{

/**
    Clock Enable ID

    This is for pll_enableClock() and pll_disableClock().
*/
typedef enum {
	DMA_CLK = 1,        ///< DMA(SDRAM) clock
	SIE_MCLK,           ///< SIE MCLK
	SIE_MCLK2,          ///< SIE MCLK2
	SIE_CLK,            ///< SIE clock
	SIE2_CLK,           ///< SIE2 Clock
	TGE_CLK,            ///< TGE clock
	IPE_CLK,            ///< IPE clock
	DIS_CLK,            ///< DIS clock
	IME_CLK,            ///< IME clock
	FDE_CLK,            ///< FDE clock
	ISE_CLK,            ///< ISE clock
	SP_CLK,             ///< special clock
	IFE_CLK,            ///< IFE clock
	DCE_CLK,            ///< DCE clock
	IFE2_CLK,           ///< IFE2 clock
	IDE1_CLK = 16,      ///< IDE clock
	TV_CLK = 18,        ///< TV clock
	CRYPTO_CLK = 23,    ///< Crypto clock
	H264_CLK = 24,      ///< H.264 clock
	H265_CLK = 24,      ///< H.265 clock
	JPG_CLK = 26,       ///< JPEG clock
	GRAPH_CLK,          ///< Graphic clock
	GRAPH2_CLK,         ///< Graphic2 clock
	DAI_CLK,            ///< DAI clock
	EAC_A_ADC_CLK,      ///< EAC analog AD clock
	EAC_A_DAC_CLK,      ///< EAC analog DA clock

	NAND_CLK = 32,      ///< NAND clock
	SDIO_CLK = 34,      ///< SDIO clock
	SDIO2_CLK,          ///< SDIO2 clock
	I2C_CLK,            ///< I2C clock
	I2C2_CLK,           ///< I2C2 clock
	SPI_CLK,            ///< SPI clock
	SPI2_CLK,           ///< SPI2 clock
	SPI3_CLK = 40,      ///< SPI3 clock
	SIF_CLK,            ///< SIF clock
	UART_CLK,           ///< UART clock
	UART2_CLK,          ///< UART2 clock
	REMOTE_CLK,         ///< Remote clock
	ADC_CLK,            ///< ADC clock
	SDIO3_CLK,          ///< SDIO3 clock
	WDT_CLK = 49,       ///< WDT clock
	TMR_CLK,            ///< Timer clock
	EAC_D_CLKEN = 52,   ///< EAC digital clock
	UART3_CLK = 54,     ///< UART3 clock
	UART4_CLK,          ///< UART4 clock
	EFUSE_CLK = 60,     ///< EFUSE clock
	ETH_CLK,            ///< ETH clock
	SP2_CLK,            ///< SP2 clock
	I2C3_CLK,           ///< I2C3 clock

	MIPI_LVDS_CLK = 65, ///< MIPI/LVDS clock
	MIPI_LVDS2_CLK = 66,///< MIPI/LVDS2 clock
	SIE_PXCLK = 68,     ///< SIE PX clock
	SIE2_PXCLK,         ///< SIE2 PX clock

	PWM_CCNT_CLK = 72,  ///< PWM CCNT clock
	PWM_CCNT0_CLK = 72, ///< PWM CCNT0 clock
	PWM_CCNT1_CLK,      ///< PWM CCNT1 clock
	PWM_CCNT2_CLK,      ///< PWM CCNT2 clock
	PWM_CCNT3_CLK,      ///< PWM CCNT3 clock
	TSE_CLK = 77,       ///< TSE  clock
	CC_CLK = 82,        ///< CC clock
	RHE_CLK,            ///< RHE clock
	DRTC_CLK = 86,      ///< DRTC clock
	NOE_CLK,            ///< NOE clock
	ETHPHY_CLK,         ///< ETH PHY clock
	TRNG_CLK,           ///< TRNG clock
	RSA_CLK,            ///< RSA clock
	HASH_CLK,           ///< HASH clock
	JPG2_CLK = 93,       ///< JPEG2 clock
	PWM0_CLK = 96,      ///< PWM0 clock
	PWM1_CLK,           ///< PWM1 clock
	PWM2_CLK,           ///< PWM2 clock
	PWM3_CLK,           ///< PWM3 clock
	PWM4_CLK,           ///< PWM4 clock
	PWM5_CLK,           ///< PWM5 clock
	PWM6_CLK,           ///< PWM6 clock
	PWM7_CLK,           ///< PWM7 clock
	PWM8_CLK,           ///< PWM8 clock
	PWM9_CLK,           ///< PWM9 clock
	PWM10_CLK,          ///< PWM10 clock
	PWM11_CLK,          ///< PWM11 clock
	PWM12_CLK,          ///< PWM12 clock
	PWM13_CLK,          ///< PWM13 clock
	PWM14_CLK,          ///< PWM14 clock
	PWM15_CLK,          ///< PWM15 clock

	CPU2_CLK = 256,     ///< CPU2 clock
	DSP_CLK,            ///< DSP clock

	ENUM_DUMMY4WORD(CG_EN)
} CG_EN;




/**
    Clock Gating Select ID

    This is for pll_setClkAutoGating() / pll_clearClkAutoGating() /
    pll_getClkAutoGating() / pll_setPclkAutoGating() /
    pll_clearPclkAutoGating() / pll_getPclkAutoGating().
*/
typedef enum {
	SIE_GCLK,           ///< Gating SIE clock (Gating PCLK only)
	IPE_GCLK = 2,       ///< Gating IPE clock
	IME_GCLK,           ///< Gating IME clock
	DIS_GCLK,           ///< Gating DIS clock
	FDE_GCLK,           ///< Gating FDE clock
	DCE_GCLK,           ///< Gating DCE clock
	IFE_GCLK,           ///< Gating IFE clock
	GRA_GCLK = 8,       ///< Gating Graphic clock
	GRA2_GCLK,          ///< Gating Graphic2 clock
	IDE_GCLK,           ///< Gating IDE clock  (Gating PCLK only)
	IFE2_GCLK = 14,     ///< Gating IFE2 clock
	JPG_GCLK,           ///< Gating JPEG clock
	H264_GCLK = 16,     ///< Gating H264 clock
	H265_GCLK = 16,     ///< Gating H265 clock
	DAI_GCLK,           ///< Gating DAI clock  (Gating PCLK only)
	EAC_GCLK,           ///< Gating EAC clock  (Gating PCLK only)
	NAND_GCLK,          ///< Gating xD/Nand clock
	SDIO_GCLK,          ///< Gating SDIO clock
	SDIO2_GCLK,         ///< Gating SDIO2 clock
	I2C_GCLK,           ///< Gating I2C clock  (Gating PCLK only)
	I2C2_GCLK,          ///< Gating I2C2 clock  (Gating PCLK only)
	SPI_GCLK = 24,      ///< Gating SPI clock
	SPI2_GCLK,          ///< Gating SPI2 clock
	SPI3_GCLK,          ///< Gating SPI3 clock
	SIF_GCLK,           ///< Gating SIF clock
	UART_GCLK,          ///< Gating UART clock  (Gating PCLK only)
	UART2_GCLK,         ///< Gating UART2 clock  (Gating PCLK only)
	RM_GCLK,            ///< Gating Remote clock  (Gating PCLK only)
	ADC_GCLK,           ///< Gating ADC clock  (Gating PCLK only)

	TMR_GCLK = 32,      ///< Gating TMR clock  (Gating PCLK only)
	WDT_GCLK,           ///< Gating Watchdog clock  (Gating PCLK only)
	LVDS_GCLK = 35,     ///< Gating LVDS clock  (Gating PCLK only)
	MIPI_GCLK = 35,     ///< Gating MIPI clock  (Gating PCLK only)
	MIPI_LVDS_GCLK = 35,///< Gating MIPI/LVDS clock  (Gating PCLK only)
	MIPI2_GCLK = 36,    ///< Gating MIPI2 clock  (Gating PCLK only)
	MIPI_LVDS2_GCLK = 36,///< Gating MIPI/LVDS2 clock  (Gating PCLK only)
	TV_GCLK,            ///< Gating TV clock  (Gating PCLK only)
	ISE_GCLK = 40,      ///< Gating ISE clock
	SIE2_GCLK,          ///< Gating SIE2 clock  (Gating PCLK only)
	PWM_GCLK = 43,      ///< Gating PWM clock
	SDIO3_GCLK = 47,    ///< Gating SDIO3 clock
	UART3_GCLK,         ///< Gating UART3 clock (Gating PCLK only)
	UART4_GCLK,         ///< Gating UART4 clock (Gating PCLK only)
	TGE_GCLK = 52,      ///< Gating TGE clock (Gating PCLK only)

	RHE_GCLK = 55,      ///< Gating RHE clock (Gating PCLK only)
	I2C3_GCLK,          ///< Gating I2C3 clock (Gating PCLK only)
	TSE_GCLK,           ///< Gating TSE clock (Gating PCLK only)
	NOE_GCLK,           ///< Gating NOE clock (Gating PCLK only)
	ETH_GCLK,           ///< Gating ETH clock
	JPG2_GCLK,          ///< Gating JPEG2 clock

	GPIO_GCLK = 64,     ///< Gating GPIO PCLK (Gating PCLK only)
	INTC_GCLK,          ///< Gating INTC PCLK (Gating PCLK only)
	DMA_GCLK = 67,      ///< Gating DMAC PCLK (Gating PCLK only)

	PCLKGAT_MAXNUM,

	ENUM_DUMMY4WORD(GATECLK)
} GATECLK;


#define PRE_GCLK        1 ///< Backward compatible
#define IDE2_GCLK       1 ///< Backward compatible
#define MI_GCLK         1 ///< Backward compatible
#define HDMI_GCLK       1 ///< Backward compatible
#define MIPI_DSI_GCLK   1 ///< Backward compatible
#define AFFINE_GCLK     1 ///< Backward compatible
#define PWMCCNT_GCLK    1 ///< Backward compatible
#define RDE_GCLK        1 ///< Backward compatible
#define DRE_GCLK        1 ///< Backward compatible
#define SPI4_GCLK       1 ///< Backward compatible
#define SPI5_GCLK       1 ///< Backward compatible
#define SIE3_GCLK       1 ///< Backward compatible
#define SIE4_GCLK       1 ///< Backward compatible

/*
    @name Default Gating Clock Select definition

    This is for pll_configClkAutoGating() & pll_configPclkAutoGating().
*/
//@{
#define PLL_CLKSEL_DEFAULT_CLKGATE1     0x00000000
#define PLL_CLKSEL_DEFAULT_CLKGATE2     0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE1    0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE2    0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE3    0x00000000
//@}


/**
    PLL ID
*/
typedef enum {
	PLL_ID_1        = 1,        ///< PLL1 (internal 480 MHz)
	PLL_ID_3        = 3,        ///< PLL3 (for DMA)
	PLL_ID_4        = 4,        ///< PLL4 (for SSPLL)
	PLL_ID_5        = 5,        ///< PLL5 (for sensor/MCLK)
	PLL_ID_6        = 6,        ///< PLL6 (for IDE/eth)
	PLL_ID_7        = 7,        ///< PLL7 (for audio)
	PLL_ID_8        = 8,        ///< PLL8 (for CPU)
	PLL_ID_10       = 10,       ///< PLL10 (for DSP)
	PLL_ID_13       = 13,       ///< PLL13 (for IPP/H265/H264)

	PLL_ID_FIXED320 = 24,       ///< Fixed 320MHz PLL
	PLL_ID_MAX,
	ENUM_DUMMY4WORD(PLL_ID)
} PLL_ID;

#define PLL_ID_2    PLL_ID_MAX  ///< Backward compatible
#define PLL_ID_9    PLL_ID_MAX  ///< Backward compatible
#define PLL_ID_11   PLL_ID_MAX  ///< Backward compatible
#define PLL_ID_12   PLL_ID_MAX  ///< Backward compatible


extern ER       pll_setPLL(PLL_ID id, UINT32 uiSetting);
extern BOOL     pll_getPLLEn(PLL_ID id);
extern ER       pll_setPLLEn(PLL_ID id, BOOL bEnable);
extern ER 		pll_setPLLFreq(PLL_ID id, UINT32 uiFrequency);
extern UINT32   pll_getPLLFreq(PLL_ID id);
extern ER       pll_setPLLSpreadSpectrum(PLL_ID id, UINT32 uiLowerFreq, UINT32 uiUpperFreq);
extern ER       pll_getPLLSpreadSpectrum(PLL_ID id, UINT32 *puiLowerFreq, UINT32 *puiUpperFreq);

extern void     pll_setPWMClockRate(UINT32 uiPWMNumber, UINT32 uiDiv);
extern BOOL     pll_isClockEnabled(CG_EN Num);

extern void     pll_setClkAutoGating(GATECLK ClkSel);
extern void     pll_clearClkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getClkAutoGating(GATECLK ClkSel);

extern void     pll_setPclkAutoGating(GATECLK ClkSel);
extern void     pll_clearPclkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getPclkAutoGating(GATECLK ClkSel);

extern void     pll_setTRNGROSel(UINT32 uiTRNGROSel, UINT32 uiDiv);
extern void     pll_getTRNGROSel(UINT32 *puiTRNGROSel, UINT32 *puiDiv);

//@}

#endif



