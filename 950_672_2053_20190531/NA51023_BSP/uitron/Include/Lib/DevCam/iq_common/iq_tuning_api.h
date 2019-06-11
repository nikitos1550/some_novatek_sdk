#ifndef _IQ_TUNING_API_H_
#define _IQ_TUNING_API_H_

#define ISP_MAX_ISONUM      IQS_ISOMAPTAB_NUM

#define IQ_FEATURE_COMMON           0x00000000
#define IQ_FEATURE_HDR              0x00000001
#define IQ_FEATURE_WDR              0x00000002
#define IQ_FEATURE_DEFOG            0x00000004
#define IQ_FEATURE_RAW_AVAILABLE    0x00000010
#define IQ_FEATURE_RGBIR            0x00000100


/**
    Typedef struct for IQ Information.
*/
typedef struct{
	UINT32 id;
    UINT32 cur_sen_mode;
    UINT32 iq_feature;
	char sensor_name[32];
	char iq_setting_name[32];
    char ae_setting_name[32];
    char awb_setting_name[32];

	UINT32 iqs_mode_table[SENSOR_MODE_Num];
} iqs_isp_info;

typedef struct{
	UINT32 id;
	UINT32 size;
	char isp_name[32];
	iqs_isp_info iqs_info[Total_SENIDNum];
} sISP_isp_info;

typedef struct _sISP_SIE_PAUSE {
	UINT32 Id;
	UINT32 size;

    IPL_PAUSEDMA_INFO sie_pause;
} sISP_SIE_PAUSE;

typedef struct _sISP_SIE_RESUME {
	UINT32 Id;
	UINT32 size;

    IPL_RESUMEDMA_INFO sie_resume;
} sISP_SIE_RESUME;

typedef struct _sISP_RAW_Info {
	UINT32 Id;
	UINT32 size;

    HEADER_RAW_DATA raw_info;
} sISP_RAW_Info;

typedef struct _sISP_HDR_RAW_Info {
	UINT32 Id;
	UINT32 size;

	UINT32 mf_bit;
    HEADER_RAW_DATA raw_info[4];
} sISP_HDR_RAW_Info;

typedef struct _sISP_IME_INFO {
	UINT32 Id;
	UINT32 size;

    IPL_IME_INFOR ime_info;
} sISP_IME_INFO;

typedef struct{
	UINT32 id;
	UINT32 size;

	UINT32 wdr_strength;
    UINT32 hdr_strength;
    UINT32 reserved_2;
    UINT32 reserved_3;
    UINT32 reserved_4;
    UINT32 reserved_5;
} sISP_IQ_STATUS;
/************************************************************/

/**
    Struct of All IQ Parameter Pointers.
*/
typedef struct {
	ISOMAP *ISOIDX_MAP;

    ENGINE_CTRL *Engine_Ctrl;

	//***********
	//**  SIE  **
	//***********
	//SIE-OB
	UINT32 *OB;             // variable

	//SIE-ECS
	UINT32 *ECS_tab;        // array, size=4225, NONISO
	SIE_EmbeddedCS *ECS;    // NONISO

	//***********
	//**  RHE  **
	//***********
	//RHE-WDR
	UINT8 *WDR_strength;                // NONISO
	IQS_WDR_LCE_OFFSET *WDR_LCE_offset; // NONISO

	//RHE-LCE
    IQS_LCE *LCE_strength;    // variable

	//RHE-GCE
    SR_WDR_GCE_PARAM *GCE_strength;

	//***********
	//** Defog **
	//***********
	//defog
    S_DEFOG_LIB_INIT_PARA *defog_init_config;
	UINT32 *defog_wet;

	//***********
	//**  IFE  **
	//***********
	//IFE-Outlier
	UINT32 *Outl_Bri;       // array, size=5
	UINT32 *Outl_Dark;      // array, size=5
	UINT32 *Outl_Cnt;       // array, size=2
	IFE_OUTL *Outl;

	//IFE-Spatial
	UINT32 *Spatial_W;      // array, size=10
	Spatial_Filter *Spatial;// NONISO

	//IFE-Range
	UINT32 *RangeA_Thr;     // array, size=6
	UINT32 *RangeA_Lut;     // array, size=17
	UINT32 *RangeB_Thr;     // array, size=6
	UINT32 *RangeB_Lut;     // array, size=17
	RangeCFGB *RangeB;

	//IFE-Clamp
	RANGE_FILT_CLAMP_PARAM *CLAMP;

	//IFE-Vig
	IQS_VIG_CENTER_RATIO *Vig_Center;   // NONISO
	IQS_VIG_SET *Vig;                   // NONISO
	IQS_VIG_ISO_TH *Vig_ISO_TH;         // NONISO

	//IFE-Gbal
	UINT8  *Gbal_Stab;      // array, size=16
	UINT8  *Gbal_Dtab;      // array, size=16
	IFE_GBAL *Gbal;

    //IFE-RBFill
	UINT8  *rbfill_luma_tab;      // array, size=17
	UINT8  *rbfill_ratio_tab;     // array, size=32
	IFE_RBFILL *rbfill;

	//***********
	//**  DCE  **
	//***********
	//DCE-DIST
	DIST *DIST;             // NONISO

	//DCE-RADIOUS
	Fact_Norm  *Radious;    // NONISO

	//DCE-CFA
	DCE_CFAPARAM *CFA;      // NONISO

	//DCE-GDC
	FOV    *FOV;            // NONISO
	UINT32 *LDC_tab;        // array, size=65, NONISO
	INT32  *LDC_Rtab;       // array, size=65, NONISO
	INT32  *LDC_Btab;       // array, size=65, NONISO

	//DCE-CAC
	Aberation *CAC;         // NONISO

	//***********
	//**  IPE  **
	//***********
	//IPE-Edge Ext
	INT16 *Edge_Ker;        // array, size=12, NONISO
	UINT8 *EDIR_Tab;        // array, size=8, NONISO
	IPE_DEE *EDIR_Ext;
	IPE_DirScorCal *DIR_SCORE_TAB;      // NONISO
	IPE_DirScorCalTh *SCORETH;
	IPE_KAEdgeW *EWA;
	IPE_KBEdgeW *EWB;       // NONISO
	IPE_EdgeW *EWparam;

	//IPE-Edge Enhance
	IPE_EDGEENH *ED_Enh;

	//IPE-Edge Map
	EdgeMap  *ED_map;
	EdgeMapS *ES_map;
	UINT8  *ED_tab;         // array, size=16
	UINT8  *ES_tab;         // array, size=16

	//IPE-LPF
	LPF *LPF;

	//IPE-CC
	IQS_COLOR_TEMPERATURE_TH       *CC_TH;          // NONISO
	IQS_COLOR_TEMPERATURE_PARAM    *Color[3];
	UINT8  *FS_tab;         // array, size=16
	UINT8  *FD_tab;         // array, size=16
	IPE_CC *CC;             // NONISO

	//IPE-Y curve
	UINT32 *Y_curve;        // array, size=129, NONISO

	//IPE-Gamma
	IQS_GAMMA_LV_TH *GammaLut_TH;   // NONISO
	UINT32 *GammaLut_Day;   // array, size=129, NONISO
	UINT32 *GammaLut_Night; // array, size=129, NONISO

	//IPE-3DCC
	UINT32 *Color3D_Lut;        // array, size=900, NONISO
	IPE_3DCCRound  *Color3D_Round; // NONISO

	//IPE-CCtrl
	UINT8  *edg_tab;        // array, size=24, NONISO
	UINT8  *dds_tab;        // array, size=8, NONISO
	INT8   *saturation;     // variable
	INT8   *contrast;       // variable
	UINT32 *ccon_tab;
	IQS_CCTRL_INFO   *cctrl_info;

	//IPE-CST
	INT16  *CST_Coef;       // array, size=9, NONISO
	IPE_CST *CST;           // NONISO

	//IPE-YCCFIX
	IPE_YCCFIX *YCC_Fix;    // NONISO

	//IPE-NoiseParam
	IPE_RAND_NR *Noise_Param;   // NONISO

	//IPE-GammaRand
	IPE_GamRand *Gamma_Rand;    // NONISO

	//***********
	//** IFE2  **
	//***********
	//IFE2-ReferenceCenter
	UINT32 *RefCenter_YTh;      // array, size=3
	UINT32 *RefCenter_YWt;      // array, size=4
	IFE2_REFCENT_Set *RefCenter_Y;
	UINT32 *RefCenter_UVTh;     // array, size=3
	UINT32 *RefCenter_UVWt;     // array, size=4
	IFE2_REFCENT_Set *RefCenter_UV;

	//IFE2-Filter
	UINT32 *IFE2Filter_YTh;     // array, size=5
	UINT32 *IFE2Filter_YWt;     // array, size=6
	UINT32 *IFE2Filter_UVTh;    // array, size=5
	UINT32 *IFE2Filter_UVWt;    // array, size=6
	IQS_IFE2_FILTER *IFE2Filter;

	//IFE2-Gray
	IFE2_GRAY_Statal *GrayStatical;     // NONISO

	//***********
	//**  IME  **
	//***********
	//IME-LCA
	IME_CHRA_ADAP *ChromaAdapt;
	IME_CHRA_CADAP *ChromaAdapt_Color;
	IME_CHRA_LADAP *ChromaAdapt_Lum;

	//IME-ChromaSuppression
	UINT32 *ChromaSupp_WeightY; // array, size=16, NONISO
	UINT32 *ChromaSupp_WeightC; // array, size=16, NONISO
	IME_ChromaSupp *ChromaSupp; // NONISO

	//IME-TMNR
    IME_HAL_TMNR_INFO **TMNR_INFO_Config;
    UINT32 *TMNR_Momap_Sel;
    TMNR_AUTO_INFO *TMNR_Auto_Info;

	//***********
	//** Codec **
	//***********
	//Codec
	INT8   *TNR_level;      // variable
	UINT32  TNR_level_num;  // variable, NONISO
	H26XEncTnr **TNR_Config;

	//***********
	//**  HDR  **
	//***********
	//HDR
    SHDR_IQ_PARA *hdr_config;
} sISP_PTR;

/************************************************************/

/**
    All IQ Items for NOTE Page.
*/
typedef enum  {
	ISP_ISOINDEX,         // 0
	ISP_ISOMAP,
	ISP_ENGINE_CTRL,
	ISP_ALL_ISO,
	ISP_ALL_NONISO,
	ISP_OB,               // 5
	ISP_ECS,
    ISP_WDR,
    ISP_CE,
    ISP_DEFOG_CONFIG,
	ISP_OUTLIER,          // 10
	ISP_SPATIAL,
	ISP_RANGE,
	ISP_RBFILL,
	ISP_VIG,
	ISP_GBAL,             // 15
	ISP_LDC,
	ISP_EDGE,
	ISP_RGBLPF,
	ISP_CC,
	ISP_Y_CURVE,          // 20
	ISP_GAMMA,
	ISP_3DCC,
	ISP_CCTRL,
	ISP_CST,
	ISP_IFE2FILTER,       // 25
	ISP_LCA,
	ISP_CHROMA_SUPP,
	ISP_TMNR,
	ISP_TMNR_DEBUG,
    ISP_TMNR_AUTO,        // 30
	ISP_TNR,
	ISP_TNR_CONFIG,
	ISP_FRAME_NUM,
	ISP_HDR_CONFIG,
	ISP_MAX
} ISP_IQ_ITEM;
/************************************************************/

/**
    All Structs for NOTE Page.
*/
// Struct of Index Information
typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT32 num; //fixed, cannot be changed
	UINT32 curIdx;
} sISP_ISOINDEX;

typedef struct {
	UINT32 Id;
	UINT32 size;
	ISOMAP ISOIDX_MAP[IQS_ISOMAPTAB_NUM];
} sISP_ISOMAP;

typedef struct {
	UINT32 Id;
	UINT32 size;
	ENGINE_CTRL e_ctrl;
} sISP_ENGINE_CTRL;

// Struct of all parameters which WILL change by ISO Index.
typedef struct {
	UINT32 Id;
	UINT16 ISO_index;

	//***********
	//**  SIE  **
	//***********
	//SIE-OB
	UINT32 OB[5];

	//***********
	//**  RHE  **
	//***********
    //RHE-LCE
    IQS_LCE LCE_strength;    // variable

    //RHE-GCE
    SR_WDR_GCE_PARAM GCE_strength;

	//***********
	//**  IFE  **
	//***********
	//IFE-Outlier
	UINT32 Outl_Bri[5];
	UINT32 Outl_Dark[5];
	UINT32 Outl_Cnt[2];
	IFE_OUTL Outl;

	//IFE-Spatial
	UINT32 Spatial_W[10];
	Spatial_Filter Spatial;

	//IFE-Range
	UINT32 RangeA_Thr[6];
	UINT32 RangeA_Lut[17];
	UINT32 RangeB_Thr[6];
	UINT32 RangeB_Lut[17];
	RangeCFGB RangeB;

	//IFE-CLAMP
	RANGE_FILT_CLAMP_PARAM CLAMP;

	//IFE-Gbal
	UINT8    Gbal_Stab[17];
	UINT8    Gbal_Dtab[17];
	IFE_GBAL Gbal;

    //IFE-RBFill
	UINT8 rbfill_luma_tab[17];
	UINT8 rbfill_ratio_tab[32];
	IFE_RBFILL rbfill;

	//***********
	//**  DCE  **
	//***********

	//***********
	//**  IPE  **
	//***********
	//IPE-Edge Ext
	//IPE-Edge EXT
	INT16           Edge_Ker[12];
	UINT8           EDIR_Tab[8];
	IPE_DEE         EDIR_Ext;
	IPE_DirScorCal  DIR_SCORE_TAB;
	IPE_DirScorCalTh SCORETH;
	IPE_KAEdgeW     EWA;
	IPE_KBEdgeW     EWB;
	IPE_EdgeW       EWparam;

	//IPE-Edge Enhance
	IPE_EDGEENH ED_Enh;

	//IPE-Edge Map
	EdgeMap  ED_map;
	EdgeMapS ES_map;
	UINT8  ED_tab[16];
	UINT8  ES_tab[16];

	//IPE-LPF
	LPF LPF;

	//IPE-CC
	UINT8  FS_tab[16];
	UINT8  FD_tab[16];

	//IPE-CCtrl
	INT8   saturation;
	INT8   contrast;
    UINT32 ccon_tab[17];
	IQS_CCTRL_INFO   cctrl_info;

	//***********
	//** IFE2  **
	//***********
	//IFE2-ReferenceCenter
	UINT32 RefCenter_YTh[3];
	UINT32 RefCenter_YWt[4];
	IFE2_REFCENT_Set RefCenter_Y;
	UINT32 RefCenter_UVTh[3];
	UINT32 RefCenter_UVWt[4];
	IFE2_REFCENT_Set RefCenter_UV;

	//IFE2-Filter
	UINT32 IFE2Filter_YTh[5];
	UINT32 IFE2Filter_YWt[6];
	UINT32 IFE2Filter_UVTh[5];
	UINT32 IFE2Filter_UVWt[6];
	IQS_IFE2_FILTER IFE2Filter;

	//IFE2-Gray
	IFE2_GRAY_Statal GrayStatical;

	//***********
	//**  IME  **
	//***********
	//IME-LCA
	IME_CHRA_ADAP ChromaAdapt;
	IME_CHRA_CADAP ChromaAdapt_Color;
	IME_CHRA_LADAP ChromaAdapt_Lum;

	//IME-ChromaSuppression
	UINT32 ChromaSupp_WeightY[16];
	UINT32 ChromaSupp_WeightC[16];
	IME_ChromaSupp ChromaSupp;

    //IME-TMNR
    IME_HAL_TMNR_INFO TMNR_INFO_Config;

	//***********
	//** Codec **
	//***********
	//Codec
	UINT16 TNR_level;
	H26XEncTnr TNR_Config;
} sISP_ALL_ISO;

// Struct of all parameters which WILL NOT change by ISO Index.
typedef struct {
	UINT32 Id;

	ISOMAP ISOIDX_MAP[IQS_ISOMAPTAB_NUM];

	ENGINE_CTRL e_ctrl;

	//***********
	//**  SIE  **
	//***********
	//SIE-ECS
	UINT32 ECS_tab[4225];
	SIE_EmbeddedCS ECS;

	//***********
	//**  RHE  **
	//***********
	//RHE-WDR
	UINT8 WDR_auto_strength[21];
    IQS_WDR_LCE_OFFSET WDR_LCE_offset;

	//***********
	//** Defog **
	//***********
	//defog
    S_DEFOG_LIB_INIT_PARA defog_init_config;
	UINT32 defog_wet[17];

	//***********
	//**  IFE  **
	//***********
	//IFE-Vig
	IQS_VIG_CENTER_RATIO Vig_Center;
	IQS_VIG_SET Vig;
    IQS_VIG_ISO_TH Vig_ISO_TH;

	//***********
	//**  DCE  **
	//***********
	//DCE-DIST
	DIST DIST;

	//DCE-RADIOUS
	Fact_Norm Radious;

	//DCE-CFA
	DCE_CFAPARAM CFA;

	//DCE-GDC
	FOV FOV;
	UINT32 LDC_tab[65];
	INT32 LDC_Rtab[65];
	INT32 LDC_Btab[65];

	//DCE-CAC
	Aberation CAC;

	//***********
	//**  IPE  **
	//***********
	//IPE-CC
	IQS_COLOR_TEMPERATURE_TH CC_TH;
	IQS_COLOR_TEMPERATURE_PARAM Color[3];
	IPE_CC CC;

	//IPE-Y curve
	UINT32 Y_curve[129];

	//IPE-Gamma
	IQS_GAMMA_LV_TH GammaLut_TH;
	UINT32 GammaLut_Day[129];
	UINT32 GammaLut_Night[129];

	//IPE-3DCC
	UINT32 Color3D_Lut[900];
	IPE_3DCCRound  Color3D_Round;

	//IPE-CCtrl
	UINT8  edg_tab[24];
	UINT8  dds_tab[8];

	//IPE-CST
	INT16  CST_Coef[9];
	IPE_CST CST;

	//IPE-YCCFIX
	IPE_YCCFIX YCC_Fix;

	//IPE-NoiseParam
	IPE_RAND_NR Noise_Param;

	//IPE-GammaRand
	IPE_GamRand Gamma_Rand;

	//***********
	//** IFE2  **
	//***********

	//***********
	//**  IME  **
	//***********
    IME_TMNR_DISPMOMAP TMNR_Momap_Sel;
    TMNR_AUTO_INFO TMNR_Auto_Info;

	//***********
	//** Codec **
	//***********
	//Codec
	UINT32  TNR_level_num;

	//***********
	//**  HDR  **
	//***********
	//HDR
    SHDR_IQ_PARA hdr_config;
} sISP_ALL_NONISO;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//SIE-OB
	UINT32 OB[5];
} sISP_OB;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//SIE-ECS
	UINT32 ECS_tab[4225];
	SIE_EmbeddedCS ECS;
} sISP_ECS;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//RHE-WDR
	UINT32 WDR_mode;
    UINT32 WDR_manual_level;    // range:0~128
	UINT8 WDR_auto_strength[21];//size 24byte(alignment 4byte), range:0~128
    IQS_WDR_LCE_OFFSET WDR_LCE_offset;// variable
} sISP_WDR;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT32 ISO_index;

    //RHE-LCE
    IQS_LCE LCE_strength;    // variable

    //RHE-GCE
    SR_WDR_GCE_PARAM GCE_strength;
} sISP_CE;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//RHE-Defog
	UINT32 defog_mode;
    UINT32 defog_level;    // range:0~255, 1X=128
	S_DEFOG_LIB_INIT_PARA defog_init_config;
	UINT32 defog_wet[17];
}sISP_DEFOG_CONFIG;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE-Outlier
	UINT32 Outl_Bri[5];
	UINT32 Outl_Dark[5];
	UINT32 Outl_Cnt[2];
	IFE_OUTL Outl;
} sISP_OUTLIER;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE-Spatial
	UINT32 Spatial_W[6];
	Spatial_Filter Spatial;
} sISP_SPATIAL;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE-Range
	UINT32 RangeA_Thr[6];
	UINT32 RangeA_Lut[17];
	UINT32 RangeB_Thr[6];
	UINT32 RangeB_Lut[17];
	RangeCFGB RangeB;

	//IFE-CLAMP
	RANGE_FILT_CLAMP_PARAM CLAMP;
} sISP_RANGE;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

    //IFE-RBFill
	UINT8 rbfill_luma_tab[17];
	UINT8 rbfill_ratio_tab[32];
	IFE_RBFILL rbfill;
} sISP_RBFILL;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IFE-Vig
	IQS_VIG_CENTER_RATIO Vig_Center;
	IQS_VIG_SET Vig;
    IQS_VIG_ISO_TH Vig_ISO_TH;
} sISP_VIG;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE-Gbal
	UINT8    Gbal_Stab[17];
	UINT8    Gbal_Dtab[17];
	IFE_GBAL Gbal;
} sISP_GBAL;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//DCE-DIST
	DIST DIST;

	//DCE-RADIOUS
	Fact_Norm Radious;

	//DCE-CFA
	DCE_CFAPARAM CFA;

	//DCE-GDC
	FOV FOV;
	UINT32 LDC_tab[65];
	INT32 LDC_Rtab[65];
	INT32 LDC_Btab[65];

	//DCE-CAC
	Aberation CAC;
} sISP_LDC;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IPE-Edge Ext
	INT16 Edge_Ker[12];
	UINT8 EDIR_Tab[8];
	IPE_DEE EDIR_Ext;
	IPE_DirScorCal DIR_SCORE_TAB;
	IPE_DirScorCalTh SCORETH;
	IPE_KAEdgeW EWA;
	IPE_KBEdgeW EWB;
	IPE_EdgeW EWparam;

	//IPE-Edge Enhance
	IPE_EDGEENH ED_Enh;

	//IPE-Edge Map
	EdgeMap  ED_map;
	EdgeMapS ES_map;
	UINT8  ED_tab[16];
	UINT8  ES_tab[16];
} sISP_EDGE;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IPE-LPF
	LPF LPF;
} sISP_RGBLPF;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-CC
	IQS_COLOR_TEMPERATURE_TH CC_TH;
	IQS_COLOR_TEMPERATURE_PARAM Color[3];
	IPE_CC CC;

	//IPE-CCtrl
	UINT8  edg_tab[24];
	UINT8  dds_tab[8];
} sISP_CC;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-Y curve
	UINT32 Y_curve[129];
} sISP_Y_CURVE;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-Gamma
	IQS_GAMMA_LV_TH GammaLut_TH;
	UINT32 GammaLut_Day[129];
	UINT32 GammaLut_Night[129];
} sISP_GAMMA;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-3DCC
	UINT32 Color3D_Lut[900];
	IPE_3DCCRound  Color3D_Round;
} sISP_3DCC;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IPE-CC
	UINT8  FS_tab[16];
	UINT8  FD_tab[16];

	//IPE-CCtrl
	INT8   saturation;
	INT8   contrast;
    UINT32 ccon_tab[17];
	IQS_CCTRL_INFO   cctrl_info;
} sISP_CCTRL;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-CST
	INT16  CST_Coef[9];
	IPE_CST CST;

	//IPE-YCCFIX
	IPE_YCCFIX YCC_Fix;

	//IPE-NoiseParam
	IPE_RAND_NR Noise_Param;

	//IPE-GammaRand
	IPE_GamRand Gamma_Rand;
} sISP_CST;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE2-ReferenceCenter
	UINT32 RefCenter_YTh[3];
	UINT32 RefCenter_YWt[4];
	IFE2_REFCENT_Set RefCenter_Y;
	UINT32 RefCenter_UVTh[3];
	UINT32 RefCenter_UVWt[4];
	IFE2_REFCENT_Set RefCenter_UV;

	//IFE2-Filter
	UINT32 IFE2Filter_YTh[5];
	UINT32 IFE2Filter_YWt[6];
	UINT32 IFE2Filter_UVTh[5];
	UINT32 IFE2Filter_UVWt[6];
	IQS_IFE2_FILTER IFE2Filter;

	//IFE2-Gray
	IFE2_GRAY_Statal GrayStatical;
} sISP_IFE2FILTER;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IME-LCA
	IME_CHRA_ADAP ChromaAdapt;
	IME_CHRA_CADAP ChromaAdapt_Color;
	IME_CHRA_LADAP ChromaAdapt_Lum;
} sISP_LCA;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IME-ChromaSuppression
	UINT32 ChromaSupp_WeightY[16];
	UINT32 ChromaSupp_WeightC[16];
	IME_ChromaSupp ChromaSupp;
} sISP_CHROMA_SUPP;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;
    IME_HAL_TMNR_INFO TMNR_INFO_Config;
} sISP_TMNR;

typedef struct {
	UINT32 Id;
	UINT32 size;
    IME_TMNR_DISPMOMAP TMNR_Momap_Sel;
} sISP_TMNR_DEBUG;

typedef struct {
	UINT32 Id;
	UINT32 size;
    TMNR_AUTO_INFO TMNR_Auto_Info;
} sISP_TMNR_AUTO;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//Codec
	UINT16 TNR_level;
	UINT32  TNR_level_num;
} sISP_TNR;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT32 ISO_index;

	//Codec
	H26XEncTnr TNR_Config;
} sISP_TNR_CONFIG;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//HDR
	UINT32 hdr_mode;
    UINT32 hdr_level;   // range:0~255, 1X=128
    SHDR_TEST_PARA test_param;
    SHDR_IQ_PARA hdr_config;
}sISP_HDR_CONFIG;

extern UINT32 iqt_get_param(ISP_IQ_ITEM item, UINT32 addr, UINT16 size);
extern UINT32 iqt_get_param_update(UINT32 id, ISP_IQ_ITEM item);
extern UINT32 iqt_set_param(ISP_IQ_ITEM item, UINT32 addr, UINT16 size);
extern void iqt_set_param_update(UINT32 id, ISP_IQ_ITEM item);
extern void iqt_reset_param_update(UINT32 id, ISP_IQ_ITEM item);
extern void iqt_init_param(UINT32 Id, UINT32 iso_idx, sISP_PTR *info);
extern void iqt_init_iso_idx(UINT32 Id, sISP_ISOINDEX *addr);
extern BOOL iq_upd_param_from_cfg(UINT32 Id);

#endif
