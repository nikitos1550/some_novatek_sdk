#ifndef _IQ_PARAM_COMMON_IMX307_EVB_H_
#define _IQ_PARAM_COMMON_IMX307_EVB_H_

#include "iq_common.h"

extern ISOMAP iso_map_tab_imx307_evb[IQS_ISOMAPTAB_NUM];
extern DIST dist_imx307_evb;
extern Fact_Norm   radious_imx307_evb;
extern DCE_CFAPARAM cfa_imx307_evb;
extern FOV fov_imx307_evb;
extern UINT32 ldc_g_lut_imx307_evb[65];
extern INT32 ldc_r_lut_imx307_evb[65];
extern INT32 ldc_b_lut_imx307_evb[65];
extern Aberation cac_imx307_evb;
extern IQS_COLOR_TEMPERATURE_TH color_temperature_th_imx307_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_low_imx307_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_middle_imx307_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_high_imx307_evb;
extern UINT8 fs_tab_imx307_evb[TOTAL_GAIN_NUM][16];
extern UINT8 fd_tab_imx307_evb[TOTAL_GAIN_NUM][16];
extern IPE_CC cc_imx307_evb;
extern UINT32 y_curve_imx307_evb[129];
extern IQS_GAMMA_LV_TH gamma_lv_th_imx307_evb;
extern UINT32 gamma_day_imx307_evb[129];
extern UINT32 gamma_night_imx307_evb[129];
extern UINT8 edg_tab_imx307_evb[24];
extern UINT8 dds_tab_imx307_evb[8];
extern INT16 cst_coef_imx307_evb[9];
extern IPE_CST cst_imx307_evb;
extern IPE_YCCFIX ycc_fix_imx307_evb;
extern IPE_RAND_NR noise_param_imx307_evb;
extern IPE_GamRand gamma_rand_imx307_evb;
extern UINT32 color_3d_lut_imx307_evb[900] ;
extern IPE_3DCCRound color_3d_imx307_evb;
extern S_DEFOG_LIB_INIT_PARA defog_init_param_imx307_evb;
extern UINT32 defog_wet_imx307_evb[17];
extern SHDR_IQ_PARA hdr_imx307_evb;
extern IME_TMNR_DISPMOMAP tmnr_momap_sel_imx307_evb;
extern TMNR_AUTO_INFO tmnr_auto_info_imx307_evb;

extern void iq_param_imx307_evb_table1_init(UINT32 id,UINT32 mode_idx);

#endif
