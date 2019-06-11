/**
    IPL_Ctrl module(type define)

    @file       ipl_ctrl_par.h
    @ingroup    mILibIPH
    @note

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _IPL_CTRL_PAR_H_
#define _IPL_CTRL_PAR_H_
#include "ipl_cmd.h"
#include "ipl_hal_op.h"
/** \addtogroup mILibIPH */
//@{

/**
     callback when pipeline output image dma ready

     Detailed description.
     @note (Optional)

     @param[in]     IPL_ID      ipl id
     @param[in]     ImgIdx      source index of current output image during this streaming.
     @param[in,out] pathx       in:current ready img addr and size. out: next frame output addr.
     @param[in] pathx           in:total path number
     @return Description: none
*/
typedef void (*fpImgRdy)(IPL_PROC_ID id, IPL_BUF_IO_CFG buf_io_cfg, IPL_YCC_ADDR_INFO *path[]);
typedef IPH_CLK(*fpEngCLK)(UINT32);
typedef UINT32(*fpSIEPclkPad)(UINT32);
typedef UINT32(*fpSIEClkSrc)(UINT32);
typedef UINT32(*fpEngInit)(UINT32, ENG_SUBFUNC *, IPL_HAL_GROUP *); //eng_subfunc and IPL_HAL_GROUP is output
typedef BOOL (*fpRegDump)(UINT32);
typedef UINT32(*fpccirFmt)(UINT32, SIE_DVI *);

/*
    return type is cascaded to void* from indicated SubFunction
*/
typedef void   *(*fpEngSubFunc)(UINT32, IPL_RAW_IMG_INFO, IPL_YCC_CH_INFO *); //IPL_YCC_CH_INFO ife2 image size

/**
     optional structure of ICF_FLOW

     this sturcture is optional configuration,ipl can start without these configuration.
     @note:
\n      fp_DceInit can't be NULL!! Because of hw stripe constraint.
\n      fp_SIEClk can't be NULL when sensor interface is LVDS!!  because LVDS + SIE hw constraint!
\n      if ime path1 and path3 need to be enabled during IPL running, IPL must be initialized with ime p1 and p3 output address
*/
typedef struct _IPLC_EXT {
	fpEngCLK fp_SIEClk;                     ///< engine clk CB, default clk will be applied to when CB is NULL!
	fpSIEClkSrc fp_SIEClkSrc;               ///< sie engine clk source selection CB
	fpSIEPclkPad fp_SIEPclkSel;             ///< sie engine pclk selection CB
	fpEngCLK fp_RHEClk;                     ///< -
	fpEngCLK fp_IFEClk;                     ///< -
	fpEngCLK fp_DCEClk;                     ///< -
	fpEngCLK fp_IPEClk;                     ///< -
	fpEngCLK fp_IFE2Clk;                    ///< -
	fpEngCLK fp_IMEClk;                     ///< -
	fpEngCLK fp_ISEClk;                     ///< -
	fpccirFmt fp_CCIRFmt;                   ///< dvi Format setting
	fpEngInit fp_SieInit;                   ///< engine sub-func initial CB, default cfg will be applied to when CB is NULL!
	fpEngInit fp_RheInit;                   ///< -
	fpEngInit fp_IfeInit;                   ///< -
	fpEngInit fp_DceInit;                   ///< -
	fpEngInit fp_IpeInit;                   ///< -
	fpEngInit fp_ImeInit;                   ///< -
	fpEngInit fp_IseInit;                   ///< -
	fpEngInit fp_Ife2Init;                  ///< -
} IPLC_EXT;


typedef struct _IPLC_IME_SPRT_INFO {
	UINT32 enable;      ///< enable path SPRT output
	STITCH_IMG img;     ///< stitch image
	INT32 YBufOfs;      ///< stitch p2 Y buffer offset L2 or R2
	INT32 UVBufOfs;     ///< stitch p2 UV buffer offset L2 or R2
} IPLC_IME_SPRT_INFO;


typedef struct _IPLC_IME_PATH_INFO {

	/*
	                            img_info(H)
	----------------------------------------------------------------
	|                                                              |
	|    crop_info(x, y)                                           |
	|    ----------------------------------------------            |
	|    |     sprt_info(start_pos)|                  |            |
	|    |                         |                  |            |
	|    |                         |                  |            |
	|    |                         |                  |            |
	|    |                         |                  |            |
	|    |                         |                  |            |
	|    |                         |                  |crop_info(V)|  img_info(H)
	|    |                         |                  |            |
	|    |                         |                  |            |
	|    |                         |                  |            |
	|    |                         |                  |            |
	|    |<-  y/cc line offset   ->|                  |            |
	|    |                         |                  |            |
	|    |                         |                  |            |
	|    ----------------------------------------------            |
	|                              crop_info(H)                    |
	|                                                              |
	----------------------------------------------------------------

	*/

	UINT32 enable;
	UINT32 sample_rate;                 ///< Sample rate of each ime path(DMA Enable control)
	///< IPL_SAMPLE_RATE_ZERO:      never enable
	///< IPL_SAMPLE_RATE_SINGLE:    enable once(for single mode)
	///< IPL_SAMPLE_RATE_STREAM:    trigger for each frame
	///< sample rate (dst_fps:bit[31:16], src_fps:bit[15:0])
	IPL_YCC_IMG_INFO img_info;
	IPL_YCC_CROP_INFO crop_info;
	IPLC_IME_SPRT_INFO sprt_info;
	UINT32 md_evt_out_en;               ///< attach md evt output in this path
} IPLC_IME_PATH_INFO;

typedef struct _IPL_STRP_INFO {
	enum {
		IPL_STRPMODE_AUTO_124ST     = 0,        ///< sstrp/2strp/4strp auto switch, GDC 11%
		IPL_STRPMODE_AUTO_14ST      = 1,        ///< sstrp/4strp auto switch, GDC 15%
		IPL_STRPMODE_FORCE_2ST      = 2,        ///< force 2strp
		IPL_STRPMODE_FORCE_4ST      = 3,        ///< force 4strp
		IPL_STRPMODE_CUSTOM_ST      = 4,        ///< custome strp info
		IPL_STRPMODE_MAX
	} strp_mode;

	// below is for IPL_STRPMODE_CUSTOMST
	UINT32 strp_num;        ///< stripe number
	UINT32 strp_tbl[16];    ///< stripe table for dce register
	UINT32 inc_num;         ///< max input horizontal stripe Increment pixel number
	UINT32 dec_num;         ///< max input horizontal stripe Decrement pixel number
} IPL_STRP_INFO;

typedef struct _IPLC_IME_SCL_INFO {
	UINT32 threshold;			///< scale done ratio threshold, [31..16]output, [15..0]input
	IPL_IME_SCALER method_l;	///< scale method when scale down ratio <= scl_th(output/input)
	IPL_IME_SCALER method_h;	///< scale method when scale down ratio >  scl_th(output/input)
} IPLC_IME_SCL_INFO;

/**
     basic configuration of ICF_FLOW

     these configuration must be fulfilled, and address can't be 0!
     @note (Optional)
*/
#define IPL_TRIGGER_MAX_BUF_NUM (10)
#define IPL_TRIGGER_BUF_NUM (4)

#define IPL_DFT_BUFNUM (2)
#define IPL_SIE_CA_BUFNUM (2)
#define IPL_IME_MD_STA_BUFNUM (2)
#define IPL_IME_MD_EVT_BUFNUM (2)
#define IPL_IPE_ETH_BUFNUM (3)

typedef struct _IPLC_BASEINFO {
	SIE_ACT_MODE     SIE_Act;            ///< sie and sensor communication interface
	SIE_WINDOW       SIE_In;             ///< sie input window
	IPP_SIE_SIGNAL   Sensor_In;          ///< sensor input format
	SIE_BURSTLENGTH_SET Burst_Length;    ///< sie dram burst length

	IPL_RAW_IMG_INFO SIE_Ch0_Img;        ///< raw image information
									 	 ///< CCIR Y image information
	BAYER_TYPE       SIE_Ch0_Bayer_Type; 						///< bayer format
	UINT32           SIE_Ch0_Ppb_Addr[IPL_TRIGGER_MAX_BUF_NUM]; ///< raw out ch0 output address idx 0 ~ 9 total 10
																///< CCIR Y ch0 output address idx 0 ~ 9 total 10
	UINT32           SIE_Ch0_Ppb_Num;       					///< sie ppb num, for raw/sideinfo/La
	UINT32           SIE_Ch0_Ring_Buf_height;   				///< sie output ring buffer height, for direct mode HDR SIE2 only

	IPL_RAW_IMG_INFO SIE_Ch1_Img; 					          	///< CCIR UV image information
	UINT32           SIE_Ch1_Ppb_Num;       					///< ca buffer number(only valid for Ca)
	UINT32           SIE_Ch1_Ppb_FOfs;      					///< ca buffer frame offset(only valid for Ca)
	UINT32           SIE_Ch1_Ppb_Addr[IPL_TRIGGER_MAX_BUF_NUM]; ///< ca out idx 0: output start addr
	///< CCIR UV output address idx 0 ~ 9 total 10

	UINT32           SIE_Ch2_Ppb_Addr[IPL_TRIGGER_MAX_BUF_NUM]; ///< LA out ch2 output address idx 0 ~ 9 total 10

	/************** IPLCFModeExt & IPL_CFEXT_CH3_OUT **************/
	IPL_RAW_IMG_INFO SIE_Ch3_Img;           					///< shdr sub-raw image information
																///< scaled image information
	UINT32           SIE_Ch3_Ppb_Num;       					///< buffer number(only valid for Va)
	UINT32           SIE_Ch3_Ppb_FOfs;      					///< buffer frame offset, va only
	UINT32           SIE_Ch3_Ppb_Addr[IPL_TRIGGER_MAX_BUF_NUM]; ///< Va out output start address
																///< shdr sub-raw ch3 idx 0 ~ 9 total 10
																///< sub-raw ch3 idx 0 ~ 9 total 10
	CH3_SRC          SIE_Ch3_Src;          						///< ch3 source

	UINT32           SIE_Ch5_Ppb_Addr[IPL_TRIGGER_MAX_BUF_NUM]; ///< raw encode infor ch5 idx 0 ~ 9 total 10

	UINT32           SIE_Reset_BP;          					///< break point configuration

	IPL_PROC_BIT_ID  proc_bit_id;           					///< SHDR process bit id

	enum {
		IPL_CF_NORMAL           = 0x00000000,     ///< SIEx + RHE + IFE + DCE + IPE + IME + IFE2
		IPL_CF_CCIR             = 0x00000001,     ///< CCIR mode SIEx + IME
		IPL_CF_DIRECT           = 0x00000002,     ///< SIE1 + RHE2IME, SIE direct to RHE
	} IPLCFMode;                                  ///< IPL control mode

	enum {
		IPL_CFEXT_NONE                  = 0x00000000,   ///< None
		IPL_CFEXT_CCIR_DEINTERLACE      = 0x00000001,   ///< notify de-interlace, default, select odd
		IPL_CFEXT_CCIR_DEINTERLACE_ODD  = 0x00000001,   ///< notify de-interlace, select odd frame
		IPL_CFEXT_CCIR_DEINTERLACE_EVEN = 0x00000002,   ///< notify de-interlace, select even frame
		IPL_CFEXT_CH3_OUT               = 0x00000004,   ///< select ch3 output
		IPL_CFEXT_CH4_OUT               = 0x00000008,   ///< select ch4 output
		IPL_CFEXT_PIPE                  = 0x00000010,   ///< select pipe mode
		IPL_CFEXT_VIRTUAL               = 0x00000020,   ///< select virtual mode(no sie)
		IPL_CFEXT_SIEONLY               = 0x00000040,   ///< select sie only mode
		IPL_CFEXT_VR360                 = 0x00000080,   ///< select vr360 flow
		IPL_CFEXT_CCIR_ISE				= 0x00000100,   ///< select ccir ise d2d flow
	} IPLCFModeExt;

	IPL_RAW_IMG_INFO IPL_In;                ///< pipeline dma in Buffer 0
	Coordinate       IPL_StPos;             ///< Start Position (left-top 0, 0)

	IPL_STRP_INFO    strp_info;             ///< ipl stripe info

	USIZE            DCE_Ref_1X;            ///< sensor output size(for dce reference)
	Coordinate       DCE_Ref_1XCenter;      ///< distortion center of sensor output
	iCoordinate      DCE_Ref_1XCenterOfs;   ///< distortion center offset
	USIZE            SIE_Ref_1X;            ///< sie crop output size(for dce reference)
	Coordinate       SIE_Ref_1XDCECropCen;  ///< distortion center on sie crop image
	Coordinate       SIE_Ref_1XVIGCropCen;  ///< vig center on sie crop image
	UINT32           SenHBinRatio;          ///< h ratio, default 1000(SEN_BINNING_UNIT)
	UINT32           SenVBinRatio;          ///< v ratio, default 1000(SEN_BINNING_UNIT)

	RHE_HAL_FUNC_MODE RHE_Mode;

	struct {
		UINT32          defog_en;          	 	///< rhe defog subout enable/disable
		UINT32          addr[IPL_DFT_BUFNUM];	///< rhe defog subout buffer address
		UINT32			subout_width;			///< rhe defog subout width
		UINT32			subout_height;			///< rhe defog subout height
	} RHE_Defog_info;

	struct {
		UINT32          EthEn;              				 ///< ipe eth enable/disable
		UINT32          LineOfs;            				 ///< eth output line offset for original size
		UINT32          eth_addr[IPL_IPE_ETH_BUFNUM];        ///< ipe eth buffer for original size
		UINT32          eth_addr_subspl[IPL_IPE_ETH_BUFNUM]; ///< ipe eth buffer for subsample(w/2, h/2)

		UINT32          va_en;              				 ///< ipe va enable/disable
		UINT32          va_addr[IPL_DFT_BUFNUM];         	 ///< ipe va buffer
	} IPE_EthVA_info;

	struct {
		UINT32           LcaEn;					///< ime lca enable/disable
		UINT32           addr[IPL_DFT_BUFNUM];	///< lca YCbCr sub output addr
	} LCA_info;

	UINT32 IME_p1_encode_en;                ///< ime path_1 encode enable/disable
	IPLC_IME_PATH_INFO IME_p1_info;         ///< ime path information

	IPLC_IME_PATH_INFO IME_p2_info;         ///< ime path information
	IPLC_IME_PATH_INFO IME_p3_info;         ///< ime path information

	struct {
		IPLC_IME_PATH_INFO info;            ///< ime path information
		BOOL ext_out_enable;                ///< enable path4 dma ext output
		UINT32 EdgeMapBufOfs;               ///< relative P4 Y output address
		UINT32 HistoBufOfs;                 ///< relative P4 Y output address
	} IME_p4_info;

	struct {
		UINT32 pixelation_en;               ///< privacy mask enable/disable
		UINT32 addr[IPL_DFT_BUFNUM];        ///< YCbCr output addr (input image size H / block size = buf H)
											///< YCbCr output addr (input image size V / block size = buf V)
											///< YCbCr output buf size = buf H * buf V * 3(YUV)
	} PriMsk;

	struct {
		UINT32 tmnr_en;                     ///< tmnr enable/disable
		UINT32 tmnr_buf_share_en;			///< tmnr ref buffer share with ime path1 enable/disable
		UINT32 mot_addr[IPL_DFT_BUFNUM];    ///< tmnr mot output address
		UINT32 mot_bufsize;                 ///< tmnr mot buffer size
		UINT32 sta_addr[IPL_DFT_BUFNUM];    ///< tmnr sta output address
		UINT32 period;                      ///< tmnr refout encode period(bit)
		UINT32 period_length;               ///< tmnr refout encode period length(number of bits)
	} ime_tmnr_info;

	struct {
		UINT32 md_en;                       	///< motion detection enable/disable
		UINT32 sta_addr[IPL_IME_MD_STA_BUFNUM]; ///< motion detection sta output address
		UINT32 sta_bufsize;                 	///< motion detection sta buffer size
		UINT32 lv_addr;                     	///< motion detection lv input buffer address
		UINT32 md_win_manual;               	///< motion detection windows manual mode/auto mode
		UINT32 md_win_x_size;               	///< motion detection manual window x size
		UINT32 md_win_y_size;               	///< motion detection manual window y size
		UINT32 evt_addr[IPL_IME_MD_EVT_BUFNUM];	///< motion detection evt output address
	} ime_md_info;

	IPLC_IME_SCL_INFO ime_scl_info;				///< ime scale method select info
	IPL_IME_YUV_TYPE  ime_yuv_type;				///< ime output yuv type info

	FLIP_TYPE FlipInfo;               ///< image pipeline information
} IPLC_BASEINFO, *pIPLC_BASEINFO;
/*********************************************************************************************/
/**
     data structure of ICF_FLOW_C

*/
typedef struct _ICF_FLOW_C_STRUCT {
	IPL_HAL_PARASET  ParaIdx;           ///<indicate IPL_HAL parameter set index

	IPLC_BASEINFO    BaseInfo;          ///< IPL_Ctrl main-image configuration

	///<ime chroma adapation CB
	fpEngSubFunc     fp_AdaptSetting;   ///<CB at each pre2ime fw_operation start,
	///<return NULL if no need to update, otherwise,return ptr of update data
	///<if funcEn is FALSE, must return NULL!
	///<optional information
	IPLC_EXT         Ext;
} ICF_FLOW_C_STRUCT, *PICF_FLOW_C_STRUCT;

/**
     IPL_Ctrl configuration

     @configure IPL when ipl_start
*/
typedef union _IPC_INFO {
	ICF_FLOW_C_STRUCT      ICF_FC;  ///< Flow C
} IPC_INFO, *PIPC_INFO;


/**
    feedback information when querry "ICF_FLOW_C".
*/
typedef struct {
	// input
	UINT32 width;               ///< image width
	UINT32 height;              ///< image height
	IPL_STRP_INFO *strp_info;   ///< stripe information, set NULL to use IPL_STRPMODE_AUTO_124ST
} IPL_QUE_TMNR_BUFINFO_IN;

typedef struct {
	// output
	UINT32 sta_bufsize;
	UINT32 mot_bufsize;
} IPL_QUE_TMNR_BUFINFO_OUT;

typedef struct {
	UINT32 width;               ///< image width
	UINT32 height;              ///< image height
	IPL_STRP_INFO *strp_info;   ///< stripe information, set NULL to use IPL_STRPMODE_AUTO_124ST
	UINT32 md_win_manual;       ///< md window manual mode enable/disable
	UINT32 md_win_x_size;       ///< md window manual x size
	UINT32 md_win_y_size;       ///< md window manual y size
} IPL_QUE_MD_BUFINFO_IN;

typedef struct {
	UINT32 sta_bufsize;
	UINT32 evt_bufsize;
} IPL_QUE_MD_BUFINFO_OUT;


typedef void (*FP_GET_TMNR_BUFINFO)(IPL_QUE_TMNR_BUFINFO_IN *info_in, IPL_QUE_TMNR_BUFINFO_OUT *bufsize);
typedef void (*FP_GET_MD_BUFINFO)(IPL_QUE_MD_BUFINFO_IN *info_in, IPL_QUE_MD_BUFINFO_OUT *bufsize);

typedef struct _QUE_FLOW_C {
	UINT32 nBUF_SIEPPB;         	///< sie buffer num
	UINT32 nBUF_SIE_SUBPPB;     	///< sie buffer num
	UINT32 nBUF_SIE_ENCPPB;     	///< sie buffer num(encode information)
	UINT32 nBUF_CAPPB;          	///< sie ca buffer num
	UINT32 nBUF_LAPPB;          	///< sie la buffer num
	UINT32 nBUF_VAPPB;          	///< sie va buffer num
	UINT32 nBUF_ETHPPB;         	///< sie Eth buffer num
	UINT32 nBUF_IPE_ETHPPB;     	///< ipe Eth buffer num
	UINT32 nBUF_IPE_VAPPB;      	///< ipe Eth buffer num
	UINT32 nBUF_IME_LCAPPB;     	///< ime sub out buffer num
	UINT32 nBUF_IME_PRIMASKPPB; 	///< ime privacy mask buffer num
	UINT32 nBUF_IME_3DNR_MOTPPB;    ///< ime 3dnr mot out buffer num
	UINT32 nBUF_IME_3DNR_STAPPB;    ///< ime 3dnr sta out buffer num
	UINT32 nBUF_IME_MD_STAPPB;      ///< ime MD sta out buffer num
	UINT32 nBUF_IME_MD_EVTPPB;      ///< ime MD evt out buffer num
	UINT32 nBUF_IMEPPB;         	///< only return ime 1 path requirement (all 3 pathes need the same pingpong buf num)
	UINT32 nBUF_RHE_DEFOGPPB;   	///< rhe defog subout buffer num

	//only needed when CCIR function enable
	UINT32 nBUF_BAYER2PPB;
	UINT32 nBUF_YCC2PPB;

	//function pointer for tmnr/md buffer calculation
	FP_GET_TMNR_BUFINFO fp_get_tmnr_bufinfo;
	FP_GET_MD_BUFINFO fp_get_md_bufinfo;
} QUE_FLOW_C;


typedef struct _GDC_NOR_FACTOR_IN {
	UINT32 width;
	UINT32 height;
	UINT32 centX;
	UINT32 centY;
	UINT32 Xdisf;
	UINT32 Ydisf;
} GDC_NOR_FACTOR_IN;

typedef struct _GDC_NOR_FACTOR_OUT {
	UINT32 NormFact;
	UINT32 NormBit;
} GDC_NOR_FACTOR_OUT;


typedef struct _GDC_NOR_FACTOR_INFO {
	void (*fp_GetInfo)(GDC_NOR_FACTOR_IN *InInfo, GDC_NOR_FACTOR_OUT *OutInfo);
} GDC_NOR_FACTOR_INFO;

/**
     IPL_Ctrl information

     query IPL_Ctrl relative information
*/

typedef union _IPC_QINFO {
	QUE_FLOW_C          QUE_FC;         ///< buffer number needed by Flow C
	IPE_ETH_DramRst     Rdy_Eth;        ///< current ready edge threshold output
	IPL_YCC_IMG_INFO    CCIR_Rdy;       ///< current ready CCIR output info
	SIE_LAPARAM         Rdy_LAParam;    ///< current ready ca window information(not statistic data)
	UINT32              FrameCnt;       ///< current frame count
	GDC_NOR_FACTOR_INFO NorFactorInfo;  ///< GDC normal factor information
	FLIP_TYPE           FlipType;       ///< current image pipe line flip type
	IME_GET_STATS_INFO  IMEStatsInfo;   ///< current ime statistic information
	UINT32              sample_rate[4]; ///< current ipl sample rate (dst_fps:bit[31:16], src_fps:bit[15:0])
	IPE_VA_DramRst		ipe_rdy_va;		///< current ready ipe va information
	SIE_INTE_EN			sie_prv_inte_en;///< current sie preview interrupt enable bit
	SIE_INTE_EN			sie_dft_inte_en;///< default sie preview interrupt enable bit
	SIE_YOUT_PARAM		sie_rdy_yout;	///< ready yout information
	UINT32				rhe_rdy_subout;	///< ready rhe subout address
	IME_HAL_STAINFO		ime_rdy_stainfo;///< ready ime tmnr sta info
} IPC_QINFO, *PIPC_QINFO;

//@}
#endif
