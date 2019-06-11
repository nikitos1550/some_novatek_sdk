/**
    IPH module (IME sub function type define)

    @file       IPL_Hal_IME_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IME_Info_H
#define _IPL_Hal_IME_Info_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    ime engine supported clk
*/
typedef enum _IPH_IMECLK {
	IPH_IMECLK0 = 240,  ///< -240 mhz
	IPH_IMECLK1 = 360,  ///< -360 mhz
	ENUM_DUMMY4WORD(IPH_IMECLK)
} IPH_IMECLK;


typedef enum _IME_SRCMODE {
	IME_D2D             = 0,        ///< IME only
	IME_RHE_IPP         = 1,        ///< RHE + IFE->DCE->IPE->IME
	IME_SIE_IPP         = 2,        ///< SIE + IFE->DCE->IPE->IME (all direct)
	ENUM_DUMMY4WORD(IME_SRCMODE)
} IME_SRCMODE;                     ///< IME operation mode
/**
    IME in/out data format
*/
typedef IPL_YUV_IMG_TYPE IMEFMT;

typedef struct {
	UINT32  Y;
	UINT32  Cb;
	UINT32  Cr;
} IMEADDR;

typedef struct {
	IPL_YCC_CH_INFO Y;
	UINT32  Lineoffset_CC;
} IMESIZE;

/**
    IME interrupt enable enum.
*/
typedef enum _ImeInteEn {
	IME__CLR                    = 0,
	IME__INTE_MD_RC_JAM         = 0x00004000,       ///< MD STA read fail interrupt enable
	IME__INTE_MD_WC_JAM         = 0x00008000,       ///< MD STA write fail interrupt enable
	IME__INTE_MD_JOB_OVF        = 0x00010000,       ///< MD Process fail interrupt enable
	IME__INTE_MD_TAMPER         = 0x00020000,       ///< MD detect tamper event interrupt enable
	IME__INTE_MD_SCENE_CHANGE   = 0x00040000,       ///< MD detect scene change event interrupt enable
	IME__INTE_STPE              = 0x40000000,       ///< enable interrupt: stipe end
	IME__INTE_FMD               = 0x80000000,       ///< enable interrupt: frame end
} ImeInteEn;

/**
    IME sub function enable
*/
typedef enum _ImeFuncEn {
	ChromaAdaptEn   = 0x00000001,   ///< enable function: chroma adaption (LCA)
	YUVConverterEn  = 0x00000002,   ///< enable function: YUV Converter Enable/Disable
	ChromaAdjLchrom = 0x00000004,   ///< enable function: chroma adjustment of local chroma adaptation
	ChromaAdjLum    = 0x00000008,   ///< enable function: chroma adjustment of local luma adaptation
	ChromaDBSuppres = 0x00000010,   ///< enable function: dark and bright region chroma suppression
	TMNREn          = 0x00000020,   ///< enable function: TMNR(include chroma, staout)
	FilmGrainEn     = 0x00000040,   ///< enable function: film grain
	RGBtoYCCEn      = 0x00000080,   ///< enable function: RGB spaec to YCC space
	YCCtoRGBEn      = 0x00000100,   ///< enable function: YCC space to RGB space
	MDEn            = 0x00000200,   ///< enable function: motion detection
	ENUM_DUMMY4WORD(ImeFuncEn)
} ImeFuncEn;

typedef struct _IME_GET_STATS_INFO {
	UINT32 uiGetHistMax0;      ///< max value of histogram area of set0
	UINT32 uiGetAccTagBin0;    ///< hit bin of histogram accumulation of set0

	UINT32 uiGetHistMax1;      ///< max value of histogram area of set1
	UINT32 uiGetAccTagBin1;    ///< hit bin of histogram accumulation of set1
} IME_GET_STATS_INFO;

/**
    stripe overlap enum
*/
typedef enum _IME_STPESEL {
	IME_STPE_OVERLAP_16     = 0,    ///< overlap 16 pixels
	IME_STPE_OVERLAP_24     = 1,    ///< overlap 24 pixels
	IME_STPE_OVERLAP_32     = 2,    ///< overlap 32 pixels
	IME_STPE_OVERLAP_USER   = 3,    ///< overlap 32 pixels
	ENUM_DUMMY4WORD(IME_STPESEL)
} IME_STPESEL;

typedef enum _IME_STRP_PRT_SEL {
	IME_STRP_PRT_5P  = 0,    ///< Horizontal stripe overlap partition size: 5 pixels
	IME_STRP_PRT_3P  = 1,    ///< Horizontal stripe overlap partition size: 3 pixels
	IME_STRP_PRT_2P  = 2,    ///< Horizontal stripe overlap partition size: 2 pixels
	IME_STRP_PRT_USER = 3,    ///< Horizontal stripe overlap partition size: user define
	ENUM_DUMMY4WORD(_IME_STRP_PRT_SEL)
} IME_STRP_PRT_SEL;

/**
    stripe unit
*/
typedef struct _IME_STRIPE {
	UINT32  N;          ///< Size of stripe except for last one, Horn. unit : 16 pix, Ver. unit : 1 line
	UINT32  L;          ///< Size of last stripe, Horn. unit : 16 pix, Ver. unit : 1 line
	UINT32  M;          ///< Number of total stripe - 1
	UINT32  tab;        ///< stripe size table, for varied size mode
} IME_STRIPE;

typedef struct _IME_HAL_STRIPE_INFO {
	IME_STRIPE       strp_h;        ///< hroizontal stripe parameters
	IME_STRIPE       strp_v;        ///< Vertical stripe parameters
	IME_STPESEL      overlap_sel;   ///< IME input stripe overlap size selection for horizontal direction
	UINT32           overlap_user;  ///< IME input stripe overlap size for horizontal direction
	IME_STRP_PRT_SEL prt_sel;       ///< IME input stripe overlap parition size selection for horizontal direction
	UINT32           prt_user;      ///< IME input stripe overlap parition size for horizontal direction
	BOOL             manual_mode;   ///< IME input stripe information setting mode
	BOOL             varied_mode;   ///< Stripe size mode selection, 0 -> fix, 1 -> varied
} IME_HAL_STRIPE_INFO;

/**
     ime scaling method
     @note integration is supported only when image scaling down.
*/
typedef enum _IME_SCALER {
	IME_BICUBIC     = 0,    ///< bicubic interpolation
	IME_BILINEAR    = 1,    ///< bilinear interpolation
	IME_NEAREST     = 2,    ///< nearest interpolation
	IME_INTEGRATION = 3,    ///< integration interpolation,support only scale down
	IME_AUTO        = 4,    ///< automatical calculation
	IME_SCALER_MAX,
	ENUM_DUMMY4WORD(IME_SCALER)
} IME_SCALER;

/**
    IME output configuration
*/
typedef struct _STITCH_IMG {
	UINT32      LineOfsY;       ///< path output lineoffset for separating output
	UINT32      LineOfsCC;      ///< path output lineoffset for separating output
	UINT32      StPos;          ///< path output separating position
} STITCH_IMG;

typedef struct _STITCH_INFO {
	IMEADDR     Addr;           ///< path output DMA address for separating output
	STITCH_IMG  Img;            ///< stitch image information
} STITCH_INFO;

typedef struct _OUTCH {
	BOOL    PathEn;         ///< path enable
	BOOL    DmaOutEn;       ///< data output to dram enable
	BOOL    StitchEn;       ///< path separated output to dram enable, also set "OutPathSprtPos"
	IMEFMT  Fmt;            ///< format
	IMESIZE Size;           ///< scale output size
	struct {
		Coordinate Start;  ///< -
		UINT32  SizeH;      ///< -
		UINT32  SizeV;      ///< -
	} Crop;                 ///< ime output crop window
	IMEADDR Addr;           ///< YCC address
	IME_SCALER ScalMethod;  ///< CbCr channel have no bicubic, scaling up have no integration(all 3 path).
	STITCH_INFO StitchInfo; ///< stitch information
} OUTCH;

/**
    scaling quality(enhance) parameter set

    @note: only work when ScalMethod = _Bicubic
*/
typedef struct _IMESCALE_Enh {
	UINT32  Factor;     ///< -
	UINT32  Norm_bit;   ///< -
} IMESCALE_Enh;

/**
    scaling quality(filter)parameter set
*/
typedef struct _IMESCALE_Filt {
	BOOL    CoefUserDefEn;      ///< user define mode enable bit
	struct {
		BOOL    HsfEn;          ///< H direction scaling filter
		BOOL    VsfEn;          ///< V direction scaling filter
		UINT32  HsfCoef;        ///< H direction coefficient
		UINT32  VsfCoef;        ///< V direction coefficient
	} UserDefine;               ///< user defined
} IMESCALE_Filt;

/**
    @name IME enum - Value Range Selection
*/
//@{
typedef enum _RANGE_SEL {
	RANGE_8   = 0,  ///< range 8
	RANGE_16  = 1,  ///< range 16
	RANGE_32  = 2,  ///< range 32
	RANGE_64  = 3,  ///< range 64
	RANGE_128 = 4,  ///< range 128
	ENUM_DUMMY4WORD(RANGE_SEL)
} RANGE_SEL;
//@}
/**
    chroma adaption parameter set
*/
typedef struct _IME_CHRA_ADAP {
	struct {
		UINT32          LcaRefY;        ///< Chroma reference weighting for Y channels
		UINT32          LcaRefUV;       ///< Chroma reference weighting for UV channels
		UINT32          LcaOutUV;       ///< Chroma adaptation output weighting
	} LcaRefWt;
	struct {
		RANGE_SEL       LcaY;           ///< Chroma adaptation range selection for Y channels, Max = IME_RANGE_128
		UINT32          LcaYTh;         ///< Chroma adaptation range threshold for Y channels
		RANGE_SEL       LcaYWtPrc;      ///< Chroma adaptation weighting precision for Y channel, Max = IME_RANGE_64
		UINT32          LcaYWtS;        ///< Chroma adaptation weighting start value for Y channels
		UINT32          LcaYWtE;        ///< Chroma adaptation weighting end value for Y channels
	} LcaYinfo;
	struct {
		RANGE_SEL       LcaUV;           ///< Chroma adaptation range selection for UV channels, Max = IME_RANGE_128
		UINT32          LcaUVTh;         ///< Chroma adaptation range threshold for UV channels
		RANGE_SEL       LcaUVWtPrc;      ///< Chroma adaptation weighting precision for UV channel, Max = IME_RANGE_64
		UINT32          LcaUVWtS;        ///< Chroma adaptation weighting start value for UV channels
		UINT32          LcaUVWtE;        ///< Chroma adaptation weighting end value for UV channels
	} LcaUVinfo;
	UINT32  SubRatio;       ///< sub out size: size/(1<<(SubRatio))
} IME_CHRA_ADAP;

typedef struct _IME_CHRA_CADAP {
	BOOL            LcaCLREn;           ///< Function enable
	struct {
		UINT32          LcaCaCentU;     ///< chroma adjustment reference center of U channel
		UINT32          LcaCaCentV;     ///< chroma adjustment reference center of V channel
	} LcaRefWt;

	struct {
		RANGE_SEL       LcaCLRRng;      ///< Color adaptation range selection for UV channels, Max = IME_RANGE_128
		UINT32          LcaCLRTh;       ///< Color adaptation range threshold for UV channels
		RANGE_SEL       LcaCLRWtPrc;    ///< Color adaptation weighting precision for UV channel, Max = IME_RANGE_64
		UINT32          LcaCLRWtS;      ///< Color adaptation weighting start value for UV channels
		UINT32          LcaCLRWtE;      ///< Color adaptation weighting end value for UV channels
	} LcaCLRinfo;
} IME_CHRA_CADAP;

typedef struct _IME_CHRA_LADAP {
	BOOL            LcaLumEn;           ///< Function enable

	struct {
		UINT32          LcaLumRefWt;    ///< Luma adaptation reference weighting for UV channels
		UINT32          LcaLumOutWt;    ///< Luma adaptation output weighting
	} LcaRefWt;

	struct {
		RANGE_SEL       LcaLumRng;      ///< Luma adaptation range selection for UV channels, Max = IME_RANGE_128
		UINT32          LcaLumTh;       ///< Luma adaptation range threshold for UV channels
		RANGE_SEL       LcaLumWtPrc;    ///< Luma adaptation weighting precision for UV channel, Max = IME_RANGE_64
		UINT32          LcaLumWtS;      ///< Luma adaptation weighting start value for UV channels
		UINT32          LcaLumWtE;      ///< Luma adaptation weighting end value for UV channels
	} LcaLuminfo;
} IME_CHRA_LADAP;
/**
    @name IME enum - Dark and bright chroma suppression mode
*/
//@{
typedef enum _DBCS_MODE_SEL {
	DBCS_DK_MODE  = 0,      ///< dark mode
	DBCS_BT_MODE  = 1,      ///< bright mode
	DBCS_BOTH_MODE = 2,     ///< dark and bright mode
	ENUM_DUMMY4WORD(DBCS_MODE_SEL)
} DBCS_MODE_SEL;
/**
    dark and bright region chroma supression parameters
*/
typedef struct _IME_ChromaSupp {
	UINT32 ChromaSuppEn;           ///< function eanble
	DBCS_MODE_SEL     ModeSel;     ///< Process mode
	struct {
		UINT32            uiU;      ///< Center value for U channel
		UINT32            uiV;      ///< Center value for V channel
	} Cetner;
	struct {
		UINT32            uiY;      ///< Step for luma
		UINT32            uiC;      ///<  Step for chroma
	} Step;
	UINT32            *puiWeightY;  ///< Weighting LUT for luma channel, 16 elements
	UINT32            *puiWeightC;  ///< Weighting LUT for luma channel, 16 elements
} IME_ChromaSupp;

/**
    color space transfor parameter set
*/
typedef struct _IME_CSPACE {
	BOOL        YUV2RGBEn;  ///< enable bit for YUV domain transfor to RGB domain
	BOOL        RGB2YUVEn;  ///< -
} IME_CSPACE;
/**
    film grain parameter set
*/
typedef struct _IME_FG {
	UINT32  EnFlag;         ///< enable flag
	UINT32  LumiThres;      ///< luminance-Noise difference threshold

	UINT32  GNInit_p1;      ///< Grain noise initial value for path1
	UINT32  Weight_p1;      ///< path1 weighting between pixel and noise value
	UINT32  GNInit_p2;      ///< Grain noise initial value for path2
	UINT32  Weight_p2;      ///< path2 weighting between pixel and noise value
	UINT32  GNInit_p3;      ///< Grain noise initial value for path3
	UINT32  Weight_p3;      ///< path3 weighting between pixel and noise value
} IME_FG;

/**
    3D noise reduction prarmeter set
*/
typedef struct _IME_3D_NR {
	UINT32  *pThresY;       ///< thresholds for Y channel, tab num:3, Data type:UINT32
	UINT32  *pWeightY;      ///< weightings for Y channel, tab num:4, Data type:UINT32
	UINT32  *pThresCb;      ///< thresholds for Cb channel, tab num:3, Data type:UINT32
	UINT32  *pWeightCb;     ///< weightings for Cb channel, tab num:4, Data type:UINT32
	UINT32  *pThresCr;      ///< thresholds for Cr channel, tab num:3, Data type:UINT32
	UINT32  *pWeightCr;     ///< weightings for Cr channel, tab num:4, Data type:UINT32
} IME_3D_NR;

/**
    IME structure - Data Stamp parameter setting structure
*/
//@{
/**
    @name IME enum - Data stamp mode selection
*/
//@{
typedef enum _DS_FMT_SEL {
	DS_FMT_RGB565    = 0,   ///< data stamp format RGB565
	DS_FMT_ARGB1555  = 1,   ///< data stamp format ARGB1555
	DS_FMT_ARGB4444  = 2,   ///< data stamp format ARGB4444
	ENUM_DUMMY4WORD(DS_FMT_SEL)
} DS_FMT_SEL;

//@}
typedef struct _STAMP_IMAGE_INFO {
	struct {
		UINT32 uiSizeH;             ///< horizontal size
		UINT32 uiSizeV;             ///< vertical size
	} DS_SIZE;                      ///< Image size
	DS_FMT_SEL      DsFmt;          ///< data stamp format
	Coordinate      DsBlendPos;     ///< Blending position
	UINT32          uiDSLineoffset; ///< Data lineoffset
	UINT32          uiDSAddress;    ///< Data address
} STAMP_IMAGE_INFO;


typedef struct _STAMP_IQ_INFO {
	BOOL DSColorKeyEn;       ///< color key enable
	UINT32  uiDsColorKey;           ///< Color key RGB565 format
	UINT32  uiBlendWt0;             ///< Blending weighting 0
	UINT32  uiBlendWt1;             ///< Blending weighting 1
} STAMP_IQ_INFO;

typedef struct _STAMP_SET_INFO {
	BOOL                DSEn;           ///< Function enable
	STAMP_IMAGE_INFO    DSImgInfo;      ///< Image information

	STAMP_IQ_INFO       DSColorKeyInfo; ///< Data stamp quality information
} STAMP_SET_INFO;


typedef struct _IMESTAMP_INFO {
	BOOL                DSRGB2YUVEn;        ///< RGB 2 YUV function enable
	BOOL                RGB2YUVUserdefine;  ///< user define coefficient
	UINT32              RGB2YUVCoef[4];     ///< color space transfer coefficient

	STAMP_SET_INFO      DS_Set0;        ///< data stamp set0
	STAMP_SET_INFO      DS_Set1;        ///< data stamp set1
	STAMP_SET_INFO      DS_Set2;        ///< data stamp set2
	STAMP_SET_INFO      DS_Set3;        ///< data stamp set3
} IMESTAMP_INFO;
/**
    @name IME enum - Statistical edge kernel selection
*/
//@{
typedef enum _STATS_EKER_SEL {
	STATS_EKER_HA  = 0,     ///< H map A
	STATS_EKER_HB  = 1,     ///< H map B
	STATS_EKER_VA  = 2,     ///< V map A
	STATS_EKER_VB  = 3,     ///< V map B
	ENUM_DUMMY4WORD(STATS_EKER_SEL)
} STATS_EKER_SEL;
//@}

typedef enum _IME_STATS_SRC_SEL {
	IME_STATS_SRC_SET0  = 0,  ///< source from set0
	IME_STATS_SRC_SET1  = 1,  ///< source from set1
	ENUM_DUMMY4WORD(IME_STATS_SRC_SEL)
} IME_STATS_SRC_SEL;

typedef enum _IME_STATS_HIST_SEL {
	IME_STATS_HIST_SET0  = 0,  ///< source from set0
	IME_STATS_HIST_SET1  = 1,  ///< source from set1
	ENUM_DUMMY4WORD(IME_STATS_HIST_SEL)
} IME_STATS_HIST_SEL;

typedef struct _IME_STATS_EDGE_INFO {
	BOOL            StatsEdgeKerEn0;    ///< edge kernel enable for set0
	STATS_EKER_SEL  StatsEdgeKer0;      ///< edge map kernel selection for map0
	UINT32          uiStatsShift0;      ///< edge map shift for map0

	BOOL            StatsEdgeKerEn1;    ///< edge kernel enable for set1
	STATS_EKER_SEL  StatsEdgeKer1;      ///< edge map kernel selection for map1
	UINT32          uiStatsSft1;        ///< edge map shift for map1
} IME_STATS_EDGE_INFO;

typedef struct _IME_STATS_HIST_INFO {
	IME_STATS_HIST_SEL  SetSel;             ///< operation set selection
	Coordinate          StatsHistPos;       ///< start posotion for edge statistic
	struct {
		UINT32 sizeH;                       ///< horizontal size
		UINT32 sizeV;                       ///< vertical size
	} StatsHistSize;                        ///< image size for edge statistic
	UINT32 AccTag;                          ///< histogram accumulation target
} IME_STATS_HIST_INFO;

typedef struct _IME_STATS_ROITH_INFO {
	IME_STATS_SRC_SEL     RoiSrc; ///< ///< statistical source of ROI
	UINT32  RoiTh0;     ///< statistical threshold of ROI for section0
	UINT32  RoiTh1;     ///< statistical threshold of ROI for section1
	UINT32  RoiTh2;     ///< statistical threshold of ROI for section2
} IME_STATS_ROITH_INFO;

typedef struct _IME_STATS_ROI_INFO {
	struct {
		UINT32 uiStatsRow0;  ///< row position0
		UINT32 uiStatsRow1;  ///< row posotion1
	} StatsRowPos;      ///< row position of statistic
	IME_STATS_ROITH_INFO  StatsRoi0;            ///< ROI0 for edge map
	IME_STATS_ROITH_INFO  StatsRoi1;            ///< ROI1 for edge map
	IME_STATS_ROITH_INFO  StatsRoi2;            ///< ROI2 for edge map
	IME_STATS_ROITH_INFO  StatsRoi3;            ///< ROI3 for edge map
	IME_STATS_ROITH_INFO  StatsRoi4;            ///< ROI4 for edge map
	IME_STATS_ROITH_INFO  StatsRoi5;            ///< ROI5 for edge map
	IME_STATS_ROITH_INFO  StatsRoi6;            ///< ROI6 for edge map
	IME_STATS_ROITH_INFO  StatsRoi7;            ///< ROI7 for edge map
} IME_STATS_ROI_INFO;

typedef struct _IME_STATS_INFO {
	BOOL                StatsEnable;            ///< enable of statistic
	BOOL                StatsFilterEnable;      ///< filter enable of statistic
	BOOL                StatsImgOutBF;          ///< ime out before filter
	IME_STATS_EDGE_INFO *StatsEdgeMap;          ///< edge map of statistic
	IME_STATS_HIST_INFO *StatsHistogram0;       ///< edge histogram
	IME_STATS_HIST_INFO *StatsHistogram1;       ///< edge histogram
	IME_STATS_ROI_INFO  *StatsRoi;              ///< ROI threshold of edge map
} IME_STATS_INFO;


/**
    IME enum - privacy mask type
*/
typedef enum _IME_PRI_MASK_TYPE {
	IME_PRI_MASK_TYPE_YUV = 0,  ///< Using YUV color
	IME_PRI_MASK_TYPE_PXL = 1,  ///< Using pixilation
	ENUM_DUMMY4WORD(IME_RPI_MASK_TYPE)
} IME_RPI_MASK_TYPE;

/**
    IME enum - privacy mask pixelation blocks size selection
*/
typedef enum _IME_PRI_PXL_BLK_SIZE {
	IME_PRI_PIXELATION_08 = 0,  ///< 8x8
	IME_PRI_PIXELATION_16 = 1,  ///< 16x16
	IME_PRI_PIXELATION_32 = 2,  ///< 32x32
	IME_PRI_PIXELATION_64 = 3,  ///< 64x64
	ENUM_DUMMY4WORD(IME_PRI_PXL_BLK_SIZE)
} IME_PRI_PXL_BLK_SIZE;

/**
    IME structure - privacy mask parameter of each set
*/
typedef struct _IME_PRI_MASK_ITEM {
	BOOL                EnFlag;       ///< privacy mask function enable
	Coordinate          Coord[4];     ///< privacy mask 4 coordinates, must be Convex Hull
	IME_RPI_MASK_TYPE   PmMskType;    ///< privacy mask type selection
	UINT32              PmColor[3];   ///< privacy mask using YUV color
	UINT32              AWeight;      ///< privacy mask alpha weight
} IME_PRI_MASK_ITEM;

/**
    IME structure - privacy mask parameters
*/
typedef struct _IME_PRI_MASK_INFO {
	IME_PRI_MASK_ITEM PmSet0;                    ///< privacy mask set0
	IME_PRI_MASK_ITEM PmSet1;                    ///< privacy mask set1
	IME_PRI_MASK_ITEM PmSet2;                    ///< privacy mask set2
	IME_PRI_MASK_ITEM PmSet3;                    ///< privacy mask set3
	IME_PRI_MASK_ITEM PmSet4;                    ///< privacy mask set4
	IME_PRI_MASK_ITEM PmSet5;                    ///< privacy mask set5
	IME_PRI_MASK_ITEM PmSet6;                    ///< privacy mask set6
	IME_PRI_MASK_ITEM PmSet7;                    ///< privacy mask set7

	IME_PRI_PXL_BLK_SIZE PmPxlBlkSize;   ///< block size of pixelation
	UINT32               PxlWidth;
	UINT32               PxlHeight;
	UINT32               PmPxlLofs;      ///< pixelation input image lineoffset
} IME_PRI_MASK_INFO;

/**
    YUV Converter
*/
typedef struct _IME_HAL_YUVCVT {
	BOOL yuvcvt_en;
	BOOL yuvcvt_bt709_sel;  ///< 0->BT601, 1->BT709
} IME_HAL_YUVCVT;

/**
    TMNR Paramters
*/

typedef struct _IME_HAL_TMNR_NE_CFG_ {
	UINT32 uiYNum;          // Block number of Y direction
	UINT32 uiYOffset;       // Y offset
	UINT32 uiXNum[4];       // Block number of each column
	UINT32 uiXOffset[4];    // X offset of each column
} IME_HAL_TMNR_NE_CFG;

typedef struct _IME_HAL_TMNR_Y_COEFF_ {
	UINT32  uiYBase;
	UINT32  uiSTD;
	UINT32  uiCoeffA;
	UINT32  uiCoeffB;
} IME_HAL_TMNR_Y_COEFF;

typedef struct _IME_HAL_TMNR_UV_COEFF_ {
	UINT32  uiMean;
	UINT32  uiSTD;
} IME_HAL_TMNR_UV_COEFF;

typedef struct _IME_HAL_TMNR_NOISE_MODEL_INFO_ {
	IME_HAL_TMNR_Y_COEFF TmnrY[8];
	IME_HAL_TMNR_UV_COEFF TmnrU[8];
	IME_HAL_TMNR_UV_COEFF TmnrV[8];
} IME_HAL_TMNR_NOISE_MODEL_INFO;

typedef struct _IME_HAL_TMNR_MOTION_LVTH_ {
	//UINT32          DispMoMapEn;
	//UINT32          MoMapChl;
	UINT32          MaLvThY1;//MOTION_LEVEL_TH_Y_K1;
	UINT32          MaLvThY2;// MOTION_LEVEL_TH_Y_K2;
	UINT32          MaLvThC1;
	UINT32          MaLvThC2;
} IME_HAL_TMNR_MOTION_LVTH;

typedef enum _IME_TMNR_DISPMOMAP_ {
	IME_TMNR_DISPMOMAP_NONE = 0,
	IME_TMNR_DISPMOMAP_Y = 1,
	IME_TMNR_DISPMOMAP_U = 2,
	IME_TMNR_DISPMOMAP_V = 3,
	ENUM_DUMMY4WORD(IME_TMNR_DISPMOMAP)
} IME_TMNR_DISPMOMAP;

typedef struct _IME_HAL_TMNR_INFO_ {
	UINT32  SmlVbtSuppYEn;
	UINT32  CentWzeroY2DEn;
	UINT32  CentWzeroY3DEn;
	UINT32  BlurStrY;
	UINT32  AvoidResThY;
	UINT32  AvoidResThC;
	UINT32  MaxBlkEdgvarY;

	IME_HAL_TMNR_MOTION_LVTH MatchLevelTh;
	UINT32          nrsth_Tpl3DY;               ///< spatial NR strength
	UINT32          nrsth_Tpl3DC;
	UINT32          nrsth_Spl2DY;
	UINT32          nrsth_Spl2DC;
	IME_HAL_TMNR_NOISE_MODEL_INFO NoiseModel;   ///< noise model parameter:


	UINT32          Ftr2DThY[4];    ///< Y channel 2D filter
	UINT32          Ftr3D1ThY[4];   ///< Y channel 3D_1 filter
	UINT32          Ftr3D2ThY[4];   ///< Y channel 3D_2 filter
	UINT32          Ftr2DThC[4];    ///< UV channel 2D filter
	UINT32          Ftr3DThC[4];    ///< UV channel 3D filter
} IME_HAL_TMNR_INFO;

typedef struct _IME_HAL_STABLK_INFO {
	UINT32  NeBlkSampleStep;
	UINT32  blkinfo_h_offset[4];
	UINT32  blkinfo_h_number[4];
	UINT32  blkinfo_v_offset;
	UINT32  blkinfo_v_number;
} IME_HAL_STABLK_INFO;

typedef struct _IME_HAL_TMNR_CTRL {
	//BOOL    TmnrEn;           ///< 3DNR enable, ctrl by iq(subfen)
	//BOOL  ChromaEn;       ///< 3DNR chroma channel process enable, ctrl by iq(subfen)
	//BOOL  StaOutEn;       ///< 3DNR output statistic information enable, ctrl by iq(subfen)
	BOOL    RefInDecEn;     ///< 3DNR input reference image encode enable, ctrl by flow

	IMESIZE         RefInLofs;      ///< 3DNR reference image input lineoffset
	IMEADDR         RefInAddr;      ///< 3DNR reference image input DMA buffer address
	UINT32          MotInAddr;      ///< 3DNR motion input DMA buffer address
	UINT32          MotOutAddr;     ///< 3DNR motion input DMA buffer address
	UINT32          StaOutAddr;     ///< 3DNR Statistic information output DMA buffer address

	// block info for staout
	IME_HAL_STABLK_INFO sta_blkinfo;
} IME_HAL_TMNR_CTRL;

/**
    IME Motion Detection(MD) parameters
*/
typedef struct _IME_HAL_MD_GLOBAL {
	//UINT32     MD_TYPE;
	UINT32     MD_TBG;
	UINT32     MD2_UPD_TYPE;
	UINT32     MD2_UPD_ON;
	UINT32     MD_TIME_PERIOD;
	UINT32     MD_SCENE_CHANGE_TH;
	//UINT32     MD_EVT_DUPLICATE;
	UINT32     MD_CLEAR;
} IME_HAL_MD_GLOBAL;

typedef struct _IME_HAL_MD_TAMPER {
	UINT32     MD_TAMPER_TYPE;
	UINT32     MD_TAMPER_EDG_TEX;
	UINT32     MD_TAMPER_EDG_TH;
	UINT32     MD_TAMPER_AVG_TEX;
	UINT32     MD_TAMPER_AVG_TH;
} IME_HAL_MD_TAMPER;

typedef struct _IME_HAL_MD_REGION {
	UINT32     MD_WIN_X0;
	UINT32     MD_WIN_X1;
	UINT32     MD_WIN_X2;
	UINT32     MD_WIN_X3;
	UINT32     MD_WIN_Y;
	UINT32     MD_WIN_X_NUM0;
	UINT32     MD_WIN_X_NUM1;
	UINT32     MD_WIN_X_NUM2;
	UINT32     MD_WIN_X_NUM3;
	UINT32     MD_WIN_Y_NUM;
	UINT32     MD_WIN_X_SIZE;
	UINT32     MD_WIN_Y_SIZE;
	UINT32     MD_WIN_X_START_NUM0;
	UINT32     MD_WIN_X_START_NUM1;
	UINT32     MD_WIN_X_START_NUM2;
	UINT32     MD_WIN_X_START_NUM3;
	UINT32     MD_WIN_WIDTH_NUM;
	UINT32     MD_DXDY;
} IME_HAL_MD_REGION;

typedef struct _IME_HAL_MD_LV {
	UINT32     MD_LV_ALPHA;
	UINT32     MD_LV_ONE_MIN_ALPHA;
	UINT32     MD_LV_INIT_VAL;
	UINT32     MD_LV_TB;
	UINT32     MD_LV_SIGMA;
	UINT32     MD_LV_TG;
	UINT32     MD_LV_PRUNE;
	UINT32     MD_LV_LUM_DIFF_TH;
	UINT32     MD_LV_TEX_DIFF_TH;
	UINT32     MD_LV_TEX_RATIO_TH;
	UINT32     MD_LV_GM_MD2_TH;
	UINT32     MD_LV_TEX_TH;
} IME_HAL_MD_LV;

typedef struct _IME_HAL_MD_INFO_ {
	IME_HAL_MD_GLOBAL   MdGlobal;
	IME_HAL_MD_TAMPER   MdTamper;
	IME_HAL_MD_LV       MdLv[4];
} IME_HAL_MD_INFO;

typedef struct _IME_HAL_MD_CTRL {
	BOOL mden;                      // md enable
	UINT32 evt_out_addr;            // md event output addr(multiple of 8, 2 words aligned)
	UINT32 sta_in_lofs;             // md statistic input line offset
	UINT32 sta_in_addr;             // md statistic input addr(multiple of 8, 2 words aligned)
	UINT32 sta_out_lofs;            // md statistic output line offset
	UINT32 sta_out_addr;            // md statistic output addr(multiple of 8, 2 words aligned)
	UINT32 lv_in_addr;              // md level input addr(multiple of 8, 2 words aligned)
	IME_HAL_MD_REGION md_window;    // md window setting
} IME_HAL_MD_CTRL;

// For ipl ctrl query used
typedef struct _IME_HAL_STAINFO {
	UINT32 sta_addr;
	IME_HAL_STABLK_INFO blk_info;
} IME_HAL_STAINFO;

/**
	IME Scale Method Select
*/
typedef struct _IME_HAL_SCL_SEL {
	UINT32 scl_th;			///< scale done ratio threshold, [31..16]output, [15..0]input
	IME_SCALER method_l;	///< scale method when scale down ratio <= scl_th(output/input)
	IME_SCALER method_h;	///< scale method when scale down ratio >  scl_th(output/input)
} IME_HAL_SCL_SEL;

/**
    IME output path
*/
typedef struct _IME_OUT {
	OUTCH Path1;                ///< path1 output (output fmt support y/(uv pack))
	OUTCH Path2;                ///< path2 output (output fmt support y/(uv pack))
	OUTCH Path3;                ///< path3 output (output fmt support y/(uv pack))
	OUTCH Path4;                ///< path4 output (output fmt support only y output)
	UINT32 Path4_EdgeAddr;      ///< path4 edge map output
	UINT32 Path4_EdgeLineOfs;   ///< path4 edge line offset
	UINT32 Path4_HistoAddr;     ///< path4 histo output
	BOOL Path1_EncodeEn;        ///< path1 encode enable, path1 only
	IME_HAL_SCL_SEL scl_sel;	///< path scale method select rule

	BOOL LcaOutEn;          ///< reference image output enable
	BOOL LcaOutSrcB_Sel;    ///< LCA subout source, 0 -> after lca, 1 -> before lca
	UINT32  LcaOutAddr;     ///< reference image address(output)
	IMESIZE LcaOutSize;     ///< reference image size(output)

	// Output size = Input size
	// This could be used to replace path 1 if path 1 output is not scaled/crop...
	// Need check output format limitation ?
	BOOL RefOutEn;          ///< 3DNR output reference image enable
	BOOL RefOutEncEn;       ///< 3DNR output reference image encode enable
	IMESIZE RefOutLofs;     ///< 3DNR reference image output lineoffset
	IMEADDR RefOutAddr;     ///< 3DNR reference image output DMA buffer address

	// TMNR Output
	UINT32 tmnr_sta_out_addr;
	UINT32 tmnr_mot_out_addr;
	IME_HAL_STABLK_INFO tmnr_blkinfo;

	// MD Output
	UINT32 md_sta_out_addr;
	UINT32 md_sta_out_lofs;
	UINT32 md_evt_out_addr;
	IME_HAL_MD_REGION md_wininfo;
} IME_OUT;

/**
    IME input info
*/
typedef struct {
	IMEADDR Addr;           ///< PPB0 address
	IMESIZE Size;           ///< input size
	IMEFMT Fmt;             ///< input format

	UINT32  LcaAddr;         ///< reference image address(input)
	UINT32  LcaFromIFE2;     ///< reference image source form 0:dram or 1:IFE2
	IMESIZE LcaSize;         ///< reference image size(input)

	UINT32  PmPxlDmaAddr;   ///< pixelation input imaeg buffer address

	// TMNR Input
	IMEADDR tmnr_ref_in_addr;
	IMESIZE tmnr_ref_in_lofs;
	UINT32 tmnr_ref_in_dec_en;
	UINT32 tmnr_mot_in_addr;

	// MD Input
	UINT32 md_sta_in_addr;
	UINT32 md_sta_in_lofs;
	UINT32 md_lv_in_addr;
} IME_IN;

/**
    IME Path enable info for ime_read
*/
typedef struct {
	UINT32 path_enable[5];      ///< path enable (path1~4 + refout)
	UINT32 dma_enable[5];       ///< dma enable (path1~4 + refout)
	UINT32 stitch_enable[5];    ///< stitch enalbe (path1~4 + refout)
} IME_PATHEN_INFO;

//@}
typedef struct _IME_SUBFUNC {
	ImeInteEn   imeInteEn;  ///< interrupt enable
	ImeFuncEn   imeFuncEn;  ///< function enable

	ImeFuncEn   ime_func_enable;
	ImeFuncEn   ime_func_disable;

	struct {
		IME_CHRA_ADAP   *pChromaAdaption; ///< -   Function quality information for chroma channel
		IME_CHRA_LADAP  *pChromaLumAdaption;///< - Function quality information for luma channel
		IME_CHRA_CADAP  *pChromaColorAdaption;///< -Chroma adjustment information

		BOOL    bypass;     ///< bypass input path image data, only reference image is used
	} ChromaAdap;

	IME_FG          *pFilmGrain;    ///< film grain
	IME_ChromaSupp  *pChromaSuppres;///< chroma suppression info
	IMESCALE_Enh    *pP1_Enh;       ///< scaler quality enhancement for path1
	IMESCALE_Enh    *pP2_Enh;       ///< scaler quality enhancement for path2
	IMESCALE_Enh    *pP3_Enh;       ///< scaler quality enhancement for path3
	IMESCALE_Enh    *pP4_Enh;       ///< scaler quality enhancement for path4


	IMESCALE_Filt   *pP1_Filt;      ///< scaling filter for path1
	IMESCALE_Filt   *pP2_Filt;      ///< scaling filter for path2
	IMESCALE_Filt   *pP3_Filt;      ///< scaling filter for path3
	IMESCALE_Filt   *pP4_Filt;      ///< scaling filter for path4

	IMESTAMP_INFO   *pStampInfo;    ///< Data stamp parameters
	IME_STATS_INFO  *pStatsInfo;    ///< Edge statistic parameters
	IME_PRI_MASK_INFO *pPmInfo;     ///< Privacy mask paramters

	IME_HAL_TMNR_INFO   *pTmnrInfo;     ///< 3DNR iq parameters
	IME_HAL_MD_INFO     *pMdInfo;       ///< MotionDetection parameters
	IME_HAL_YUVCVT      *pYuvCvtInfo;   ///< YUV converter

	IME_TMNR_DISPMOMAP tmnr_momap_sel; ///< for 3DNR debug use
} IME_SUBFUNC;

#define SCALING_ISD_STP_OUT_H_LIMIT	(1312)
#define SCALING_ISD_MIN_RATIO (1599) // minimum ratio is scale down to 1/15.99x

//@}
#endif
