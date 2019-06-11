#include "iq_common.h"
#include "note.h"
#include "pq_obj.h"

#include "ipl_cal_obj_drv.h"
#include "ipl_cal_rw.h"
#include "ipl_cal_api.h"

#include "NvtVerInfo.h"

NVTVER_VERSION_ENTRY(IQ_PROC_NVT, 1, 00, 000, 00)


/******************************************************************************/
/* All Parameter Addr read from iqs_commom */
sISP_ISOINDEX* g_iso_index_addr[Total_SENIDNum] = {NULL};
ISOMAP *g_iso_map_tab_addr[Total_SENIDNum] = {NULL};
ENGINE_CTRL *g_e_ctrl_addr[Total_SENIDNum] = {NULL};
SIE_EmbeddedCS *g_ecs_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ecs_tab_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ob_addr[Total_SENIDNum] = {NULL};
UINT8 *g_wdr_strength_addr[Total_SENIDNum] = {NULL};
IQS_WDR_LCE_OFFSET *g_wdr_lce_offset_addr[Total_SENIDNum] = {NULL};
IQS_LCE *g_lce_strength_addr[Total_SENIDNum] = {NULL};
SR_WDR_GCE_PARAM *g_gce_strength_addr[Total_SENIDNum] = {NULL};
SHDR_IQ_PARA *g_hdr_addr[Total_SENIDNum] = {NULL};
S_DEFOG_LIB_INIT_PARA *g_defog_addr[Total_SENIDNum] = {NULL};
UINT32 *g_defog_wet_addr[Total_SENIDNum] = {NULL};
UINT32 *g_outl_bri_addr[Total_SENIDNum] = {NULL};
UINT32 *g_outl_dark_addr[Total_SENIDNum] = {NULL};
UINT32 *g_outl_cnt_addr[Total_SENIDNum] = {NULL};
IFE_OUTL *g_outl_set_addr[Total_SENIDNum] = {NULL};
UINT32 *g_spatial_w_addr[Total_SENIDNum] = {NULL};
Spatial_Filter *g_spatial_addr[Total_SENIDNum] = {NULL};
UINT32 *g_range_a_thr_addr[Total_SENIDNum] = {NULL};
UINT32 *g_range_a_lut_addr[Total_SENIDNum] = {NULL};
UINT32 *g_range_b_thr_addr[Total_SENIDNum] = {NULL};
UINT32 *g_range_b_lut_addr[Total_SENIDNum] = {NULL};
RangeCFGB *g_range_b_addr[Total_SENIDNum] = {NULL};
RANGE_FILT_CLAMP_PARAM *g_filter_clamp_addr[Total_SENIDNum] = {NULL};
IQS_VIG_CENTER_RATIO *g_vig_center_addr[Total_SENIDNum] = {NULL};
IQS_VIG_SET *g_vig_addr[Total_SENIDNum] = {NULL};
IQS_VIG_ISO_TH *g_vig_iso_th_addr[Total_SENIDNum] = {NULL};
UINT8 *g_gbal_s_tab_addr[Total_SENIDNum] = {NULL};
UINT8 *g_gbal_d_tab_addr[Total_SENIDNum] = {NULL};
IFE_GBAL *g_gbal_addr[Total_SENIDNum] = {NULL};
UINT8 *g_rbfill_luma_tab_addr[Total_SENIDNum] = {NULL};
UINT8 *g_rbfill_ratio_tab_addr[Total_SENIDNum] = {NULL};
IFE_RBFILL *g_rbfill_addr[Total_SENIDNum] = {NULL};
DIST *g_dist_addr[Total_SENIDNum] = {NULL};
Fact_Norm *g_radious_addr[Total_SENIDNum] = {NULL};
DCE_CFAPARAM *g_cfa_addr[Total_SENIDNum] = {NULL};
FOV *g_fov_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ldc_g_lut_addr[Total_SENIDNum] = {NULL};
INT32 *g_ldc_r_lut_addr[Total_SENIDNum] = {NULL};
INT32 *g_ldc_b_lut_addr[Total_SENIDNum] = {NULL};
Aberation *g_cac_addr[Total_SENIDNum] = {NULL};
INT16 *g_edge_kernel_addr[Total_SENIDNum] = {NULL};
UINT8 *g_edir_tab_addr[Total_SENIDNum] = {NULL};
IPE_DEE *g_edir_ext_addr[Total_SENIDNum] = {NULL};
IPE_DirScorCal *g_score_tab_addr[Total_SENIDNum] = {NULL};
IPE_DirScorCalTh *g_score_th_addr[Total_SENIDNum] = {NULL};
IPE_KAEdgeW *g_ewa_addr[Total_SENIDNum] = {NULL};
IPE_KBEdgeW *g_ewb_addr[Total_SENIDNum] = {NULL};
IPE_EdgeW *g_ewparam_addr[Total_SENIDNum] = {NULL};
IPE_EDGEENH *g_edge_enh_addr[Total_SENIDNum] = {NULL};
UINT8 *g_ed_tab_addr[Total_SENIDNum] = {NULL};
EdgeMap *g_ed_map_addr[Total_SENIDNum] = {NULL};
UINT8 *g_es_tab_addr[Total_SENIDNum] = {NULL};
EdgeMapS *g_es_map_addr[Total_SENIDNum] = {NULL};
LPF *g_lpf_addr[Total_SENIDNum] = {NULL};
IQS_COLOR_TEMPERATURE_TH *g_color_temperature_th_addr[Total_SENIDNum] = {NULL};
IQS_COLOR_TEMPERATURE_PARAM *g_color_low_addr[Total_SENIDNum] = {NULL};
IQS_COLOR_TEMPERATURE_PARAM *g_color_middle_addr[Total_SENIDNum] = {NULL};
IQS_COLOR_TEMPERATURE_PARAM *g_color_high_addr[Total_SENIDNum] = {NULL};
UINT8 *g_fs_tab_addr[Total_SENIDNum] = {NULL};
UINT8 *g_fd_tab_addr[Total_SENIDNum] = {NULL};
IPE_CC *g_cc_addr[Total_SENIDNum] = {NULL};
UINT32 *g_y_curve_addr[Total_SENIDNum] = {NULL};
IQS_GAMMA_LV_TH *g_gamma_lv_th_addr[Total_SENIDNum] = {NULL};
UINT32 *g_gamma_day_addr[Total_SENIDNum] = {NULL};
UINT32 *g_gamma_night_addr[Total_SENIDNum] = {NULL};
UINT32 *g_color_3d_lut_addr[Total_SENIDNum] = {NULL};
IPE_3DCCRound *g_color_3d_addr[Total_SENIDNum] = {NULL};
UINT8 *g_edg_tab_addr[Total_SENIDNum] = {NULL};
UINT8 *g_dds_tab_addr[Total_SENIDNum] = {NULL};
INT8 *g_saturation_addr[Total_SENIDNum] = {NULL};
INT8 *g_contrast_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ccon_tab_addr[Total_SENIDNum] = {NULL};
IQS_CCTRL_INFO *g_cctrl_info_addr[Total_SENIDNum] = {NULL};
INT16 *g_cst_coef_addr[Total_SENIDNum] = {NULL};
IPE_CST *g_cst_addr[Total_SENIDNum] = {NULL};
IPE_YCCFIX *g_ycc_fix_addr[Total_SENIDNum] = {NULL};
IPE_RAND_NR *g_noise_param_addr[Total_SENIDNum] = {NULL};
IPE_GamRand *g_gamma_rand_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ref_center_y_th_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ref_center_y_wt_addr[Total_SENIDNum] = {NULL};
IFE2_REFCENT_Set *g_ref_center_y_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ref_center_uv_th_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ref_center_uv_wt_addr[Total_SENIDNum] = {NULL};
IFE2_REFCENT_Set *g_ref_center_uv_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ife2filter_y_th_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ife2filter_y_wt_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ife2filter_uv_th_addr[Total_SENIDNum] = {NULL};
UINT32 *g_ife2filter_uv_wt_addr[Total_SENIDNum] = {NULL};
IQS_IFE2_FILTER *g_ife2filter_addr[Total_SENIDNum] = {NULL};
IFE2_GRAY_Statal *g_gray_statical_addr[Total_SENIDNum] = {NULL};
IME_CHRA_ADAP *g_chroma_adapt_addr[Total_SENIDNum] = {NULL};
IME_CHRA_CADAP *g_chroma_adapt_color_addr[Total_SENIDNum] = {NULL};
IME_CHRA_LADAP *g_chroma_adapt_lum_addr[Total_SENIDNum] = {NULL};
UINT32 *g_chroma_supp_w_y_addr[Total_SENIDNum] = {NULL};
UINT32 *g_chroma_supp_w_c_addr[Total_SENIDNum] = {NULL};
IME_ChromaSupp *g_chroma_supp_addr[Total_SENIDNum] = {NULL};
IME_HAL_TMNR_INFO **g_tmnr_info_addr[Total_SENIDNum] = {NULL};
UINT32 *g_tmnr_momap_sel_addr[Total_SENIDNum] = {NULL};
TMNR_AUTO_INFO *g_tmnr_auto_info_addr[Total_SENIDNum] = {NULL};
UINT32 *g_tnr_total_num_addr[Total_SENIDNum] = {NULL};
INT8 *g_tnr_level_addr[Total_SENIDNum] = {NULL};
H26XEncTnr **g_tnr_param_addr[Total_SENIDNum] = {NULL};
/******************************************************************************/

static UINT32 g_gamma_addr_buf[Total_SENIDNum];
static UINT32 g_y_curve_addr_buf[Total_SENIDNum];
static BOOL g_gamma_update[Total_SENIDNum] = {FALSE};
static BOOL g_y_curve_update[Total_SENIDNum] = {FALSE};
static BOOL g_y_curve_flag[Total_SENIDNum] = {FALSE};
static UINT32 g_iq_table_update_bit[Total_SENIDNum] = {0};
static UINT32 g_iq_engine_update_bit[Total_SENIDNum] = {0};
static UINT32 iq_dbg_mode = 0;
static iq_dbg_element_addr iq_dbg_address;
static void iq_all_param_init(UINT32 id, UINT32 mode_idx);

static UINT32 pre_ir_sub_rg[Total_SENIDNum] = {0};
static UINT32 pre_ir_sub_b[Total_SENIDNum] = {0};
static UINT32 pre_ir_sat_gain[Total_SENIDNum] = {0};

///////////////////////////////////////////////
// Body
///////////////////////////////////////////////
static ER SIE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, SIE_SUBFUNC *subf, IPL_HAL_GROUP *group)
{
#ifndef IQS_SIMT_ENABLE
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
	UINT32 RGain = 256, GGain = 256, BGain = 256, IRGain=256;
    UINT32 DGain = AE_AlgGetUIInfo(SenId_Value, AE_SEL_ISP_GAIN);
	IQS_VIG_INFO VigInfo = {0};
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sensor_status = {0};
	ER rt = E_OK;
	sen_info.mode = NULL;
    awb_ca_info awb_cainfo;
    UINT32 ir_level;
    UINT16 i;
    UINT16 idx = 1;

	switch ((UINT32)MsgID) {
	case IQCB_PRV_SIE_INIT:
	case IQCB_PREIME_D2D_SIE_SUB:
	case IQCB_PREIME_D2D_SIE_PRI:
	case IQS_AUTO_TRIG:
		subf->sieFuncEn |= (
                                0
						   );
		group->sieUpdate |= (
                                SIE_SUBFEN          |   ///< update func enable/disable
                                SIE_OBOFS_          |   ///< update ob offset
                                SIE_CGAIN           |   ///< update Color gain
                                SIE_STATS_LA_GAMMA  |   ///< update LA Gamma
                                0
							);

		if (iq_check_capture(MsgID) == TRUE) {
			sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_CAPSENMODE);
			rt = sensor_get_info((SENSOR_ID)SenId_Value, SEN_GET_STATUS, (void *)&sensor_status);
			sen_info = sensor_status.sensor_info;
		} else {
			sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_PRVSENMODE);
			rt = sensor_get_info((SENSOR_ID)SenId_Value, SEN_GET_STATUS, (void *)&sensor_status);
			sen_info = sensor_status.sensor_info;
		}
		if (rt != E_OK || sen_info.mode == NULL) {
			DBG_ERR("NULL Info.Mode\r\n");
			return E_SYS;
		}

        //*************** OB ***************//
        if (iq_get_ob_sub_mode(SenId_Value) == IQS_SIE_OBSUB) {
			subf->sieFuncEn &= ~OBByPassEn;
		} else {
			subf->sieFuncEn |= OBByPassEn;
		}

        //*************** VIG ***************//
        if(g_e_ctrl_addr[SenId_Value]->vig.enable == VIG_ON) {
    		group->sieUpdate |= SIE_STATS_VIG;      ///< update VIG
        } else {
    		group->sieUpdate &= ~SIE_STATS_VIG;
        }
 		if (!(MsgID == IQCB_PRV_SIE_INIT || MsgID == IQCB_PREIME_D2D_SIE_PRI || MsgID == IQCB_PREIME_D2D_SIE_SUB || (g_iq_table_update_bit[SenId_Value]&IQS_TBL_VIG))) {
			group->sieUpdate &= ~SIE_STATS_VIG;
		}

        //*************** ECS ***************//
        if(g_e_ctrl_addr[SenId_Value]->ecs.enable == ECS_ON) {
			subf->sieFuncEn |= ECSEn;               ///< enable Color shading compensation
			group->sieUpdate |= SIE_ECS_;           ///< update ECS/including ECS table addr
        } else {
 			subf->sieFuncEn &= ~ECSEn;
			group->sieUpdate &= ~SIE_ECS_;
        }
		if (sen_info.mode->mode_type == SENSOR_MODE_CCIR || sen_info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
			subf->sieFuncEn &= ~ECSEn;
			group->sieUpdate &= ~SIE_ECS_;
		}
		if (!(MsgID == IQCB_PRV_SIE_INIT || MsgID == IQCB_PREIME_D2D_SIE_PRI || MsgID == IQCB_PREIME_D2D_SIE_SUB || (g_iq_table_update_bit[SenId_Value]&IQS_TBL_ECS))) {
			group->sieUpdate &= ~SIE_ECS_;
		}

        //*************** LA Gamma ***************//
		if (!(MsgID == IQCB_PRV_SIE_INIT || MsgID == IQCB_PREIME_D2D_SIE_PRI || MsgID == IQCB_PREIME_D2D_SIE_SUB || (g_iq_table_update_bit[SenId_Value]&IQS_TBL_GAMMA))) {
			group->sieUpdate &= ~SIE_STATS_LA_GAMMA;
		}

		//////////////////
		// SIE-NonISO param update
		//////////////////
        if (iqt_get_param_update(SenId_Value, ISP_VIG) == TRUE){
            DBG_DUMP("^Gupdate VIG table!!!!\r\n");
            g_vig_center_buf[SenId_Value] = *(g_vig_center_addr[SenId_Value]);
            g_vig_set_temp[SenId_Value] = *(g_vig_addr[SenId_Value]);
            iqt_reset_param_update(SenId_Value, ISP_VIG);
        }
        if (iqt_get_param_update(SenId_Value, ISP_ECS) == TRUE){
            DBG_DUMP("^Gupdate ECS table!!!!\r\n");
        	memcpy(g_ecs_tab_buf[SenId_Value], g_ecs_tab_addr[SenId_Value],    sizeof(UINT32) * 4225);
        	g_ecs_buf[SenId_Value] = *(g_ecs_addr[SenId_Value]);
            iqt_reset_param_update(SenId_Value, ISP_ECS);
        }

		////////////////////
		// SIE-OB Substration
		////////////////////
		subf->OBSub.Offset = (g_rhe_ob_buf[SenId_Value][1] + g_rhe_ob_buf[SenId_Value][2]) >> 1;

		////////////////////
		// SIE-ECS
		////////////////////
		if (group->sieUpdate & SIE_ECS_) {
        	UINT32 rotate = 0;
            rt = sensor_get_info((SENSOR_ID)SenId_Value, SEN_GET_USER_OPTION, (void *)&rotate);
        	if (rt != E_OK ) {
        		DBG_ERR("NULL sensor rotate info\r\n");
        	}

            CAL_RW_HEADER_INFO rw_header_info = {0};
            rw_header_info.sec.cal_item = CAL_ITEM_ECS;
            rw_header_info.key.id = (IPL_PROC_ID)SenId_Value;
            rw_header_info.key.cal_cond = CAL_COND_DFT;
            rw_header_info.key.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)SenId_Value, IPL_SEL_PRVSENMODE);

            UINT32 data_addr = 0;
            rt =  ipl_cal_getcalrst((IPL_PROC_ID)SenId_Value, &rw_header_info, &data_addr);

            if ((rt == E_OK) && (rw_header_info.value.cal_status == CAL_STATUS_OK)) {
                if (rw_header_info.value.read_data_size == 16900){ //65*65*4byte
                    DBG_DUMP("^Gget 65x65 ECS Calibration data\r\n");
    				subf->Ecs.EcsMap = ecs65x65;
			        subf->Ecs.Inaddr = (UINT32)iq_get_ecs_tab(SenId_Value, (UINT32 *)data_addr ,rotate);
    				subf->Ecs.pecs = &g_ecs_buf[SenId_Value];
        		} else {
                    DBG_ERR("read AWB Calibration data Fail\r\n");
        			subf->Ecs.EcsMap = ecs65x65;
        			subf->Ecs.Inaddr = (UINT32)iq_get_ecs_tab(SenId_Value, (UINT32 *)g_ecs_tab_buf[SenId_Value] ,rotate);
        			subf->Ecs.pecs = &g_ecs_buf[SenId_Value];
                }
            } else {
                DBG_IND("NO ECS cal data\r\n");
    			subf->Ecs.EcsMap = ecs65x65;
    			subf->Ecs.Inaddr = (UINT32)iq_get_ecs_tab(SenId_Value, (UINT32 *)g_ecs_tab_buf[SenId_Value] ,rotate);
    			subf->Ecs.pecs = &g_ecs_buf[SenId_Value];
            }
		}

		////////////////////
		// SIE-LA GAMMA
		////////////////////
		if (group->sieUpdate & SIE_STATS_LA_GAMMA) {
			subf->La.GmaTbl = SIE_xfer2CAGamma(SenId_Value, (UINT32 *)g_gamma_buf[SenId_Value]);
			subf->pStatPathInfo.bLaGama1 = ENABLE;
		}

        //////////////////
        // SIE-VIG Gain
        //////////////////
        if (group->sieUpdate & SIE_STATS_VIG) {
            VigInfo.uiCntrPosX = g_vig_center_buf[SenId_Value].CenterXOffset;
            VigInfo.uiCntrPosY = g_vig_center_buf[SenId_Value].CenterYOffset;

            subf->pStatPathInfo.bVig = ENABLE;
            if (iq_check_capture(MsgID) == FALSE) {
                SIE_GetVigSetting(SenId_Value, FALSE, &VigInfo);
            } else {
                SIE_GetVigSetting(SenId_Value, TRUE, &VigInfo);
            }
            g_ca_vig_buf[SenId_Value].Center.x = VigInfo.uiVigX;
            g_ca_vig_buf[SenId_Value].Center.y = VigInfo.uiVigY;
            g_ca_vig_buf[SenId_Value].T        = VigInfo.uiVigT;
            g_ca_vig_buf[SenId_Value].XDIV     = VigInfo.uiVigXDiv;
            g_ca_vig_buf[SenId_Value].YDIV     = VigInfo.uiVigYDiv;
            g_ca_vig_buf[SenId_Value].TabGain  = g_vig_set_buf[SenId_Value].TabGain;
            g_ca_vig_buf[SenId_Value].LUT_TblAddr = (UINT32)g_vig_set_buf[SenId_Value].Tab;
            subf->pVig = &g_ca_vig_buf[SenId_Value];
        }

		////////////////////
		// SIE-CGain
		////////////////////
		group->sieUpdate |=  SIE_STATS_CA_IRSUB;
		group->sieUpdate |=  SIE_STATS_CA_;
		group->sieUpdate |=  SIE_STATS_LA_;
		group->sieUpdate |=  SIE_STATS_LA_CG;
		subf->pStatPathInfo.bCaThreshold = ENABLE;

		AWB_GetCAInfo(SenId_Value, &awb_cainfo);
		subf->Ca.CA_TH_INFO.Rth.Upper = awb_cainfo.RGu;
		subf->Ca.CA_TH_INFO.Rth.Lower = awb_cainfo.RGl;
		subf->Ca.CA_TH_INFO.Gth.Upper = awb_cainfo.GGu;
		subf->Ca.CA_TH_INFO.Gth.Lower = awb_cainfo.GGl;
		subf->Ca.CA_TH_INFO.Bth.Upper = awb_cainfo.BGu;
		subf->Ca.CA_TH_INFO.Bth.Lower = awb_cainfo.BGl;
		subf->Ca.CA_TH_INFO.Pth.Upper = awb_cainfo.RBGu;
		subf->Ca.CA_TH_INFO.Pth.Lower = awb_cainfo.RBGl;

        //RGBIr
        if((iq_check_rgbir(SenId_Value) == TRUE) ){
            awb_get_rgbir_level(SenId_Value, g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubR, g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubB, &ir_level);

            if (ir_level < ir_level_lut[0]){
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubR = ir_sub_rg_lut[0];
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubG = ir_sub_rg_lut[0];
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubB = ir_sub_b_lut[0];
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSatGain = ir_sat_gain_lut[0];
            } else if(ir_level >= ir_level_lut[15]){
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubR = ir_sub_rg_lut[15];
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubG = ir_sub_rg_lut[15];
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubB = ir_sub_b_lut[15];
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSatGain = ir_sat_gain_lut[15];
            } else {
                for (i=1; i<16; i++){
                    if (ir_level < ir_level_lut[i]){
                            idx=i;
                            break;
                        }
                }
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubR = IQS_Intpl(ir_level,ir_sub_rg_lut[idx-1],ir_sub_rg_lut[idx],ir_level_lut[idx-1],ir_level_lut[idx]);
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubG = IQS_Intpl(ir_level,ir_sub_rg_lut[idx-1],ir_sub_rg_lut[idx],ir_level_lut[idx-1],ir_level_lut[idx]);
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubB = IQS_Intpl(ir_level,ir_sub_b_lut[idx-1],ir_sub_b_lut[idx],ir_level_lut[idx-1],ir_level_lut[idx]);
                g_cfa_buf[SenId_Value].CfaIrSub.uiIrSatGain = IQS_Intpl(ir_level,ir_sat_gain_lut[idx-1],ir_sat_gain_lut[idx],ir_level_lut[idx-1],ir_level_lut[idx]);
            }
            //smooth
            pre_ir_sub_rg[SenId_Value] = ((g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubR << 4) + pre_ir_sub_rg[SenId_Value]*31)>>5;
            pre_ir_sub_b[SenId_Value] = ((g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubB << 4) + pre_ir_sub_b[SenId_Value]*31)>>5;
            pre_ir_sat_gain[SenId_Value] = ((g_cfa_buf[SenId_Value].CfaIrSub.uiIrSatGain << 3) + pre_ir_sat_gain[SenId_Value]*31)>>5;
            g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubR = pre_ir_sub_rg[SenId_Value]>>4;
            g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubG = pre_ir_sub_rg[SenId_Value]>>4;
            g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubB = pre_ir_sub_b[SenId_Value]>>4;
            g_cfa_buf[SenId_Value].CfaIrSub.uiIrSatGain = pre_ir_sat_gain[SenId_Value]>>3;

            subf->Ca.CA_IR_SUB.R_Weight = g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubR;
            subf->Ca.CA_IR_SUB.G_Weight = g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubG;
            subf->Ca.CA_IR_SUB.B_Weight = g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubB;

        }
        DBG_IND("ir_level=%d, idx=%d, SubR=%d, SubG=%d, SubB=%d, sat_gain=%d\r\n",ir_level,idx,subf->Ca.CA_IR_SUB.R_Weight,subf->Ca.CA_IR_SUB.G_Weight,subf->Ca.CA_IR_SUB.B_Weight,g_cfa_buf[SenId_Value].CfaIrSub.uiIrSatGain);

		if (iq_check_capture(MsgID) == FALSE) {
            AWB_GetColorGain(SenId_Value, &RGain, &GGain, &BGain);
		} else {
            awb_get_cap_color_gain(SenId_Value, &RGain, &GGain, &BGain);
		}

        //LA CGain
        subf->pStatPathInfo.bLaCg = ENABLE;
        RGain = (((RGain >> 1)*DGain)>>7);  //LA 1x=128(sie fix 3.7)
        GGain = (((GGain >> 1)*DGain)>>7);
        BGain = (((BGain >> 1)*DGain)>>7);
        IRGain = (((IRGain >> 1)*DGain)>>7);
		subf->La.LA_CG_INFO.RGain = (RGain > 1023) ? 1023 : RGain;
		subf->La.LA_CG_INFO.GGain = (GGain > 1023) ? 1023 : GGain;
		subf->La.LA_CG_INFO.BGain = (BGain > 1023) ? 1023 : BGain;
        subf->La.LA_CG_INFO.IRGain = (IRGain > 1023) ? 1023 : IRGain;

		////////////////////
		// SIE-DGain
		////////////////////
		subf->sieFuncEn |= CGainEn;
        subf->CGain.CGain_SEL_3_7 = TRUE;
        subf->CGain.CGain_R  = DGain;
        subf->CGain.CGain_Gb = DGain;
        subf->CGain.CGain_Gr = DGain;
        subf->CGain.CGain_B  = DGain;
        subf->CGain.CGain_Ir = DGain;
		break;

    case IQS_AWB_END:
        ////////////////////
		// SIE-CGain
		////////////////////
		group->sieUpdate |=  SIE_STATS_CA_;
		group->sieUpdate |=  SIE_STATS_LA_;
		group->sieUpdate |=  SIE_STATS_LA_CG;
		subf->pStatPathInfo.bCaThreshold = ENABLE;

		AWB_GetCAInfo(SenId_Value, &awb_cainfo);
		subf->Ca.CA_TH_INFO.Rth.Upper = awb_cainfo.RGu;
		subf->Ca.CA_TH_INFO.Rth.Lower = awb_cainfo.RGl;
		subf->Ca.CA_TH_INFO.Gth.Upper = awb_cainfo.GGu;
		subf->Ca.CA_TH_INFO.Gth.Lower = awb_cainfo.GGl;
		subf->Ca.CA_TH_INFO.Bth.Upper = awb_cainfo.BGu;
		subf->Ca.CA_TH_INFO.Bth.Lower = awb_cainfo.BGl;
		subf->Ca.CA_TH_INFO.Pth.Upper = awb_cainfo.RBGu;
		subf->Ca.CA_TH_INFO.Pth.Lower = awb_cainfo.RBGl;

		if (iq_check_capture(MsgID) == FALSE) {
            AWB_GetColorGain(SenId_Value, &RGain, &GGain, &BGain);
		} else {
            awb_get_cap_color_gain(SenId_Value, &RGain, &GGain, &BGain);
		}

        //LA CGain
        subf->pStatPathInfo.bLaCg = ENABLE;
        RGain = (((RGain >> 1)*DGain)>>7);  //LA 1x=128(sie fix 3.7)
        GGain = (((GGain >> 1)*DGain)>>7);
        BGain = (((BGain >> 1)*DGain)>>7);
		subf->La.LA_CG_INFO.RGain = (RGain > 1023) ? 1023 : RGain;
		subf->La.LA_CG_INFO.GGain = (GGain > 1023) ? 1023 : GGain;
		subf->La.LA_CG_INFO.BGain = (BGain > 1023) ? 1023 : BGain;
		break;

	default :
		DBG_ERR("SIE_IQparam() param mode(%d) selection error!\r\n", MsgID);
		return E_SYS;
	}
	return E_OK;
#else
    DBG_DUMP("==========\r\n SIMT SIE_IQparam\r\n==========\r\n");
	return E_OK;
#endif //end #ifndef IQS_SIMT_ENABLE
}

static ER RHE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, RHE_SUBFUNC *subf, IPL_HAL_GROUP *group)
{
#ifndef IQS_SIMT_ENABLE
    UINT32 SenId_Value = iq_get_sensor_idx(Id);
    UINT32 RGain = 256, GGain = 256, BGain = 256;

	switch ((UINT32)MsgID) {
	case IQCB_PRV_RHE_INIT:
	case IQCB_PREIME_D2D_RHE_SUB:
	case IQCB_PREIME_D2D_RHE_PRI:
	case IQS_AUTO_TRIG:
		subf->rheFuncEn |= (
                                ColorGainEn         |
                                0
						   );
		group->rheUpdate |= (
                                RHE_COLORGAIN       |   ///< update color gain info
								RHE_SUBF_EN         |   ///< update func enable/disable by rheFuncEn
								0
							);

        //*************** LCE ***************//
        if (g_e_ctrl_addr[SenId_Value]->lce.enable == LCE_ON) {
			subf->rheFuncEn |= LocalContrastEn;
            group->rheUpdate |= RHE_LOCAL_CONTRAST;
		} else {
			subf->rheFuncEn &= ~LocalContrastEn;
            group->rheUpdate &= ~RHE_LOCAL_CONTRAST;
		}

		//////////////////
		// RHE-Color Gain
		//////////////////
		if (iq_get_cg_mode(SenId_Value) == IQS_RHE_CG) {
    		if (iq_check_capture(MsgID) == FALSE) {
                AWB_GetColorGain(SenId_Value, &RGain, &GGain, &BGain);
    		} else {
                awb_get_cap_color_gain(SenId_Value, &RGain, &GGain, &BGain);
    		}
    		if (RGain > 1023 || BGain > 1023) {
    			g_rhe_color_gain_buf[SenId_Value].CGainRng = RHE_HAL_CG_RNG_37;
    			g_rhe_color_gain_buf[SenId_Value].uiCGainR      = RGain >> 1;
    			g_rhe_color_gain_buf[SenId_Value].uiCGainGr     = GGain >> 1;
    			g_rhe_color_gain_buf[SenId_Value].uiCGainGb     = GGain >> 1;
    			g_rhe_color_gain_buf[SenId_Value].uiCGainB      = BGain >> 1;
        		g_rhe_color_gain_buf[SenId_Value].uiCGainIR     = 128;
    		} else {
    			g_rhe_color_gain_buf[SenId_Value].CGainRng = RHE_HAL_CG_RNG_28;
    			g_rhe_color_gain_buf[SenId_Value].uiCGainR      = RGain;
    			g_rhe_color_gain_buf[SenId_Value].uiCGainGr     = GGain;
    			g_rhe_color_gain_buf[SenId_Value].uiCGainGb     = GGain;
    			g_rhe_color_gain_buf[SenId_Value].uiCGainB      = BGain;
        		g_rhe_color_gain_buf[SenId_Value].uiCGainIR     = 256;
    		}
		} else {
            g_rhe_color_gain_buf[SenId_Value].CGainRng = RHE_HAL_CG_RNG_37;
            g_rhe_color_gain_buf[SenId_Value].uiCGainR      = 128;
            g_rhe_color_gain_buf[SenId_Value].uiCGainGr     = 128;
            g_rhe_color_gain_buf[SenId_Value].uiCGainGb     = 128;
            g_rhe_color_gain_buf[SenId_Value].uiCGainB      = 128;
            g_rhe_color_gain_buf[SenId_Value].uiCGainIR     = 128;
		}

		if (iq_get_ob_sub_mode(SenId_Value) == IQS_RHE_OBSUB) {
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsR   = g_rhe_ob_buf[SenId_Value][0];
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsGr  = g_rhe_ob_buf[SenId_Value][1];
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsGb  = g_rhe_ob_buf[SenId_Value][2];
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsB   = g_rhe_ob_buf[SenId_Value][3];
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsIR  = g_rhe_ob_buf[SenId_Value][4];
		} else {
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsR   = 0;
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsGr  = 0;
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsGb  = 0;
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsB   = 0;
    		g_rhe_color_gain_buf[SenId_Value].uiCOfsIR  = 0;
		}
        //DBG_DUMP("RHE gain:%d,%d,%d,%d\r\n",g_rhe_color_gain_buf[SenId_Value].uiCGainR,g_rhe_color_gain_buf[SenId_Value].uiCGainGr,g_rhe_color_gain_buf[SenId_Value].uiCGainGb,g_rhe_color_gain_buf[SenId_Value].uiCGainB);
        //DBG_DUMP("RHE ob:%d,%d,%d,%d\r\n",g_rhe_color_gain_buf[SenId_Value].uiCOfsR,g_rhe_color_gain_buf[SenId_Value].uiCOfsGr,g_rhe_color_gain_buf[SenId_Value].uiCOfsGb,g_rhe_color_gain_buf[SenId_Value].uiCOfsB);
        subf->color_gain_info = g_rhe_color_gain_buf[SenId_Value];

		//////////////////
		// RHE-LCE
		//////////////////
		g_lce_buf[SenId_Value].puiDifGain = g_lce_dif_gain[SenId_Value];
		g_lce_buf[SenId_Value].puiLumaGain = g_lce_luma_gain[SenId_Value];
		g_lce_buf[SenId_Value].RefSrc = RHE_HAL_LTC_MI;
        subf->local_contrast_info = g_lce_buf[SenId_Value];

		//////////////////
		// RHE-WDR Strength
		//////////////////
		UINT32 aLv, lv_idx_l, lv_idx_h = 0;
		if (iq_check_capture(MsgID) == FALSE) {
			aLv = AE_AlgGetUIInfo(SenId_Value, AE_SEL_PRV_LV);
		} else {
			aLv = AE_AlgGetUIInfo(SenId_Value, AE_SEL_CAP_LV);
		}
        lv_idx_l = aLv/100;
        lv_idx_h = lv_idx_l + 1;

    	g_wdr_strength_buf[SenId_Value] = IQS_Intpl(aLv, *(g_wdr_strength_addr[SenId_Value]+lv_idx_l), *(g_wdr_strength_addr[SenId_Value]+lv_idx_h), lv_idx_l*100, lv_idx_h*100);
        //DBG_DUMP("LV:%d,lv_l:%d,lv_h:%d,WDR strength:%d\r\n", aLv, lv_idx_l, lv_idx_h, g_wdr_strength_buf[SenId_Value]);
		break;

	default:
		DBG_ERR("RHE_IQparam() param mode(%d) selection error!\r\n", MsgID);
		return E_SYS;
	}
	return E_OK;
#else
    DBG_DUMP("==========\r\n SIMT RHE_IQparam\r\n==========\r\n");
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);

    subf->rheFuncEn |=  ChInfo->RHE_FuncEn;

    group->rheUpdate |= (
                            RHE_PATH_CURVE      |       ///< update path curve param(multi frame blending)(curve A~E)
                            RHE_COLORGAIN       |       ///< update color gain info
                            RHE_SUBIMG_FILTER   |       ///< update sub-image filter info
                            RHE_BLEND           |       ///< update blending info
                            RHE_LOCAL_CONTRAST  |       ///< update local contrast enhancement
                            RHE_TONE_CURVE      |       ///< update tone curve info
                            RHE_COLOR_SAT       |       ///< update color saturation info
                            RHE_COLOR_NOISE_SUP |       ///< update color noise suppression info
                            RHE_COLOR_CONTRAST  |       ///< update color contrast adjustment info
                            RHE_SUBF_EN         |       ///< update func enable/disable by rheFuncEn
                            0);

    //////////////////
    // RHE-Color Gain
    //////////////////
    subf->color_gain_info = ChInfo->IQS_RHE_CGain;

    //////////////////
    // RHE-Filter
    //////////////////
    subf->subimg_filter_info = ChInfo->IQS_RHE_Lpf;

    //////////////////
    // RHE-Blend
    //////////////////
    subf->blending_info = ChInfo->IQS_RHE_Blend;

    //////////////////
    // RHE-Local
    //////////////////
    subf->local_contrast_info = ChInfo->IQS_RHE_Local;

    //////////////////
    // RHE-Tone
    //////////////////
    subf->tone_curve_info = ChInfo->IQS_RHE_Tone;

    //////////////////
    // RHE-SAT
    //////////////////
    subf->color_sat_info = ChInfo->IQS_RHE_Sat;

    //////////////////
    // RHE-CNS
    //////////////////
    subf->cns_info = ChInfo->IQS_RHE_CNS;

    //////////////////
    // RHE-CAdj
    //////////////////
    subf->contrast_adj_info = ChInfo->IQS_RHE_ContrastAdj;
	return E_OK;
#endif //end #ifndef IQS_SIMT_ENABLE
}

static ER IFE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IFE_SUBFUNC *subf, IPL_HAL_GROUP *group)
{
#ifndef IQS_SIMT_ENABLE
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
    UINT32 RGain = 256, GGain = 256, BGain = 256;
	IQS_VIG_INFO VigInfo;

	switch ((UINT32)MsgID) {
	case IQCB_PRV_IFE_INIT:
	case IQCB_PREIME_D2D_IFE_SUB:
	case IQCB_PREIME_D2D_IFE_PRI:
	case IQS_AUTO_TRIG:
		subf->ifeFuncEn |= (
                                FilterEn    |  ///< enable bilateral filter
                                CgainEn     |  ///< enable color gain
                                0
						   );
		group->ifeUpdate |= (
                                IFE_SUBFEN      |       ///< update func enable/disable
                                IFE_CGAIN       |       ///< update color gain
                                IFE_CGOFS       |       ///< update offset(bayer stage)
                                IFE_CGEFFECT    |       ///< update color gain output inverse/mask
                                IFE_FILTER      |       ///< update noise reduction filter
                                0
							);

        //*************** OUTL **************//
        if(g_e_ctrl_addr[SenId_Value]->outl.enable == OUTL_ON) {
    		subf->ifeFuncEn |= OutlierEn;                       ///< enable outLier
    		group->ifeUpdate |= IFE_OUTLIER;                    ///< update outlier
        } else {
			subf->ifeFuncEn &= ~OutlierEn;
			group->ifeUpdate &= ~IFE_OUTLIER;

        }
        if (iq_get_ui_info(Id, IQ_UI_IPPNR) == IQ_NR_OFF) {
			subf->ifeFuncEn &= ~OutlierEn;
			group->ifeUpdate &= ~IFE_OUTLIER;
        }

        //*************** VIG ***************//
        if(g_e_ctrl_addr[SenId_Value]->vig.enable == VIG_ON) {
    		subf->ifeFuncEn |= VIGEN;                           ///< enable function: vignette
    		group->ifeUpdate |= (IFE_VIGPARAM | IFE_VIG_CEN);   ///< update vignette parameters / update vig center parameters
        } else {
			subf->ifeFuncEn &= ~VIGEN;
			group->ifeUpdate &= ~(IFE_VIGPARAM | IFE_VIG_CEN);
        }
		if (!(MsgID == IQCB_PRV_IFE_INIT || MsgID == IQCB_PREIME_D2D_IFE_PRI || MsgID == IQCB_PREIME_D2D_IFE_SUB || (g_iq_table_update_bit[SenId_Value]&IQS_TBL_VIG))) {
			group->ifeUpdate &= ~(IFE_VIGPARAM | IFE_VIG_CEN);
		}

        //*************** GBAL **************//
        if(g_e_ctrl_addr[SenId_Value]->gbal.enable == GBAL_ON) {
    		subf->ifeFuncEn |= GBALEN;                          ///< enable function: Gbalance
    		group->ifeUpdate |= IFE_GBALPARAM;                  ///< update Gbalance parameters
        } else {
			subf->ifeFuncEn &= ~GBALEN;
			group->ifeUpdate &= ~IFE_GBALPARAM;
        }
        if (iq_get_ui_info(Id, IQ_UI_IPPNR) == IQ_NR_OFF) {
			subf->ifeFuncEn &= ~GBALEN;
			group->ifeUpdate &= ~IFE_GBALPARAM;
        }

        //*************** RBFill **************//
        if (iq_check_rgbir(SenId_Value) == TRUE) {
            subf->ifeFuncEn |= RGBIR_RB_NRFILLEN;
            group->ifeUpdate |= IFE_RB_FILTER_IR;
        } else {
            subf->ifeFuncEn &= ~RGBIR_RB_NRFILLEN;
            group->ifeUpdate &= ~IFE_RB_FILTER_IR;
        }

		//////////////////
		// IFE-NonISO param update
		//////////////////
        if (iqt_get_param_update(SenId_Value, ISP_VIG) == TRUE){
            DBG_DUMP("^Gupdate VIG table!!!!\r\n");
         	g_vig_center_buf[SenId_Value] = *(g_vig_center_addr[SenId_Value]);
            g_vig_set_temp[SenId_Value] = *(g_vig_addr[SenId_Value]);
         	iqt_reset_param_update(SenId_Value, ISP_VIG);
        }

		//////////////////
		// IFE-Vignette
		//////////////////
		if (group->ifeUpdate & IFE_VIGPARAM) {
			VigInfo.uiCntrPosX = g_vig_center_buf[SenId_Value].CenterXOffset;
			VigInfo.uiCntrPosY = g_vig_center_buf[SenId_Value].CenterYOffset;

			if (iq_check_capture(MsgID) == FALSE) {
                IFE_GetVigSetting(SenId_Value, FALSE, &VigInfo);
			} else {
                IFE_GetVigSetting(SenId_Value, TRUE, &VigInfo);
			}
			g_vig_buf[SenId_Value].Center.ch0.x = VigInfo.uiVigX;
			g_vig_buf[SenId_Value].Center.ch0.y = VigInfo.uiVigY;
			g_vig_buf[SenId_Value].Center.ch1 = g_vig_buf[SenId_Value].Center.ch0;
			g_vig_buf[SenId_Value].Center.ch2 = g_vig_buf[SenId_Value].Center.ch0;
			g_vig_buf[SenId_Value].Center.ch3 = g_vig_buf[SenId_Value].Center.ch0;
			g_vig_buf[SenId_Value].T = VigInfo.uiVigT;
			g_vig_buf[SenId_Value].TabGain = g_vig_set_buf[SenId_Value].TabGain;
			g_vig_buf[SenId_Value].XDIV = VigInfo.uiVigXDiv;
			g_vig_buf[SenId_Value].YDIV = VigInfo.uiVigYDiv;
			g_vig_buf[SenId_Value].CH0_LUT_TblAddr = (UINT32)g_vig_set_buf[SenId_Value].Tab;
			g_vig_buf[SenId_Value].CH1_LUT_TblAddr = (UINT32)g_vig_set_buf[SenId_Value].Tab;
			g_vig_buf[SenId_Value].CH2_LUT_TblAddr = (UINT32)g_vig_set_buf[SenId_Value].Tab;
			g_vig_buf[SenId_Value].CH3_LUT_TblAddr = (UINT32)g_vig_set_buf[SenId_Value].Tab;
			g_vig_buf[SenId_Value].bDither = FALSE;
			g_vig_buf[SenId_Value].DitherRstEn = FALSE;
		}

		//////////////////
		// IFE-Outlier
		//////////////////
		g_outl_set_buf[SenId_Value].pBrightThres = g_outl_bri_buf[SenId_Value];
		g_outl_set_buf[SenId_Value].pDarkThres = g_outl_dark_buf[SenId_Value];
		g_outl_set_buf[SenId_Value].pOutlCnt = g_outl_cnt_buf[SenId_Value];

		//////////////////
		// IFE-Spatial Filter
		//////////////////
		if (!iq_dbg_mode) {
			g_spatial_buf[SenId_Value].pWeight = g_spatial_w_buf[SenId_Value];
		}
		g_filter_buf[SenId_Value].Spatial = &g_spatial_buf[SenId_Value];

		//////////////////
		// IFE-Range Filter
		//////////////////
        if ((iq_get_ui_info(Id, IQ_UI_IPPNR) == IQ_NR_OFF)||(g_e_ctrl_addr[SenId_Value]->range_nr.enable == RANGE_NR_OFF)) {
			memset(g_range_a_thr_buf[SenId_Value], 0, sizeof(UINT32) * 6);
			memset(g_range_b_thr_buf[SenId_Value], 0, sizeof(UINT32) * 6);
			memset(g_range_a_lut_buf[SenId_Value], 0, sizeof(UINT32) * 17);
			memset(g_range_b_lut_buf[SenId_Value], 0, sizeof(UINT32) * 17);
        }

		g_range_a_buf[SenId_Value].pThres = g_range_a_thr_buf[SenId_Value];
		g_range_a_buf[SenId_Value].pThLut = g_range_a_lut_buf[SenId_Value];
		g_range_b_buf[SenId_Value].pThres = g_range_b_thr_buf[SenId_Value];
		g_range_b_buf[SenId_Value].pThLut = g_range_b_lut_buf[SenId_Value];
		g_range_filter_buf[SenId_Value].A = &g_range_a_buf[SenId_Value];
		g_range_filter_buf[SenId_Value].B = &g_range_b_buf[SenId_Value];
		g_filter_buf[SenId_Value].Range = &g_range_filter_buf[SenId_Value];

		if (!iq_dbg_mode) {
			g_filter_buf[SenId_Value].Clamp.Th = g_filter_clamp_buf[SenId_Value].Clamp.Th;
			g_filter_buf[SenId_Value].Clamp.Mul = g_filter_clamp_buf[SenId_Value].Clamp.Mul;
		}
		g_filter_buf[SenId_Value].Clamp.Dlt = g_filter_clamp_buf[SenId_Value].Clamp.Dlt;
        g_filter_buf[SenId_Value].BilatW = g_filter_clamp_buf[SenId_Value].BilatW ;
		g_filter_buf[SenId_Value].Rth_W = g_filter_clamp_buf[SenId_Value].Rth_W;
		g_filter_buf[SenId_Value].Bin = g_filter_clamp_buf[SenId_Value].Bin;
		g_filter_buf[SenId_Value].BitDither = g_filter_clamp_buf[SenId_Value].BitDither;

		//////////////////
		// IFE-Gbalance
		//////////////////
		g_gbal_buf[SenId_Value].pGbalStab = g_gbal_s_tab_buf[SenId_Value];
		g_gbal_buf[SenId_Value].pGbalDtab = g_gbal_d_tab_buf[SenId_Value];

		//////////////////
		// IFE-CGain & OB
		//////////////////
        if (iq_get_cg_mode(SenId_Value) == IQS_IFE_CG){
			if (iq_check_capture(MsgID) == FALSE) {
	            AWB_GetColorGain(SenId_Value, &RGain, &GGain, &BGain);
			} else {
	            awb_get_cap_color_gain(SenId_Value, &RGain, &GGain, &BGain);
			}
            if (RGain > 1023 || BGain > 1023) {
    			g_ife_color_gain_buf[SenId_Value].IfeGainSel = _3_7;
    			g_ife_c_gain_buf[SenId_Value][0] = RGain >> 1;
    			g_ife_c_gain_buf[SenId_Value][1] = GGain >> 1;
    			g_ife_c_gain_buf[SenId_Value][2] = GGain >> 1;
    			g_ife_c_gain_buf[SenId_Value][3] = BGain >> 1;
    			g_ife_c_gain_buf[SenId_Value][4] = 128;
            } else {
                g_ife_color_gain_buf[SenId_Value].IfeGainSel = _2_8;
                g_ife_c_gain_buf[SenId_Value][0] = RGain;
                g_ife_c_gain_buf[SenId_Value][1] = GGain;
                g_ife_c_gain_buf[SenId_Value][2] = GGain;
                g_ife_c_gain_buf[SenId_Value][3] = BGain;
    			g_ife_c_gain_buf[SenId_Value][4] = 256;
            }
        } else {
			g_ife_color_gain_buf[SenId_Value].IfeGainSel = _3_7;
			g_ife_c_gain_buf[SenId_Value][0] = 128;
			g_ife_c_gain_buf[SenId_Value][1] = 128;
			g_ife_c_gain_buf[SenId_Value][2] = 128;
			g_ife_c_gain_buf[SenId_Value][3] = 128;
			g_ife_c_gain_buf[SenId_Value][4] = 128;
        }
        //DBG_DUMP("IFE gain:%d,%d,%d,%d\r\n",g_ife_c_gain_buf[SenId_Value][0],g_ife_c_gain_buf[SenId_Value][1],g_ife_c_gain_buf[SenId_Value][2],g_ife_c_gain_buf[SenId_Value][3],g_ife_c_gain_buf[SenId_Value][4]);
		g_ife_color_gain_buf[SenId_Value].CG_Inv.Inv = FALSE;
		g_ife_color_gain_buf[SenId_Value].CG_Inv.Hinv = FALSE;
		g_ife_color_gain_buf[SenId_Value].CG_Mask.GainMask = (UINT32)0xFFF;
		g_ife_color_gain_buf[SenId_Value].pCGain = g_ife_c_gain_buf[SenId_Value];

		if (iq_get_ob_sub_mode(SenId_Value) == IQS_IFE_OBSUB) {
            g_ife_color_gain_buf[SenId_Value].pCOfs = g_ife_ob_buf[SenId_Value];
		} else {
		    g_ife_ob_buf[SenId_Value][0] = 0;
		    g_ife_ob_buf[SenId_Value][1] = 0;
		    g_ife_ob_buf[SenId_Value][2] = 0;
		    g_ife_ob_buf[SenId_Value][3] = 0;
		    g_ife_ob_buf[SenId_Value][4] = 0;
		    g_ife_color_gain_buf[SenId_Value].pCOfs = g_ife_ob_buf[SenId_Value];
		}
        //DBG_DUMP("IFE ob:%d,%d,%d,%d\r\n",g_ife_ob_buf[SenId_Value][0],g_ife_ob_buf[SenId_Value][1],g_ife_ob_buf[SenId_Value][2],g_ife_ob_buf[SenId_Value][3]);

		///////////////////
		// IFE-Image Setting Adjust
		///////////////////
		subf->pOutl = &g_outl_set_buf[SenId_Value];
		subf->pFilter = &g_filter_buf[SenId_Value];
		subf->pGbal = &g_gbal_buf[SenId_Value];
		subf->CGain = g_ife_color_gain_buf[SenId_Value];
		if (group->ifeUpdate & IFE_VIGPARAM) {
			subf->pVig = &g_vig_buf[SenId_Value];
		}

		///////////////////
		// IFE-RBFill
		///////////////////
		g_rbfill_buf[SenId_Value].puiRBFillLuma =  g_rbfill_luma_tab_buf[SenId_Value];
		g_rbfill_buf[SenId_Value].puiRBFillRatio = g_rbfill_ratio_tab_buf[SenId_Value];
        subf->rbfill_param = g_rbfill_buf[SenId_Value];
        //debug_msg("~B g_rbfill_buf[].puiRBFillLuma = %d, %d, %d, %d, %d, %d\r\n",g_rbfill_buf[SenId_Value].puiRBFillLuma[0],g_rbfill_buf[SenId_Value].puiRBFillLuma[1],g_rbfill_buf[SenId_Value].puiRBFillLuma[2],g_rbfill_buf[SenId_Value].puiRBFillLuma[3],g_rbfill_buf[SenId_Value].puiRBFillLuma[4],g_rbfill_buf[SenId_Value].puiRBFillLuma[5]);
        //debug_msg("~B g_rbfill_buf[].puiRBFillRatio = %d, %d, %d, %d, %d, %d\r\n",g_rbfill_buf[SenId_Value].puiRBFillRatio[0],g_rbfill_buf[SenId_Value].puiRBFillRatio[1],g_rbfill_buf[SenId_Value].puiRBFillRatio[2],g_rbfill_buf[SenId_Value].puiRBFillRatio[3],g_rbfill_buf[SenId_Value].puiRBFillRatio[4],g_rbfill_buf[SenId_Value].puiRBFillRatio[5]);
        //debug_msg("~B g_rbfill_buf[].uiRBRatioMode = %d\r\n",g_rbfill_buf[SenId_Value].uiRBRatioMode);
		break;

	default:
		DBG_ERR("IFE_IQparam() param mode(%d) selection error!\r\n", MsgID);
		return E_SYS;
	}
	return E_OK;
#else
    DBG_DUMP("==========\r\n SIMT IFE_IQparam\r\n==========\r\n");
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);

    subf->ifeFuncEn |=  ChInfo->IFE_FuncEn;

    group->ifeUpdate |= (
                            IFE_SUBFEN      |       ///< update func enable/disable
                            IFE_OUTLIER     |
                            IFE_CGAIN       |       ///< update color gain
                            IFE_CGOFS       |       ///< update offset(bayer stage)
                            IFE_CGEFFECT    |       ///< update color gain output inverse/mask
                            IFE_FILTER      |       ///< update noise reduction filter
                            IFE_VIGPARAM    |       ///< update vignette parameters
                            IFE_GBALPARAM   |       ///< update Gbalance parameters
                            0);

    /////////////////////////////////
    // OUTL & FILTER & GBAL & CGain
    /////////////////////////////////
    subf->pOutl    = &ChInfo->IQS_IFE_Outl;
    subf->pFilter  = &ChInfo->IQS_IFE_Filter;
    subf->pGbal = &ChInfo->IQS_IFE_Gbal;
    subf->CGain = ChInfo->IQS_IFE_CGain;

    //////////////////
    // VIG Gain
    //////////////////
    subf->pVig  = &ChInfo->IQS_IFE_Vig;

    //////////////////
    // RBFill
    //////////////////
    subf->rbfill_param = ChInfo->IQS_IFE_rbfill;
	return E_OK;
#endif //end #ifndef IQS_SIMT_ENABLE
}

static ER DCE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, DCE_SUBFUNC *subf, IPL_HAL_GROUP *group)
{
#ifndef IQS_SIMT_ENABLE
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
	UINT32 ui_image_effect = iq_get_ui_info(Id, IQ_UI_IMAGEEFFECT);
    static UINT32 pre_ui_gdc_en[Total_SENIDNum] = {0};
    UINT32 ui_gdc_en = iq_get_ui_info(Id, IQ_UI_GDCCOMP);
    UINT32 RGain = 256, GGain = 256, BGain = 256;

	switch ((UINT32)MsgID) {
	case IQCB_PRV_DCE_INIT:
	case IQCB_PREIME_D2D_DCE_SUB:
	case IQCB_PREIME_D2D_DCE_PRI:
	case IQS_AUTO_TRIG:
		subf->dceFuncEn |= (
                                FUNC_CFA        |   ///< CFA interpolation function
                                FUNC_CFA_GC     |   ///< G channel correction function
                                FUNC_CFA_RBC    |   ///< RB channel correction function
                                FUNC_CFA_CL     |
                                FUNC_CFA_HF     |
                                0
						   );
		group->dceUpdate |= (
                                DCE_CENTER         |    ///< update distortion center
                                DCE_DIST           |    ///< update H/V distance factor/ratio selection
                                DCE_RADIUS         |    ///< update radius norm term
                                DCE_CAB            |    ///< update color aberration factor
                                DCE_LUT            |    ///< update LUT table address
                                DCE_FOV            |    ///< update FOV
                                DCE_ENH            |    ///< update enhancement
                                DCE_H_FILT         |    ///< update h direction filter enable/disable(UV only)
                                DCE_DISTOR         |    ///< update distortion parameter set
                                DCE_CFAINFO        |    ///< update cfa interpolation
                                DCE_SUBFEN         |    ///< update function switch
                                0
							);

        //*************** CFA ***************//
        if (iq_check_rgbir(SenId_Value) == TRUE) {
            subf->dceFuncEn |= FUNC_CFA_PINKR;
            subf->dceFuncEn |= FUNC_CFA_4X4RB_DIR;
        } else {
            subf->dceFuncEn &= ~FUNC_CFA_PINKR;
            subf->dceFuncEn &= ~FUNC_CFA_4X4RB_DIR;
        }
		if(IPL_CtrlGetInfor(SenId_Value, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR)
		{
			subf->dceFuncEn &= ~FUNC_CFA;
			subf->dceFuncEn &= ~FUNC_CFA_PINKR;
		}else
		{
			subf->dceFuncEn |= FUNC_CFA;
		}
        //*************** GDC ***************//
        if(g_e_ctrl_addr[SenId_Value]->gdc.enable == GDC_ON) {
    		subf->dceFuncEn |= FUNC_DC;         ///< Distortion correction function
        } else {
            if(g_e_ctrl_addr[SenId_Value]->cac.enable == CAC_ON) {
                subf->dceFuncEn |= FUNC_DC;
            } else {
        		subf->dceFuncEn &= ~FUNC_DC;
            }
        }
        if(ui_gdc_en != pre_ui_gdc_en[SenId_Value]) {
            if (ui_gdc_en == IQ_GDCCOMP_ON) {
                g_e_ctrl_addr[SenId_Value]->gdc.enable = GDC_ON;
                subf->dceFuncEn |= FUNC_DC;
                iqt_set_param_update(SenId_Value, ISP_LDC);
                DBG_DUMP("^GGDC UI enable!!!!\r\n");
    		} else {
    		    g_e_ctrl_addr[SenId_Value]->gdc.enable = GDC_OFF;
                g_e_ctrl_addr[SenId_Value]->cac.enable = CAC_OFF;
    		    subf->dceFuncEn &= ~FUNC_DC;
                DBG_DUMP("^GGDC UI disable!!!!\r\n");
    		}
            pre_ui_gdc_en[SenId_Value] = ui_gdc_en;
        }

        //*************** CAC ***************//
        if(g_e_ctrl_addr[SenId_Value]->cac.enable == CAC_ON) {
    		subf->dceFuncEn |= FUNC_CAC;        ///< Chromatic aberration correction function
        } else {
    		subf->dceFuncEn &= ~FUNC_CAC;
        }

		//////////////////
		// DCE-NonISO param update
		//////////////////
        if (iqt_get_param_update(SenId_Value, ISP_LDC) == TRUE){
            DBG_DUMP("^Gupdate GDC table!!!!\r\n");
            g_dist_buf[SenId_Value] = *(g_dist_addr[SenId_Value]);
            g_radious_buf[SenId_Value] = *(g_radious_addr[SenId_Value]);
            g_cfa_buf[SenId_Value] = *(g_cfa_addr[SenId_Value]);
            g_fov_buf[SenId_Value] = *(g_fov_addr[SenId_Value]);
			if (g_e_ctrl_addr[SenId_Value]->gdc.enable == GDC_ON) {
	            IQS_DCE_Intpl(g_ldc_g_lut_addr[SenId_Value], g_ldc_g_lut_buf[SenId_Value], g_e_ctrl_addr[SenId_Value]->gdc.ratio, 0, 200);
			} else {
                memcpy(g_ldc_g_lut_buf[SenId_Value],   IQS_DCE_GLUT_Init,          sizeof(UINT32) * 65);
			}
            memcpy(g_ldc_r_lut_buf[SenId_Value],   g_ldc_r_lut_addr[SenId_Value],   sizeof(INT32) * 65);
            memcpy(g_ldc_b_lut_buf[SenId_Value],   g_ldc_b_lut_addr[SenId_Value],   sizeof(INT32) * 65);
            g_cac_buf[SenId_Value] = *(g_cac_addr[SenId_Value]);
            iqt_reset_param_update(SenId_Value, ISP_LDC);
        }

		//////////////////
		// DCE-CFA
		//////////////////
        if (iq_check_rgbir(SenId_Value) == TRUE) {
            g_cfa_buf[SenId_Value].RawFmt = DCE_FMT_RGBIR_4X4;
        } else {
            g_cfa_buf[SenId_Value].RawFmt = DCE_FMT_BAYER_2X2;
        }

        DBG_IND("SubR=%d,SubG=%d,SubB=%d,SatGain=%d\r\n",g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubR,g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubG,
            g_cfa_buf[SenId_Value].CfaIrSub.uiIrSubB,g_cfa_buf[SenId_Value].CfaIrSub.uiIrSatGain);

        if (iq_get_cg_mode(SenId_Value) == IQS_DCE_CG){
			if (iq_check_capture(MsgID) == FALSE) {
	            AWB_GetColorGain(SenId_Value, &RGain, &GGain, &BGain);
			} else {
	            awb_get_cap_color_gain(SenId_Value, &RGain, &GGain, &BGain);
			}
    		if (RGain > 1023 || BGain > 1023) {
    			g_cfa_buf[SenId_Value].CfaCGain.GainRange_3_7  = 1;
    			g_cfa_buf[SenId_Value].CfaCGain.uiGainR        = RGain >> 1;
    			g_cfa_buf[SenId_Value].CfaCGain.uiGainG        = GGain >> 1;
    			g_cfa_buf[SenId_Value].CfaCGain.uiGainB        = BGain >> 1;
    		} else {
    			g_cfa_buf[SenId_Value].CfaCGain.GainRange_3_7  = 0;
    			g_cfa_buf[SenId_Value].CfaCGain.uiGainR        = RGain;
    			g_cfa_buf[SenId_Value].CfaCGain.uiGainG        = GGain;
    			g_cfa_buf[SenId_Value].CfaCGain.uiGainB        = BGain;
    		}
        } else {
			g_cfa_buf[SenId_Value].CfaCGain.GainRange_3_7  = 1;
			g_cfa_buf[SenId_Value].CfaCGain.uiGainR        = 128;
			g_cfa_buf[SenId_Value].CfaCGain.uiGainG        = 128;
			g_cfa_buf[SenId_Value].CfaCGain.uiGainB        = 128;
        }

		//////////////////
		// DCE-GDC
		//////////////////
        if (subf->dceFuncEn & FUNC_DC) {
            if ((g_e_ctrl_addr[SenId_Value]->gdc.enable == GDC_OFF) && (g_e_ctrl_addr[SenId_Value]->cac.enable == CAC_ON)) {
                memcpy(g_ldc_g_lut_buf[SenId_Value],   IQS_DCE_GLUT_Init,          sizeof(UINT32) * 65);
            }

            if (ui_image_effect == IQ_IMAGEEFFECT_FISHEYE) {
			    memcpy(g_ldc_g_lut_buf[SenId_Value], IE_FISYEYE_LUT, sizeof(UINT32) * 65);
            }
        } else {
			memcpy(g_ldc_g_lut_buf[SenId_Value],   IQS_DCE_GLUT_Init,          sizeof(UINT32) * 65);
        }

		iq_get_dce_param(SenId_Value, MsgID, SenId_Value, &g_ldc_buf[SenId_Value], &subf->Dce_Dist, &subf->Radious);

		subf->pDistortion = &g_ldc_buf[SenId_Value];
		subf->pCFAInterpolation = &g_cfa_buf[SenId_Value];
		break;

	default:
		DBG_ERR("DCE_IQparam() param mode(%d) selection error!\r\n", MsgID);
		return E_SYS;
	}
	return E_OK;
#else
    DBG_DUMP("==========\r\n SIMT DCE_IQparam\r\n==========\r\n");
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);

    subf->dceFuncEn |=  ChInfo->DCE_FuncEn;
    if(ChInfo->IQS_DCE_CFAInter.GC.En) subf->dceFuncEn |= FUNC_CFA_GC;
    if(ChInfo->IQS_DCE_CFAInter.RBC.En) subf->dceFuncEn |= FUNC_CFA_RBC;
    if(ChInfo->IQS_DCE_CFAInter.hfc.bCLCheckEn) subf->dceFuncEn |= FUNC_CFA_CL;
    if(ChInfo->IQS_DCE_CFAInter.hfc.bHFCheckEn) subf->dceFuncEn |= FUNC_CFA_HF;

    group->dceUpdate |= (
                        DCE_CENTER         |    ///< update distortion center
                        DCE_DIST           |    ///< update H/V distance factor/ratio selection
                        DCE_RADIUS         |    ///< update radius norm term
                        DCE_CAB            |    ///< update color aberration factor
                        DCE_LUT            |    ///< update LUT table address
                        DCE_FOV            |    ///< update FOV
                        DCE_ENH            |    ///< update enhancement
                        DCE_H_FILT         |    ///< update h direction filter enable/disable(UV only)
                        DCE_DISTOR         |    ///< update distortion parameter set
                        DCE_CFAINFO        |    ///< update cfa interpolation
                        //DCE_SUBFEN         |    ///< update
                        //DCE_CFAPAT         |    ///< update
                        0
                      );

    subf->pDistortion = &ChInfo->IQS_DCE_Distortion;
    subf->Dce_Dist = ChInfo->IQS_DCE_Dist;
    subf->pCFAInterpolation = &ChInfo->IQS_DCE_CFAInter;
	return E_OK;
#endif //end #ifndef IQS_SIMT_ENABLE
}

static ER IPE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPE_SUBFUNC *subf, IPL_HAL_GROUP *group)
{
#ifndef IQS_SIMT_ENABLE
    UINT32 i = 0;
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
    UINT32 ui_image_effect = iq_get_ui_info(Id, IQ_UI_IMAGEEFFECT);
	UINT32 ui_cctrl_sat = iq_get_ui_info(Id, IQ_UI_CCID);
    UINT32 ui_hue = iq_get_ui_info(SenId_Value, IQ_UI_HUE);
	IQS_COLOR_EFFECT_PARAM ColorEffectTable;
	ER ret = E_OK;

	switch ((UINT32)MsgID) {
	case IQCB_PRV_IPE_INIT:
	case IQCB_PREIME_D2D_IPE_SUB:
	case IQCB_PREIME_D2D_IPE_PRI:
	case IQS_AUTO_TRIG:
		subf->IpeFuncEn |= (
                                IPE_CSTEn           |       ///< Color space transform function enable, RGB->YCC
                                IPE_CCTRLEn         |       ///< Color control function enable
                                IPE_CADJEn          |       ///< Color component adjust function enable
                                IPE_YCTHEn          |       ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJEn
                                IPE_CSTPEn          |       ///< Color space transform protect function enable, RGB->YCC
                                0
						   );

		if (MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_SUB || MsgID == IQCB_PREIME_D2D_IPE_PRI) {
            group->ipeUpdate |= (   // must to update at the first time init
                                    IPE_3DCC_INFO   |   ///< update 3d color correction dram addr
                                    IPE_RGBGAMMA    |   ///< update rgb gamma dram addr
                                    IPE_YCURVE          ///< update y gamma dram addr
    							);
		}

		group->ipeUpdate |= (
                                IPE_SUBFEN       |   ///< update func enable/disable
                                IPE_EDGEEXT_INFO |   ///< update edge extraction
                                IPE_EDGEMAP_INFO |   ///< update edge map
                                IPE_CST_INFO     |   ///< update color space transfor
                                IPE_C_CTRL       |   ///< update color control
                                IPE_C_OFS        |   ///< update Cb/Cr offset
                                IPE_RANDNOISE    |   ///< update YCC random noise
                                IPE_YCCFIX_INFO  |   ///< update YCC fix replace
                                IPE_CSTP         |   ///< update color space transfor protection
                                0
							);

        //*************** Edge **************//
        if(g_e_ctrl_addr[SenId_Value]->edge.enable == EDGE_ON) {
    		subf->IpeFuncEn |= IPE_CADJ_Y_ENHEn;    ///< Color component edge enhance function enable, sub-function of IPE_CADJEn
    		group->ipeUpdate |= IPE_EDGEENH_INFO;   ///< update positive/negative edge enhance/inverse
        } else {
    		subf->IpeFuncEn &= ~IPE_CADJ_Y_ENHEn;
            group->ipeUpdate &= ~IPE_EDGEENH_INFO;
        }
		if (iq_get_ui_info(Id, IQ_UI_IPPEDGE) == IQ_EDGE_OFF) {
    		subf->IpeFuncEn &= ~IPE_CADJ_Y_ENHEn;
            group->ipeUpdate &= ~IPE_EDGEENH_INFO;
		}

        //*************** CCM ***************//
        if(g_e_ctrl_addr[SenId_Value]->ccm.enable == CCM_ON) {
    		subf->IpeFuncEn |= IPE_CCREn;           ///< Color correction function enable
    		group->ipeUpdate |= IPE_CC_INFO;        ///< update color correction
        } else {
    		subf->IpeFuncEn &= ~IPE_CCREn;
            group->ipeUpdate &= ~IPE_CC_INFO;
        }

        //*************** CC3D **************//
        if(g_e_ctrl_addr[SenId_Value]->cc3d.enable == CC3D_ON) {
    		subf->IpeFuncEn |= IPE_3DCCREn;         ///< 3D color correction function enable
    		group->ipeUpdate |= IPE_3DCC_INFO;      ///< update 3d color correction dram addr
        } else {
    		subf->IpeFuncEn &= ~IPE_3DCCREn;
        }
		if (!(MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_PRI || MsgID == IQCB_PREIME_D2D_IPE_SUB || (g_iq_table_update_bit[Id]&IQS_TBL_3DCC))) {
            group->ipeUpdate &= ~IPE_3DCC_INFO;
		}

        //************** CCTRL **************//
        if(g_e_ctrl_addr[SenId_Value]->cctrl.enable == CCTRL_ON) {
    		subf->IpeFuncEn |= (IPE_CADJ_Y_CONEn    |   ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJEn
                                IPE_CADJ_CC_CONEn       ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJEn
                                );
            group->ipeUpdate |= IPE_C_CON;              ///< update color contrast(Y/CC)
        } else {
    		subf->IpeFuncEn &= ~(IPE_CADJ_Y_CONEn|IPE_CADJ_CC_CONEn);
            group->ipeUpdate &= ~IPE_C_CON;
        }

        //*************** HSV **************//
        if(g_e_ctrl_addr[SenId_Value]->hsv.enable == HSV_ON) {
    		subf->IpeFuncEn |= IPE_HADJEn;          ///< Hue adjustment function enable
        } else {
    		subf->IpeFuncEn &= ~IPE_HADJEn;
        }

        //************** GAMMA **************//
        if(g_e_ctrl_addr[SenId_Value]->gamma.enable == GAMMA_ON) {
    		subf->IpeFuncEn |= IPE_RGBGAMMAEn;      ///< RGB Gamma function enable
    		group->ipeUpdate |= IPE_RGBGAMMA;       ///< update rgb gamma dram addr
        } else {
    		subf->IpeFuncEn &= ~IPE_RGBGAMMAEn;
        }
		if (!(MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_PRI || MsgID == IQCB_PREIME_D2D_IPE_SUB || (g_iq_table_update_bit[Id]&IQS_TBL_GAMMA))) {
			group->ipeUpdate &= ~IPE_RGBGAMMA;
		}

        //************* Y_CURVE *************//
        if(g_e_ctrl_addr[SenId_Value]->y_curve.enable == Y_CURVE_ON) {
    		subf->IpeFuncEn |= IPE_YGAMMAEn;        ///< Y Gamma function enable,(curve share with ife chroma curve table)
    		group->ipeUpdate |= IPE_YCURVE;         ///< update y gamma dram addr
        } else {
    		subf->IpeFuncEn &= ~IPE_YGAMMAEn;
        }
		if (!(MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_PRI || MsgID == IQCB_PREIME_D2D_IPE_SUB || (g_iq_table_update_bit[Id]&IQS_TBL_YCURVE))) {
			group->ipeUpdate &= ~IPE_YCURVE;
		}

        //*************** LPF ***************//
        if(g_e_ctrl_addr[SenId_Value]->lpf.enable == LPF_ON) {
    		subf->IpeFuncEn |= IPE_RGBLPEn;         ///< RGB low-pass filter function enable
    		group->ipeUpdate |= IPE_RGBLPF_INFO;    ///< update rgb low pass
        } else {
    		subf->IpeFuncEn &= ~IPE_RGBLPEn;
            group->ipeUpdate &= ~IPE_RGBLPF_INFO;
        }
		if (iq_get_ui_info(Id, IQ_UI_IPPNR) == IQ_NR_OFF) {
			subf->IpeFuncEn &= ~IPE_RGBLPEn;
            group->ipeUpdate &= ~IPE_RGBLPF_INFO;
		}

		if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_ETH_OUT_EN) == ENABLE) {
			group->ipeUpdate |= IPE_ETH_INFO;
		}

		if ((IPL_CtrlGetInfor(SenId_Value, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR) &&
			(g_e_ctrl_addr[SenId_Value]->ccm.enable != CCM_ON)){
			subf->IpeFuncEn &= ~IPE_CSTEn;
			subf->IpeFuncEn &= ~IPE_CSTPEn;
		}

		//////////////////
		// IPE-NonISO param update
		//////////////////
		if (iqt_get_param_update(SenId_Value, ISP_CC) == TRUE){
            g_cc_buf[SenId_Value] = *(g_cc_addr[SenId_Value]);
            memcpy(g_edg_tab_buf[SenId_Value],     g_edg_tab_addr[SenId_Value],    sizeof(UINT8) * 24);
            memcpy(g_dds_tab_buf[SenId_Value],     g_dds_tab_addr[SenId_Value],    sizeof(UINT8) * 8);
            iqt_reset_param_update(SenId_Value, ISP_CC);
		}
        if (iqt_get_param_update(SenId_Value, ISP_Y_CURVE) == TRUE){
            if(iq_get_ycurve_mode(SenId_Value) == IQS_Y_MANUAL) {
                g_y_curve_update[SenId_Value] = TRUE;
                memcpy(g_y_curve_buf[SenId_Value], g_y_curve_addr[SenId_Value], sizeof(UINT32) * 129);
            }
            iqt_reset_param_update(SenId_Value, ISP_Y_CURVE);
        }
        if (iqt_get_param_update(SenId_Value, ISP_GAMMA) == TRUE) {
            g_gamma_update[SenId_Value] = TRUE;
            iqt_reset_param_update(SenId_Value, ISP_GAMMA);
        }
        if (iqt_get_param_update(SenId_Value, ISP_3DCC) == TRUE){
            g_color_3d_buf[SenId_Value].TCC_TblAddr = (UINT32)g_color_3d_lut_addr[SenId_Value];
            g_color_3d_buf[SenId_Value].pCC3dRnd = g_color_3d_addr[SenId_Value];
            iqt_reset_param_update(SenId_Value, ISP_3DCC);
        }
        if (iqt_get_param_update(SenId_Value, ISP_CST) == TRUE) {
        	memcpy(g_cst_coef_buf[SenId_Value],    g_cst_coef_addr[SenId_Value],   sizeof(INT16) * 9);
        	g_cst_buf[SenId_Value] = *(g_cst_addr[SenId_Value]);
            g_ycc_fix_buf[SenId_Value] = *(g_ycc_fix_addr[SenId_Value]);
            g_noise_param_buf[SenId_Value] = *(g_noise_param_addr[SenId_Value]);
            g_gamma_rand_buf[SenId_Value] = *(g_gamma_rand_addr[SenId_Value]);
            iqt_reset_param_update(SenId_Value, ISP_CST);
        }

		//////////////////
		// Dynamic Gamma & CC
		//////////////////
		if (group->ipeUpdate & IPE_RGBGAMMA) {
			UINT32 aLv = 0;
			static UINT32 aLv_old[Total_SENIDNum] = {0};
            static UINT32 pre_ui_image_effect[Total_SENIDNum] = {0};

			if (iq_check_capture(MsgID) == FALSE) {
				aLv = AE_AlgGetUIInfo(SenId_Value, AE_SEL_PRV_LV);
			} else {
				aLv = AE_AlgGetUIInfo(SenId_Value, AE_SEL_CAP_LV);
			}
			if ((aLv != aLv_old[SenId_Value]) && !((aLv > (g_gamma_lv_th_addr[SenId_Value]->DAY)) && (aLv_old[SenId_Value] > (g_gamma_lv_th_addr[SenId_Value]->DAY))) && !((aLv < (g_gamma_lv_th_addr[SenId_Value]->NIGHT)) && (aLv_old[SenId_Value] < (g_gamma_lv_th_addr[SenId_Value]->NIGHT)))) {
				g_gamma_update[SenId_Value] = TRUE;
			}
            if (ui_image_effect != pre_ui_image_effect[SenId_Value]) {
                 g_gamma_update[SenId_Value] = TRUE;
            }
			aLv_old[SenId_Value] = aLv;
            pre_ui_image_effect[SenId_Value] = ui_image_effect;

			if (g_gamma_update[SenId_Value] == TRUE) {
				IQS_Gamma_Intpl(g_gamma_night_addr[SenId_Value], g_gamma_day_addr[SenId_Value], g_gamma_lv_th_addr[SenId_Value]->NIGHT, g_gamma_lv_th_addr[SenId_Value]->DAY, aLv, g_gamma_buf[SenId_Value]);
                //DBG_DUMP("SenId:%d, GammaBuf:%d,%d,%d,%d,%d",SenId_Value,g_gamma_buf[SenId_Value][0],g_gamma_buf[SenId_Value][1],g_gamma_buf[SenId_Value][2],g_gamma_buf[SenId_Value][3],g_gamma_buf[SenId_Value][4]);
				g_gamma_addr_buf[SenId_Value] = (UINT32)IPE_GammaReorgon(SenId_Value, (UINT32 *)g_gamma_buf[SenId_Value], (UINT32 *)g_gamma_buf[SenId_Value], (UINT32 *)g_gamma_buf[SenId_Value]);
			}

            if (ui_image_effect == IQ_IMAGEEFFECT_NEGATIVE_DEFOG) {
                g_gamma_addr_buf[SenId_Value] = (UINT32)IPE_GammaReorgon(SenId_Value, (UINT32 *)IE_Gamma_Negative, (UINT32 *)IE_Gamma_Negative, (UINT32 *)IE_Gamma_Negative);
            }
		}

		if (group->ipeUpdate & IPE_CC_INFO) {
			static AWBStatus AWBStatus;
			AWB_GetStatus(SenId_Value, &AWBStatus);

			//DBG_DUMP("id:%d TH_M:%d,%d,%d\r\n",SenId_Value,(g_color_temperature_th_addr[3]->Middle),(g_color_temperature_th_addr[3]->Middle),(g_color_temperature_th_addr[3]->Middle));
			//DBG_DUMP("id:%d CCM:%x,%x,%x\r\n",SenId_Value,g_color_low_addr[SenId_Value]->IQS_CC[0],g_color_low_addr[SenId_Value]->IQS_CC[0],g_color_low_addr[SenId_Value]->IQS_CC[0]);
			//DBG_DUMP("id:%d hue:%d,%d,%d\r\n",SenId_Value,g_color_low_addr[SenId_Value]->IQS_hueTab[0],g_color_low_addr[SenId_Value]->IQS_hueTab[0],g_color_low_addr[SenId_Value]->IQS_hueTab[0]);
			if ((AWBStatus.ColorTemperature < (g_color_temperature_th_addr[SenId_Value]->Middle)) && (AWBStatus.ColorTemperature != 0)) {
				IQS_CC_Intpl(g_color_low_addr[SenId_Value]->IQS_CC, g_color_middle_addr[SenId_Value]->IQS_CC, g_color_temperature_th_addr[SenId_Value]->Low, g_color_temperature_th_addr[SenId_Value]->Middle, AWBStatus.ColorTemperature, g_cc_matrix_buf[SenId_Value]);
				{
					int i;
					for (i = 0; i < 24; i++) {
						g_sat_tab_blend[SenId_Value][i] = IQS_Intpl(AWBStatus.ColorTemperature, (int)(g_color_low_addr[SenId_Value]->IQS_satTab)[i], (int)(g_color_middle_addr[SenId_Value]->IQS_satTab)[i], g_color_temperature_th_addr[SenId_Value]->Low, g_color_temperature_th_addr[SenId_Value]->Middle);
						g_int_tab_blend[SenId_Value][i] = IQS_Intpl(AWBStatus.ColorTemperature, (int)(g_color_low_addr[SenId_Value]->IQS_intTab)[i], (int)(g_color_middle_addr[SenId_Value]->IQS_intTab)[i], g_color_temperature_th_addr[SenId_Value]->Low, g_color_temperature_th_addr[SenId_Value]->Middle);
					}
				}
				IQS_IntplTblUint8(g_color_low_addr[SenId_Value]->IQS_hueTab, g_color_middle_addr[SenId_Value]->IQS_hueTab, 24, g_hue_tab_blend[SenId_Value], AWBStatus.ColorTemperature, g_color_temperature_th_addr[SenId_Value]->Low, g_color_temperature_th_addr[SenId_Value]->Middle);
			} else if (AWBStatus.ColorTemperature >= (g_color_temperature_th_addr[SenId_Value]->Middle)) {
				IQS_CC_Intpl(g_color_middle_addr[SenId_Value]->IQS_CC, g_color_high_addr[SenId_Value]->IQS_CC, g_color_temperature_th_addr[SenId_Value]->Middle, g_color_temperature_th_addr[SenId_Value]->High, AWBStatus.ColorTemperature, g_cc_matrix_buf[SenId_Value]);
				{
					int i;
					for (i = 0; i < 24; i++) {
						g_sat_tab_blend[SenId_Value][i] = IQS_Intpl(AWBStatus.ColorTemperature, (int)(g_color_middle_addr[SenId_Value]->IQS_satTab)[i], (int)(g_color_high_addr[SenId_Value]->IQS_satTab)[i], g_color_temperature_th_addr[SenId_Value]->Middle, g_color_temperature_th_addr[SenId_Value]->High);
						g_int_tab_blend[SenId_Value][i] = IQS_Intpl(AWBStatus.ColorTemperature, (int)(g_color_middle_addr[SenId_Value]->IQS_intTab)[i], (int)(g_color_high_addr[SenId_Value]->IQS_intTab)[i], g_color_temperature_th_addr[SenId_Value]->Middle, g_color_temperature_th_addr[SenId_Value]->High);

					}
				}
				IQS_IntplTblUint8(g_color_middle_addr[SenId_Value]->IQS_hueTab, g_color_high_addr[SenId_Value]->IQS_hueTab, 24, g_hue_tab_blend[SenId_Value], AWBStatus.ColorTemperature, g_color_temperature_th_addr[SenId_Value]->Middle, g_color_temperature_th_addr[SenId_Value]->High);

			} else {
				memcpy(g_cc_matrix_buf[SenId_Value], g_color_high_addr[SenId_Value]->IQS_CC, sizeof(UINT16) * 9);
				memcpy(g_sat_tab_blend[SenId_Value], g_color_high_addr[SenId_Value]->IQS_satTab, sizeof(INT32) * 24);
				memcpy(g_hue_tab_blend[SenId_Value], g_color_high_addr[SenId_Value]->IQS_hueTab, sizeof(UINT8) * 24);
				memcpy(g_int_tab_blend[SenId_Value], g_color_high_addr[SenId_Value]->IQS_intTab, sizeof(INT32) * 24);

			}
            if ((g_e_ctrl_addr[SenId_Value]->hsv.angle == 0)&&(ui_hue == 0)) {
    			memcpy(g_hue_tab_buf[SenId_Value], g_hue_tab_blend[SenId_Value], sizeof(UINT8) * 24);
            } else {
        		for (i = 0; i < 24; i++) {
        			g_hue_tab_buf[SenId_Value][i] = (UINT32)IQS_IQ_Cal(IQ_CLAMP(g_e_ctrl_addr[SenId_Value]->hsv.angle + ui_hue,0,360), g_hue_tab_blend[SenId_Value][i], 0, 255, IQS_CAL_HUE);
        		}
                //DBG_DUMP("ori HUE table:%d,%d,,,%d\r\n",g_hue_tab_blend[SenId_Value][0],g_hue_tab_blend[SenId_Value][1],g_hue_tab_blend[SenId_Value][23]);
                //DBG_DUMP("Set HUE table:%d,%d,,,%d\r\n",g_hue_tab_buf[SenId_Value][0],g_hue_tab_buf[SenId_Value][1],g_hue_tab_buf[SenId_Value][23]);
            }
            //DBG_DUMP("HUE table:%d,%d,,,%d\r\n",g_hue_tab_buf[SenId_Value][0],g_hue_tab_buf[SenId_Value][1],g_hue_tab_buf[SenId_Value][23]);
			memcpy(g_sat_tab_buf[SenId_Value], g_sat_tab_blend[SenId_Value], sizeof(INT32) * 24);
			memcpy(g_int_tab_buf[SenId_Value], g_int_tab_blend[SenId_Value], sizeof(INT32) * 24);
		}

		//////////////////
		// IPE-YCURVE
		//////////////////
		if (group->ipeUpdate & IPE_YCURVE) {
    		if(( g_y_curve_update[SenId_Value] == TRUE )&&(g_y_curve_flag[SenId_Value] == FALSE ))
    		{
    			g_y_curve_addr_buf[SenId_Value] = (UINT32)IPE_YCurveReorgon(SenId_Value, (UINT32 *)g_y_curve_buf[SenId_Value]);
    		}
		}

		//////////////////
		// Edge Gamma
		//////////////////
		if ((g_gamma_update[SenId_Value] == TRUE)&&(iq_get_ycurve_mode(SenId_Value) != IQS_Y_AUTO)) {
			memcpy(g_edge_gamma_buf[SenId_Value], IPE_xfer2EdgeGamma(SenId_Value, (UINT32 *)g_gamma_buf[SenId_Value]), sizeof(UINT32) * 65);
		}

		//////////////////
		// IPE-Edge Ext
		//////////////////
		g_edge_ext_buf[SenId_Value].GamSel = E_POS_GAMMA;
		if(IPL_CtrlGetInfor(SenId_Value, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR) {
			g_edge_ext_buf[SenId_Value].ChSel = EDGE_G_CH;
		} else {
    		g_edge_ext_buf[SenId_Value].ChSel = EDGE_Y_CH;
		}
		g_edge_ext_buf[SenId_Value].EEXT_TblAddr = (UINT32)g_edge_kernel_buf[SenId_Value];
		g_edge_ext_buf[SenId_Value].EdgeDiv = EdgeDiv_2;
		g_edge_ext_buf[SenId_Value].EDirTabAddr = (UINT32)g_edir_tab_buf[SenId_Value];
		g_edge_ext_buf[SenId_Value].pDirEdgeExt = &g_edir_ext_buf[SenId_Value];
		g_edge_ext_buf[SenId_Value].ScrCalTab = &g_score_tab_buf[SenId_Value];
		g_edge_ext_buf[SenId_Value].ScrCalThTab = &g_score_th_buf[SenId_Value];
		g_edge_ext_buf[SenId_Value].pEWA = &g_ewa_buf[SenId_Value];
		g_edge_ext_buf[SenId_Value].pEWB = &g_ewb_buf[SenId_Value];
		g_edge_ext_buf[SenId_Value].pEWParam = &g_ewparam_buf[SenId_Value];
		g_edge_ext_buf[SenId_Value].ToneMapLutTabAddr = (UINT32)g_edge_gamma_buf[SenId_Value];

		//////////////////
		// IPE-Edge Map
		//////////////////
		g_ed_map_buf[SenId_Value].TAB_TblAddr = (UINT32)g_ed_tab_buf[SenId_Value];
		g_es_map_buf[SenId_Value].TAB_TblAddr = (UINT32)g_es_tab_buf[SenId_Value];
		g_edge_map_buf[SenId_Value].edMap = g_ed_map_buf[SenId_Value];
		g_edge_map_buf[SenId_Value].esMap = g_es_map_buf[SenId_Value];
		g_edge_map_buf[SenId_Value].EMapOut = _EEXT;

		//////////////////
		// IPE-RGBLPF
		//////////////////
		g_lpf_buf[SenId_Value].G = g_lpf_buf[SenId_Value].R;
		g_lpf_buf[SenId_Value].B = g_lpf_buf[SenId_Value].R;

		//////////////////
		// IPE-Color Correction
		//////////////////
		g_cc_buf[SenId_Value].CCcoef_TblAddr = (UINT32)g_cc_matrix_buf[SenId_Value];
		g_cc_buf[SenId_Value].Fstab_TblAddr = (UINT32)g_fs_tab_buf[SenId_Value];
		g_cc_buf[SenId_Value].Fdtab_TblAddr = (UINT32)g_fd_tab_buf[SenId_Value];

		//////////////////
		// IPE-color space transfor
		//////////////////
		g_cst_buf[SenId_Value].CSTCoefTabAddr = (UINT32)g_cst_coef_buf[SenId_Value];

		///////////////////
		// IPE-Image Effect
		///////////////////
        if     ( ui_image_effect == IQ_IMAGEEFFECT_BW )         ColorEffectTable = ColorEffectTable_BW;
        else if( ui_image_effect == IQ_IMAGEEFFECT_SEPIA )      ColorEffectTable = ColorEffectTable_SEPIA;
        else if( ui_image_effect == IQ_IMAGEEFFECT_VIVID )      ColorEffectTable = ColorEffectTable_VIVID;
        else if( ui_image_effect == IQ_IMAGEEFFECT_SKETCH )     ColorEffectTable = ColorEffectTable_PencilSketch;
        else if( ui_image_effect == IQ_IMAGEEFFECT_COLORPENCIL) ColorEffectTable = ColorEffectTable_ColorPencil;
        else if( ui_image_effect == IQ_IMAGEEFFECT_COOLGREEN)   ColorEffectTable = ColorEffectTable_CoolGreen;
        else if( ui_image_effect == IQ_IMAGEEFFECT_WARMYELLOW)  ColorEffectTable = ColorEffectTable_WarmYellow;
        else                                                    ColorEffectTable = ColorEffectTable_Normal;

		///////////////////
		// IPE-Color Control
		///////////////////
		g_cctrl_buf[SenId_Value].SatOfs = ColorEffectTable.SatOfs;
		g_int_ofs_buf[SenId_Value] = (INT16)IQS_IQ_Cal(g_e_ctrl_addr[SenId_Value]->cctrl.bri_ratio, ColorEffectTable.IntOfs, -128, 127, IQS_CAL_INTPL);
		g_cctrl_buf[SenId_Value].IntOfs = (INT16)IQ_CLAMP(ColorEffectTable.IntOfs + g_int_ofs_buf[SenId_Value], -128, 127);
		g_cctrl_buf[SenId_Value].HueRotateEn = ColorEffectTable.HueRotateEn;
		g_cctrl_buf[SenId_Value].CctrlSel = g_cctrl_info_buf[SenId_Value].CctrlSel;
        if(g_e_ctrl_addr[SenId_Value]->cctrl.enable == CCTRL_ON) {
		    g_cctrl_buf[SenId_Value].uiVdetDiv = g_cctrl_info_buf[SenId_Value].uiVdetDiv;
        } else {
            g_cctrl_buf[SenId_Value].uiVdetDiv = 0;
        }
		g_cctrl_buf[SenId_Value].HueTab_TblAddr = (UINT32)g_hue_tab_buf[SenId_Value];
		g_cctrl_buf[SenId_Value].SatTab_TblAddr = (UINT32)g_sat_tab_buf[SenId_Value];
		g_cctrl_buf[SenId_Value].IntTab_TblAddr = (UINT32)g_int_tab_buf[SenId_Value];
		g_cctrl_buf[SenId_Value].EdgTab_TblAddr = (UINT32)g_edg_tab_buf[SenId_Value];
		g_cctrl_buf[SenId_Value].DDSTab_TblAddr = (UINT32)g_dds_tab_buf[SenId_Value];

		///////////////////
		// IPE-Y/CC Contrast
		///////////////////
		if ((iq_get_ui_info(Id, IQ_UI_IRCUT) == IQ_IRCUT_AUTO && AE_GetIRCutEnable(SenId_Value) == FALSE) || iq_get_ui_info(Id, IQ_UI_IRCUT) == IQ_IRCUT_NIGHT) {
			g_ycc_contrast_buf[SenId_Value].UVCon = 0;
		} else if ((ui_image_effect == IQ_IMAGEEFFECT_BW) || (ui_image_effect == IQ_IMAGEEFFECT_SEPIA) || (ui_image_effect == IQ_IMAGEEFFECT_SKETCH)) {
			g_ycc_contrast_buf[SenId_Value].UVCon = (INT8)ColorEffectTable.CCon;
		} else {
			g_ycc_contrast_buf[SenId_Value].UVCon = (UINT8)IQ_CLAMP((INT16)ColorEffectTable.CCon/*=128*/ + g_saturation_buf[SenId_Value]/*-128~127*/ + g_cont2sat_adj_buf[SenId_Value]/*-18~18*/, 0, 255);
		}
		g_ycc_contrast_buf[SenId_Value].YCon = (UINT8)IQ_CLAMP((INT16)ColorEffectTable.YCon/*=128*/ + g_contrast_buf[SenId_Value]/*-128~127*/, 0, 255);
        g_ycc_contrast_buf[SenId_Value].puiCConTab = g_ccon_tab_buf[SenId_Value];
		g_cctrl_buf[SenId_Value].IntOfs = (INT16)IQ_CLAMP(g_cctrl_buf[SenId_Value].IntOfs+(128-g_ycc_contrast_buf[SenId_Value].YCon), -128, 127);

		///////////////////
		// IPE-Cb/Cr Color Offset
		///////////////////
		g_cc_ofs_buf[SenId_Value].Cbofs = ColorEffectTable.CbOfs;
		g_cc_ofs_buf[SenId_Value].Crofs = ColorEffectTable.CrOfs;

		///////////////////
		// IPE-Y/CC Fix
		///////////////////
		switch (ui_image_effect) {
        case IQ_IMAGEEFFECT_COLORPENCIL:
            memcpy(&g_ycc_fix_buf[SenId_Value], &IE_ColorPencil_Param, sizeof(IPE_YCCFIX));
            g_edge_enh_buf[SenId_Value].EnhP = 0;
            g_edge_enh_buf[SenId_Value].EnhN = ((g_edge_enh_buf[SenId_Value].EnhN*200)/100);
            g_edge_enh_buf[SenId_Value].EnhN = (g_edge_enh_buf[SenId_Value].EnhN > 1023) ? 1023 : g_edge_enh_buf[SenId_Value].EnhN;
            break;

        case IQ_IMAGEEFFECT_SKETCH:
            memcpy(&g_ycc_fix_buf[SenId_Value], &IE_PencilSketch_Param, sizeof(IPE_YCCFIX));
            g_edge_enh_buf[SenId_Value].EnhP = 0;
            g_edge_enh_buf[SenId_Value].EnhN = ((g_edge_enh_buf[SenId_Value].EnhN*200)/100);
            g_edge_enh_buf[SenId_Value].EnhN = (g_edge_enh_buf[SenId_Value].EnhN > 1023) ? 1023 : g_edge_enh_buf[SenId_Value].EnhN;
            break;

		case IQ_IMAGEEFFECT_ROCK:
			memcpy(&g_ycc_fix_buf[SenId_Value], &IPE_RockEffect_YccFix, sizeof(IPE_YCCFIX));
			//IQS_IPE_CSTP.CSTPratio = 15;
			break;

		case IQ_IMAGEEFFECT_CCID:
			g_cctrl_buf[SenId_Value].SatTab_TblAddr = (UINT32)IPE_SelectiveCC_SatTab[ui_cctrl_sat];
		// fall through

		default:
    		memcpy(&g_ycc_fix_buf[SenId_Value], g_ycc_fix_addr[SenId_Value], sizeof(IPE_YCCFIX));
			break;
		}

		///////////////////
		// IPE-Image Setting Adjust
		///////////////////
		subf->pedgeExt = &g_edge_ext_buf[SenId_Value];
		subf->pedgeEnh = &g_edge_enh_buf[SenId_Value];
		subf->pedgeMap = &g_edge_map_buf[SenId_Value];
		subf->prgblpf = &g_lpf_buf[SenId_Value];
		subf->pCC = &g_cc_buf[SenId_Value];
		subf->pCST = &g_cst_buf[SenId_Value];
		subf->pCctrl = &g_cctrl_buf[SenId_Value];
		subf->pYCCcon = &g_ycc_contrast_buf[SenId_Value];
		subf->pCbCrofs = &g_cc_ofs_buf[SenId_Value];
		subf->pRandNoise = &g_noise_param_buf[SenId_Value];
		subf->pYccFix = &g_ycc_fix_buf[SenId_Value];
		subf->pEthparam = &g_ipe_eth;
		if (group->ipeUpdate & IPE_3DCC_INFO) {
			subf->p3d_cc = &g_color_3d_buf[SenId_Value];
		}
		if (group->ipeUpdate & IPE_RGBGAMMA) {
			subf->GammaAddr = g_gamma_addr_buf[SenId_Value];
		}
		if (group->ipeUpdate & IPE_YCURVE) {
			subf->YGammaAddr = g_y_curve_addr_buf[SenId_Value];
		}
		if (group->ipeUpdate & IPE_GAMRAND) {
			subf->pGamRand = &g_gamma_rand_buf[SenId_Value];
		}

		g_gamma_update[SenId_Value] = FALSE;
		g_y_curve_update[SenId_Value] = FALSE;
		break;

	default:
		DBG_ERR("IPE_IQparam() param mode(%d) selection error!\r\n", MsgID);
		ret = E_SYS;
		break;
	}
	return ret;
#else
    DBG_DUMP("==========\r\n SIMT IPE_IQparam\r\n==========\r\n");
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);

    subf->IpeFuncEn |= ChInfo->IPE_FuncEn;

    group->ipeUpdate |= (
                        IPE_SUBFEN     |
                        IPE_RGBGAMMA   |
                        IPE_YCURVE     |
                        IPE_EDGEEXT_INFO |   ///< update edge extraction
                        IPE_EDGEENH_INFO |   ///< update positive/negative edge enhance/inverse
                        IPE_EDGEMAP_INFO |   ///< update edge map
                        IPE_RGBLPF_INFO  |   ///< update rgb low pass
                        IPE_CC_INFO      |   ///< update color correction
                        IPE_CST_INFO     |   ///< update color space transfor
                        IPE_C_CTRL       |   ///< update color control
                        IPE_C_CON       |   ///< update color contrast(Y/CC)
                        IPE_C_OFS       |   ///< update Cb/Cr offset
                        IPE_RANDNOISE   |   ///< update YCC random noise
                        IPE_YCCFIX_INFO |   ///< update YCC fix replace
                        IPE_CSTP        |   ///< update color space transfor protection
                        IPE_3DCC_INFO   |   ///< update 3d color correction dram addr
                        IPE_GAMRAND     |   ///< update gamma random noise
                        0
                      );

    //////////////////
    // Edge Ext
    //////////////////
    ChInfo->IQS_IPE_EdgeExt.EEXT_TblAddr = (UINT32)ChInfo->IQS_IPE_EdgeKernel;
    ChInfo->IQS_IPE_EdgeExt.EDirTabAddr = (UINT32)ChInfo->IQS_IPE_EDirTab;
    ChInfo->IQS_IPE_EdgeExt.pDirEdgeExt = &ChInfo->IQS_IPE_DirEdgeExt;
    ChInfo->IQS_IPE_EdgeExt.ScrCalTab = &ChInfo->IQS_IPE_DirScoreTab;
    ChInfo->IQS_IPE_EdgeExt.ScrCalThTab = &ChInfo->IQS_IPE_DirScoreTh;
    ChInfo->IQS_IPE_EdgeExt.pEWA = &ChInfo->IQS_IPE_EWA;
    ChInfo->IQS_IPE_EdgeExt.pEWB = &ChInfo->IQS_IPE_EWB;
    ChInfo->IQS_IPE_EdgeExt.pEWParam = &ChInfo->IQS_IPE_EWParam;
    ChInfo->IQS_IPE_EdgeExt.ToneMapLutTabAddr = (UINT32)ChInfo->IQS_IPE_EdgeGammaLut;
    subf->pedgeExt          = &ChInfo->IQS_IPE_EdgeExt;

    //////////////////
    // Edge Map
    //////////////////
    subf->pedgeMap   = &ChInfo->IQS_IPE_EdgeMap;

    //////////////////
    // Edge Enh
    //////////////////
    subf->pedgeEnh   = &ChInfo->IQS_IPE_EdgeEnh;


    //////////////////
    // RGB lpf
    //////////////////
    subf->prgblpf    = &ChInfo->IQS_IPE_RGBLpf;

    //////////////////
    // Color Matrix
    //////////////////
    ChInfo->IQS_IPE_ColorCorrection.Fstab_TblAddr = (UINT32)ChInfo->IQS_IPE_FStab;
    ChInfo->IQS_IPE_ColorCorrection.Fdtab_TblAddr = (UINT32)ChInfo->IQS_IPE_FDtab;
    subf->pCC = &ChInfo->IQS_IPE_ColorCorrection;

    //////////////////
    // Color Control
    //////////////////
    ChInfo->IQS_IPE_Cctrl.HueTab_TblAddr = (UINT32)ChInfo->IQS_IPE_hueTab;
    ChInfo->IQS_IPE_Cctrl.SatTab_TblAddr = (UINT32)ChInfo->IQS_IPE_satTab;
    ChInfo->IQS_IPE_Cctrl.IntTab_TblAddr = (UINT32)ChInfo->IQS_IPE_intTab;
    ChInfo->IQS_IPE_Cctrl.EdgTab_TblAddr = (UINT32)ChInfo->IQS_IPE_edgTab;
    ChInfo->IQS_IPE_Cctrl.DDSTab_TblAddr = (UINT32)ChInfo->IQS_IPE_ddsTab;
    subf->pCctrl = &ChInfo->IQS_IPE_Cctrl;

    /////////////////
    //Image Effect
    /////////////////
    subf->pCbCrofs = &ChInfo->IQS_IPE_CCOfs;
    subf->pYCCcon = &ChInfo->IQS_IPE_YCCcon;

    subf->pRandNoise = &ChInfo->IQS_IPE_NoiseParam;

    ////////////////////////////
    // color space transfor
    ////////////////////////////
    subf->pCST = &ChInfo->IQS_IPE_CSTParam;

    ////////////////////////////
    // YCC fix effect
    ////////////////////////////
    subf->pYccFix = &ChInfo->IQS_IPE_YCCFix;

    //////////////////
    // GAMMA & YCurve
    //////////////////
    subf->GammaAddr = (UINT32)IPE_GammaReorgon(Id, (UINT32 *)ChInfo->IQS_IPE_GammaLut, (UINT32 *)ChInfo->IQS_IPE_GammaLut, (UINT32 *)ChInfo->IQS_IPE_GammaLut);
    subf->YGammaAddr = (UINT32)IPE_YCurveReorgon(Id, (UINT32 *)ChInfo->IQS_IPE_YCurveLut);
    subf->pGamRand = &ChInfo->IQS_IPE_GammaRand;

    //////////////////
    // 3D Color Control
    //////////////////
    subf->p3d_cc = &ChInfo->IQS_IPE_3DColorCtrl;
	return E_OK;
#endif //end #ifndef IQS_SIMT_ENABLE
}

static ER IFE2_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IFE2_SUBFUNC *subf, IPL_HAL_GROUP *group)
{
#ifndef IQS_SIMT_ENABLE
	UINT32 SenId_Value = iq_get_sensor_idx(Id);

	switch ((UINT32)MsgID) {
	case IQCB_PRV_IFE2_INIT:
	case IQCB_D2D_IFE2_SUB:
	case IQCB_D2D_IFE2_PRI:
	case IQS_AUTO_TRIG:
		group->ife2Update |= (
								 IFE2_REFCENTERPARA |    //< update reference center para.
								 IFE2_FILTERPARAM   |    //< update filter para.
								 IFE2_GRAYSTAL      |    //< update filter para.
								 0
							 );
		subf->ife2FuncEn |= (
								YFtrEn             |    ///< enable y filter
								0
							);

		//////////////////
		// IFE2-Reference Center
		//////////////////
		g_ref_center_y_buf[SenId_Value].uiRThAddr = (UINT32)g_ref_center_y_th_buf[SenId_Value];
		g_ref_center_y_buf[SenId_Value].uiRWtAddr = (UINT32)g_ref_center_y_wt_buf[SenId_Value];
		g_ref_center_buf[SenId_Value].RefCentY = g_ref_center_y_buf[SenId_Value];
		g_ref_center_uv_buf[SenId_Value].uiRThAddr = (UINT32)g_ref_center_uv_th_buf[SenId_Value];
		g_ref_center_uv_buf[SenId_Value].uiRWtAddr = (UINT32)g_ref_center_uv_wt_buf[SenId_Value];
		g_ref_center_buf[SenId_Value].RefCentUV = g_ref_center_uv_buf[SenId_Value];
		subf->pRefcent = &g_ref_center_buf[SenId_Value];

		//////////////////
		// IFE2-Filter
		//////////////////
		g_ife2filter_buf[SenId_Value].Filter.FiltSetY.uiFtrThAddr = (UINT32)g_ife2filter_y_th_buf[SenId_Value];
		g_ife2filter_buf[SenId_Value].Filter.FiltSetY.uiFtrWtAddr = (UINT32)g_ife2filter_y_wt_buf[SenId_Value];
		g_ife2filter_buf[SenId_Value].Filter.FiltSetU.uiFtrThAddr = (UINT32)g_ife2filter_uv_th_buf[SenId_Value];
		g_ife2filter_buf[SenId_Value].Filter.FiltSetV.uiFtrThAddr = (UINT32)g_ife2filter_uv_th_buf[SenId_Value];
		g_ife2filter_buf[SenId_Value].Filter.FiltSetU.uiFtrWtAddr = (UINT32)g_ife2filter_uv_wt_buf[SenId_Value];
		g_ife2filter_buf[SenId_Value].Filter.FiltSetV.uiFtrWtAddr = (UINT32)g_ife2filter_uv_wt_buf[SenId_Value];
		subf->pFilter = &g_ife2filter_buf[SenId_Value].Filter;

		//////////////////
		// IFE2-Gray Statistical
		//////////////////
		subf->GrayStatal = g_gray_statical_buf[SenId_Value];
		break;

	default:
		DBG_ERR("IFE2_IQparam() param mode(%d) selection error!\r\n", MsgID);
		break;
	}
	return E_OK;
#else
    DBG_DUMP("==========\r\n SIMT IFE2_IQparam\r\n==========\r\n");
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);

    group->ife2Update |= (  IFE2_REFCENTERPARA | //< update reference center para.
                            IFE2_FILTERPARAM   | //< update filter para.
                            IFE2_GRAYSTAL      | //< update filter para.
                            0
                         );

    subf->ife2FuncEn |= ChInfo->IFE2_FuncEn;

    subf->GrayStatal = ChInfo->IQS_IFE2_GrayStatical;
    subf->pRefcent = &ChInfo->IQS_IFE2_RefCenter;

    subf->pFilter = &ChInfo->IQS_IFE2_Filter.Filter;
	return E_OK;
#endif //end #ifndef IQS_SIMT_ENABLE
}

static ER IME_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IME_SUBFUNC *subf, IPL_HAL_GROUP *group)
{
#ifndef IQS_SIMT_ENABLE
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
    BOOL check_capture;

	switch ((UINT32)MsgID) {
	case IQCB_PRV_IME_INIT:
	case IQCB_PREIME_D2D_IME_SUB:
	case IQCB_PREIME_D2D_IME_PRI:
	case IQS_AUTO_TRIG:
		subf->imeFuncEn |= (
							   ChromaAdaptEn   |   ///< enable function: chroma adaption (LCA)
							   ChromaDBSuppres |   ///< enable function: dark and bright region chroma suppression
							   TMNREn          |   ///< enable function: TMNR(include chroma, staout)
							   0
						   );
		group->imeUpdate |= (
								IME_SUBFEN      |   ///< update function enable/disable
								IME_CHROMAA     |   ///< update chroma adaption
								IME_CHROMAA_LUM |   ///< update chroma lumaination adaption
								IME_CHROMAA_CLR |   ///< update chroma color adaption
								IME_CHROMAA_CTR_UV |   ///< update chroma adjust6ment center for UV channel(data from IFE2)
								IME_CHROMADB_SUP   |  ///< update chroma suppression.
                                IME_TMNR_IQPARAM   |   ///< update tmnr IQ parameters
								0
							);

		if (iq_check_capture(MsgID) == TRUE) {
			check_capture = TRUE;
	    } else {
	        check_capture = FALSE;
	    }

		if (iq_check_ife2_lca(Id, check_capture) == DISABLE) {
			subf->imeFuncEn &= (~ChromaAdaptEn);
		}

		if (IPL_CtrlGetInfor(Id, IPLCTRL_IFE2_LCA_EN) == DISABLE) {
			subf->imeFuncEn &= (~ChromaAdaptEn);
		}

		if (iq_get_ui_info(Id, IQ_UI_IPPNR) == IQ_NR_OFF) {
			subf->imeFuncEn &= ~(ChromaDBSuppres | ChromaAdjLum | ChromaAdjLchrom);
		}

        //*************** TMNR ***************//
        if((g_e_ctrl_addr[SenId_Value]->tmnr.enable == TMNR_ON) && (check_capture == FALSE) ){
    		subf->imeFuncEn |= TMNREn;
    		group->imeUpdate |= IME_TMNR_IQPARAM;
        } else {
    		subf->imeFuncEn &= ~TMNREn;
            group->imeUpdate &= ~IME_TMNR_IQPARAM;
        }

		//////////////////
		// IME-NonISO param update
		//////////////////
        if (iqt_get_param_update(SenId_Value, ISP_TMNR_AUTO) == TRUE){
            g_tmnr_auto_info_buf[SenId_Value] = *g_tmnr_auto_info_addr[SenId_Value];
            iqt_reset_param_update(SenId_Value, ISP_TMNR_AUTO);
        }

		//////////////////
		// IME-Chroma Adaption
		//////////////////

        //************** LCA_C **************//
        if(g_e_ctrl_addr[SenId_Value]->lca_c.enable == LCA_C_OFF) {
    		g_chroma_adapt_color_buf[SenId_Value].LcaCLREn = DISABLE;
        }

        //************** LCA_Y **************//
        if(g_e_ctrl_addr[SenId_Value]->lca_y.enable == LCA_Y_OFF) {
    		g_chroma_adapt_lum_buf[SenId_Value].LcaLumEn = DISABLE;
        }

        //*************** LCA ***************//
        if(g_e_ctrl_addr[SenId_Value]->lca.enable == LCA_OFF) {
    		g_chroma_adapt_buf[SenId_Value].LcaRefWt.LcaOutUV = 0;
        }

		subf->ChromaAdap.pChromaAdaption = &g_chroma_adapt_buf[SenId_Value];
		subf->ChromaAdap.pChromaLumAdaption = &g_chroma_adapt_lum_buf[SenId_Value];
		subf->ChromaAdap.pChromaColorAdaption = &g_chroma_adapt_color_buf[SenId_Value];

		//////////////////
		// IME-Chroma Suppression
		//////////////////
		g_chroma_supp_buf[SenId_Value].puiWeightY = g_chroma_supp_w_y_buf[SenId_Value];
		g_chroma_supp_buf[SenId_Value].puiWeightC = g_chroma_supp_w_c_buf[SenId_Value];
		subf->pChromaSuppres = &g_chroma_supp_buf[SenId_Value];

        //////////////////
        // IME-TMNR
        //////////////////
        subf->pTmnrInfo = &g_tmnr_info_buf[SenId_Value];
        subf->tmnr_momap_sel = *(g_tmnr_momap_sel_addr[SenId_Value]);

        #if 0 //Disable tmnr auto mode
        if (g_tmnr_auto_info_buf[SenId_Value].enable == ENABLE) {
            INT32 x_blk_num, y_blk_num, sta_addr;
            INT32 i;
            IME_HAL_TMNR_NOISE_MODEL_INFO *noiseSAD_model_tmp;
            IPC_QINFO ipc_qinfo_tmp;
            UINT32 select_noise_model = TMNR_NOISE_MODEL_ALL;
            static UINT32 fps_count = 0, iso_idx = 0, pre_iso_idx = 0;
            static BOOL all_calc = TRUE;
            // Perf:
            #if 0
            static UINT32 spend_time_y = 0, spend_time_cb = 0, spend_time_cr = 0;
            Perf_Mark();
            #endif

            // 1. Set auto info. from ISP tool
            //debug_msg("~G auto info %d %d, %d, %d, %d, %d \r\n",g_tmnr_auto_info_buf[SenId_Value].enable,g_tmnr_auto_info_buf[SenId_Value].coefa_Max,g_tmnr_auto_info_buf[SenId_Value].blkMeanY_minTh,g_tmnr_auto_info_buf[SenId_Value].blkMeanY_maxTh,g_tmnr_auto_info_buf[SenId_Value].blkEdgVarY_maxTh,g_tmnr_auto_info_buf[SenId_Value].LearnRate);
            iq_tmnr_set_auto_info(&g_tmnr_auto_info_buf[SenId_Value]);

            // 2. Get statistics data from IPL
            ipc_qinfo_tmp = IPL_Ctrl_Query(SenId_Value, ICF_IME_STAINFO_RDY);
            if (ipc_qinfo_tmp.ime_rdy_stainfo.sta_addr == 0x0) {
                debug_msg("~G get addr = 0x0 \r\n");
                break;
            }

            x_blk_num = ipc_qinfo_tmp.ime_rdy_stainfo.blk_info.blkinfo_h_number[0]+ipc_qinfo_tmp.ime_rdy_stainfo.blk_info.blkinfo_h_number[1];
            y_blk_num = ipc_qinfo_tmp.ime_rdy_stainfo.blk_info.blkinfo_v_number;
            sta_addr = ipc_qinfo_tmp.ime_rdy_stainfo.sta_addr;

            // 3. Parsing statistics data
            if (iq_tmnr_parse_stats_data(sta_addr, x_blk_num, y_blk_num) == FALSE) {
                debug_msg("~G get value = 0 \r\n");
            }

            //debug_msg("~B x_blk_num = %d, y_blk_num = %d, %d, %d, %d  \r\n",  x_blk_num, y_blk_num, ipc_qinfo_tmp.ime_rdy_stainfo.blk_info.NeBlkSampleStep,ipc_qinfo_tmp.ime_rdy_stainfo.blk_info.blkinfo_h_offset[0],ipc_qinfo_tmp.ime_rdy_stainfo.blk_info.blkinfo_v_offset);
            //debug_msg("~B mean = 0x%3X, sad = 0x%3X, edge var = 0x%3X  \r\n", pBlk_Mean[4399],pBlk_SAD_Y[4399], pBlk_EdgVar[4399]);
            //debug_msg("~B parse = 0x%3X, 0x%3X, 0x%3X, 0x%3X, 0x%2X  \r\n", pBlk_SAD_Cb[0], pBlk_SAD_Cr[0], pBlk_EdgVar[0], pBlk_SAD_Y[0], pBlk_Mean[0]);
            //debug_msg("~B addr = 0x%X, data = 0x%X, 0x%X, parse = 0x%X, 0x%X, 0x%X, 0x%X, 0x%X  \r\n", ipc_qinfo_tmp.ime_rdy_stainfo.sta_addr,data_h_byte,data_l_byte, Blk_SAD_Cr, Blk_SAD_Cb, Blk_EdgVar, Blk_SAD_Y, Blk_Mean);

            // 4. Calculate noise model
            fps_count++;
            if (fps_count == 6) {
                fps_count = 0;
                select_noise_model = TMNR_NOISE_MODEL_NONE;
            } else if (fps_count == 1) {
                select_noise_model = TMNR_NOISE_MODEL_Y;
            } else if (fps_count == 3) {
                select_noise_model = TMNR_NOISE_MODEL_CB;
            } else if (fps_count == 5) {
                select_noise_model = TMNR_NOISE_MODEL_CR;
            } else {
                select_noise_model = TMNR_NOISE_MODEL_NONE;
            }

            iso_idx = iq_get_iso_idx(SenId_Value,MsgID);
            if (iso_idx-pre_iso_idx > 1) {
                all_calc = TRUE;
            }
            pre_iso_idx = iso_idx;

            if (all_calc == TRUE) {
                //debug_msg("~G calclate all \r\n");
                all_calc = FALSE;
                select_noise_model = TMNR_NOISE_MODEL_ALL;
            }

            iq_tmnr_set_noise_estimation_sad(x_blk_num, y_blk_num, select_noise_model);

            // 5. Update noise model
            noiseSAD_model_tmp = iq_tmnr_get_noise_model();

            for (i=0; i<8; i++) {
                subf->pTmnrInfo->NoiseModel.TmnrY[i].uiYBase =  noiseSAD_model_tmp->TmnrY[i].uiYBase;
                subf->pTmnrInfo->NoiseModel.TmnrY[i].uiSTD =    noiseSAD_model_tmp->TmnrY[i].uiSTD;
                subf->pTmnrInfo->NoiseModel.TmnrY[i].uiCoeffA = noiseSAD_model_tmp->TmnrY[i].uiCoeffA;
                subf->pTmnrInfo->NoiseModel.TmnrY[i].uiCoeffB = noiseSAD_model_tmp->TmnrY[i].uiCoeffB;
                subf->pTmnrInfo->NoiseModel.TmnrU[i].uiMean =   noiseSAD_model_tmp->TmnrU[i].uiMean;
                subf->pTmnrInfo->NoiseModel.TmnrU[i].uiSTD =    noiseSAD_model_tmp->TmnrU[i].uiSTD;
                subf->pTmnrInfo->NoiseModel.TmnrV[i].uiMean =   noiseSAD_model_tmp->TmnrV[i].uiMean;
                subf->pTmnrInfo->NoiseModel.TmnrV[i].uiSTD =    noiseSAD_model_tmp->TmnrV[i].uiSTD;
            }

            // Perf:
            #if 0
            switch (select_noise_model) {
                case TMNR_NOISE_MODEL_Y:
                    spend_time_y = Perf_GetCurrent();
                    break;
                case TMNR_NOISE_MODEL_CB:
                    spend_time_cb = Perf_GetCurrent();
                    break;
                case TMNR_NOISE_MODEL_CR:
                    spend_time_cr = Perf_GetCurrent();
                    break;
                case TMNR_NOISE_MODEL_ALL:
                default :
                    spend_time_y = spend_time_cb = spend_time_cr = Perf_GetCurrent();
                    break;
            }
            #endif

            //if (1) to enable display noise model
            if (0) {
                static UINT32  fps = 0;
                if (fps == 0) {
                    fps = 60;
                    debug_msg("\r\n");
                    debug_msg("~B idx    =   0    1    2    3    4    5    6    7 \r\n");
                    debug_msg("~G Y base = %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d \r\n",subf->pTmnrInfo->NoiseModel.TmnrY[0].uiYBase,subf->pTmnrInfo->NoiseModel.TmnrY[1].uiYBase,subf->pTmnrInfo->NoiseModel.TmnrY[2].uiYBase,subf->pTmnrInfo->NoiseModel.TmnrY[3].uiYBase,subf->pTmnrInfo->NoiseModel.TmnrY[4].uiYBase,subf->pTmnrInfo->NoiseModel.TmnrY[5].uiYBase,subf->pTmnrInfo->NoiseModel.TmnrY[6].uiYBase,subf->pTmnrInfo->NoiseModel.TmnrY[7].uiYBase);
                    debug_msg("~G Y std  = %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d \r\n",subf->pTmnrInfo->NoiseModel.TmnrY[0].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrY[1].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrY[2].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrY[3].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrY[4].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrY[5].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrY[6].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrY[7].uiSTD);
                    debug_msg("~G Y coeA = %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d \r\n",subf->pTmnrInfo->NoiseModel.TmnrY[0].uiCoeffA,subf->pTmnrInfo->NoiseModel.TmnrY[1].uiCoeffA,subf->pTmnrInfo->NoiseModel.TmnrY[2].uiCoeffA,subf->pTmnrInfo->NoiseModel.TmnrY[3].uiCoeffA,subf->pTmnrInfo->NoiseModel.TmnrY[4].uiCoeffA,subf->pTmnrInfo->NoiseModel.TmnrY[5].uiCoeffA,subf->pTmnrInfo->NoiseModel.TmnrY[6].uiCoeffA,subf->pTmnrInfo->NoiseModel.TmnrY[7].uiCoeffA);
                    debug_msg("~G Y coeB = %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d \r\n",subf->pTmnrInfo->NoiseModel.TmnrY[0].uiCoeffB,subf->pTmnrInfo->NoiseModel.TmnrY[1].uiCoeffB,subf->pTmnrInfo->NoiseModel.TmnrY[2].uiCoeffB,subf->pTmnrInfo->NoiseModel.TmnrY[3].uiCoeffB,subf->pTmnrInfo->NoiseModel.TmnrY[4].uiCoeffB,subf->pTmnrInfo->NoiseModel.TmnrY[5].uiCoeffB,subf->pTmnrInfo->NoiseModel.TmnrY[6].uiCoeffB,subf->pTmnrInfo->NoiseModel.TmnrY[7].uiCoeffB);
                    debug_msg("^G U mean = %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d \r\n",subf->pTmnrInfo->NoiseModel.TmnrU[0].uiMean,subf->pTmnrInfo->NoiseModel.TmnrU[1].uiMean,subf->pTmnrInfo->NoiseModel.TmnrU[2].uiMean,subf->pTmnrInfo->NoiseModel.TmnrU[3].uiMean,subf->pTmnrInfo->NoiseModel.TmnrU[4].uiMean,subf->pTmnrInfo->NoiseModel.TmnrU[5].uiMean,subf->pTmnrInfo->NoiseModel.TmnrU[6].uiMean,subf->pTmnrInfo->NoiseModel.TmnrU[7].uiMean);
                    debug_msg("^G U std  = %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d \r\n",subf->pTmnrInfo->NoiseModel.TmnrU[0].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrU[1].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrU[2].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrU[3].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrU[4].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrU[5].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrU[6].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrU[7].uiSTD);
                    debug_msg("~G V mean = %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d \r\n",subf->pTmnrInfo->NoiseModel.TmnrV[0].uiMean,subf->pTmnrInfo->NoiseModel.TmnrV[1].uiMean,subf->pTmnrInfo->NoiseModel.TmnrV[2].uiMean,subf->pTmnrInfo->NoiseModel.TmnrV[3].uiMean,subf->pTmnrInfo->NoiseModel.TmnrV[4].uiMean,subf->pTmnrInfo->NoiseModel.TmnrV[5].uiMean,subf->pTmnrInfo->NoiseModel.TmnrV[6].uiMean,subf->pTmnrInfo->NoiseModel.TmnrV[7].uiMean);
                    debug_msg("~G V std  = %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d \r\n",subf->pTmnrInfo->NoiseModel.TmnrV[0].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrV[1].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrV[2].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrV[3].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrV[4].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrV[5].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrV[6].uiSTD,subf->pTmnrInfo->NoiseModel.TmnrV[7].uiSTD);
                    debug_msg("\r\n");
                    // Perf:
                    #if 0
                    debug_msg("~G Time spend: Y %d, Cb %d, Cr %d  \r\n",spend_time_y,spend_time_cb,spend_time_cr);
                    #endif
                }else {
                    fps--;
                }
            }
        }
        #endif
		break;

	default:
		DBG_ERR("IME_IQparam() param mode(%d) selection error!\r\n", MsgID);
		return E_SYS;
	}

	// LCA off when image flip index changes
	if ((UINT32)MsgID == IQS_AUTO_TRIG) {
		static UINT32 cnt[Total_SENIDNum] = {0}, ImageFlipIdx[Total_SENIDNum] = {0};
		if (ImageFlipIdx[SenId_Value] != IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_IMAGEFLIP_IMM)) {
			subf->imeFuncEn &= ~(ChromaAdaptEn | ChromaDBSuppres | ChromaAdjLum | ChromaAdjLchrom);
			cnt[SenId_Value]++;
			if (cnt[SenId_Value] == 5) {
				ImageFlipIdx[SenId_Value] = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_IMAGEFLIP_IMM);
				cnt[SenId_Value] = 0;
			}
		}
	}
	return E_OK;
#else
    DBG_DUMP("==========\r\n IME_IQparam\r\n==========\r\n");
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);

    subf->imeFuncEn |= ChInfo->IME_FuncEn;
    group->imeUpdate |= (   IME_SUBFEN      |   ///< update function enable/disable
                            IME_CHROMAA     |   ///< update chroma adaption
                            IME_CHROMAA_LUM |   ///< update chroma lumaination adaption
                            IME_CHROMAA_CLR |   ///< update chroma color adaption
                            IME_CHROMAA_CTR_UV |   ///< update chroma adjust6ment center for UV channel(data from IFE2)
                            IME_CHROMADB_SUP|   ///< update chroma suppression.
                            0);
    if ( iq_check_ife2_lca(Id, iq_check_capture(MsgID)) == ENABLE )
         subf->imeFuncEn |= ChromaAdaptEn;
    if ( iq_check_ife2_lca(Id, iq_check_capture(MsgID)) == DISABLE )
         subf->imeFuncEn &= (~ChromaAdaptEn);

    subf->ChromaAdap.pChromaAdaption      = &ChInfo->IQS_IME_ChromaAdapt;
    subf->ChromaAdap.pChromaLumAdaption   = &ChInfo->IQS_IME_ChromaAdapt_Lum;
    subf->ChromaAdap.pChromaColorAdaption = &ChInfo->IQS_IME_ChromaAdapt_Color;
    subf->pChromaSuppres = &ChInfo->IQS_IME_ChromaSupp;

	return E_OK;
#endif //end #ifndef IQS_SIMT_ENABLE
}

static ER iq_set_wdr_flow(UINT32 sen_id)
{
    static BOOL y_curve_reset[Total_SENIDNum] = {FALSE};
    if ((IPL_CtrlGetInfor(sen_id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) ||
        ((IPL_CtrlGetInfor(sen_id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) && (IPL_AlgGetUIInfo((IPL_PROC_ID)sen_id, IPL_SEL_SHDR) == SEL_SHDR_WDR)))
    {
        if (iq_get_ycurve_mode(sen_id) == IQS_Y_MANUAL) {
            if(y_curve_reset[sen_id] == TRUE) {
                memcpy(g_y_curve_buf[sen_id], g_y_curve_addr[sen_id], sizeof(UINT32) * 129);
                g_y_curve_update[sen_id] = TRUE;
                IQS_SetUpdateBit(sen_id, (IQS_ENG_IPE | IQS_TBL_YCURVE));
                y_curve_reset[sen_id] = FALSE;
            }
            return E_OK;
        }

        SR_WDR_PARAM wdr_param = {0};
        SR_WDR_PATH_CURVE_PARAM	get_path_param; ///< path parameters
        SR_WDR_BLEND_PARAM get_blend_param; ///< blend parameters
        SR_WDR_LCE_PARAM get_lce_param; ///< local contrast enhancement parameters
        SR_WDR_GTC_PARAM get_gtc_param; ///< global tone curve parameters

        wdr_param.pGetPathParam = &get_path_param;
        wdr_param.pGetBlendParam = &get_blend_param;
        wdr_param.pGetLceParam = &get_lce_param;
        wdr_param.pGetGtcParam = &get_gtc_param;
        SceneRender_WDR_GetParam((SR_WDR_PROCID)sen_id, &wdr_param);

        //IPE Ycurve
        g_y_curve_flag[sen_id] = TRUE;
        if(wdr_param.pGetYCurveParam != NULL) {
            memcpy(g_y_curve_buf[sen_id], wdr_param.pGetYCurveParam, sizeof(UINT32)*129);
        } else {
            DBG_ERR("WDR get y_curve addr error!!\r\n");
        }
        g_y_curve_flag[sen_id] = FALSE;

        //IPE EdgeGamma
        if(wdr_param.pGetEdgeGamParam != NULL) {
            memcpy(g_edge_gamma_buf[sen_id], wdr_param.pGetEdgeGamParam, sizeof(UINT32)*65);
        } else {
            DBG_ERR("WDR get edge_gamma addr error!!\r\n");
        }

        g_y_curve_update[sen_id] = TRUE;
        y_curve_reset[sen_id] = TRUE;

        IQS_SetUpdateBit(sen_id, (IQS_ENG_RHE | IQS_ENG_IPE | IQS_TBL_YCURVE));
    }
    return E_OK;
}

static ER iq_set_hdr_flow(UINT32 sen_id)
{
    if ((IPL_CtrlGetInfor(sen_id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) &&
        ((IPL_AlgGetUIInfo((IPL_PROC_ID)sen_id, IPL_SEL_SHDR) == SEL_SHDR_OFF) || (IPL_AlgGetUIInfo((IPL_PROC_ID)sen_id, IPL_SEL_SHDR) == SEL_SHDR_ON)))
    {
        if (iq_get_ycurve_mode(sen_id) == IQS_Y_MANUAL) {
            g_y_curve_update[sen_id] = TRUE;
            IQS_SetUpdateBit(sen_id, (IQS_ENG_IPE | IQS_TBL_YCURVE));
            return E_OK;
        }

        SHDR_PARAM hdr_param = {0};
        SensorHDR_GetParam(&hdr_param);

        //IPE Ycurve
        g_y_curve_flag[sen_id] = TRUE;
        if(hdr_param.pGetYCurveParam != NULL) {
            memcpy(g_y_curve_buf[sen_id], hdr_param.pGetYCurveParam, sizeof(UINT32)*129);
        } else {
            DBG_ERR("HDR get y_curve addr error!!\r\n");
        }
        g_y_curve_flag[sen_id] = FALSE;

        g_y_curve_update[sen_id] = TRUE;

        IQS_SetUpdateBit(sen_id, (IQS_ENG_RHE | IQS_ENG_IPE | IQS_TBL_YCURVE));
    }
    return E_OK;
}

///////////////////////////////////////////////
// FlowSetting Interface
///////////////////////////////////////////////
ER iq_set_flow(UINT32 Id, IQS_FLOW_MSG MsgID)
{
	IPL_HAL_PARASET HalId;
	IPL_MODE mode;
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sensor_status = {0};
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
	UINT32 IPL_id_Value = IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID);
	ER rt = E_OK;
	sen_info.mode = NULL;
	UINT32 rotate = 0;
    static UINT32 rotate_pri = 0;
    rt = sensor_get_info((SENSOR_ID)SenId_Value, SEN_GET_USER_OPTION, &rotate);
	if (rt != E_OK ) {
		DBG_ERR("NULL sensor rotate info\r\n");
	}

    mode = IPL_CtrlGetInfor(IPL_id_Value, IPLCTRL_CUR_MODE);
    if (mode  == IPL_MODE_CAP)
        HalId = IPL_HAL_PAR_CAP;
    else
        HalId = IPL_HAL_PAR_PRV;

	switch ((UINT32)MsgID) {
	case IQS_AUTO_TRIG:
		sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)IPL_id_Value, IPL_SEL_PRVSENMODE);
		rt = sensor_get_info((SENSOR_ID)SenId_Value, SEN_GET_STATUS, (void *)&sensor_status);
		sen_info = sensor_status.sensor_info;
		if (rt != E_OK || sen_info.mode == NULL) {
			DBG_ERR("NULL Info.Mode\r\n");
			return E_SYS;
		}

		if (!iq_dbg_mode) {
			iq_intpl_iso(SenId_Value, MsgID);
		}
        if (iqt_get_param_update(SenId_Value, ISP_ECS) == TRUE){
    		IQS_SetUpdateBit(SenId_Value, (IQS_ENG_SIE | IQS_TBL_ECS));
        }
        if (iqt_get_param_update(SenId_Value, ISP_LDC) == TRUE){
    		IQS_SetUpdateBit(SenId_Value, IQS_ENG_DCE);
        }
        if (iqt_get_param_update(SenId_Value, ISP_Y_CURVE) == TRUE){
    		IQS_SetUpdateBit(SenId_Value, IQS_TBL_YCURVE);
        }
        if (iqt_get_param_update(SenId_Value, ISP_GAMMA) == TRUE){
    		IQS_SetUpdateBit(SenId_Value, IQS_TBL_GAMMA);
        }
        if (iqt_get_param_update(SenId_Value, ISP_3DCC) == TRUE){
    		IQS_SetUpdateBit(SenId_Value, IQS_TBL_3DCC);
        }
        if (rotate != rotate_pri){
    		IQS_SetUpdateBit(SenId_Value, (IQS_ENG_SIE | IQS_TBL_ECS));
            rotate_pri = rotate;
        }
        if (sen_info.mode->mode_type == SENSOR_MODE_CCIR || sen_info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
            IQS_SetUpdateBit(SenId_Value, IQS_ENG_IME);
        }
		g_iq_table_update_bit[SenId_Value] = IQS_GetUpdateBit(SenId_Value) & IQS_TBL_MASK;
		g_iq_engine_update_bit[SenId_Value] = IQS_GetUpdateBit(SenId_Value) & IQS_ENG_MASK;
		IQS_ClearUpdateBit(SenId_Value);
		if (g_iq_engine_update_bit[SenId_Value] & IQS_ENG_SIE) {
    		if (sen_info.mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
    			IQS_SIEx_IQparam(IPL_ID_1, MsgID, HalId, SIE_IQparam);
    			IQS_SIEx_IQparam(IPL_ID_2, MsgID, HalId, SIE_IQparam);
    		} else {
    			IQS_SIEx_IQparam(IPL_id_Value, MsgID, HalId, SIE_IQparam);
    		}
		}
		if (g_iq_engine_update_bit[SenId_Value] & IQS_ENG_DCE) {
			IQS_DCE_IQparam(IPL_id_Value, MsgID, HalId, DCE_IQparam);
		}
		if (g_iq_engine_update_bit[SenId_Value] & IQS_ENG_IFE) {
			IQS_IFE_IQparam(IPL_id_Value, MsgID, HalId, IFE_IQparam);
		}
		if (g_iq_engine_update_bit[SenId_Value] & IQS_ENG_IPE) {
			IQS_IPE_IQparam(IPL_id_Value, MsgID, HalId, IPE_IQparam);
		}
		if (g_iq_engine_update_bit[SenId_Value] & IQS_ENG_IFE2) {
			IQS_IFE2_IQparam(IPL_id_Value, MsgID, HalId, IFE2_IQparam);
		}
		if (g_iq_engine_update_bit[SenId_Value] & IQS_ENG_IME) {
			IQS_IME_IQparam(IPL_id_Value, MsgID, HalId, IME_IQparam);
		}
		if (g_iq_engine_update_bit[SenId_Value] & IQS_ENG_RHE) {
			IQS_RHE_IQparam(IPL_id_Value, MsgID, HalId, RHE_IQparam);
		}
        //#NT#2018/01/15#Photon Lin -begin
        //#NT#Mark unused code
		//IQS_SetIsrUpdateBit(SenId_Value, g_iq_engine_update_bit[SenId_Value]);
        //#NT#2018/01/15#Photon Lin -end
		g_iq_engine_update_bit[SenId_Value] = 0;
		g_iq_table_update_bit[SenId_Value] = 0;
		break;

	case IQS_AWB_END:
        IQS_SetUpdateBit(SenId_Value, (IQS_ENG_SIE | IQS_ENG_RHE | IQS_ENG_IFE | IQS_ENG_DCE | IQS_ENG_IPE));
		break;

	case IQS_AE_END:
		IQS_SetUpdateBit(SenId_Value, (IQS_ENG_SIE | IQS_ENG_RHE | IQS_ENG_IFE | IQS_ENG_IPE | IQS_ENG_IFE2 | IQS_ENG_IME | IQS_TBL_GAMMA | IQS_TBL_VIG));
		break;

	case IQS_OZOOM_END:
		IQS_SetUpdateBit(SenId_Value, (IQS_ENG_SIE | IQS_ENG_IFE | IQS_ENG_DCE | IQS_TBL_ECS | IQS_TBL_3DCC));
		break;

	case IQS_DZOOM_END:
		//Dzoom crop between SIE & IFE
		//no ratio change, IFE & DCE no need to change parameter
		//do not impact ECS
		//IQS_SetUpdateBit(Id, (IQS_ENG_IFE | IQS_ENG_DCE | IQS_TBL_ECS));
		break;

    case IQS_WDR_END:
        if(iq_get_ycurve_mode(SenId_Value) == IQS_Y_OFF) {
            g_e_ctrl_addr[SenId_Value]->y_curve.enable = Y_CURVE_OFF;
        } else {
            g_e_ctrl_addr[SenId_Value]->y_curve.enable = Y_CURVE_ON;
        }
        iq_set_wdr_flow(SenId_Value);
        break;

    case IQS_SHDR_END:
        if(iq_get_ycurve_mode(SenId_Value) == IQS_Y_OFF) {
            g_e_ctrl_addr[SenId_Value]->y_curve.enable = Y_CURVE_OFF;
        } else {
            g_e_ctrl_addr[SenId_Value]->y_curve.enable = Y_CURVE_ON;
        }
        iq_set_hdr_flow(SenId_Value);
        break;

	case IQS_IMAGE_EFFECT:
		IQS_SetUpdateBit(SenId_Value, (IQS_ENG_DCE | IQS_ENG_IPE));
		break;

	default:
		DBG_ERR("^R-E- MsgID(%x)\r\n", MsgID);
		break;
	}
	return E_OK;
}

///////////////////////////////////////////////
// CBSetting Interface
///////////////////////////////////////////////
ER iq_set_flow_cb(UINT32 Id, IQS_FLOW_MSG MsgID, ENG_SUBFUNC *Info, IPL_HAL_GROUP *group)
{
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
	UINT32 IPL_id_Value = IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID);
	switch ((UINT32)MsgID) {
	//preview
	case IQCB_PRV_SIE_INIT:
	case IQCB_PREIME_D2D_SIE_PRI:
	case IQCB_PREIME_D2D_SIE_SUB:
		iq_intpl_iso(SenId_Value, MsgID);
		if (Info == NULL || Info->pSie_sub == NULL || group == NULL) {
			return E_PAR;
		}
		SIE_IQparam(IPL_id_Value, MsgID, Info->pSie_sub, group);
		break;

	case IQCB_PRV_IPE_INIT:
		g_gamma_update[SenId_Value] = TRUE;
		g_y_curve_update[SenId_Value] = TRUE;
	// fall through
	case IQCB_PREIME_D2D_IPE_PRI:
	case IQCB_PREIME_D2D_IPE_SUB:
		if (Info == NULL || Info->pIpe_sub == NULL || group == NULL) {
			return E_PAR;
		}
		IPE_IQparam(IPL_id_Value, MsgID, Info->pIpe_sub, group);
		break;

	case IQCB_PRV_DCE_INIT:
	case IQCB_PREIME_D2D_DCE_SUB:
	case IQCB_PREIME_D2D_DCE_PRI:
		if (Info == NULL || Info->pDce_sub == NULL || group == NULL) {
			return E_PAR;
		}
		DCE_IQparam(IPL_id_Value, MsgID, Info->pDce_sub, group);
		break;

	case IQCB_PRV_IFE_INIT:
	case IQCB_PREIME_D2D_IFE_PRI:
	case IQCB_PREIME_D2D_IFE_SUB:
		if (Info == NULL || Info->pIfe_sub == NULL || group == NULL) {
			return E_PAR;
		}
		IFE_IQparam(IPL_id_Value, MsgID, Info->pIfe_sub, group);
		break;

	case IQCB_PRV_IME_INIT:
		if (Info == NULL || Info->pIme_sub == NULL || group == NULL) {
			return E_PAR;
		}
		IME_IQparam(IPL_id_Value, MsgID, Info->pIme_sub, group);
		break;

	case IQCB_PREIME_D2D_IME_SUB:
	case IQCB_PREIME_D2D_IME_PRI:
		if (Info == NULL || Info->pIme_sub == NULL || group == NULL) {
			return E_PAR;
		}
		IME_IQparam(IPL_id_Value, MsgID, Info->pIme_sub, group);
		break;

	case IQCB_PRV_IFE2_INIT:
	case IQCB_D2D_IFE2_SUB:
	case IQCB_D2D_IFE2_PRI:
		if (Info == NULL || Info->pIfe2_sub == NULL || group == NULL) {
			return E_PAR;
		}
		IFE2_IQparam(IPL_id_Value, MsgID, Info->pIfe2_sub, group);
		break;

	case IQCB_PRV_RDE_INIT:
		break;

	case IQCB_PRV_RHE_INIT:
	case IQCB_PREIME_D2D_RHE_SUB:
	case IQCB_PREIME_D2D_RHE_PRI:
		if (Info == NULL || Info->pRhe_sub == NULL || group == NULL) {
			return E_PAR;
		}
		RHE_IQparam(IPL_id_Value, MsgID, Info->pRhe_sub, group);
		break;

    case IQS_WDR_END:
        if(iq_get_ycurve_mode(SenId_Value) == IQS_Y_OFF) {
            g_e_ctrl_addr[SenId_Value]->y_curve.enable = Y_CURVE_OFF;
        } else {
            g_e_ctrl_addr[SenId_Value]->y_curve.enable = Y_CURVE_ON;
        }
        iq_set_wdr_flow(SenId_Value);
        break;

    case IQS_SHDR_END:
        if(iq_get_ycurve_mode(SenId_Value) == IQS_Y_OFF) {
            g_e_ctrl_addr[SenId_Value]->y_curve.enable = Y_CURVE_OFF;
        } else {
            g_e_ctrl_addr[SenId_Value]->y_curve.enable = Y_CURVE_ON;
        }
        iq_set_hdr_flow(SenId_Value);
        break;

	default:
		DBG_ERR("^R-E- MsgID(%x)\r\n", MsgID);
		break;
	}
	return E_OK;
}

void iq_init(UINT32 Id)
{
    //DBG_MSG("========== IQ INIT id=%d==========\r\n",Id);
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
	UINT32 IPL_id_Value = IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID);
    SENSOR_MODE cur_sensor_mode = SENSOR_MODE_UNKNOWN;
	UINT32 mode_idx;


	if (!pqs_iq_check(SenId_Value)) {
		IQS_HANDLE handler = {0};
		if (pqs_iq_init(SenId_Value, &handler)) {
		    handler._iqs_param_init(SenId_Value);
		}
	}

	cur_sensor_mode = IPL_CtrlGetInfor(SenId_Value, IPLCTRL_SEN_MODE);
	if (cur_sensor_mode == SENSOR_MODE_UNKNOWN) {
		DBG_WRN("Sensor mode(%d), get from IPL\r\n",cur_sensor_mode);
		cur_sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)IPL_id_Value, IPL_SEL_PRVSENMODE);
	}

	mode_idx = iq_tbl[SenId_Value][cur_sensor_mode];
	if((mode_idx == IQS_PARAM_TABLE_UNKONWN)||(mode_idx >= ISP_IQ_MAX_TABLE_NUM)) {
		DBG_WRN("IQS TABLE UNKNOWN(%d), use table 1\r\n",mode_idx);
		mode_idx = IQS_PARAM_TABLE_1;
	}

    IQS_Ctrl_register_flow_cb(iq_set_flow);

    isp_iq_init(SenId_Value,mode_idx);

    iq_all_param_init(SenId_Value,mode_idx);

	g_iq_engine_update_bit[SenId_Value] = 0;
	g_iq_table_update_bit[SenId_Value] = 0;

	//***********
	//**  SIE  **
	//***********
	memcpy(g_ecs_tab_buf[SenId_Value], g_ecs_tab_addr[SenId_Value], sizeof(UINT32) * 4225);
	g_ecs_buf[SenId_Value] = *(g_ecs_addr[SenId_Value]);
    pre_ir_sub_rg[SenId_Value] = ir_sub_rg_lut[0]<<4;
    pre_ir_sub_b[SenId_Value] = ir_sub_b_lut[0]<<4;
    pre_ir_sat_gain[SenId_Value] = 256<<4;

	//***********
	//**  RHE  **
	//***********
	memcpy(g_rhe_ob_buf[SenId_Value], g_ob_addr[SenId_Value], sizeof(UINT32) * 5);

	//***********
	//**  IFE  **
	//***********
 	g_vig_center_buf[SenId_Value] = *(g_vig_center_addr[SenId_Value]);
	g_vig_set_buf[SenId_Value] = *(g_vig_addr[SenId_Value]);

	//***********
	//**  DCE  **
	//***********
	UINT32 ui_gdc_en = iq_get_ui_info(Id, IQ_UI_GDCCOMP);
	g_e_ctrl_addr[SenId_Value]->gdc.enable = ui_gdc_en;
	//g_e_ctrl_addr[SenId_Value]->cac.enable = ui_gdc_en;

	g_dist_buf[SenId_Value] = *(g_dist_addr[SenId_Value]);
	g_radious_buf[SenId_Value] = *(g_radious_addr[SenId_Value]);
	g_cfa_buf[SenId_Value] = *(g_cfa_addr[SenId_Value]);
	g_fov_buf[SenId_Value] = *(g_fov_addr[SenId_Value]);
    IQS_DCE_Intpl(g_ldc_g_lut_addr[SenId_Value], g_ldc_g_lut_buf[SenId_Value], g_e_ctrl_addr[SenId_Value]->gdc.ratio, 0, 200);
    memcpy(g_ldc_r_lut_buf[SenId_Value],   g_ldc_r_lut_addr[SenId_Value],   sizeof(INT32) * 65);
    memcpy(g_ldc_b_lut_buf[SenId_Value],   g_ldc_b_lut_addr[SenId_Value],   sizeof(INT32) * 65);
    g_cac_buf[SenId_Value] = *(g_cac_addr[SenId_Value]);

	//***********
	//**  IPE  **
	//***********
	//**  IPE-CC
	memcpy(g_cc_matrix_buf[SenId_Value], &(g_color_high_addr[SenId_Value]->IQS_CC), sizeof(UINT16) * 9);
	g_cc_buf[SenId_Value] = *(g_cc_addr[SenId_Value]);

    //**  IPE-Gamma
    memcpy(g_gamma_buf[SenId_Value], g_gamma_day_addr[SenId_Value], sizeof(UINT32) * 129);
    g_gamma_addr_buf[SenId_Value] = (UINT32)IPE_GammaReorgon(SenId_Value, (UINT32 *)g_gamma_buf[SenId_Value], (UINT32 *)g_gamma_buf[SenId_Value], (UINT32 *)g_gamma_buf[SenId_Value]);

    //**  IPE-Y curve
    memcpy(g_y_curve_buf[SenId_Value], g_y_curve_addr[SenId_Value], sizeof(UINT32) * 129);
    g_y_curve_addr_buf[SenId_Value] = (UINT32)IPE_YCurveReorgon(SenId_Value, (UINT32 *)g_y_curve_buf[SenId_Value]);

    //**  IPE-EdgeGamma
    memcpy(g_edge_gamma_buf[SenId_Value], IPE_xfer2EdgeGamma(SenId_Value, (UINT32 *)g_gamma_buf[SenId_Value]), sizeof(UINT32) * 65);

	//**  IPE-3DCC
	g_color_3d_buf[SenId_Value].TCC_TblAddr = (UINT32)g_color_3d_lut_addr[SenId_Value];
	g_color_3d_buf[SenId_Value].pCC3dRnd = g_color_3d_addr[SenId_Value];

	//**  IPE-CCtrl
	memcpy(g_edg_tab_buf[SenId_Value],     g_edg_tab_addr[SenId_Value],    sizeof(UINT8) * 24);
	memcpy(g_dds_tab_buf[SenId_Value],     g_dds_tab_addr[SenId_Value],    sizeof(UINT8) * 8);
    g_awb2sat_ratio_buf[SenId_Value] = 100;

	//**  IPE-CST
	memcpy(g_cst_coef_buf[SenId_Value],    g_cst_coef_addr[SenId_Value],   sizeof(INT16) * 9);
	g_cst_buf[SenId_Value] = *(g_cst_addr[SenId_Value]);

	//**  IPE-YCCFIX
	g_ycc_fix_buf[SenId_Value] = *(g_ycc_fix_addr[SenId_Value]);

	//**  IPE-NoiseParam
	g_noise_param_buf[SenId_Value] = *(g_noise_param_addr[SenId_Value]);

	//**  IPE-GammaRand
	g_gamma_rand_buf[SenId_Value] = *(g_gamma_rand_addr[SenId_Value]);

	//***********
	//**  IME  **
	//***********
	g_tmnr_auto_info_buf[SenId_Value] = *g_tmnr_auto_info_addr[SenId_Value];

	iq_dbg_address.sf = (UINT32)&g_spatial_buf[0];
	iq_dbg_address.gbal = (UINT32)&g_gbal_buf[0];
	iq_dbg_address.filter_addr = (UINT32)&g_filter_buf[0];
	iq_dbg_address.edge_enh_addr = (UINT32)&g_edge_enh_buf[0];
	iq_dbg_address.ed_map = (UINT32)&g_ed_map_buf[0];
	iq_dbg_address.score_th = (UINT32)&g_score_th_buf[0];
	iq_dbg_address.ewp = (UINT32)&g_ewparam_buf[0];
	iq_dbg_address.lca = (UINT32)&g_chroma_adapt_buf[0];
	iq_dbg_address.la = (UINT32)&g_chroma_adapt_lum_buf[0];
	iq_dbg_address.IFE2_ref_param = (UINT32)&g_ref_center_buf[0];
	iq_dbg_address.IFE2_filter_param = (UINT32)&g_ife2filter_buf[0].Filter;
	set_iq_dbg_addr(&iq_dbg_address, &iq_dbg_mode);
    //#NT#2018/04/09#Photon Lin -begin
    //#NT#Mark unused code
 	//IQS_Ctrl_cb1(iq_all_param_init);
    //#NT#2018/04/09#Photon Lin -end
}

static void iq_all_param_init(UINT32 id, UINT32 mode_idx)
{
	//DBG_DUMP("=========================================\r\n");
	//DBG_DUMP("============iq_all_param_init============\r\n");
	//DBG_DUMP("=========================================\r\n");
	UINT32 *iq_param_addr[IQ_PARAM_MAX];
    iq_get_param_addr(id, mode_idx, iq_param_addr);

    g_iso_index_addr[id] = (sISP_ISOINDEX *)iq_param_addr[IQ_ISOIDX];
	g_iso_map_tab_addr[id] = (ISOMAP *)iq_param_addr[IQ_ISOIDX_MAP];

	g_e_ctrl_addr[id] = (ENGINE_CTRL *)iq_param_addr[IQ_ENGINE_CTRL];

	//***********
	//**  SIE  **
	//***********
	g_ecs_tab_addr[id] = (UINT32 *)iq_param_addr[IQ_ECS_TAB];
	g_ecs_addr[id] = (SIE_EmbeddedCS *)iq_param_addr[IQ_ECS];

	//***********
	//**  RHE  **
	//***********
	g_ob_addr[id] = (UINT32 *)iq_param_addr[IQ_OBSUB];

	g_wdr_strength_addr[id] = (UINT8 *)iq_param_addr[IQ_WDR];
    g_wdr_lce_offset_addr[id] = (IQS_WDR_LCE_OFFSET *)iq_param_addr[IQ_WDR_LCE_OFFSET];
    g_lce_strength_addr[id] = (IQS_LCE *)iq_param_addr[IQ_LCE];
    g_gce_strength_addr[id] = (SR_WDR_GCE_PARAM *)iq_param_addr[IQ_GCE];

    g_hdr_addr[id] = (SHDR_IQ_PARA *) iq_param_addr[IQ_HDR];

    g_defog_addr[id] = (S_DEFOG_LIB_INIT_PARA *) iq_param_addr[IQ_DEFOG_INIT_PARAM];
	g_defog_wet_addr[id] = (UINT32 *) iq_param_addr[IQ_DEFOG_WET];

	//***********
	//**  IFE  **
	//***********
	g_outl_bri_addr[id] = (UINT32 *)iq_param_addr[IQ_OUTL_BRI];
	g_outl_dark_addr[id] = (UINT32 *)iq_param_addr[IQ_OUTL_DARK];
	g_outl_cnt_addr[id] = (UINT32 *)iq_param_addr[IQ_OUTL_CNT];
	g_outl_set_addr[id] = (IFE_OUTL *)iq_param_addr[IQ_OUTL];

	g_spatial_w_addr[id] = (UINT32 *)iq_param_addr[IQ_SPATIAL_W];
	g_spatial_addr[id] = (Spatial_Filter *)iq_param_addr[IQ_SPATAL];
	g_range_a_thr_addr[id] = (UINT32 *)iq_param_addr[IQ_RANGEA_THR];
	g_range_a_lut_addr[id] = (UINT32 *)iq_param_addr[IQ_RANGEA_LUT];
	g_range_b_thr_addr[id] = (UINT32 *)iq_param_addr[IQ_RANGEB_THR];
	g_range_b_lut_addr[id] = (UINT32 *)iq_param_addr[IQ_RANGEB_LUT];
	g_range_b_addr[id] = (RangeCFGB *)iq_param_addr[IQ_RANGEB];

	g_filter_clamp_addr[id] = (RANGE_FILT_CLAMP_PARAM *)iq_param_addr[IQ_CLAMP];

	g_vig_center_addr[id] = (IQS_VIG_CENTER_RATIO *)iq_param_addr[IQ_VIG_CENTER];
	g_vig_addr[id] = (IQS_VIG_SET *)iq_param_addr[IQ_VIG_TAB];
    g_vig_iso_th_addr[id] = (IQS_VIG_ISO_TH *)iq_param_addr[IQ_VIG_ISO_TH];

	g_gbal_s_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_GBAL_STAB];
	g_gbal_d_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_GBAL_DTAB];
	g_gbal_addr[id] = (IFE_GBAL *)iq_param_addr[IQ_GBAL];

    g_rbfill_luma_tab_addr[id] =   (UINT8 *)iq_param_addr[IQ_RBFILL_LUMA_TAB];
    g_rbfill_ratio_tab_addr[id] =   (UINT8 *)iq_param_addr[IQ_RBFILL_RATIO_TAB];
    g_rbfill_addr[id] = (IFE_RBFILL *)iq_param_addr[IQ_RBFILL];

	//***********
	//**  DCE  **
	//***********
	g_dist_addr[id] = (DIST *)iq_param_addr[IQ_DCE_DIST];

	g_radious_addr[id] = (Fact_Norm *)iq_param_addr[IQ_DCE_RADIOUS];

	g_cfa_addr[id] = (DCE_CFAPARAM *)iq_param_addr[IQ_CFAINTER];

	g_fov_addr[id] = (FOV *)iq_param_addr[IQ_GDC_FOV];
	g_ldc_g_lut_addr[id] = (UINT32 *)iq_param_addr[IQ_GDC_GLUT];
	g_ldc_r_lut_addr[id] = (INT32 *)iq_param_addr[IQ_GDC_RLUT];
	g_ldc_b_lut_addr[id] = (INT32 *)iq_param_addr[IQ_GDC_BLUT];
	g_cac_addr[id] = (Aberation *)iq_param_addr[IQ_DCE_CA];

	//***********
	//**  IPE  **
	//***********
	g_edge_kernel_addr[id] = (INT16 *)iq_param_addr[IQ_EDGEKER];
	g_edir_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_DIR_TAB];
	g_edir_ext_addr[id] = (IPE_DEE *)iq_param_addr[IQ_DIREDGE_EXT];
	g_score_tab_addr[id] = (IPE_DirScorCal *)iq_param_addr[IQ_DIRSCORETAB];
	g_score_th_addr[id] = (IPE_DirScorCalTh *)iq_param_addr[IQ_DIRSCORETH];
	g_ewa_addr[id] = (IPE_KAEdgeW *)iq_param_addr[IQ_EWA];
	g_ewb_addr[id] = (IPE_KBEdgeW *)iq_param_addr[IQ_EWB];
	g_ewparam_addr[id] = (IPE_EdgeW *)iq_param_addr[IQ_EWPARAM];

	g_edge_enh_addr[id] = (IPE_EDGEENH *)iq_param_addr[IQ_EDGEENH];

	g_ed_map_addr[id] = (EdgeMap *)iq_param_addr[IQ_EDMAP];
	g_ed_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_EDTAB];
	g_es_map_addr[id] = (EdgeMapS *)iq_param_addr[IQ_ESMAP];
	g_es_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_ESTAB];

	g_lpf_addr[id] = (LPF *)iq_param_addr[IQ_RGBLPF];

	g_color_temperature_th_addr[id] = (IQS_COLOR_TEMPERATURE_TH *)iq_param_addr[IQ_CCM_TH];
	g_color_low_addr[id] = (IQS_COLOR_TEMPERATURE_PARAM *)iq_param_addr[IQ_Color_L];
	g_color_middle_addr[id] = (IQS_COLOR_TEMPERATURE_PARAM *)iq_param_addr[IQ_Color_M];
	g_color_high_addr[id] = (IQS_COLOR_TEMPERATURE_PARAM *)iq_param_addr[IQ_Color_H];
	g_fs_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_FSTAB];
	g_fd_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_FDTAB];
	g_cc_addr[id] = (IPE_CC *)iq_param_addr[IQ_CC];

	g_y_curve_addr[id] = (UINT32 *)iq_param_addr[IQ_Y_CURVE];

	g_gamma_lv_th_addr[id] = (IQS_GAMMA_LV_TH *)iq_param_addr[IQ_GAMMA_TH];
	g_gamma_day_addr[id] = (UINT32 *)iq_param_addr[IQ_GAMMA_DAY];
	g_gamma_night_addr[id] = (UINT32 *)iq_param_addr[IQ_GAMMA_NIGHT];

	g_color_3d_lut_addr[id] = (UINT32 *)iq_param_addr[IQ_3DCC_TAB];
	g_color_3d_addr[id] = (IPE_3DCCRound *)iq_param_addr[IQ_3DCC_ROUND];

	g_edg_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_EDGTAB];
	g_dds_tab_addr[id] = (UINT8 *)iq_param_addr[IQ_DDSTAB];

	g_saturation_addr[id] = (INT8 *)iq_param_addr[IQ_SATURATION];
	g_contrast_addr[id] = (INT8 *)iq_param_addr[IQ_CONTRAST];
    g_ccon_tab_addr[id] = (UINT32 *)iq_param_addr[IQ_CCON_TAB];
    g_cctrl_info_addr[id] = (IQS_CCTRL_INFO *)iq_param_addr[IQ_CCTRL_INFO];

	g_cst_coef_addr[id] = (INT16 *)iq_param_addr[IQ_CST_COEF];
	g_cst_addr[id] = (IPE_CST *)iq_param_addr[IQ_CST_PARAM];

	g_ycc_fix_addr[id] = (IPE_YCCFIX *)iq_param_addr[IQ_YCCFIX];

	g_noise_param_addr[id] = (IPE_RAND_NR *)iq_param_addr[IQ_NOISE_PARAM];

	g_gamma_rand_addr[id] = (IPE_GamRand *)iq_param_addr[IQ_GAMMA_RAND];

	//***********
	//** IFE2  **
	//***********
	g_ref_center_y_th_addr[id] = (UINT32 *)iq_param_addr[IQ_REFCENTER_YTH];
	g_ref_center_y_wt_addr[id] = (UINT32 *)iq_param_addr[IQ_REFCENTER_YWT];
	g_ref_center_y_addr[id] = (IFE2_REFCENT_Set *)iq_param_addr[IQ_REFCENTER_Y];
	g_ref_center_uv_th_addr[id] = (UINT32 *)iq_param_addr[IQ_REFCENTER_UVTH];
	g_ref_center_uv_wt_addr[id] = (UINT32 *)iq_param_addr[IQ_REFCENTER_UVWT];
	g_ref_center_uv_addr[id] = (IFE2_REFCENT_Set *)iq_param_addr[IQ_REFCENTER_UV];

	g_ife2filter_y_th_addr[id] = (UINT32 *)iq_param_addr[IQ_IFE2FILTER_YTH];
	g_ife2filter_y_wt_addr[id] = (UINT32 *)iq_param_addr[IQ_IFE2FILTER_YWT];
	g_ife2filter_uv_th_addr[id] = (UINT32 *)iq_param_addr[IQ_IFE2FILTER_UVTH];
	g_ife2filter_uv_wt_addr[id] = (UINT32 *)iq_param_addr[IQ_IFE2FILTER_UVWT];
	g_ife2filter_addr[id] = (IQS_IFE2_FILTER *)iq_param_addr[IQ_IFE2FILTER];

	g_gray_statical_addr[id] = (IFE2_GRAY_Statal *)iq_param_addr[IQ_GRAY];

	//***********
	//**  IME  **
	//***********
	g_chroma_adapt_addr[id] = (IME_CHRA_ADAP *)iq_param_addr[IQ_CHROMAADAPT];
	g_chroma_adapt_color_addr[id] = (IME_CHRA_CADAP *)iq_param_addr[IQ_CHROMAADAPT_COLOR];
	g_chroma_adapt_lum_addr[id] = (IME_CHRA_LADAP *)iq_param_addr[IQ_CHROMAADAPT_LUM];

	g_chroma_supp_w_y_addr[id] = (UINT32 *)iq_param_addr[IQ_CHROMASUPP_WEIGHTY];
	g_chroma_supp_w_c_addr[id] = (UINT32 *)iq_param_addr[IQ_CHROMASUPP_WEIGHTC];
	g_chroma_supp_addr[id] = (IME_ChromaSupp *)iq_param_addr[IQ_CHROMASUPP];

    g_tmnr_info_addr[id] = (IME_HAL_TMNR_INFO **)iq_param_addr[IQ_TMNR];
    g_tmnr_momap_sel_addr[id] = iq_param_addr[IQ_TMNR_DEBUG];
    g_tmnr_auto_info_addr[id] = (TMNR_AUTO_INFO  *)iq_param_addr[IQ_TMNR_AUTO];

	//***********
	//** Codec **
	//***********
	g_tnr_total_num_addr[id] = (UINT32 *)iq_param_addr[IQ_NUM_3DNR_PARAM];
	g_tnr_level_addr[id] = (INT8 *)iq_param_addr[IQ_3DNR_LEVEL];
	g_tnr_param_addr[id] = (H26XEncTnr **)iq_param_addr[IQ_3DNR_PARAM];
}
