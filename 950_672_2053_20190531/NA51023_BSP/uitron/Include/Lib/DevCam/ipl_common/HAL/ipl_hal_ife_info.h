/**
    IPH module (IFE sub function type define)

    @file       IPL_Hal_IFE_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_Hal_IFE_Info_H
#define _IPL_Hal_IFE_Info_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    engine clk
*/
typedef enum _IPH_IFECLK {
	IPH_IFECLK0 = 240,  ///< -240 mhz
	IPH_IFECLK1 = 360,  ///< -360 mhz
	ENUM_DUMMY4WORD(IPH_IFECLK)
} IPH_IFECLK;

/**
    IFE Input Data Source
*/
typedef enum _IFE_SRCMODE {
	IFE_D2D     = 0,    ///< IFE only
	IFE_IPP     = 1,    ///< IFE->DCE->IPE->IME
	IFE_RHE_IPP = 3,    ///< RHE + IFE->DCE->IPE->IME(IFE2)
	IFE_SIE_IPP = 4,    ///< ALL DIRECT
	ENUM_DUMMY4WORD(IFE_SRCMODE)
} IFE_SRCMODE;

/**
    ife data format
*/
typedef enum _IFEFMT {
	IFE_Normal  = 0,    ///< non uv pack
	IFE_RAW     = 1,    ///< byaer
	IFE_UVPack  = 2,    ///< uv packed
	ENUM_DUMMY4WORD(IFEFMT)
} IFEFMT;

/**
    IFE input Bayer format selection

    Select intput Bayer format
*/
typedef enum _IFE_BAYERFMT_SEL {
	IFE_COLOR   = 0, ///< Traditional RGGB format
	IFE_RGBIR   = 1, ///< RGBIr format
	ENUM_DUMMY4WORD(IFE_BAYERFMT_SEL)
} IFE_BAYERFMT_SEL;

/**
    IFE data bitdepth
*/
typedef IPL_RAW_BIT IFE_PackBus;

/**
    IFE bayer CFA
*/
typedef IPL_RAW_PIX IFE_CFA;

/**
    IFE input information
*/
typedef struct _IFE_IN_DMA {
	IPL_RAW_IMG Info;
} IFE_IN_DMA;

/**
    ife dma output information
*/
typedef struct _IFE_DMA_OUT {
	UINT32  Addr;           ///< must be needed if IFE output to dram
	UINT32  LineOffset;     ///< -
	IFE_PackBus pack;       ///< -
} IFE_DMA_OUT;

/**
    IFE interrupt enable enum
*/
typedef enum _IfeInteEn {
	IFE__CLR        = 0,
	IFE__INTE_FMD   = 0x00000001,   ///< frame end interrupt
	IFE__INTE_ALL   = 0x00000001,   ///< all interrupts
	ENUM_DUMMY4WORD(IfeInteEn)
} IfeInteEn;

/**
    IFE sub function enable enum
*/
typedef enum _IfeFuncEn {
	OutlierEn           = 0x00000002,       ///< enable outLier
	FilterEn            = 0x00000004,       ///< enable bilateral filter
	CgainEn             = 0x00000008,       ///< enable color gain
	VIGEN               = 0x00000010,       ///< enable function: vignette
	GBALEN              = 0x00000020,       ///< enable function: Gbalance
	RGBIR_RB_NRFILLEN   = 0x00000040,       ///< enable function: R-B hole filling before NR when RGBIR
	GBAL_LOCAL_WEIGTEN  = 0x00000080,       ///< enable function: G-Balance local weighting

	ENUM_DUMMY4WORD(IfeFuncEn)
} IfeFuncEn;

/**
    range filter
*/
typedef struct _RangeCFGA {
	UINT32  *pThres;    ///< range filter threshold, tab num:6, Data type: UINT32
	UINT32  *pThLut;    ///< range filter threshold adjustment, tab num:17, Data type: UINT32
} RangeCFGA;
typedef struct _RangeCFGB {
	UINT32  *pThres;    ///< range filter threshold, tab num:6, Data type: UINT32
	UINT32  *pThLut;    ///< range filter threshold adjustment, tab num:17, Data type: UINT32

	struct {
		BOOL   BilatThEn;   ///< 3x3 bilateral filter threshold enable
		UINT32 BilatTh1;    ///< 3x3 bilateral filter threshold 1
		UINT32 BilatTh2;    ///< 3x3 bilateral filter threshold 2
	} Rangeparam;
} RangeCFGB;

/**
    ife vignette shading compensation
*/
typedef struct _IFE_VIG {
	struct {
		iCoordinate  ch0;     ///< channel 0 < Legal range : -8192~8191,VIG center of R
		iCoordinate  ch1;     ///< channel 1 < Legal range : -8192~8191,VIG center of G
		iCoordinate  ch2;     ///< channel 2 < Legal range : -8192~8191,VIG center of B
		iCoordinate  ch3;     ///< channel 3 < Legal range : -8192~8191,VIG center of B
	} Center;               ///< center coordinate

	UINT32  T;              ///<disabled area < Legal range : 0~1023,VIG disabled area
	UINT32  TabGain;        ///<gain factor
	UINT32  XDIV;           ///<scaling factor in x direction. unsigned 6.6
	UINT32  YDIV;           ///<scaling factor in y direction. unsigned 6.6
	UINT32  CH0_LUT_TblAddr;  ///<LUT of channel 0(tab num:17, Data type:UINT32)< Legal range : 0~1023,VIG LUT of R channel
	UINT32  CH1_LUT_TblAddr;  ///<LUT of channel 1(tab num:17, Data type:UINT32)< Legal range : 0~1023,VIG LUT of G channel
	UINT32  CH2_LUT_TblAddr;  ///<LUT of channel 2(tab num:17, Data type:UINT32)< Legal range : 0~1023,VIG LUT of B channel
	UINT32  CH3_LUT_TblAddr;  ///<LUT of channel 3(tab num:17, Data type:UINT32)< Legal range : 0~1023,VIG LUT of B channel

	BOOL    bDither;        ///<dithering enable/disable
	BOOL    DitherRstEn;    ///<dithering reset enable
} IFE_VIG;

/**
    IFE structure - IFE gbalance parameter set.
*/
typedef struct _IFE_GBAL {
	UINT32 GbalThr;         ///< Legal range : 0~1023, Gbal threshold for decision
	UINT8  *pGbalStab;      ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different luminance, 0 : NO Gbal, 7 : Max Gbal
	UINT8  *pGbalDtab;      ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different variation, 0 : NO Gbal, 7 : Max Gbal
	UINT8   uiGbalSmtThr;   ///< Gbalance local adjustment threshold
} IFE_GBAL;

/**
    IFE outlier func
*/
typedef struct _IFE_OUTL {
	UINT32 *pBrightThres;   ///< tab num:5 Data type:UINT32
	///< 0: avg<32 , (bit range: 8-bit)
	///< 1: avg<64
	///< 2: avg<128
	///< 3: max:255
	///< 4: for interpolation
	UINT32 *pDarkThres;     ///< same as above
	UINT32 *pOutlCnt;       ///< outlier counter
	UINT32  OutlWeight;     ///< outlier weighting
	UINT32  OutlDarkOfs;    ///< outlier dark offset
	UINT32  OutlBrightOfs;  ///< outlier bright offset

} IFE_OUTL;

/**
    IFE R-B Filling post-weight parameters
    Only active when RGBIR format sensor is used
*/
typedef struct _IFE_RBFILL {
	UINT8   *puiRBFillLuma;  ///< Luminance-related weighting LUT for filled points
	UINT8   *puiRBFillRatio; ///< Gain ratio-related weighting LUT for filled points
	UINT8   uiRBRatioMode;   ///< Ratio description mode for fractional part bit numbers
} IFE_RBFILL;


/**
    IFE color gain
*/
typedef struct _IFE_CG {
	struct {
		BOOL    Inv;        ///< -
		BOOL    Hinv;       ///< -
	} CG_Inv;
	enum {
		_2_8    = 0,        ///< 2bits decimal, 8bits fraction
		_3_7    = 1,        ///< 3bits decimal, 7bits fraction
		ENUM_DUMMY4WORD(IfeGainSel)
	} IfeGainSel;           ///< gain bit field
	struct {
		UINT32  GainMask;   ///< -
	} CG_Mask;
	UINT32  *pCGain;        ///< -Color gain(R GR GB B Ir): tab num:5
	UINT32  *pCOfs;         ///< -Color offset(R GR GB B Ir): tab num:5
} IFE_CG;
typedef struct {
	BOOL    sOnlyEn;        ///< spatial filter only
	enum {
		Filt_3x3    = 0,    ///<    convolution window:3x3
		Filt_5x5    = 1,    ///<    5x5
		ENUM_DUMMY4WORD(sOnlyFiltLeng)
	} sOnlyFiltLeng;        ///< filter length when spatial only enable
	UINT32  *pWeight;       ///< spatial weighting, tab num: 6, Data type: UINT32
} Spatial_Filter;
typedef struct {
	RangeCFGA *A;           ///< -
	RangeCFGB *B;           ///< -
} Range_Filter;

/**
    IFE bilateral filter
*/
typedef struct _IFE_FILT {
	Spatial_Filter *Spatial;///< spatial filter
	Range_Filter *Range;    ///< range filter
	struct {
		UINT32  Th;         ///< threshold
		UINT32  Mul;        ///< weighting multiplier
		UINT32  Dlt;        ///< threshold adjustment
	} Clamp;                ///< clamp configuration

	UINT32  BilatW;         ///< range filter A and B weighting
	UINT32  Rth_W;          ///< range filter and spatial filter weighting
	UINT32  Bin;            ///< range:0~7.     ( Denominator: 2<<bin  )
	UINT8   BitDither;      ///< Select bit dithering while truncation
} IFE_FILT;

/**
     IFE sub function configuration.
*/
typedef struct _IFE_SUBFUNC {
	IfeInteEn   ifeInteEn;      ///< ife interrupt enable
	IfeFuncEn   ifeFuncEn;      ///< ife function enable

	IFE_CG      CGain;          ///< color gain configuration

	IFE_OUTL    *pOutl;         ///< outlier configuration
	IFE_FILT    *pFilter;       ///< bilateral configuration
	IFE_GBAL    *pGbal;         ///< G balance
	IFE_VIG     *pVig;          ///< vignette configuration

	IFE_RBFILL  rbfill_param;   ///< RBFill for NR setting while RGBIr raw

} IFE_SUBFUNC;
//@}
#endif
