/**
    IPH module (DCE sub function type define)

    @file       ipl_hal_dce_info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_Hal_DCE_Info_H
#define _IPL_Hal_DCE_Info_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    engine clk
*/
typedef enum _IPH_DCECLK {
	IPH_DCECLK0 = 240,  ///< -240 mhz
	IPH_DCECLK1 = 360,  ///< -360 mhz
	ENUM_DUMMY4WORD(IPH_DCECLK)
} IPH_DCECLK;

typedef enum {
	IPH_DCE_SRAM_CLK_DCE = 0,   ///< DCE has SRAM control
	IPH_DCE_SRAM_CLK_PCLK,      ///< CPU has SRAM control
	ENUM_DUMMY4WORD(IPH_DCE_SRAM_CLK_SEL)
} IPH_DCE_SRAM_CLK_SEL;

/**
    DCE Input Data Source
*/
typedef enum _DCE_SRCMODE {
	DCE_IPP     = 0,    ///< Flow 0(RHE->IFE->DCE->IPE->IME)
	DCE_DCE_IPP = 1,    ///< Flow 1(DCE->IPE->IME)
	DCE_SIE_IPP = 2,    ///< Flow 2(SIE->RHE->IFE->DCE->IPE->IME)
	DCE_D2D     = 3,    ///< DCE only
	ENUM_DUMMY4WORD(DCE_SRCMODE)
} DCE_SRCMODE;

/**
    Radius factor
*/
typedef struct _Fact_Norm {
	BOOL    Manual;     ///< if Manual is FALSE, Factor and Bit will be calculated automatically.
	UINT32  Factor;     ///< normalize factor
	UINT32  Bit;        ///< normalize bit
} Fact_Norm;

/**
    DCE RAW data format
*/
typedef enum {
	DCE_FMT_BAYER_2X2 = 0,
	DCE_FMT_RGBIR_4X4,
	ENUM_DUMMY4WORD(DCE_HAL_RAW_FMT)
} DCE_HAL_RAW_FMT;


/**
    DCE input bayer CFA
*/
typedef IPL_RAW_PIX DCE_CFA;

/**
    DCE Pink reduction mode
*/
typedef enum {
	DCE_HAL_PINKR_G = 0,    ///< modify G channel
	DCE_HAL_PINLR_RB,       ///< modify RB channel
	ENUM_DUMMY4WORD(DCE_HAL_PINKR_MODE)
} DCE_HAL_PINKR_MODE;

/**
    CFA interpolation parameter set
*/
typedef struct _DCE_HAL_CFA_HFC {
	BOOL      bCLCheckEn;   ///< Enable cross line check
	BOOL      bHFCheckEn;   ///< Enable high frequency check
	BOOL      bAverageMode; ///< Interpolation method selection when flat region detect
	BOOL      bCfaCLSel;    ///< operation type when cross line is not found
	UINT32    uiHFGTh;      ///< G channel smoothness checking threshold
	UINT32    uiCLTh;       ///< Cross line checking threshold
	UINT32    uiHFDiff;     ///< Difference threshold value between Gr and Gb to determing high frequency pattern detected or not
	UINT32    uiHFEThr;     ///< Threshold to determine H or V direction high frequency pattern detected
} DCE_HAL_CFA_HFC;

typedef struct _DCE_HAL_CFA_IR {
	UINT32    uiIrSubR;         ///< R channel IR sub.
	UINT32    uiIrSubG;         ///< G channel IR sub.
	UINT32    uiIrSubB;         ///< B channel IR sub.
	UINT32    uiLocSubLBound;   ///< Lower bound of IR local weighting
	UINT32    uiLocSubThr;      ///< Threshold to decay IR local weighting
	UINT32    uiLocSubRange;    ///< Range to decay IR local weighting
	UINT32    uiIrSatGain;      ///< Saturation gain multiplied after IR sub
} DCE_HAL_CFA_IR;

typedef struct _DCE_HAL_CFA_CGAIN {
	UINT32    uiGainR;          ///< R channel gain
	UINT32    uiGainG;          ///< G channel gain
	UINT32    uiGainB;          ///< B channel gain
	BOOL      GainRange_3_7;    ///< select fraction type of gain: 0->2.8 or 1->3.7
} DCE_HAL_CFA_CGAIN;

typedef struct _DCE_HAL_CFA_PINKR {
	DCE_HAL_PINKR_MODE PinkRMode;   ///< pink reduction mode
	UINT32    uiPinkRThr1;          ///< pink reduction threshold 1
	UINT32    uiPinkRThr2;          ///< pink reduction threshold 2
	UINT32    uiPinkRThr3;          ///< pink reduction threshold 3
	UINT32    uiPinkRThr4;          ///< pink reduction threshold 4
} DCE_HAL_CFA_PINKR;

typedef struct _DCE_CFAPARAM {
	DCE_HAL_RAW_FMT RawFmt; ///< CFA start pattern
	UINT32  NsmarEdge;      ///< Noise margin for edge
	UINT32  NsmarDiff;      ///< Noise margin for pixel difference
	UINT32  DIFNormBit;     ///< Legal range : 0~3, Normalize bit of difference decision
	struct {
		BOOL    En;         ///< Enable for G channel correction
		UINT32  NormBit;    ///< G correction norm bit
		UINT32  CorrTh;     ///< G correction noise threshold
	} GC;                   ///< G channel correction
	struct {
		BOOL    En;         ///< Enable for RB channel correction
		UINT32  NormBit;    ///< RB correction norm bit
		UINT32  CorrTh1;    ///< RB correction noise threshold
		UINT32  CorrTh2;    ///< RB correction noise threshold
	} RBC;                  ///< RB channel correction
	DCE_HAL_CFA_HFC hfc;    ///< CFA high frequency check term
	DCE_HAL_CFA_IR CfaIrSub;        ///< CFA IR subtraction term
	DCE_HAL_CFA_PINKR CfaPinkReduc; ///< CFA pink reduction term
	DCE_HAL_CFA_CGAIN CfaCGain;     ///< CFA color gain term
} DCE_CFAPARAM;
/**
    sie interrupt enable
*/
typedef enum _DceInteEn {
	DCE__CLR        = 0,
	DCE__INTE_FMS   = 0x00000001,   ///< frame start interrupt
	DCE__INTE_FMD   = 0X00000002,   ///< frame end interrupt
	DCE__INTE_STRPE = 0x00000004,   ///< current stripe end interrupt
	DCE__INT_STPERR = 0x00000008,   ///< Interrupt for DCE stripe error
	DCE__INT_LBOVF  = 0x00000010,   ///< Interrupt for DCE line buffer overflow error
	DCE__INT_STPOB  = 0x00000040,   ///< Interrupt for DCE stripe boundary overflow error
	DCE__INT_YBACK  = 0x00000080,   ///< Interrupt for DCE Y coordinate backward skip error
	ENUM_DUMMY4WORD(DceInteEn)
} DceInteEn;
/**
    DCE sub function enable enum
*/
typedef enum _DceFuncEn {
	FUNC_CFA            = 0x00000001,   ///< CFA interpolation function
	FUNC_DC             = 0x00000002,   ///< Distortion correction function
	FUNC_CAC            = 0x00000004,   ///< Chromatic aberration correction function
	FUNC_D2D_RAND       = 0x00000008,   ///< D2D random LSB function for verifcation
	FUNC_D2D_UVFILT     = 0x00000010,   ///< D2D UV filter function
	FUNC_CROP           = 0x00000020,   ///< Output crop function
	FUNC_D2D_IOSTOP     = 0x00000040,   ///< D2D In/Out Dram Stop
	FUNC_CFA_GC         = 0x00000080,   ///< G channel correction function
	FUNC_CFA_RBC        = 0x00000100,   ///< RB channel correction function
	FUNC_CFA_PINKR      = 0x00000200,   ///< Pink reduction function
	FUNC_CFA_CL         = 0x00000400,   ///< -
	FUNC_CFA_HF         = 0x00000800,   ///< -
	FUNC_CFA_4X4RB_DIR  = 0x00001000,   ///< -
	ENUM_DUMMY4WORD(DceFuncEn)
} DceFuncEn;
typedef enum {
	FOV_BOUND_DUPLICATE = 0,        ///< Replace out of boundary pixels with duplicate boundary pixels
	FOV_BOUND_REG_RGB   = 1,        ///< Replace out of boundary pixels with GEO_BOUNDR, GEO_BOUNDG, GEO_BOUNDB
	ENUM_DUMMY4WORD(FOV_SELECT)
} FOV_SELECT;
/**
    Field of view
*/
typedef struct _FOV {
	UINT32  Gain;       ///< FOV lut gain
	FOV_SELECT FOV_SEL;
	struct {
		UINT32  R;      ///< boundary pixel of R channel,(enable when user mode enabled)
		UINT32  G;      ///< boundary pixel of G channel
		UINT32  B;      ///< boundary pixel of B channel
	} BoundryProcess;
} FOV;                  ///< Field of view setting

/**
    Geometric enhancement
*/

typedef enum {
	CAC_RGB_GAIN = 0,   ///< Correct CA with G LUT and R, G, B LUTGAIN
	CAC_RGB_LUT  = 1,   ///< Correct CA with R, G, B LUT
	ENUM_DUMMY4WORD(CAC_SEL)
} CAC_SEL;
/**
    Color aberration correction factor
*/
typedef struct _Aberation {
	CAC_SEL Cacsel;
	struct {
		UINT32  Fact_R;     ///<  R channel lut gain
		UINT32  Fact_G;     ///<  G channel lut gain
		UINT32  Fact_B;     ///<  R channel lut gain
	} CACGain;
} Aberation;

/**
    horizontal/vertical distance factors and distortion selection
*/
typedef struct _DIST {
	UINT32  Fact_X;         ///< X input distance factor for oval shape modeling
	UINT32  Fact_Y;         ///< Y input distance factor for oval shape modeling
} DIST;

//@}

/**
    @name DCE struct - 2D lut offset
*/
//@{
typedef struct {
	UINT32    XOfsInt;   ///< 2D lut x integer offset
	UINT32    XOfsFrac;  ///< 2D lut x fraction offset
	UINT32    YOfsInt;   ///< 2D lut y integer offset
	UINT32    YOfsFrac;  ///< 2D lut y fraction offset
} _2DLUT_OFFSET;
//@}

/**
    distortion compensation
*/
typedef struct _DCE_Distortion {
	BOOL gdc_mode_xonly;        ///< 0 -> both X-Y, 1 -> X only
	Coordinate  Center;         ///< Distortion center
	FOV         Fov;            ///< Field of view setting
	Aberation   ColorAber;      ///< Color aberration correction factor setting
	struct {
		UINT32 *pGGeoLut;           ///< GDC/CAC G geo lut(tab num:65, Data type:UINT32)
		INT32 *pRGeoLut;            ///< GDC/CAC R geo lut(tab num:65, Data type:INT32)
		INT32 *pBGeoLut;            ///< GDC/CAC B geo lut(tab num:65, Data type:INT32)
	} GeoLUT;
	BOOL        UV_FilterEn;     ///< ///< Horizontal filter for UV channel
} DCE_Distortion;

/**
     DCE sub function configuration.
*/
typedef struct _DCE_SUBFUNC {
	DceInteEn   dceInteEn;              ///< interrupt enable
	UINT32      dceFuncEn;              ///< Function enable selection
	DIST        Dce_Dist;               ///< Distance configuration
	Fact_Norm   Radious;                ///< GEO normaliztion factor and shift bit setting
	DCE_Distortion  *pDistortion;       ///< distortion configuration
	DCE_CFAPARAM    *pCFAInterpolation; ///< CFA interpolation
} DCE_SUBFUNC;
//@}

#endif
