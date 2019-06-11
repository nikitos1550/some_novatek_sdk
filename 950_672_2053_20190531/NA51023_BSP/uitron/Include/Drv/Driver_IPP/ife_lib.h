/*
    Public header file for IFE module.

    This file is the header file that define the API and data type for IFE
    module.

    @file       ife_lib.h
    @ingroup    mIDrvIPP_IFE

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef _IFE_LIB_660_H
#define _IFE_LIB_660_H

/**
    @addtogroup mIDrvIPP_IFE
*/
//@{

/*
    Define IFE function enable.
*/
//@{
//#define IFE_ROWDEFC_EN                  0x00001000  ///< enable function: row defect concealment
#define IFE_OUTL_EN                     0x00002000  ///< enable function: outlier
#define IFE_FILTER_EN                   0x00004000  ///< enable function: filter
#define IFE_CGAIN_EN                    0x00008000  ///< enable function: color gain
#define IFE_VIG_EN                      0x00010000  ///< enable function: vignette
#define IFE_GBAL_EN                     0x00020000  ///< enable function: Gbalance
//#define IFE_FLIP_EN                     0x00200000  ///< enable function: Gbalance
#define IFE_RGBIR_RB_NRFILLEN           0x00400000  ///< enable function: R-B hole filling before NR when RGBIR
#define IFE_GBAL_LOCWEIGHT_EN           0x00800000  ///< enable function: G-Balance local weighting
#define IFE_FUNC_ALL                    0x00C3E000  ///< all func enable mask
//@}


/**
    Define IFE interrput enable.
*/
//@{
#define IFE_INTE_FRMEND                 0x00000001  ///< frame end interrupt
//#define IFE_INTE_BP1                    0x00000002  ///< break point 1 interrupt
//#define IFE_INTE_ROWDEFFAIL             0x00000004  ///< row def conc interrupt
//#define IFE_INTE_STRPEND                0x00000008  ///< H stripe end interrupt
#define IFE_INTE_ALL                    0x00000001  ///< all interrupts
//@}

/**
    Define IFE interrput status.
*/
//@{
#define IFE_INT_FRMEND                 0x00000001  ///< frame end interrupt
//#define IFE_INT_BP1                    0x00000002  ///< break point 1 interrupt
//#define IFE_INT_ROWDEFFAIL             0x00000004  ///< row def conc interrupt
//#define IFE_INT_STRPEND                0x00000008  ///< H stripe end interrupt
#define IFE_INT_ALL                    0x00000001  ///< all interrupts
//@}

/**
    IFE Function Selection.

    IFE Function Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_OPMODE_D2D      = 0,    ///< D2D
	IFE_OPMODE_IPP      = 1,    ///< IFE->DCE->IPE->IME
	IFE_OPMODE_RDE_IPP  = 2,    ///< RDE + IFE->DCE->IPE->IME
	IFE_OPMODE_RHE_IPP  = 3,    ///< RHE + IFE->DCE->IPE->IME
	IFE_OPMODE_SIE_IPP  = 4,    ///< SIE + IFE->DCE->IPE->IME (all direct)
	IFE_OPMODE_UNKNOWN  = 5,    ///< Undefined mode
	ENUM_DUMMY4WORD(IFE_OPMODE)
} IFE_OPMODE;
//@}

/**
    IFE Register type.

    IFE Register type.
\n  Used for ife_changeAll()
*/
//@{
typedef enum {
	IFE_ALL_REG         = 0,    ///< Set all registers
	IFE_FD_LATCHED      = 1,    ///< Update VD-latched registers only
	IFE_NON_FD_LATCHED  = 2,    ///< Updat Non-VD-latched registers only
	ENUM_DUMMY4WORD(IFE_REGTYPE)
} IFE_REGTYPE;
//@}

/**
    IFE Source Format.

    Set IFE input data format
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_FMT_NORMAL   = 0,           ///< Prograssive data input
	IFE_FMT_RAW      = 1,           ///< RAW data input
	IFE_FMT_UVPACK   = 2,           ///< UV packing input
	ENUM_DUMMY4WORD(IFE_SRCFMT)
} IFE_SRCFMT;
//@}

/**
    IFE Filter mode Selection.

    Set IFE filter mode
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_NORMAL_9x9 = 0,           ///< standard filter in 9x9 area
	IFE_SONLY_9x9  = 1,           ///< spatial filter only in 9x9 area
	ENUM_DUMMY4WORD(IFE_FILTMODE)
} IFE_FILTMODE;
//@}

/**
    IFE binning Selection.

    binning level selection
*/
//@{
typedef enum {
	IFE_BINNING_DISABLE = 0,        ///< Disable binning
	IFE_BINNINGX2       = 1,        ///< binning x2
	IFE_BINNINGX4       = 2,        ///< binning x4
	IFE_BINNINGX8       = 3,        ///< binning x8
	IFE_BINNINGX16      = 4,        ///< binning x16
	IFE_BINNINGX32      = 5,        ///< binning x32
	IFE_BINNINGX64      = 6,        ///< binning x64
	IFE_BINNINGX128     = 7,        ///< binning x128
	ENUM_DUMMY4WORD(IFE_BINNSEL)
} IFE_BINNSEL;
//@}

/**
    IFE CFA Pattern Selection.

    Set IFE start pixel of the CFA pattern
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_SONLY_3x3 = 0,           ///< 3x3 spatial filter in 5x5 area
	IFE_SONLY_5x5 = 1,           ///< spatial filter only in 5x5 area
	ENUM_DUMMY4WORD(IFE_SONLYLEN)
} IFE_SONLYLEN;
//@}

#if 0
/**
    IFE Delta of Curve Mapping Selection.

    Set IFE step size between two table entries.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_DELTA_1 = 0,           ///< step size = 1
	IFE_DELTA_2 = 1,           ///< step size = 2
	IFE_DELTA_4 = 2,           ///< step size = 4
	ENUM_DUMMY4WORD(IFE_CRVMAP_DELTA)
} IFE_CRVMAP_DELTA;
//@}
#endif

#if 0///< removed in NT96510
/**
    IFE Outlier Count Selection.

    Set IFE number of neighbors to establish outlier condition.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_EIGHT_NEIGHBOURS = 0,           ///< eight neighbours fall in the criteria
	IFE_SEVEN_NEIGHBOURS = 1,           ///< seven neighbours fall in the criteria
	ENUM_DUMMY4WORD(IFE_OUTL_SEL)
} IFE_OUTL_SEL;
//@}
#endif

/**
    IFE Row Defect Concealment Factor Selection.

    Set IFE factor in row defect concealment.
\n  Used for ife_setMode()
*/
//@{
/*
typedef enum
{
    IFE_FACT_p5 = 0,           ///< factor = 0.5
    IFE_FACT_p25 = 1,          ///< factor = 0.25
    IFE_FACT_p0  = 2,          ///< factor = 0
    ENUM_DUMMY4WORD(IFE_ROWDEF_FACT)
} IFE_ROWDEF_FACT;
//@}
*/
/**
    IFE Wait Mode Selection.

    IFE Wait Mode Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_WAIT    = 0,                ///< Wait for frame end
	IFE_NOWAIT  = 1,                ///< Do not wait for frame end
	ENUM_DUMMY4WORD(IFE_WAITMODE)
} IFE_WAITMODE;
//@}


/**
    IFE Bit Depth.

    Set IFE input/output bitdepth
*/
//@{
typedef enum {
	IFE_8BIT    = 0,                ///< 8bit data,  image width should be 4x
	IFE_10BIT   = 1,                ///< 8bit data,  image width should be 16x
	IFE_12BIT   = 2,                ///< 12bit data, image width should be 8x
	IFE_16BIT   = 3,                ///< 16bit data, image width should be 4x
	ENUM_DUMMY4WORD(IFE_BITDEPTH)
} IFE_BITDEPTH;
//@}

/**
    IFE color gain range selection

    Select IFE color gain range
*/
//@{
typedef enum {
	IFE_CGRANGE_2_8 = 0,
	IFE_CGRANGE_3_7 = 1,
	ENUM_DUMMY4WORD(IFE_CGRANGE)
} IFE_CGRANGE;
//@}

/**
    IFE input Bayer data start CFA selection

    Select intput Bayer raw data start channel - R, Gr(Grb), Gb(Gir), B, Ir
*/
//@{
typedef enum {
	IFE_PATR  = 0,  ///< Start channel is R for input Bayer raw data
	IFE_PATGR = 1,  ///< Start channel is Gr for input Bayer raw data
	IFE_PATGB = 2,  ///< Start channel is Gb for input Bayer raw data
	IFE_PATB  = 3,  ///< Start channel is B for input Bayer raw data
	IFE_PATRGBG_GIGI = 4,///< Channel starts as RGBG_GIGI for input RGBIr raw data
	IFE_PATGBGR_IGIG = 5,///< Channel starts as GBGR_IGIG for input RGBIr raw data
	IFE_PATGIGI_BGRG = 6,///< Channel starts as GIGI_BGRG for input RGBIr raw data
	IFE_PATIGIG_GRGB = 7,///< Channel starts as IGIG_GRGB for input RGBIr raw data
	IFE_PATBGRG_GIGI = 8,///< Channel starts as BGRG_GIGI for input RGBIr raw data
	IFE_PATGRGB_IGIG = 9,///< Channel starts as GRGB_IGIG for input RGBIr raw data
	IFE_PATGIGI_RGBG = 10,///< Channel starts as GIGI_RGBG for input RGBIr raw data
	IFE_PATIGIG_GBGR = 11,///< Channel starts as IGIG_GBGR for input RGBIr raw data

	ENUM_DUMMY4WORD(IFE_CFASEL)
} IFE_CFASEL;
//@}


/**
    IFE burst length define
*/
//@{
typedef enum {
	IFE_BURST_32W    = 0,       ///< burst length 64 word
	IFE_BURST_16W    = 1,       ///< burst length 32 word
	//IFE_BURST_16W    = 3,       //removed in NT96680
	ENUM_DUMMY4WORD(IFE_BURST_SEL)
} IFE_BURST_SEL;

typedef struct _IFE_BURST_LENGTH {
	IFE_BURST_SEL   BurstLenInput;  ///< Input burst length
	IFE_BURST_SEL   BurstLenOutput; ///< Output burst length
} IFE_BURST_LENGTH;
//@}

/**
    Struct IFE open object.

    ISR callback function
*/
//@{
typedef struct _IFE_OPENOBJ {
	VOID (*FP_IFEISR_CB)(UINT32 uiIntStatus); ///< isr callback function
	UINT32 uiIfeClockSel; ///< support 220/192/160/120/80/60/48 Mhz
} IFE_OPENOBJ;
//@}

/**
    Struct IFE Range Filter Setting A.

    Parameters of range filter
*/
//@{
typedef struct _IFE_RANGESETA {
	UINT32 *pRngTh;            ///< range filter threshold
	UINT32 *pRngLut;           ///< range filter threshold adjustment
} IFE_RANGESETA;
//@}

/**
    Struct IFE Range Filter Advanced Parameter.

    Parameters of range filter
*/
//@{
typedef struct _IFE_RANGEPARM {
	BOOL   bBilatThEn;      ///< 3x3 bilateral filter threshold enable
	UINT32 uiBilatTh1;      ///< 3x3 bilateral filter threshold 1
	UINT32 uiBilatTh2;      ///< 3x3 bilateral filter threshold 2
} IFE_RANGEPARM;
//@}

/**
    Struct IFE Range Filter Setting B.

    Parameters of range filter
*/
//@{
typedef struct _IFE_RANGESETB {
	UINT32 *pRngTh;            ///< range filter threshold
	UINT32 *pRngLut;           ///< range filter threshold adjustment
	IFE_RANGEPARM RngParm;     ///< range filter advanced parameter
} IFE_RANGESETB;
//@}

/**
    Struct IFE Clamp and Weighting.

    Parameters of clamp and weight
*/
//@{
typedef struct _IFE_CLAMPWTSET {
	UINT32 uiClampTh;             ///< clamp threshold
	UINT32 uiClampMul;            ///< weighting multiplier
	UINT32 uiClampDlt;            ///< clamp threshold adjustment
} IFE_CLAMPWTSET;
//@}

/**
    Struct IFE Color Gain.

    Parameters of color gain
*/
//@{
typedef struct _IFE_CGAINSET {
	BOOL        bCgainInv;
	BOOL        bCgainHinv;
	IFE_CGRANGE CgainRange;
	UINT32      uiCgainMask;
	UINT32      *puiCGain;          ///< Color Gain, always R/Gr/Gb/B/Ir order
	UINT32      *puiCOfs;           ///< Color Offset, always R/Gr/Gb/B/Ir order
} IFE_CGAINSET;
//@}

/**
    Struct IFE Outlier Filter.

    Parameters of outlier filter
*/
//@{
typedef struct _IFE_OUTLSET {
	UINT32          *pOutlBriTh;    ///< outlier threshold for bright points
	UINT32          *pOutlDarkTh;   ///< outlier threshold for dark points
	UINT32          *pOutlCnt;      ///< outlier counter
	UINT32          OutlWeight;     ///< outlier weighting
	//IFE_OUTL_SEL    OutlOp;         ///< outlier option for neighbour condition
	UINT32          OutlDarkOfs;    ///< outlier dark offset
	UINT32          OutlBrightOfs;  ///< outlier bright offset
} IFE_OUTLSET;
//@}

/**
    Struct IFE Row Defect Concealment.

    Parameters of row defect concealment
*/
//@{
/*
typedef struct _IFE_ROWDEFSET
{
    IFE_ROWDEF_FACT RowDefFact; ///< factor of row defect concealment
    UINT32 uiStIdx;             ///< start index of row defect table
    UINT32 uiOffs;              ///< offset of row defect table entry
    UINT32 *pRowDefTbl;         ///< table of row defect concealment
} IFE_ROWDEFSET;
*/
//@}

/**
    IFE structure - IFE VIG.
*/
//@{
typedef struct _IFE_VIG_PARAM {
	UINT32 uiVigXC0;      ///< Legal range : -8192~8191,VIG x center of C0
	UINT32 uiVigYC0;      ///< Legal range : -8192~8191,VIG y center of C0
	UINT32 uiVigXC1;      ///< Legal range : -8192~8191,VIG x center of C1
	UINT32 uiVigYC1;      ///< Legal range : -8192~8191,VIG y center of C1
	UINT32 uiVigXC2;      ///< Legal range : -8192~8191,VIG x center of C2
	UINT32 uiVigYC2;      ///< Legal range : -8192~8191,VIG y center of C2
	UINT32 uiVigXC3;      ///< Legal range : -8192~8191,VIG x center of C3
	UINT32 uiVigYC3;      ///< Legal range : -8192~8191,VIG y center of C3
	UINT32  uiVigT;       ///< Legal range : 0~1023,VIG disabled area
	UINT8  uiVigTabGain; ///< Legal range : 0~3,VIG gain factor
	BOOL   bVigDitherEn; ///< VIG dithering enable
	BOOL   bVigDitherRst;///< VIG dithering reset, HW clear
	UINT32 uiVigXDiv;    ///< Legal range : 0~4095,VIG distance gain factor in x direction. unsigned 6.6
	UINT32 uiVigYDiv;    ///< Legal range : 0~4095,VIG distance gain factor in y direction. unsigned 6.6
	UINT32 *puiVigLutC0;   ///< Legal range : 0~1023,VIG LUT of C0 channel
	UINT32 *puiVigLutC1;   ///< Legal range : 0~1023,VIG LUT of C1 channel
	UINT32 *puiVigLutC2;   ///< Legal range : 0~1023,VIG LUT of C2 channel
	UINT32 *puiVigLutC3;   ///< Legal range : 0~1023,VIG LUT of C3 channel
} IFE_VIG_PARAM;
//@}

/**
    IFE structure - IFE Gbal.
*/
//@{
typedef struct _IFE_GBAL_PARAM {
	UINT32 uiGbalThr;  ///< Legal range : 0~1023, Gbal threshold for decision
	UINT8 *puiGbalStab; ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different luminance, 0 : NO Gbal, 7 : Max Gbal
	UINT8 *puiGbalDtab; ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different variation, 0 : NO Gbal, 7 : Max Gbal
	UINT8 uiGbalSmtThr; ///< Gbalance local adjustment threshold
} IFE_GBAL_PARAM;
//@}


/**
    IFE bayer format selection

    Select IFE bayer format (color Bayer or RGBIr)
*/
//@{
typedef enum {
	IFE_BAYER_COLOR = 0,
	IFE_BAYER_RGBIR = 1,
	ENUM_DUMMY4WORD(IFE_BAYERFORMAT)
} IFE_BAYERFORMAT;


//@}

/**
    IFE structure - IFE R-B Filling post-weight parameters.
    Only active when RGBIR format sensor is used.
*/
//@{
typedef struct _IFE_RBFILL_PARAM {
	UINT8   *puiRBFillLuma;  ///< Luminance-related weighting LUT for filled points
	UINT8   *puiRBFillRatio; ///< Gain ratio-related weighting LUT for filled points
	UINT8   uiRBRatioMode;   ///< Ratio description mode for fractional part bit numbers
} IFE_RBFILL_PARAM;
//@}


/**
    Struct IFE Filter Parameters.

    IFE filter detailed parameters.
*/
//@{
typedef struct _IFE_FILT_PARA {
	IFE_SRCFMT      SrcFmt;         ///< source format select
	IFE_CFASEL      CfaPat;         ///< CFA pattern if RAW format selected
	IFE_FILTMODE    FiltMode;       ///< filter mode select
	//BOOL            bRowDefEn;      ///< enable of row defect concealment
	BOOL            bOutlEn;        ///< enable of outlier filter
	BOOL            bFilterEn;      ///< enable of IFE filter
	BOOL            bCgainEn;       ///< enable of color gain
	BOOL            bVigEn;         ///< enable of vignette
	BOOL            bGbalEn;        ///< enable of Gbalance
	//BOOL            bFlipEn;        ///< enable of flip
	BOOL            bRBFillEn;      ///< enable R-B gain hole filling for RGBIr 4x4 raw
	BOOL            bGbalLocWEn;    ///< enable g-balance local adaptive weight modification
	UINT32          *puiWeightS;    ///< spatial weighting
	IFE_RANGESETA   RngThA;         ///< range filter threshold setting for filterA
	IFE_RANGESETB   RngThB;         ///< range filter threshold setting for filterB
	UINT32          uiBilatW;       ///< range filter A and B weighting
	UINT32          uiRthW;         ///< range filter and spatial filter weighting
	IFE_BINNSEL     Binn;           ///< image binning
	IFE_BAYERFORMAT BayerFormat;    ///< raw format
	BOOL            bSOnlyEn;       ///< spatial only enable
	IFE_SONLYLEN    SOnlyLen;       ///< filter length of spatial only
	IFE_CLAMPWTSET  ClampWtSet;     ///< clamp and weighting setting
	IFE_CGAINSET    CgainSet;       ///< color gain setting
	IFE_OUTLSET     OutlSet;        ///< outlier filter setting
	//IFE_ROWDEF_FACT RowDefFact;     ///< factor of row defect concealment
	UINT32          uiRowDefStIdx;  ///< start index of row defect table
	UINT32          uiRowDefVIni;   ///< vertical start of cropped window
	UINT32          *puiRowDefTbl;  ///< table of row defect concealment
	IFE_VIG_PARAM   VigSet;         ///< vignette setting
	IFE_GBAL_PARAM  GbalSet;        ///< gbalance setting
	IFE_RBFILL_PARAM RBFillSet;     ///< RBFill for NR setting while RGBIr raw
	UINT8           uiBitDither;    ///< Select bit dithering while truncation
	UINT32          uiRepeat;       ///< repeat times
} IFE_FILT_PARA;
//@}

/**
    Struct IFE Filter Parameters.

    IFE Size related parameters.
*/
//@{
typedef struct _IFE_SIZE_PARA {
	UINT32 uiWidth;               ///< image width
	UINT32 uiHeight;              ///< image height
	UINT32 uiOfsi;                ///< image input lineoffset
	UINT32 uiOfso;                ///< image output lineoffset
} IFE_SIZE_PARA;
//@}

/**
    Struct IFE ROI Parameters.

    IFE Address, Cropping detailed parameters.
*/
//@{
typedef struct _IFE_ROI_PARA {
	IFE_SIZE_PARA   Size;           ///< input starting address
	UINT32          uiInAddr;       ///< input starting address
	UINT32          uiOutAddr;      ///< output starting address
	//UINT32          uiCropStx;      ///< image horizontal crop pixel
	IFE_BITDEPTH    InBit;          ///< input bit select
	IFE_BITDEPTH    OutBit;         ///< out bit select
	//BOOL            bFlipEn;        ///< enable of flip
} IFE_ROI_PARA;
//@}



/**
    Struct IFE Information.

    IFE parameter and function selection

\n  Used for ife_setMode()
*/
//@{
typedef struct _IFE_PARAM {
	IFE_OPMODE        Mode;           ///< operation mode
	IFE_SRCFMT      SrcFmt;         ///< source format select
	IFE_CFASEL      CfaPat;         ///< CFA pattern if RAW format selected
	IFE_FILTMODE    FiltMode;       ///< filter mode select
	IFE_BITDEPTH    InBit;          ///< input bit select
	IFE_BITDEPTH    OutBit;         ///< out bit select
	//BOOL            bRowDefEn;      ///< enable of row defect concealment
	BOOL            bOutlEn;        ///< enable of outlier filter
	BOOL            bFilterEn;      ///< enable of IFE filter
	BOOL            bCgainEn;       ///< enable of color gain
	BOOL            bVigEn;         ///< enable of vignette
	BOOL            bGbalEn;        ///< enable of Gbalance
	//BOOL            bFlipEn;        ///< enable of flip
	BOOL            bRBFillEn;      ///< enable R-B gain hole filling for RGBIr 4x4 raw
	BOOL            bGbalLocWEn;    ///< enable g-balance local adaptive weight modification
	UINT32          *puiWeightS;    ///< spatial weighting
	IFE_RANGESETA   RngThA;         ///< range filter threshold setting for filterA
	IFE_RANGESETB   RngThB;         ///< range filter threshold setting for filterB
	UINT32          uiBilatW;       ///< range filter A and B weighting
	UINT32          uiRthW;         ///< range filter and spatial filter weighting
	IFE_BINNSEL     Binn;           ///< image binning
	IFE_BAYERFORMAT BayerFormat;    ///< raw format
	BOOL            bSOnlyEn;       ///< spatial only enable
	IFE_SONLYLEN    SOnlyLen;       ///< filter length of spatial only
	IFE_CLAMPWTSET  ClampWtSet;     ///< clamp and weighting setting
	IFE_CGAINSET    CgainSet;       ///< color gain setting
	IFE_OUTLSET     OutlSet;        ///< outlier filter setting
	//IFE_ROWDEF_FACT RowDefFact;     ///< factor of row defect concealment
	UINT32          uiRowDefStIdx;  ///< start index of row defect table
	UINT32          uiRowDefVIni;   ///< vertical start of cropped window
	UINT32          *puiRowDefTbl;  ///< table of row defect concealment
	IFE_VIG_PARAM   VigSet;         ///< vignette setting
	IFE_GBAL_PARAM  GbalSet;        ///< gbalance setting
	IFE_RBFILL_PARAM RBFillSet;     ///< RBFill for NR setting while RGBIr raw
	UINT8           uiBitDither;    ///< Select bit dithering while truncation
	UINT32          uiInAddr;       ///< input starting address
	UINT32          uiOutAddr;      ///< output starting address
	UINT32          uiWidth;        ///< image width
	UINT32          uiHeight;       ///< image height
	//UINT32          uiStartshift;   ///< image start shift
	//UINT32          uiCropStx;      ///< image horizontal crop pixel
	UINT32          uiInOfs;        ///< image input lineoffset
	UINT32          uiOutOfs;       ///< image output lineoffset
	UINT32          uiIntrEn;       ///< interrupt enable
} IFE_PARAM;
//@}

extern ER       ife_setMode(IFE_PARAM *pFilterInfo);
extern ER       ife_changeAll(IFE_PARAM *pFilterInfo, IFE_REGTYPE RegType);
extern ID       ife_getLockStatus(VOID);
extern VOID     ife_setHwReset(BOOL bEnable);
extern ER       ife_open(IFE_OPENOBJ *pObjCB);
extern BOOL     ife_isOpened(VOID);
extern ER       ife_close(VOID);
extern ER       ife_enableInt(UINT32 uiIntr);
extern UINT32   ife_getIntEnable(VOID);
extern UINT32   ife_getIntStatus(VOID);
extern ER       ife_clearInt(UINT32 uiIntr);
extern VOID     ife_waitFlagFrameEnd(VOID);
extern ER       ife_enableFunction(BOOL bEnable, UINT32 uiFunc);
extern BOOL     ife_checkFunctionEnable(UINT32 uiFunc);
extern IFE_OPMODE ife_checkOpMode(VOID);
extern UINT32 ife_getStripeCnt(VOID);
extern BOOL     ife_D2DisBusy(VOID);
extern VOID     ife_changeParam(IFE_FILT_PARA *pFiltPara);
extern VOID     ife_changeSize(IFE_SIZE_PARA *pSizePara);
extern VOID     ife_changeROI(IFE_ROI_PARA *pROIPara);
extern ER       ife_start(VOID);
extern ER       ife_dbgStart(UINT32 loadtype);
extern ER       ife_pause(VOID);
extern UINT32   ife_getClockRate(VOID);
extern ER       ife_getBurstLength(IFE_BURST_LENGTH *pBurstLen);
//FPGA USE
extern ER ife_setOpMode(IFE_OPMODE Mode);
#endif

//@}
