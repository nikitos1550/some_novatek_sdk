#include "Type.h"
#include "awb_alg.h"
#include "awb_api.h"
#include "awb_proc_nvt_int.h"
/******************************/
/**
    AWB CA infomation definition
*/
static awb_ca_info awb_param_ca_info = {
    AWB_WIN, AWB_WIN,
    4095,          //UINT32 RGu; (R/G)<<10
    0,          //UINT32 RGl;
    4095,       //UINT32 GGu; G (12Bits)
    0,          //UINT32 GGl;
    4095,       //UINT32 BGu; (B/G)<<10
    0,          //UINT32 BGl;
    4095,       //UINT32 RBGu;((R+B)/G)<<9
    0           //UINT32 RBGl;
};

/******************************/
/**
    CT gain
*/
#define AWB_CT_2300K_RGAIN 266
#define AWB_CT_2300K_BGAIN 480

#define AWB_CT_2800K_RGAIN 296
#define AWB_CT_2800K_BGAIN 457

#define AWB_CT_3700K_RGAIN 336
#define AWB_CT_3700K_BGAIN 416

#define AWB_CT_4700K_RGAIN 383
#define AWB_CT_4700K_BGAIN 383

#define AWB_CT_6500K_RGAIN 446
#define AWB_CT_6500K_BGAIN 326

#define AWB_CT_11000K_RGAIN 523
#define AWB_CT_11000K_BGAIN 296

/******************************/
/**
    AWB Method1 table
*/
static awb_th awb_param_th = {
    5, //YL
    245, //YU
    116, //RpBL
    262, //RpBU
    -55, //RsBL
    80, //RsBU
    56, //R2GL
    153, //R2GU
    60, //B2GL
    109, //B2GU
    52, //RmBL
    75, //RmBU
};

/******************************/
/**
    LV check
*/
static awb_lv_check awb_param_lv_check[AWB_TUNING_CHECK_LV_MAX] = {
    { 400, 100}, //LV4~LV1
    { 900, 600}, //LV9~LV6
    { 2600, 1000}, //LV26~LV10
};

/******************************/
/**
    CT Weight
*/
static ctmp_lut ct_map[AWB_TUNING_CT_WEIGHT_MAX] = {
     {898, 2300},
     {934, 2800},
     {980, 3700},
     {1024, 4700},
     {1089, 6500},
     {1138, 11000},
};

static cwgt_lut out_cwgt_lut[AWB_TUNING_CT_WEIGHT_MAX] = {
     {2300, 1},
     {2800, 1},
     {3700, 1},
     {4700, 1},
     {6500, 1},
     {11000, 1},
};

static cwgt_lut in_cwgt_lut[AWB_TUNING_CT_WEIGHT_MAX] = {
     {2300, 1},
     {2800, 1},
     {3700, 1},
     {4700, 1},
     {6500, 1},
     {11000, 1},
};

static cwgt_lut night_cwgt_lut[AWB_TUNING_CT_WEIGHT_MAX] = {
     {2300, 1},
     {2800, 1},
     {3700, 1},
     {4700, 1},
     {6500, 1},
     {11000, 1},
};

static awb_ct_weight awb_param_ct_weight = {
    ct_map,
    out_cwgt_lut,
    in_cwgt_lut,
    night_cwgt_lut,
};
/******************************/
/**
    AWB target
*/

static awb_target awb_param_target[AWB_TUNING_TARGET_MAX] = {
    //cx rgratio bgratio
    { 934, 1024, 1024}, //L-CT
    { 1024, 1024, 1024}, //M-CT
    { 1089, 1024, 1024}, //H-CT
};

/******************************/
/**
    AWB CT table
    The color temperature must be steeled sort
*/
static awb_ct_info awb_param_ct_table[AWB_TUNING_CT_MAX] = {
     {2300, { AWB_CT_2300K_RGAIN,  256, AWB_CT_2300K_BGAIN}},
     {2800, { AWB_CT_2800K_RGAIN,  256, AWB_CT_2800K_BGAIN}},
     {3700, { AWB_CT_3700K_RGAIN,  256, AWB_CT_3700K_BGAIN}},
     {4700, { AWB_CT_4700K_RGAIN,  256, AWB_CT_4700K_BGAIN}},
     {6500, { AWB_CT_6500K_RGAIN,  256, AWB_CT_6500K_BGAIN}},
     {11000, { AWB_CT_11000K_RGAIN,  256, AWB_CT_11000K_BGAIN}},
};

/******************************/
/**
    AWB Manual table
*/
static awb_mwb_gain awb_param_mwb_tab[AWB_TUNING_MWB_MAX] = {
    { 256, 256, 256},
    { AWB_CT_6500K_RGAIN, 256, AWB_CT_6500K_BGAIN * 0.85}, //AWB_MODE_CLOUDY
    { AWB_CT_6500K_RGAIN, 256, AWB_CT_6500K_BGAIN}, //AWB_MODE_DAYLIGHT
    { AWB_CT_2800K_RGAIN, 256, AWB_CT_2800K_BGAIN}, //AWB_MODE_SUNSET
    { AWB_CT_2300K_RGAIN, 256, AWB_CT_2300K_BGAIN}, //AWB_MODE_TUNGSTEN
    { AWB_CT_4700K_RGAIN, 256, AWB_CT_4700K_BGAIN}, //AWB_MODE_FLUORESCENT1
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256}
};

static awb_converge awb_converge_par = {
     4, //skip frame
     10, //speed
     259, //tolerance 259/256=1%
};

static awb_expand_block awb_expand_block_par[AWB_TUNING_BLOCK_MAX] = {
    {1, 400, 1000, 6, 225, 154, 167, -19, -6},
    {0, 600, 1000, 6, 225, 105, 122, 56, 74},
    {0, 600, 1000, 6, 225, 91, 105, 18, 32},
    {0, 600, 1000, 6, 225, 114, 128, -67, -53},
};

awb_param awb_param_jxk03_evb = {
    &awb_param_ca_info,
    &awb_param_th,
    &awb_param_lv_check[0],
    &awb_param_ct_weight,
    &awb_param_target[0],
    &awb_param_ct_table[0],
    &awb_param_mwb_tab[0],
    &awb_converge_par,
    &awb_expand_block_par[0],
};

void awb_get_param_jxk03_evb(UINT32* param);

void awb_get_param_jxk03_evb(UINT32* param)
{
	*param =  (UINT32)(&awb_param_jxk03_evb);
		//DBG_ERR("%x \r\n", *param);
}
