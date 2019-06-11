#ifndef _IQ_PARAM_COMMON_SC4236_EVB_H_
#define _IQ_PARAM_COMMON_SC4236_EVB_H_

#include "iq_common.h"

extern ISOMAP iso_map_tab_sc4236_evb[IQS_ISOMAPTAB_NUM];
extern DIST dist_sc4236_evb;
extern Fact_Norm   radious_sc4236_evb;
extern DCE_CFAPARAM cfa_sc4236_evb;
extern FOV fov_sc4236_evb;
extern UINT32 ldc_g_lut_sc4236_evb[65];
extern INT32 ldc_r_lut_sc4236_evb[65];
extern INT32 ldc_b_lut_sc4236_evb[65];
extern Aberation cac_sc4236_evb;
extern IQS_COLOR_TEMPERATURE_TH color_temperature_th_sc4236_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_low_sc4236_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_middle_sc4236_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_high_sc4236_evb;
extern UINT8 fs_tab_sc4236_evb[TOTAL_GAIN_NUM][16];
extern UINT8 fd_tab_sc4236_evb[TOTAL_GAIN_NUM][16];
extern IPE_CC cc_sc4236_evb;
extern UINT32 y_curve_sc4236_evb[129];
extern IQS_GAMMA_LV_TH gamma_lv_th_sc4236_evb;
extern UINT32 gamma_day_sc4236_evb[129];
extern UINT32 gamma_night_sc4236_evb[129];
extern UINT8 edg_tab_sc4236_evb[24];
extern UINT8 dds_tab_sc4236_evb[8];
extern INT16 cst_coef_sc4236_evb[9];
extern IPE_CST cst_sc4236_evb;
extern IPE_YCCFIX ycc_fix_sc4236_evb;
extern IPE_RAND_NR noise_param_sc4236_evb;
extern IPE_GamRand gamma_rand_sc4236_evb;
extern UINT32 color_3d_lut_sc4236_evb[900] ;
extern IPE_3DCCRound color_3d_sc4236_evb;
extern S_DEFOG_LIB_INIT_PARA defog_init_param_sc4236_evb;
extern UINT32 defog_wet_sc4236_evb[17];
extern SHDR_IQ_PARA hdr_sc4236_evb;
extern IME_TMNR_DISPMOMAP tmnr_momap_sel_sc4236_evb;
extern TMNR_AUTO_INFO tmnr_auto_info_sc4236_evb;

extern void iq_param_sc4236_evb_table1_init(UINT32 id,UINT32 mode_idx);

#endif
