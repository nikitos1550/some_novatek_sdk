/**
    Ctrl sample.

    @file       ipl_ext_ctrl.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "ipl_ext_common.h"
#include "dis_alg.h"

CHAR *ipl_dbg_mode_tbl[] = {
	"OFF",
	"PRV",
	"VDO",
	"REC",
	"CAP",
	"MAX_NUM",
	"BUF_QUERY",
	""
};

CHAR *ipl_dbg_trigflg_tbl[] = {
	"NONE",
	"START",
	"STOP",
	"PAUSE",
	"RESTART",
	"RESUME",
	"OFF",
	""
};

static BOOL gCapFlg = FALSE;

void IPL_SIESetOB_FCB(UINT32 id, SIE_SUBFUNC *sie_sub_func, SIE_UPDATE *Update)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	//auto ob init
	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVAUTO_OB_EN) == ENABLE) {
		sie_sub_func->OBAvg.StartPix.x = sen_status.sensor_info.mode->ob.top_win[0];
		sie_sub_func->OBAvg.StartPix.y = sen_status.sensor_info.mode->ob.top_win[1];
		sie_sub_func->OBAvg.SizeH = (sen_status.sensor_info.mode->ob.top_win[2] - sen_status.sensor_info.mode->ob.top_win[0]);
		sie_sub_func->OBAvg.SizeV = (sen_status.sensor_info.mode->ob.top_win[3] - sen_status.sensor_info.mode->ob.top_win[1]);
		sie_sub_func->OBSub.Offset = 0;
		sie_sub_func->OBAvg.SubRatio_X = 0;
		sie_sub_func->OBAvg.AvgActThres = 0x80;
		sie_sub_func->OBAvg.ObSubRatio = 0x20;
		sie_sub_func->sieFuncEn |= (OBSubSelEn | OBAvgEn);
		*Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SUBFEN);
	}
}

void IPL_SIESetCA_FCB(UINT32 id, SIE_SUBFUNC *sie_sub_func, SIE_UPDATE *Update)
{
	THRESHOL_SET CA_ThrInit = {0, 4095};
    UINT32 IR_SUB[3] = {0, 0, 0}; // R, G, B

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_EN) == ENABLE) {
		sie_sub_func->Ca.CACrop.StartPix.x = 0;
		sie_sub_func->Ca.CACrop.StartPix.y = 0;
		sie_sub_func->Ca.CACrop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
		sie_sub_func->Ca.CACrop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);

		sie_sub_func->Ca.CA_WIN.WinNum_X = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_WINXNUM);
		sie_sub_func->Ca.CA_WIN.WinNum_Y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_WINYNUM);

		//ca threshold init
		sie_sub_func->Ca.CA_TH_INFO.Gth = CA_ThrInit;
		sie_sub_func->Ca.CA_TH_INFO.Rth = CA_ThrInit;
		sie_sub_func->Ca.CA_TH_INFO.Bth = CA_ThrInit;
		sie_sub_func->Ca.CA_TH_INFO.Pth = CA_ThrInit;

        sie_sub_func->Ca.CA_IR_SUB.R_Weight = IR_SUB[0];
        sie_sub_func->Ca.CA_IR_SUB.G_Weight = IR_SUB[1];
        sie_sub_func->Ca.CA_IR_SUB.B_Weight = IR_SUB[2];

		sie_sub_func->pStatPathInfo.bVig = DISABLE;
		sie_sub_func->pStatPathInfo.bCaThreshold = ENABLE;

		sie_sub_func->sieFuncEn |= CAEn;
		*Update |= (SIE_STATS_CA_SIZE | SIE_STATS_CA_);
	} else {
		sie_sub_func->sieFuncEn &= ~CAEn;
	}
	*Update |= SIE_SUBFEN;
}

void IPL_SIESetLA_FCB(UINT32 id, SIE_SUBFUNC *sie_sub_func, SIE_UPDATE *Update)
{
	SEN_STATUS_PARAM sensor_status = {0};
	ER rt = E_OK;
    UINT32 IR_SUB[3] = {0, 0, 0}; // R, G, B

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_EN) == ENABLE) {

		sie_sub_func->La.LACrop.StartPix.x = (IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H) - IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H)) >> 1;
		sie_sub_func->La.LACrop.StartPix.y = (IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V) - IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V)) >> 1;
		sie_sub_func->La.LACrop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H);
		sie_sub_func->La.LACrop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V);

		sie_sub_func->La.LA_WIN.WinNum_X = IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_WINXNUM);
		sie_sub_func->La.LA_WIN.WinNum_Y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_WINYNUM);

#if 1
		sie_sub_func->pStatPathInfo.bLaCg = DISABLE;
		sie_sub_func->pStatPathInfo.bLaGama1 = DISABLE;
		sie_sub_func->pStatPathInfo.bHistogramSel = STATS_HISTO_PRE_GAMMA;
		sie_sub_func->pStatPathInfo.bLa1SrcSel = STATS_LA1_PRE_CG;
#else
		sie_sub_func->pStatPathInfo.bLaCg = ENABLE;
		sie_sub_func->pStatPathInfo.bLaGama1 = ENABLE;
		sie_sub_func->pStatPathInfo.bHistogramSel = STATS_HISTO_POST_GAMMA;
		sie_sub_func->pStatPathInfo.bLa1SrcSel = STATS_LA1_POST_CG;
#endif
		rt |= sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sensor_status);
        if(rt == E_OK && sensor_status.sensor_info.mode != NULL){
            if(sensor_status.sensor_info.mode->stpix <= SENSOR_STPIX_B){
                sie_sub_func->pStatPathInfo.la_rgb2y1_sel = STATS_LA_RGB;
                sie_sub_func->pStatPathInfo.la_rgb2y2_sel = STATS_LA_RGB;
            }
            else{
                sie_sub_func->pStatPathInfo.la_rgb2y1_sel = STATS_LA_G;
                sie_sub_func->pStatPathInfo.la_rgb2y2_sel = STATS_LA_G;
                sie_sub_func->La.LA_IR_SUB.R_Weight = IR_SUB[0];
                sie_sub_func->La.LA_IR_SUB.G_Weight = IR_SUB[1];
                sie_sub_func->La.LA_IR_SUB.B_Weight = IR_SUB[2];
            }
        }
        else{
            sie_sub_func->pStatPathInfo.la_rgb2y1_sel = STATS_LA_RGB;
            sie_sub_func->pStatPathInfo.la_rgb2y2_sel = STATS_LA_RGB;
        }

		sie_sub_func->sieFuncEn |= (LAEn | HistoYEn);
		*Update |= (SIE_STATS_LA_ | SIE_STATS_LA_SIZE | SIE_STATS_LA_CG | SIE_STATS_LA_GAMMA);
	} else {
		sie_sub_func->sieFuncEn &= ~LAEn;
	}
	*Update |= SIE_SUBFEN;
}

void IPL_SIESetBP_FCB(UINT32 id, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
	//bp
	Sub->BP.BP1 = 16;
	Sub->BP.BP2 = 32;
	Sub->BP.BP3 = 48;

	*InteEn |= (SIE__INTE_BP1 | SIE__INTE_BP2 | SIE__INTE_BP3);
	*Update |= (SIE_BP_ | SIE_SUBFEN);
}

void IPL_SIESetDMAOutSrc_FCB(UINT32 id, SieFuncEn *func_en, SIE_UPDATE *Update)
{
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_EN) == ENABLE) {
		*func_en |= DMAOutCh0En;
		*Update |= (SIE_SUBFEN | SIE_OUTINFO_CH0);
	} else {
		*func_en &= ~DMAOutCh0En;
		*Update |= SIE_SUBFEN;
	}
}

void IPL_SIESetDMAOutCh3_FCB(UINT32 id, SieFuncEn *func_en, SIE_UPDATE *Update, SIE_HAL_YOUT_INFO *yinfo)
{
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_EN) == ENABLE) {
		yinfo->win_num_x = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM);
		yinfo->win_num_y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM);
		yinfo->crp_size_x = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
		yinfo->crp_size_y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);
		yinfo->rgb2y_sel = ENABLE;	// g channel

		*func_en |= DMAOutCh3En | SHDRYOutEn;
		*Update |= (SIE_SUBFEN | SIE_CH3_YOUT_SIZE | SIE_OUTINFO_CH3);
	}
}

void IPL_SIESetDP_FCB(UINT32 id, SIE_SUBFUNC *sie_sub_func, SIE_UPDATE *Update, SieFuncEn *func_en)
{
	ER rt = E_OK;
	UINT32 data_addr = 0;
	CAL_RW_HEADER_INFO rw_header_info = {0};

	rw_header_info.sec.cal_item = CAL_ITEM_DP;
	rw_header_info.key.id = (IPL_PROC_ID)id;
	rw_header_info.key.cal_cond = CAL_COND_DFT;
	rw_header_info.key.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);

	rt =  ipl_cal_getcalrst(id, &rw_header_info, &data_addr);

	if (rt == E_OK) {
		if ((rw_header_info.value.cal_status == CAL_STATUS_OK) && (data_addr != 0) && (rw_header_info.value.read_data_size != 0)) {
			sie_sub_func->Dpc.DP_TblAddr = data_addr;
			sie_sub_func->Dpc.TblSize = rw_header_info.value.read_data_size;
			sie_sub_func->Dpc.Weight = _50F00_PERCENT;
			*func_en |= DpcEn;
			*Update |= (SIE_DPC_ | SIE_SUBFEN);
		} else {
			*func_en &= ~DpcEn;
			*Update |= (SIE_DPC_ | SIE_SUBFEN);
		}
	} else {
		if (rt != E_NOSPT) { // E_NOSPT: CALIBRATION_FUNC DISABLE
			DBG_ERR("ipl_cal_getcalrst error %d id %d\r\n", rt, id);
		}
		*func_en &= ~DpcEn;
		*Update |= (SIE_DPC_ | SIE_SUBFEN);
	}
}

#if 0
#endif
void IPL_SIEPatGen_FCB(UINT32 id, SieFuncEn *func_en, SIE_UPDATE *Update, PAT_GEN_INFO *PatGen)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if (sen_status.sensor_info.data_type == SENSOR_DATA_SIEPATGEN) {
		*Update |= SIE_PATGEN;
		*func_en |= PatGenEn;
		PatGen->type = SIE_PAT_COLORBAR;
		PatGen->val = sen_status.sensor_info.mode->valid_width / 7;
		PatGen->val &= ~0x1;
	} else {
		*func_en &= ~PatGenEn;
	}
}


void IPL_SIESetRawEnc_FCB(UINT32 id, SieFuncEn *func_en,  SIE_UPDATE *Update, SIE_RAWENC *RawEnc)
{
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAWENC_EN) == ENABLE) {
		*Update |= SIE_SUBFEN;
		*func_en |= RawEncEn;
	} else {
		*func_en &= ~RawEncEn;
	}
}


void IPL_SIECCIR_FCB(UINT32 id, SieFuncEn *func_en,  SIE_UPDATE *Update, SIE_DVI *pDvi)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if ((sen_status.sensor_info.mode->mode_type != SENSOR_MODE_CCIR) && (sen_status.sensor_info.mode->mode_type != SENSOR_MODE_CCIR_INTERLACE)) {
		return;
	}

	switch (sen_status.sensor_info.mode->dvi->fmt) {
	case SENSOR_DVI_CCIR601:
		pDvi->SIE_DVIFmt = SIE_CCIR601;
		break;

	case SENSOR_DVI_CCIR656_EAV:
		pDvi->SIE_DVIFmt = SIE_CCIR656_EAV;
		break;

	case SENSOR_DVI_CCIR656_ACT:
		pDvi->SIE_DVIFmt = SIE_CCIR656_ACT;
		break;

	default:
		DBG_ERR("id: %d, unsupport SIE_DVIFmt = %d\r\n", id, sen_status.sensor_info.mode->dvi->fmt);
		break;
	}

	switch (sen_status.sensor_info.mode->dvi->mode) {
	case SENSOR_DVI_MODE_SD:
		pDvi->SIE_DVIMode = SIE_SDMode;
		break;

	case SENSOR_DVI_MODE_HD:
		pDvi->SIE_DVIMode = SIE_HDMode;
		break;

	case SENSOR_DVI_MODE_HD_INV:
		pDvi->SIE_DVIMode = SIE_HDMode_Inv;
		break;

	default:
		DBG_ERR("id: %d, unsupport SIE_DVIMode = %d\r\n", id, sen_status.sensor_info.mode->dvi->mode);
		break;
	}

	switch (sen_status.sensor_info.mode->dvi->data_fmt) {
	case SENSOR_DVI_YUYV:
		pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
		break;

	case SENSOR_DVI_YVYU:
		pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
		break;

	case SENSOR_DVI_UYVY:
		pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
		break;

	case SENSOR_DVI_VYUY:
		pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
		break;

	default:
		DBG_ERR("id: %d, unsupport DVIOUT_SWAPSEL = %d\r\n", id, sen_status.sensor_info.mode->dvi->data_fmt);
		break;
	}

	*Update |= SIE_DVI_;
	*func_en |= DviEn;
	pDvi->FldEn = sen_status.sensor_info.mode->dvi->fld_en;
	pDvi->FldSelEn = sen_status.sensor_info.mode->dvi->fld_inv_en;
	pDvi->OutSplit = sen_status.sensor_info.mode->dvi->out_split_en;
	pDvi->CCIR656VdSel = sen_status.sensor_info.mode->dvi->ccir656_info.vd_mode;
	pDvi->DataPeriod = sen_status.sensor_info.mode->dvi->ahd_info.combine_num;
	pDvi->DataIdx = sen_status.sensor_info.mode->dvi->ahd_info.select_id;
	pDvi->AutoAlign = sen_status.sensor_info.mode->dvi->ahd_info.auto_align;
}

#if 0
#endif
void IPL_CtrlInitIpc_SIEAct(UINT32 id, SENSOR_INFO *SenInfo, SIE_ACT_MODE *ActMode)
{
	*ActMode = IPL_UTI_CONV2_SEN_ACT_MODE(SenInfo->sig_mode);
}

void IPL_CtrlInitIpc_SIEIn(UINT32 id, SENSOR_INFO *SenInfo, SIE_WINDOW *Win, BAYER_TYPE *bayer_type)
{
	UINT32 i, frm_cnt = 0;
	INT32 crop_shift_x, crop_shift_y;

	if (SenInfo->mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
		for (i = IPL_ID_1 ; i < SEN_MFRAME_MAX_NUM ; i++) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT) & (1 << i)) {
				if (i == id) {
					break;
				}
				frm_cnt++;
			}
		}
	}

	if(SenInfo->mode->dvi != NULL) {
		//adjust valid_width when HD mode(16bit), align to 8bit
		if(SenInfo->mode->dvi->mode != SENSOR_DVI_MODE_SD){
			Win->Act.StartPix.x = SenInfo->mode->trans_hd[frm_cnt]->data_start << 1;
			Win->Act.SizeH = SenInfo->mode->trans_hd[frm_cnt]->data_size << 1;
		} else {
			Win->Act.StartPix.x = SenInfo->mode->trans_hd[frm_cnt]->data_start;
			Win->Act.SizeH = SenInfo->mode->trans_hd[frm_cnt]->data_size;
		}
	} else {
		Win->Act.StartPix.x = SenInfo->mode->trans_hd[frm_cnt]->data_start;
		Win->Act.SizeH = SenInfo->mode->trans_hd[frm_cnt]->data_size;
	}

	Win->Act.StartPix.y = SenInfo->mode->trans_vd[frm_cnt]->data_start;
	Win->Act.SizeV = SenInfo->mode->trans_vd[frm_cnt]->data_size;

	Win->Act.CFA = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVSTPIX);

	Win->Crop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
	Win->Crop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);
	Win->Crop.CFA = Win->Act.CFA;

	if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV)) {
		DBG_ERR("id: %d, act size (%d %d) < crop Size(%d %d)\r\n", id, Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
		Win->Crop.StartPix.x = 0;
		Win->Crop.StartPix.y = 0;
		Win->Crop.SizeH = Win->Act.SizeH;
		Win->Crop.SizeV = Win->Act.SizeV;
	} else {
		Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
		Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;

		if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_DIRECT || IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR) {
			crop_shift_x = IPL_Ctrl_cal_input_offset(Win->Act.SizeH, Win->Crop.SizeH, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), &Win->Crop.StartPix.x);
			crop_shift_y = IPL_Ctrl_cal_input_offset(Win->Act.SizeV, Win->Crop.SizeV, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), &Win->Crop.StartPix.y);

			if (crop_shift_x != 0 || crop_shift_y != 0) {
				DBG_WRN("Shift Overflow, start: (%d, %d), dest_size: (%d, %d), src_size: (%d, %d)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), Win->Crop.SizeH, Win->Crop.SizeV, Win->Act.SizeH, Win->Act.SizeV);
			}

		}
	}

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_X, Win->Crop.StartPix.x);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_Y, Win->Crop.StartPix.y);

	switch (SenInfo->mode->stpix) {
	case SENSOR_STPIX_R:
	case SENSOR_STPIX_GR:
	case SENSOR_STPIX_GB:
	case SENSOR_STPIX_B:
		*bayer_type = BAYER_FORMAT;
		break;

	case SENSOR_STPIX_RGBIR_BG:
	case SENSOR_STPIX_RGBIR_RG:
		*bayer_type = RGBIR_FORMAT;
		break;

	case SENSOR_STPIX_RCCB_RC:
	case SENSOR_STPIX_RCCB_CR:
	case SENSOR_STPIX_RCCB_CB:
	case SENSOR_STPIX_RCCB_BC:
		*bayer_type = RCCB_FORMAT;
		break;

	default:
		*bayer_type = BAYER_FORMAT;
		break;
	}
}

void IPL_CtrlInitIpc_SIESignal(UINT32 id, IPP_SIE_SIGNAL *Signal)
{
	Signal->Sie_HDVD.VD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_INV);
	Signal->Sie_HDVD.HD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_INV);

	Signal->SensorPhase.VD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_PHASE);
	Signal->SensorPhase.HD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_PHASE);
	Signal->SensorPhase.Data = IPL_CtrlGetInfor(id, IPLCTRL_SIE_DATA_PHASE);
}

void IPL_CtrlInitIpc_SIECh0PPB(UINT32 id, IPLC_BASEINFO *iplc_base_info)
{
	UINT32 buf_size;
	UINT32 i, raw_buf_num = 0;

	iplc_base_info->SIE_Ch0_Img.width = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
	iplc_base_info->SIE_Ch0_Img.height = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
	iplc_base_info->SIE_Ch0_Img.bit = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT);
	iplc_base_info->SIE_Ch0_Img.line_offset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
	iplc_base_info->SIE_Ch0_Img.st_pix = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVSTPIX);

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM) != 0) {
		raw_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM);
		iplc_base_info->SIE_Ch0_Ppb_Num = raw_buf_num;
		iplc_base_info->SIE_Ch0_Ring_Buf_height = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_RING_BUF_V);
		for (i = 0; i < raw_buf_num; i ++) {
			IPL_BufGet(id, IPLBUF_SIE_CH0_1 + i, &iplc_base_info->SIE_Ch0_Ppb_Addr[i], &buf_size);
		}
	}
}

void IPL_CtrlInitIpc_SIECh1PPB(UINT32 id, IPL_RAW_IMG_INFO *dma_out, UINT32 *addr)
{
	UINT32 buf_size;
	UINT32 i, ch1_buf_num;

	ch1_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH1_BUFNUM);
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH1_EN) == ENABLE) {
		dma_out->line_offset = IPL_UTI_RAWH_CONV2_LOFS(IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H), IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT));
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH1_EN) == ENABLE || IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_EN) == ENABLE) {
		for (i = 0; i < ch1_buf_num; i ++) {
			IPL_BufGet(id, (IPLBUF_SIE_CH1_1 + i), &addr[i], &buf_size);
			memset((void*)addr[i], 0x80, buf_size);
		}
	}
}

void IPL_CtrlInitIpc_SIECh2PPB(UINT32 id, UINT32 *addr)
{
	UINT32 buf_size;
	UINT32 i, ch2_buf_num;

	ch2_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_BUFNUM);

	for (i = 0; i < ch2_buf_num; i ++) {
		IPL_BufGet(id, (IPLBUF_SIE_CH2_1 + i), &addr[i], &buf_size);
	}
}

void IPL_CtrlInitIpc_SIECh3PPB(UINT32 id, IPL_RAW_IMG_INFO *raw_info, UINT32 *addr, UINT32 *buf_num)
{
	UINT32 buf_size;
	UINT32 i, ch3_buf_num;

	ch3_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_BUFNUM);
	*buf_num = ch3_buf_num;

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SRC) == CH3_ACTIVE_WIN2) {
		raw_info->width = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SIZE_H);
		raw_info->height = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SIZE_V);
		raw_info->bit = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_PRVBIT);
		raw_info->line_offset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SIZE_HLOS);
		raw_info->st_pix = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVSTPIX);
	}

	for (i = 0; i < ch3_buf_num; i ++) {
		IPL_BufGet(id, (IPLBUF_SIE_CH3_1 + i), &addr[i], &buf_size);
	}
}

void IPL_CtrlInitIpc_SIECh5PPB(UINT32 id, UINT32 *addr)
{
	UINT32 buf_size;
	UINT32 i, buf_num = 0;

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM) != 0) {
		buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM);
		for (i = 0; i < buf_num; i ++) {
			IPL_BufGet(id, IPLBUF_SIE_CH5_1 + i, &addr[i], &buf_size);
		}
	}
}
void IPL_CtrlInitIpc_IPLInPPB(UINT32 id, IPL_RAW_IMG_INFO *raw_info, Coordinate *st_pos, RHE_HAL_FUNC_MODE *rhe_mode)
{
	INT32 crop_shift_x, crop_shift_y;

	raw_info->width = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H);
	raw_info->height = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V);
	st_pos->x = (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H) - raw_info->width) >> 1;
	st_pos->y = (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V) - raw_info->height) >> 1;
	raw_info->bit = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_BIT);
	raw_info->line_offset = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_HLOS);
	raw_info->st_pix = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVSTPIX);

	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_C || IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_VIRTUAL) {
		crop_shift_x = IPL_Ctrl_cal_input_offset(IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H), raw_info->width, IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_X), &st_pos->x);
		crop_shift_y = IPL_Ctrl_cal_input_offset(IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V), raw_info->height, IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_Y), &st_pos->y);

		if (crop_shift_x != 0 || crop_shift_y != 0) {
			DBG_WRN("Shift Overflow, start: (%d, %d), dest_size: (%d, %d), src_size: (%d, %d)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_X), IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_Y), raw_info->width, raw_info->height, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H), IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V));
		}
	}

	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_START_X, st_pos->x);
	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_START_Y, st_pos->y);

    if (IPL_CtrlGetInfor(id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) {

        // Only for 510 direct mode, must be SHDR
        if      (IPL_CtrlGetInfor(id, IPLCTRL_EXT_CTRL_EN) & IPL_EXT_CTRL_RAW_DIRECT)
            *rhe_mode = RHE_HAL_FUNC_MODE_SHDR;

        else if (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR) == SEL_SHDR_OFF)
            *rhe_mode = RHE_HAL_FUNC_MODE_LINEAR;

        else if (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR) == SEL_SHDR_ON)
            *rhe_mode = RHE_HAL_FUNC_MODE_SHDR;

        else if (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR) == SEL_SHDR_WDR)
            *rhe_mode = RHE_HAL_FUNC_MODE_PHDR;

        else {
            DBG_ERR("Wrong DR setting(%x, %u)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_EXT_FUNC_EN), IPL_AlgGetUIInfo(id, IPL_SEL_SHDR));
            *rhe_mode = RHE_HAL_FUNC_MODE_LINEAR;
        }
    }
    else if (IPL_CtrlGetInfor(id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) {
        *rhe_mode = RHE_HAL_FUNC_MODE_PHDR;
    } else {
        *rhe_mode = RHE_HAL_FUNC_MODE_LINEAR;
    }
}

void IPL_CtrlInitIpc_IPLStrpInfo(UINT32 id, IPL_STRP_INFO *strp_info)
{
    IPL_GetDceStrpInfo(id, strp_info);
}

void IPL_CtrlInitIpc_RHESubOutPPB(UINT32 id, UINT32 *addr, UINT32 *width, UINT32 *height)
{
	UINT32 buf_size;

	IPL_BufGet(id, IPLBUF_RHESUB_1, &addr[0], &buf_size);
	IPL_BufGet(id, IPLBUF_RHESUB_2, &addr[1], &buf_size);

	*width = IPL_CtrlGetInfor(id, IPLCTRL_RHE_DEFOG_SUB_OUT_SIZE);
	*height = IPL_CtrlGetInfor(id, IPLCTRL_RHE_DEFOG_SUB_OUT_SIZE);
}

void IPL_CtrlInitIpc_DCEIn(UINT32 id, SENSOR_INFO *SenInfo, IPLC_BASEINFO *iplc_base_info)
{
	if (SenInfo->data_type == SENSOR_DATA_SIEPATGEN) {
		iplc_base_info->DCE_Ref_1X.w = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
		iplc_base_info->DCE_Ref_1X.h = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);
	} else {

		iplc_base_info->DCE_Ref_1X.w = SenInfo->width_1x;
		iplc_base_info->DCE_Ref_1X.h = SenInfo->height_1x;
	}

	iplc_base_info->DCE_Ref_1XCenter.x = (iplc_base_info->DCE_Ref_1X.w >> 1);
	iplc_base_info->DCE_Ref_1XCenter.y = (iplc_base_info->DCE_Ref_1X.h >> 1);
	iplc_base_info->DCE_Ref_1XCenterOfs.x = 0;
	iplc_base_info->DCE_Ref_1XCenterOfs.y = 0;
	iplc_base_info->SenHBinRatio = SenInfo->mode->ratio.ratio_h;
	iplc_base_info->SenVBinRatio = SenInfo->mode->ratio.ratio_v;
}

void IPL_CtrlInitIpc_IPEEthPPB(UINT32 id, UINT32 *Addr, UINT32 *Addr_subspl, UINT32 *LineOfs)
{
	UINT32 buf_size;
	UINT32 buf_num, i;

	buf_num = IPL_CtrlGetInfor(id, IPLCTRL_IPE_ETH_OUT_BUF_NUM);
	for (i = 0; i < buf_num; i ++) {
		IPL_BufGet(id, IPLBUF_IPEETH_1 + i, &Addr[i], &buf_size);
		IPL_BufGet(id, IPLBUF_IPEETH_SUBSPL_1 + i, &Addr_subspl[i], &buf_size);
	}

	*LineOfs = IPL_CtrlGetInfor(id, IPLCTRL_IPE_ETH_OUT_SIZE_HLOS);
}

void IPL_CtrlInitIpc_IPEVAPPB(UINT32 id, UINT32 *Addr)
{
	UINT32 buf_size;

	IPL_BufGet(id, IPLBUF_IPEVA_1, &Addr[0], &buf_size);
	IPL_BufGet(id, IPLBUF_IPEVA_2, &Addr[1], &buf_size);
}

void IPL_CtrlInitIpc_IMESubFuncPPB(UINT32 id, UINT32 buf_idx, UINT32 *addr, UINT32 buf_num)
{
	UINT32 buf_size;
	UINT32 i;

	if (buf_num != 0) {
		for (i = 0; i < buf_num; i ++) {
			IPL_BufGet(id, buf_idx + i, &addr[i], &buf_size);
			memset((void *)addr[i], 0, buf_size);
		}
	}
}

void IPL_CtrlInitIpc_IMEPxOutInfo(UINT32 id, UINT32 path_en_idx, IPLC_IME_PATH_INFO *path_info, UINT32 *out_enc_en)
{
	UINT32 Px;
	IPLC_IME_PATH_INFO YCCInfo = {0};

	UINT32 Pxidx[4][12] = {
		{IPLCTRL_IME_P1_EN, IPLCTRL_IME_P1_MD_EVT_OUT_EN, IPLCTRL_IME_P1_SAMPLE_RATE, IPLCTRL_IME_P1_IMG_FMT, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLCTRL_IME_P1_OUT_CROP_STR_X, IPLCTRL_IME_P1_OUT_CROP_STR_Y, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V, IPLCTRL_IME_P1_ENC_EN},
		{IPLCTRL_IME_P2_EN, IPLCTRL_IME_P2_MD_EVT_OUT_EN, IPLCTRL_IME_P2_SAMPLE_RATE, IPLCTRL_IME_P2_IMG_FMT, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLCTRL_IME_P2_OUT_CROP_STR_X, IPLCTRL_IME_P2_OUT_CROP_STR_Y, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V, 0},
		{IPLCTRL_IME_P3_EN, IPLCTRL_IME_P3_MD_EVT_OUT_EN, IPLCTRL_IME_P3_SAMPLE_RATE, IPLCTRL_IME_P3_IMG_FMT, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLCTRL_IME_P3_OUT_CROP_STR_X, IPLCTRL_IME_P3_OUT_CROP_STR_Y, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V, 0},
		{IPLCTRL_IME_P4_EN, IPLCTRL_IME_P4_MD_EVT_OUT_EN, IPLCTRL_IME_P4_SAMPLE_RATE, IPLCTRL_IME_P4_IMG_FMT, IPLCTRL_IME_P4_OUT_SIZE_H, IPLCTRL_IME_P4_OUT_SIZE_V, IPLCTRL_IME_P4_OUT_SIZE_HLOS, IPLCTRL_IME_P4_OUT_CROP_STR_X, IPLCTRL_IME_P4_OUT_CROP_STR_Y, IPLCTRL_IME_P4_OUT_CROP_H, IPLCTRL_IME_P4_OUT_CROP_V, 0},
	};

	switch (path_en_idx) {
	case IPLCTRL_IME_P1_EN:
		Px = 0;
		break;

	case IPLCTRL_IME_P2_EN:
		Px = 1;
		break;

	case IPLCTRL_IME_P3_EN:
		Px = 2;
		break;

	case IPLCTRL_IME_P4_EN:
		Px = 3;
		break;

	default:
		DBG_ERR("id: %d, PathEnidx error %d\r\n", id, path_en_idx);
		return;
	}

	//calculate ycc information
	YCCInfo.enable = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
	if (YCCInfo.enable == ENABLE) {
		YCCInfo.md_evt_out_en = IPL_CtrlGetInfor(id, Pxidx[Px][1]);
		YCCInfo.sample_rate = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.img_info.type = IPL_CtrlGetInfor(id, Pxidx[Px][3]);
		YCCInfo.img_info.ch[IPL_IMG_YUV_Y].width = IPL_CtrlGetInfor(id, Pxidx[Px][4]);
		YCCInfo.img_info.ch[IPL_IMG_YUV_Y].height = IPL_CtrlGetInfor(id, Pxidx[Px][5]);
		YCCInfo.img_info.ch[IPL_IMG_YUV_Y].line_ofs = IPL_CtrlGetInfor(id, Pxidx[Px][6]);
		YCCInfo.img_info.ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.img_info.type, YCCInfo.img_info.ch[IPL_IMG_YUV_Y]);

		YCCInfo.crop_info.Start.x = IPL_CtrlGetInfor(id, Pxidx[Px][7]);
		YCCInfo.crop_info.Start.y = IPL_CtrlGetInfor(id, Pxidx[Px][8]);
		YCCInfo.crop_info.Width = IPL_CtrlGetInfor(id, Pxidx[Px][9]);
		YCCInfo.crop_info.Height = IPL_CtrlGetInfor(id, Pxidx[Px][10]);

		if (path_en_idx == IPLCTRL_IME_P1_EN && IPL_CtrlGetInfor(id, Pxidx[Px][11]) == ENABLE) {
			*out_enc_en = ENABLE;
			YCCInfo.img_info.ch[IPL_IMG_YUV_Y].line_ofs = ALIGN_CEIL(YCCInfo.img_info.ch[IPL_IMG_YUV_Y].line_ofs * 3 / 4, 16);
			YCCInfo.img_info.ch[IPL_IMG_YUV_U].line_ofs = ALIGN_CEIL(YCCInfo.img_info.ch[IPL_IMG_YUV_U].line_ofs * 3 / 4, 16);
		}
		YCCInfo.img_info.ch[IPL_IMG_YUV_V] = YCCInfo.img_info.ch[IPL_IMG_YUV_U];
	}

	*path_info = YCCInfo;
}

/* IME Output Image Set As below
-----------------------------
|           L_Sprt          |
-----------------------------
|             R             |
-----------------------------
|           |   |           |
|           |   |           |
|           |   |           |
|      L    | S |   R_Sprt  |
|           |   |           |
|           |   |           |
|           |   |           |
-----------------------------
*/
void IPL_CtrlInitIpc_IMEPxOutInfo_Stitch(UINT32 id, UINT32 path_en_idx, IPL_YCC_IMG_INFO *path_info, STITCH_IMG *stitch_img)
{
#if 0   //Jarkko tmp
	UINT32 buf_size, Px;
	IPL_IMG_BUF_INFO buf_info = {0};
	IPL_YCC_IMG_INFO YCCInfo = {0};
	UINT32 YSprtAddr, CSprtAddr;
	UINT32 SprtR = 0, SprtL = 0, SeamH = 0;   //Get from Stitch lib

	UINT32 Pxidx[3][10] = {
		{IPLCTRL_IME_P1_IMG_FMT, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLBUF_IMEP1_1, IPLCTRL_IME_P1_SPRT_YBUFOFS, IPLCTRL_IME_P1_SPRT_CBUFOFS, IPLBUF_IMEP1_SPRT, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V},
		{IPLCTRL_IME_P2_IMG_FMT, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLBUF_IMEP2_1, IPLCTRL_IME_P2_SPRT_YBUFOFS, IPLCTRL_IME_P2_SPRT_CBUFOFS, IPLBUF_IMEP2_SPRT, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V},
		{IPLCTRL_IME_P3_IMG_FMT, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLBUF_IMEP3_1, IPLCTRL_IME_P3_SPRT_YBUFOFS, IPLCTRL_IME_P3_SPRT_CBUFOFS, IPLBUF_IMEP3_SPRT, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V},
	};

	switch (path_en_idx) {
	case IPLCTRL_IME_P1_EN:
		Px = 0;
		break;

	case IPLCTRL_IME_P2_EN:
		Px = 1;
		break;

	case IPLCTRL_IME_P3_EN:
		Px = 2;
		break;

	default:
		DBG_ERR("id: %d, PathEnidx error %d\r\n", id, path_en_idx);
		return;
	}
#if 0   //Jarkko tmp
	SeamH = IPL_ALIGN_ROUNDUP(Stitch_GetStitchAreaWidthInPixel(IPL_CtrlGetInfor(id, Pxidx[Px][1])), 4);
	SprtL = IPL_ALIGN_ROUNDUP(Stitch_GetLeftImgCutPointInPixel(IPL_CtrlGetInfor(id, Pxidx[Px][1])), 4);
	SprtR = IPL_ALIGN_ROUNDUP(Stitch_GetRightImgCutPointInPixel(IPL_CtrlGetInfor(id, Pxidx[Px][1])), 4);
#endif
	if (id == IPL_ID_1) {
		//calculate Separate img information
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = SprtL;
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = YCCInfo.Ch[IPL_IMG_YUV_Y].Width;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
		IPL_BufGet(id, Pxidx[Px][7], &buf_info.Buf[0], &buf_size);
		buf_info.Buf[1] = 0;
		buf_info.Buf[2] = 0;
		IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, buf_info, &YCCInfo);

		stitch_img->StPos = IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtL;
		stitch_img->LineOfsY = YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs;
		stitch_img->LineOfsCC = YCCInfo.Ch[IPL_IMG_YUV_U].LineOfs;

		YSprtAddr = YCCInfo.PixelAddr[0];
		CSprtAddr = YCCInfo.PixelAddr[1];

		//calculate main output address
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtL;
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (YCCInfo.Ch[IPL_IMG_YUV_Y].Width + (IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtR)) + SeamH;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
		IPL_BufGet(id, Pxidx[Px][4], &buf_info.Buf[0], &buf_size);
		buf_info.Buf[1] = 0;
		buf_info.Buf[2] = 0;
		IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, buf_info, &YCCInfo);

		IPL_CtrlSetInfor(id, Pxidx[Px][5], (YCCInfo.PixelAddr[0] - YSprtAddr));
		IPL_CtrlSetInfor(id, Pxidx[Px][6], (YCCInfo.PixelAddr[1] - CSprtAddr));

		//calculate ycc information
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(id, Pxidx[Px][8]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][9]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtL) + (IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtR) + SeamH;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
	} else if (id == IPL_ID_2) {
		//calculate Separate img information
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtR;
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(IPL_ID_1, Pxidx[Px][1]) - SprtL) + (IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtR) + SeamH;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
		IPL_BufGet(id, Pxidx[Px][4], &buf_info.Buf[0], &buf_size);
		buf_info.Buf[1] = 0;
		buf_info.Buf[2] = 0;
		IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, buf_info, &YCCInfo);

		stitch_img->StPos = SprtR;
		stitch_img->LineOfsY = YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs;
		stitch_img->LineOfsCC = YCCInfo.Ch[IPL_IMG_YUV_U].LineOfs;

		YSprtAddr = YCCInfo.PixelAddr[0];
		CSprtAddr = YCCInfo.PixelAddr[1];

		//calculate main output address
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = SprtR;
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = YCCInfo.Ch[IPL_IMG_YUV_Y].Width;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
		IPL_BufGet(id, Pxidx[Px][7], &buf_info.Buf[0], &buf_size);
		buf_info.Buf[1] = 0;
		buf_info.Buf[2] = 0;
		IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, buf_info, &YCCInfo);

		IPL_CtrlSetInfor(id, Pxidx[Px][5], (YSprtAddr - YCCInfo.PixelAddr[0]));
		IPL_CtrlSetInfor(id, Pxidx[Px][6], (CSprtAddr - YCCInfo.PixelAddr[1]));

		//calculate ycc information
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(id, Pxidx[Px][8]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][9]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = SprtR;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
	}

	*path_info = YCCInfo;
#endif
}

#if 0
#endif

void IPL_CtrlInitIpc_SIEAct_Cap(UINT32 id, SIE_UPDATE *Update, SIE_ACT_MODE *ActMode)
{
	INT32 base_id;
	SEN_STATUS_PARAM sensor_status;
	ER rt = E_OK;

	base_id = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_BASE_ID);
	sensor_status.sensor_mode = IPL_AlgGetUIInfo(base_id, IPL_SEL_CAPSENMODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(base_id), SEN_GET_STATUS, (void *)&sensor_status);
	if (rt != E_OK || sensor_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", base_id, IPL_CtrlGetInfor(base_id, IPLCTRL_SEN_MODE));
		return;
	}
	*ActMode = IPL_UTI_CONV2_SEN_ACT_MODE(sensor_status.sensor_info.sig_mode);
}

void IPL_CtrlInitIpc_SIESignal_Cap(UINT32 id, SIE_UPDATE *Update, IPP_SIE_SIGNAL *Signal)
{
	Signal->Sie_HDVD.HD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_INV);
	Signal->Sie_HDVD.VD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_INV);

	Signal->SensorPhase.VD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_PHASE);
	Signal->SensorPhase.HD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_PHASE);
	Signal->SensorPhase.Data = IPL_CtrlGetInfor(id, IPLCTRL_SIE_DATA_PHASE);

	*Update |= (SIE_INIT);
}

void IPL_CtrlInitIpc_SIECh0PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out, SIE_PACKBUS *Bit)
{
	*Bit = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAPBIT);
	Out->LineOffset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS);
	*Update |= SIE_OUTINFO_CH0;
}

void IPL_CtrlInitIpc_SIECh1PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out)
{
	Out->LineOffset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
	*Update |= SIE_OUTINFO_CH1;
}

void IPL_CtrlInitIpc_SIECh2PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out)
{
	Out->LineOffset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
	*Update |= SIE_OUTINFO_CH2;
}


void IPL_CtrlInitIpc_SIECh3PPB_Cap(UINT32 id,  SIE_UPDATE *Update, SIE_Dma_Out *Out, SIE_PACKBUS *Bit)
{

	*Bit = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_CAPBIT);
	Out->LineOffset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_HLOS);
	*Update |= SIE_OUTINFO_CH3;
}

void IPL_CtrlInitIpc_SIEIn_Cap(UINT32 id,  IPC_SIE_Info *ipc_sie_info)
{
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 i, frm_cnt = 0;
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_AlgGetUIInfo(id, IPL_SEL_CAPSENMODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_AlgGetUIInfo(id, IPL_SEL_CAPSENMODE));
		return;
	}

	if (sen_status.sensor_info.mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
		for (i = IPL_ID_1 ; i < SEN_MFRAME_MAX_NUM ; i++) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT) & (1 << i)) {
				if (i == id) {
					break;
				}
				frm_cnt++;
			}
		}
	}

	ipc_sie_info->psie->SensorIn_Win.Act.StartPix.x = sen_status.sensor_info.mode->trans_hd[frm_cnt]->data_start;
	ipc_sie_info->psie->SensorIn_Win.Act.StartPix.y = sen_status.sensor_info.mode->trans_vd[frm_cnt]->data_start;
	ipc_sie_info->psie->SensorIn_Win.Act.SizeH = sen_status.sensor_info.mode->trans_hd[frm_cnt]->data_size;
	ipc_sie_info->psie->SensorIn_Win.Act.SizeV = sen_status.sensor_info.mode->trans_vd[frm_cnt]->data_size;

	ipc_sie_info->psie->SensorIn_Win.Act.CFA = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CAPSTPIX);

	ipc_sie_info->psie->SensorIn_Win.Crop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CAPCROP_SIZE_H);
	ipc_sie_info->psie->SensorIn_Win.Crop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CAPCROP_SIZE_V);
	ipc_sie_info->psie->SensorIn_Win.Crop.CFA = ipc_sie_info->psie->SensorIn_Win.Act.CFA;

	if (sen_status.sensor_info.mode->mode_type == SENSOR_MODE_CCIR || sen_status.sensor_info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
		if (sen_status.sensor_info.mode->dvi != NULL) {
			if (sen_status.sensor_info.mode->dvi->mode != SENSOR_DVI_MODE_SD) {
				ipc_sie_info->psie->SensorIn_Win.Act.SizeH  = ipc_sie_info->psie->SensorIn_Win.Act.SizeH << 1;
				ipc_sie_info->psie->SensorIn_Win.Act.StartPix.x = ipc_sie_info->psie->SensorIn_Win.Act.StartPix.x << 1;
			} else {
			}
		} else {
			DBG_ERR("dvi information NULL\r\n");
		}
	} else {
	}

	if ((ipc_sie_info->psie->SensorIn_Win.Act.SizeH < ipc_sie_info->psie->SensorIn_Win.Crop.SizeH) || (ipc_sie_info->psie->SensorIn_Win.Act.SizeV < ipc_sie_info->psie->SensorIn_Win.Crop.SizeV)) {
		DBG_ERR("id: %d, act size (%d %d) < crop Size(%d %d)\r\n", id, ipc_sie_info->psie->SensorIn_Win.Act.SizeH, ipc_sie_info->psie->SensorIn_Win.Act.SizeV, ipc_sie_info->psie->SensorIn_Win.Crop.SizeH, ipc_sie_info->psie->SensorIn_Win.Crop.SizeV);
		ipc_sie_info->psie->SensorIn_Win.Crop.StartPix.x = 0;
		ipc_sie_info->psie->SensorIn_Win.Crop.StartPix.y = 0;
		ipc_sie_info->psie->SensorIn_Win.Crop.SizeH = ipc_sie_info->psie->SensorIn_Win.Act.SizeH;
		ipc_sie_info->psie->SensorIn_Win.Crop.SizeV = ipc_sie_info->psie->SensorIn_Win.Act.SizeV;
	} else {
		ipc_sie_info->psie->SensorIn_Win.Crop.StartPix.x = (ipc_sie_info->psie->SensorIn_Win.Act.SizeH - ipc_sie_info->psie->SensorIn_Win.Crop.SizeH) >> 1;
		ipc_sie_info->psie->SensorIn_Win.Crop.StartPix.y = (ipc_sie_info->psie->SensorIn_Win.Act.SizeV - ipc_sie_info->psie->SensorIn_Win.Crop.SizeV) >> 1;
	}

	ipc_sie_info->psie->BSOutImg.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
	ipc_sie_info->psie->BSOutImg.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);

	switch (sen_status.sensor_info.mode->stpix) {
	case SENSOR_STPIX_R:
	case SENSOR_STPIX_GR:
	case SENSOR_STPIX_GB:
	case SENSOR_STPIX_B:
		ipc_sie_info->psie->Bayer_Type = BAYER_FORMAT;
		break;

	case SENSOR_STPIX_RGBIR_BG:
	case SENSOR_STPIX_RGBIR_RG:
		ipc_sie_info->psie->Bayer_Type = RGBIR_FORMAT;
		break;

	case SENSOR_STPIX_RCCB_RC:
	case SENSOR_STPIX_RCCB_CR:
	case SENSOR_STPIX_RCCB_CB:
	case SENSOR_STPIX_RCCB_BC:
		ipc_sie_info->psie->Bayer_Type = RCCB_FORMAT;
		break;

	default:
		DBG_ERR("bayer type unkonw (start pix = %d)\r\n", sen_status.sensor_info.mode->stpix);
		break;
	}

	//set dummy ch3 act window2 to prevent driver error
	ipc_sie_info->psie->SensorIn_Win.Act_2.StartPix.x = 1;
	ipc_sie_info->psie->SensorIn_Win.Act_2.StartPix.y = 1;
	ipc_sie_info->psie->SensorIn_Win.Act_2.SizeH = 0;
	ipc_sie_info->psie->SensorIn_Win.Act_2.SizeV = 0;
	ipc_sie_info->sieUpdate |= SIE_CH3_ACT2;

	ipc_sie_info->sieUpdate |= SIE_IOSIZE;
}

void IPL_CtrlInitIpc_SIEOB_Cap(UINT32 id,  SIE_UPDATE *Update, SIE_SUBFUNC *sie_sub_func)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	if ((id != IPL_ID_1) && (id != IPL_ID_2)) {
		DBG_ERR("IPL ID Error %d : OB only support IPL_ID_1 and IPL_ID_2\r\n", id);
		return;
	}

	sen_status.sensor_mode = IPL_AlgGetUIInfo(id, IPL_SEL_CAPSENMODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_AlgGetUIInfo(id, IPL_SEL_CAPSENMODE));
		return;
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_CAPAUTO_OB_EN) == DISABLE) {
		sie_sub_func->OBAvg.ObSubRatio = 0x20;
	} else {
		sie_sub_func->OBAvg.StartPix.x = sen_status.sensor_info.mode->ob.top_win[0];
		sie_sub_func->OBAvg.StartPix.y = sen_status.sensor_info.mode->ob.top_win[1];
		sie_sub_func->OBAvg.SizeH = (sen_status.sensor_info.mode->ob.top_win[2] - sen_status.sensor_info.mode->ob.top_win[0]);
		sie_sub_func->OBAvg.SizeV = (sen_status.sensor_info.mode->ob.top_win[3] - sen_status.sensor_info.mode->ob.top_win[1]);
		sie_sub_func->OBSub.Offset = 0;
		sie_sub_func->OBAvg.SubRatio_X = 0;
		sie_sub_func->OBAvg.AvgActThres = 0x80;
		sie_sub_func->OBAvg.ObSubRatio = 0x20;
		sie_sub_func->sieFuncEn |= (OBSubSelEn | OBAvgEn);
		*Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SUBFEN);
	}
}
void IPL_CtrlInitIpc_SIECCIR_Cap(UINT32 id, SIE *sie, FLIP_TYPE flip_type)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if ((sen_status.sensor_info.mode->mode_type != SENSOR_MODE_CCIR) && (sen_status.sensor_info.mode->mode_type != SENSOR_MODE_CCIR_INTERLACE)) {
		return;
	}

	switch (sen_status.sensor_info.mode->dvi->fmt) {
	case SENSOR_DVI_CCIR601:
		sie->pSubFunc->pDvi->SIE_DVIFmt = SIE_CCIR601;
		break;

	case SENSOR_DVI_CCIR656_EAV:
		sie->pSubFunc->pDvi->SIE_DVIFmt = SIE_CCIR656_EAV;
		break;

	case SENSOR_DVI_CCIR656_ACT:
		sie->pSubFunc->pDvi->SIE_DVIFmt = SIE_CCIR656_ACT;
		break;

	default:
		DBG_ERR("id: %d, unsupport SIE_DVIFmt = %d\r\n", id, sen_status.sensor_info.mode->dvi->fmt);
		break;
	}

	switch (sen_status.sensor_info.mode->dvi->mode) {
	case SENSOR_DVI_MODE_SD:
		sie->pSubFunc->pDvi->SIE_DVIMode = SIE_SDMode;
		break;

	case SENSOR_DVI_MODE_HD:
		sie->pSubFunc->pDvi->SIE_DVIMode = SIE_HDMode;
		break;

	case SENSOR_DVI_MODE_HD_INV:
		sie->pSubFunc->pDvi->SIE_DVIMode = SIE_HDMode_Inv;
		break;

	default:
		DBG_ERR("id: %d, unsupport SIE_DVIMode = %d\r\n", id, sen_status.sensor_info.mode->dvi->mode);
		break;
	}

	switch (sen_status.sensor_info.mode->dvi->data_fmt) {
	case SENSOR_DVI_YUYV:
		sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
		break;

	case SENSOR_DVI_YVYU:
		sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
		break;

	case SENSOR_DVI_UYVY:
		sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
		break;

	case SENSOR_DVI_VYUY:
		sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
		break;

	default:
		DBG_ERR("id: %d, unsupport DVIOUT_SWAPSEL = %d\r\n", id, sen_status.sensor_info.mode->dvi->data_fmt);
		break;
	}

	sie->pSubFunc->pDvi->FldEn = sen_status.sensor_info.mode->dvi->fld_en;
	sie->pSubFunc->pDvi->FldSelEn = sen_status.sensor_info.mode->dvi->fld_inv_en;
	sie->pSubFunc->pDvi->OutSplit = sen_status.sensor_info.mode->dvi->out_split_en;
	sie->pSubFunc->pDvi->CCIR656VdSel = sen_status.sensor_info.mode->dvi->ccir656_info.vd_mode;
	sie->pSubFunc->pDvi->DataPeriod = sen_status.sensor_info.mode->dvi->ahd_info.combine_num;
	sie->pSubFunc->pDvi->DataIdx = sen_status.sensor_info.mode->dvi->ahd_info.select_id;
	sie->pSubFunc->pDvi->AutoAlign = sen_status.sensor_info.mode->dvi->ahd_info.auto_align;

	//crop, adjust dviout pattern if crop start is not 4x
	if ((sie->SensorIn_Win.Crop.StartPix.x & 0x3) != 0) {
		switch (sie->pSubFunc->pDvi->DVIOUT_SWAPSEL) {
		case DVIOUT_SWAPVYUY:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
			break;

		case DVIOUT_SWAPUYVY:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
			break;

		case DVIOUT_SWAPYUYV:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
			break;

		case DVIOUT_SWAPYVYU:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
			break;

		default:
			DBG_ERR("unknown swapsel %d\r\n", sie->pSubFunc->pDvi->DVIOUT_SWAPSEL);
		}
	}

	//flip
	if (flip_type & FLIP_H) {
		sie->CH0.Out.HFlip = TRUE;
		sie->CH1.Out.HFlip = TRUE;

		switch (sie->pSubFunc->pDvi->DVIOUT_SWAPSEL) {
		case DVIOUT_SWAPVYUY:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
			break;

		case DVIOUT_SWAPUYVY:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
			break;

		case DVIOUT_SWAPYUYV:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
			break;

		case DVIOUT_SWAPYVYU:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
			break;

		default:
			DBG_ERR("unknown swapsel %d\r\n", sie->pSubFunc->pDvi->DVIOUT_SWAPSEL);
		}
	}

	if (flip_type & FLIP_V) {
		sie->CH0.Out.VFlip = TRUE;
		sie->CH1.Out.VFlip = TRUE;
	}



}
void IPL_CtrlInitIpc_SIECH3Info_Cap(UINT32 id, SIE *psie, SIE_UPDATE *Update)
{
	SR_WDR_SUBIMG_INFO wdr_sub_info = {0};
	SHDR_SUBIMG_INFO shdr_sub_info = {0};
	if (psie->CH3.Src == CH3_YOUT_ACC) {
		if (IPL_CtrlGetInfor(id, IPLCTRL_WDR_CAP_EN) == ENABLE) {
#if IPL_WDR_FUNC_SUPPORT
			SceneRender_WDR_GetSubImgInfo(id, &wdr_sub_info);
#endif
			if (wdr_sub_info.uiSubRatio == 1) {
				psie->pSubFunc->yout_info.win_num_x = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), IPL_UTI_CONV2_UINT32(1, 1), wdr_sub_info.uiSubWidth, 1);
				psie->pSubFunc->yout_info.win_num_y = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), IPL_UTI_CONV2_UINT32(1, 1), wdr_sub_info.uiSubHeight, 1);
			} else {
				psie->pSubFunc->yout_info.win_num_x = wdr_sub_info.uiSubWidth;
				psie->pSubFunc->yout_info.win_num_y = wdr_sub_info.uiSubHeight;
			}
		} else if (IPL_CtrlGetInfor(id, IPLCTRL_SHDR_CAP_EN) == ENABLE) {
#if IPL_SHDR_FUNC_SUPPORT
			shdr_sub_info = SensorHDR_GetSubImgInfo();
#endif
			if (shdr_sub_info.uiSubRatio == 1) {
				psie->pSubFunc->yout_info.win_num_x = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), IPL_UTI_CONV2_UINT32(1, 1), shdr_sub_info.uiSubWidth, 1);
				psie->pSubFunc->yout_info.win_num_y = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), IPL_UTI_CONV2_UINT32(1, 1), shdr_sub_info.uiSubHeight, 1);
			} else {
				psie->pSubFunc->yout_info.win_num_x = shdr_sub_info.uiSubWidth;
				psie->pSubFunc->yout_info.win_num_y = shdr_sub_info.uiSubHeight;
			}
		} else {
			DBG_WRN("shdr or wdr sould be opened when CH3_YOUT_ACC\r\n");
			return;
		}
		psie->pSubFunc->yout_info.crp_size_x = psie->SensorIn_Win.Crop.SizeH;
		psie->pSubFunc->yout_info.crp_size_y = psie->SensorIn_Win.Crop.SizeV;
		psie->pSubFunc->sieFuncEn |= SHDRYOutEn;
		*Update |= SIE_CH3_YOUT_SIZE ;
	}
}
#if 0   //Jarkko tmp
void IPL_CtrlInitIpc_SIEGamma_Cap(UINT32 id,  SIE_UPDATE *Update, SieFuncEn *func_en, SIE_GAMMA **pGamma)
{
	if ((id != IPL_ID_1) && (id != IPL_ID_2)) {
		DBG_ERR("IPL ID Error %d : SIE Gamma only support IPL_ID_1 and IPL_ID_2\r\n", id);
		return;
	}

	*Update |= SIE_GAMMA_;
	*func_en |= GammaEn;

	*pGamma = &SieRawEncTab;
}
#endif

void IPL_CtrlInitIpc_SIERawEnc_Cap(UINT32 id, SieFuncEn *func_en)
{
	*func_en |= RawEncEn ;
}
void IPL_CtrlInitIpc_SIEPatGen_Cap(UINT32 id, SieFuncEn *func_en, SIE_UPDATE *Update, PAT_GEN_INFO *PatGen)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if (sen_status.sensor_info.data_type == SENSOR_DATA_SIEPATGEN) {
		*Update |= SIE_PATGEN;
		*func_en |= PatGenEn;
		PatGen->type = SIE_PAT_COLORBAR;
		PatGen->val = sen_status.sensor_info.mode->valid_width / 7;
		PatGen->val &= ~0x1;
	} else {
		*func_en &= ~PatGenEn;
	}
}

void IPL_CtrlInitIpc_SIECASet_Cap(UINT32 id, SIE_SUBFUNC *sie_sub_func, SIE_UPDATE *Update,UINT32 CAEnable)
{
	THRESHOL_SET CA_ThrInit = {0, 4095};
    UINT32 IR_SUB[3] = {0, 0, 0}; // R, G, B

	if (CAEnable == ENABLE) {
		sie_sub_func->Ca.CACrop.StartPix.x = 0;
		sie_sub_func->Ca.CACrop.StartPix.y = 0;
		sie_sub_func->Ca.CACrop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
		sie_sub_func->Ca.CACrop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);

		sie_sub_func->Ca.CA_WIN.WinNum_X = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_WINXNUM);
		sie_sub_func->Ca.CA_WIN.WinNum_Y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_WINYNUM);

		//ca threshold init
		sie_sub_func->Ca.CA_TH_INFO.Gth = CA_ThrInit;
		sie_sub_func->Ca.CA_TH_INFO.Rth = CA_ThrInit;
		sie_sub_func->Ca.CA_TH_INFO.Bth = CA_ThrInit;
		sie_sub_func->Ca.CA_TH_INFO.Pth = CA_ThrInit;

        sie_sub_func->Ca.CA_IR_SUB.R_Weight = IR_SUB[0];
        sie_sub_func->Ca.CA_IR_SUB.G_Weight = IR_SUB[1];
        sie_sub_func->Ca.CA_IR_SUB.B_Weight = IR_SUB[2];

		sie_sub_func->pStatPathInfo.bVig = DISABLE;
		sie_sub_func->pStatPathInfo.bCaThreshold = ENABLE;

		sie_sub_func->sieFuncEn |= CAEn;
		*Update |= (SIE_STATS_CA_SIZE | SIE_STATS_CA_);
	} else {
		sie_sub_func->sieFuncEn &= ~CAEn;
	}
	*Update |= SIE_SUBFEN;
}

void IPL_CtrlInitCapIpcInfo(IPL_GET_CAP_RAW_DATA *cap_info, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;


	sen_status.sensor_mode = IPL_CtrlGetInfor(cap_info->id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(cap_info->id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(cap_info->id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(cap_info->id, IPLCTRL_SEN_MODE));
		return;
	}

	if (IPLCTRL_CAP_FLOW_INVID == IPL_CtrlGetInfor(cap_info->id, IPLCTRL_CAPFLOW)) {
#if 0
		//read sie original settings
		readBit.sieRead = SIE_R_SUBFEN;
		SIEx_ENG_Read(cap_info->Id, IPL_CtrlGetHalSetId(cap_info->Id), readBit, (void *)&Info->psie->pSubFunc->siefunc_en);
		//readBit.sieRead = SIE_R_IO;
		//SIEx_ENG_Read(cap_info->Id, IPL_CtrlGetHalSetId(cap_info->Id), readBit, (void *)Info->psie);
		//readBit.sieRead = SIE_R_Out;
		//SIEx_ENG_Read(cap_info->Id, IPL_CtrlGetHalSetId(cap_info->Id), readBit, (void *)Info->psie);

		readBit.sieRead = SIE_R_INSIZE;
		SIEx_ENG_Read(cap_info->Id, IPL_CtrlGetHalSetId(cap_info->Id), readBit, (void *)&Info->psie->IMG_Window);

		//read pre dram in information(special case)
		QInfo = IPL_CTRLQuery(cap_info->Id, ICF_IPLIO_RDY);

		/**************** save last sie information ****************/
		ExtInfo->Param[0] = Info->psie->CH0.Out.PPB0_Addr;
		ExtInfo->Param[1] = Info->psie->CH0.Out.PPB1_Addr;
		ExtInfo->Param[2] = Info->psie->CH0.Out.LineOffset;
		ExtInfo->Param[3] = IPL_CtrlGetInfor(cap_info->Id, IPLCTRL_PRVBURSTLENGTH);
		ExtInfo->Param[4] = Info->psie->IMG_Window.Crop.StartPix.x;
		ExtInfo->Param[5] = Info->psie->IMG_Window.Crop.StartPix.y;
		ExtInfo->Param[6] = Info->psie->IMG_Window.Crop.SizeH;
		ExtInfo->Param[7] = Info->psie->IMG_Window.Crop.SizeV;
		//ExtInfo->Param[8] = QInfo.PreDmaIn_Rdy.InDma.PixelAddr;
		//ExtInfo->Param[9] = QInfo.PreDmaIn_Rdy.PPB1_Addr;
		/**************** load new sie information ****************/
		IPL_CtrlInitIpc_SIEBurstLength_Cap(cap_info->Id, &Info->sieUpdate, &SenInfo, &Info->psie->BurstLength);
		IPL_CtrlInitIpc_SIEIn_Cap(cap_info->Id, &tmpUpdate, &tmpWin);
		Info->psie->IMG_Window.Crop.StartPix.x = tmpWin.Crop.StartPix.x;
		Info->psie->IMG_Window.Crop.StartPix.y = tmpWin.Crop.StartPix.y;
		Info->psie->IMG_Window.Crop.SizeH = tmpWin.Crop.SizeH;
		Info->psie->IMG_Window.Crop.SizeV = tmpWin.Crop.SizeV;

		IPL_CtrlInitIpc_SIECh0PPB_Cap(cap_info->Id, &Info->sieUpdate, &Info->psie->CH0.Out, &Ch0HSize, &Ch0VSize);
		Info->psie->CH0.Src = NonScaled_Img;
		Info->sieUpdate |= SIE_CH0Out_SRC;
#endif
	} else if (IPLCTRL_CAP_FLOW_SIM == IPL_CtrlGetInfor(cap_info->id, IPLCTRL_CAPFLOW)) {

		if (cap_info->out_ch0_en == ENABLE) {
			Info->psie->CH0.Out.PPB_Addr = cap_info->buf_addr_ch0;
			//Info->psie->CH0.Out.PPB1_Addr = cap_info->BufAddrCh0;
			Info->psie->pSubFunc->sieFuncEn |= DMAOutCh0En;
			Info->sieUpdate |= SIE_OUTADDR_CH0;
		}

		if (cap_info->out_ch3_yout_en == ENABLE) {
			Info->psie->CH3.Out.PPB_Addr = cap_info->buf_addr_ch3;
			//Info->psie->CH3.Out.PPB1_Addr = cap_info->BufAddrCh3;
			Info->psie->pSubFunc->sieFuncEn |= DMAOutCh3En;
			Info->sieUpdate |= (SIE_OUTADDR_CH3 | SIE_OUTINFO_CH3);
		}
	} else {
		IPL_CtrlInitIpc_SIESignal_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->Sie_Signal);
		IPL_CtrlInitIpc_SIEAct_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->ActMode);
		IPL_CtrlInitIpc_SIECh0PPB_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->CH0.Out, &Info->psie->CH0.Bit);
		IPL_CtrlInitIpc_SIEIn_Cap(cap_info->id, Info);
		if (cap_info->res[0] == 0x123) { // capture CCIR image
			IPL_CtrlInitIpc_SIECh1PPB_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->CH1.Out);
			IPL_CtrlInitIpc_SIECCIR_Cap(cap_info->id, Info->psie, cap_info->flip_info);
			Info->psie->CH0.Out.PPB_Addr = cap_info->buf_addr_ch0;
			//if y-only,set ub buffer value as 0x80
			if(sen_status.sensor_info.mode->dvi->out_split_en == FALSE) {
				memset((void*)cap_info->buf_addr_ch0_1, 0x80, IPL_CtrlGetInfor(cap_info->id,IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V) *IPL_CtrlGetInfor(cap_info->id,IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H));
			}
			Info->psie->CH1.Out.PPB_Addr = cap_info->buf_addr_ch0_1;
			Info->psie->pSubFunc->sieFuncEn |= DviEn;
			Info->sieUpdate |= (SIE_OUTADDR_CH0 | SIE_OUTADDR_CH1 | SIE_DVI_);

		} else { // capture raw image
			if (IPLCTRL_CAP_FLOW_EBURST == IPL_CtrlGetInfor(cap_info->id, IPLCTRL_CAPFLOW)) {
				ExtInfo->param[0] = cap_info->res[0];
				IPL_CtrlInitIpc_SIERawEnc_Cap(cap_info->id, &Info->psie->pSubFunc->sieFuncEn);

				// for frame rate ctrl
				if (cap_info->frm_ctrl != 0) {
					ExtInfo->frm_ctrl = cap_info->frm_ctrl;
				}

			} else {
			}
			IPL_SIESetDP_FCB(cap_info->id, Info->psie->pSubFunc, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn);
			IPL_CtrlInitIpc_SIECh3PPB_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->CH3.Out, &Info->psie->CH3.Bit);
			IPL_CtrlInitIpc_SIECASet_Cap(cap_info->id, Info->psie->pSubFunc, &Info->sieUpdate,cap_info->out_ca_en);
			if ((Info->psie->SensorIn_Win.Crop.SizeH != Info->psie->BSOutImg.SizeH) || (Info->psie->SensorIn_Win.Crop.SizeV != Info->psie->BSOutImg.SizeV)) {
				DBG_ERR("Id: %d, ch0 output size (%d %d) < crop Size(%d %d)\r\n", cap_info->id, Info->psie->BSOutImg.SizeH, Info->psie->BSOutImg.SizeV, Info->psie->SensorIn_Win.Crop.SizeH, Info->psie->SensorIn_Win.Crop.SizeV);
			}
			IPL_CtrlInitIpc_SIEOB_Cap(cap_info->id, &Info->sieUpdate, Info->psie->pSubFunc);
		}
		IPL_CtrlInitIpc_SIEPatGen_Cap(cap_info->id, &Info->psie->pSubFunc->sieFuncEn, &Info->sieUpdate, &Info->psie->pSubFunc->PatGen);
		Info->sieUpdate |= SIE_SUBFEN;
		Info->psie->pSubFunc->InteEn = SIE__INTE_FLDEND;


		//ch0
		if (cap_info->out_ch0_en == ENABLE) {
			Info->psie->CH0.Out.PPB_Addr = cap_info->buf_addr_ch0;
			Info->psie->pSubFunc->sieFuncEn |= DMAOutCh0En;
			Info->sieUpdate |= SIE_OUTADDR_CH0;

			if(cap_info->out_ca_en == ENABLE) {
				Info->psie->CH1.Out.PPB_Addr = cap_info->buf_addr_ca;
				Info->psie->CH1.Out.LineOffset = 0;
				Info->psie->CH1.Out.HFlip = 0;
				Info->psie->CH1.Out.VFlip = 0;
				Info->sieUpdate |= (SIE_OUTADDR_CH1 |SIE_OUTINFO_CH1);
			}
			//ch5
			if (cap_info->out_ch5_en == ENABLE) {
				Info->psie->CH5.Out.PPB_Addr = cap_info->buf_addr_ch5;
				Info->psie->CH5.Out.LineOffset = 0;
				Info->sieUpdate |= (SIE_OUTADDR_CH5 | SIE_OUTINFO_CH5);
			}
		}
		//ch3
		if (cap_info->out_ch3_yout_en == ENABLE) {
				Info->psie->CH3.Src = CH3_YOUT_ACC ;
				IPL_CtrlInitIpc_SIECH3Info_Cap(cap_info->id, Info->psie, &Info->sieUpdate);
				Info->psie->CH3.Out.PPB_Addr = cap_info->buf_addr_ch3;
				Info->psie->CH3.Out.LineOffset = IPL_ALIGN_ROUNDUP((Info->psie->pSubFunc->yout_info.win_num_x * 12 / 8),4);

				Info->psie->pSubFunc->sieFuncEn |= DMAOutCh3En;
				Info->sieUpdate |= (SIE_OUTADDR_CH3 | SIE_OUTINFO_CH3 | SIE_SUBFEN);
				if (cap_info->praw_info != NULL) {
					cap_info->praw_info->yout_info.yout_x_winnum = Info->psie->pSubFunc->yout_info.win_num_x;
					cap_info->praw_info->yout_info.yout_y_winnum = Info->psie->pSubFunc->yout_info.win_num_y;
			}
		}



	}

}

void IPL_CtrlInitPrvIpcInfo(UINT32 id, IPC_INFO *ipc_info, IPL_ISR_OBJ_INFO *isr_info, IPL_MODE_DATA *chg_mode)
{
	UINT32 i, idx = IPL_ISR_SIE1;

	switch (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW)) {
	case ICF_FLOW_C:
		IPL_CtrlInitIpcInfoFlowCommon(id, ipc_info);
		break;

	case ICF_FLOW_CCIR:
		IPL_CtrlInitIpcInfoFlowCCIR(id, ipc_info);
		break;

	case ICF_FLOW_VIRTUAL:
		IPL_CtrlInitIpcInfoFlowVirtual(id, ipc_info);
		break;

	case ICF_FLOW_DIRECT:
		IPL_CtrlInitIpcInfoFlowDirect(id, ipc_info);
		break;

	default:
		DBG_ERR("id: %d, unsupport IPL flow %d\r\n", id, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW));
		break;
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
		IPL_BufIOCB_Init(id, chg_mode->out_buf_cfg_cb_fp);
		isr_info->RAWHeader_CB = IPL_RAWHeaderCB;
		isr_info->raw_postproc_cb = chg_mode->raw_postproc_cb_fp;
		isr_info->datastamp_cb = chg_mode->datastamp_cb_fp;
		isr_info->pm_cb = chg_mode->pm_cb_fp;
		isr_info->md_cb = chg_mode->md_cb_fp;
		switch (id) {
		case IPL_ID_1:
			idx = IPL_ISR_SIE1;
			break;

		case IPL_ID_2:
			idx = IPL_ISR_SIE2;
			break;

		default:
			idx = IPL_ISR_SIE1;
			break;
		}
		isr_info->EventTab.FP[idx] = IPL_IsrSIECB;
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_CCIR) {
		isr_info->EventTab.FP[IPL_ISR_RHE] = IPL_IsrRHECB;
		isr_info->EventTab.FP[IPL_ISR_IFE] = IPL_IsrIFECB;
	}
	isr_info->EventTab.FP[IPL_ISR_IPE] = IPL_IsrIPECB;
	isr_info->EventTab.FP[IPL_ISR_IFE2] = NULL;
	isr_info->EventTab.FP[IPL_ISR_DCE] = IPL_IsrDCECB;
	isr_info->EventTab.FP[IPL_ISR_IME] = IPL_IsrIMECB;

	for (i = 0; i < IPL_ISR_MAX_CNT; i ++) {
		isr_info->CB_FP.FP[i] = chg_mode->isr_cb_fp[i];
	}
}

#if 0
#endif

INT32 IPL_Ctrl_cal_input_offset(UINT32 src_size, UINT32 dest_size, INT32 start_ofs, UINT32 *start_pos)
{
	INT32 start_ofs_2 = 0;

	if ((UINT32)start_ofs == IPL_CTRL_NO_USE || start_ofs == 0) {
		return start_ofs_2;
	}

	if (start_ofs > 0) {
		if (*start_pos + start_ofs + dest_size > src_size) {	//dest window > source window
			start_ofs_2 = start_ofs - (src_size - dest_size - *start_pos);
			*start_pos = src_size - dest_size;
		} else {
			start_ofs_2 = 0;
			*start_pos = *start_pos + start_ofs;
		}
	} else {
		if ((UINT32)(-start_ofs) > *start_pos) {	//start position < 0
			start_ofs_2 = start_ofs + *start_pos;
			*start_pos = 0;
		} else {
			start_ofs_2 = 0;
			*start_pos = start_ofs + *start_pos;
		}
	}

	return start_ofs_2;
}

UINT32 IPL_CtrlCalHdrRingBuf(UINT32 id, UINT32 vts)
{
    UINT32 ring_buf;
    UINT32 hdr_ratio_sum = 16;	//hdr lib suggestion

    if (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR) == SEL_SHDR_ON) {
        hdr_ratio_sum += IPL_AlgGetUIInfo(id, IPL_SEL_SHDR_DELTA_EV);
        ring_buf = ((vts<<6)/hdr_ratio_sum) + IPL_HDR_RINGBUF_RESERVE_SIZE;
    } else {
        ring_buf = 0;
    }

    //DBG_ERR("ring_buf = %d, %d, %d\r\n", ring_buf, hdr_ratio_sum, ae_get_ui_info(id, AE_UI_HDR_DEV));

    return ring_buf;
}

UINT32 IPL_CtrlGetIPEVAbuf_info(void)
{
	return (16 * 16 * 4 * 4);   //16x16 window, each window 4 words, 2 group(group1 or group 1n2)
}

void IPL_Getime_path_Info(UINT32 id, IPL_IME_PATH Pathid, IPL_IME_PATH_INFO *Info)
{
	UINT32 Px;

	UINT32 Pxidx[4][12] = {
		{IPLCTRL_IME_P1_EN, IPLCTRL_IME_P1_MD_EVT_OUT_EN, IPLCTRL_IME_P1_SAMPLE_RATE, IPLCTRL_IME_P1_IMG_FMT, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLCTRL_IME_P1_OUT_CROP_STR_X, IPLCTRL_IME_P1_OUT_CROP_STR_Y, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V,  IPLCTRL_IME_P1_ENC_EN},
		{IPLCTRL_IME_P2_EN, IPLCTRL_IME_P2_MD_EVT_OUT_EN, IPLCTRL_IME_P2_SAMPLE_RATE, IPLCTRL_IME_P2_IMG_FMT, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLCTRL_IME_P2_OUT_CROP_STR_X, IPLCTRL_IME_P2_OUT_CROP_STR_Y, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V,					0},
		{IPLCTRL_IME_P3_EN, IPLCTRL_IME_P3_MD_EVT_OUT_EN, IPLCTRL_IME_P3_SAMPLE_RATE, IPLCTRL_IME_P3_IMG_FMT, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLCTRL_IME_P3_OUT_CROP_STR_X, IPLCTRL_IME_P3_OUT_CROP_STR_Y, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V,					0},
		{IPLCTRL_IME_P4_EN, IPLCTRL_IME_P4_MD_EVT_OUT_EN, IPLCTRL_IME_P4_SAMPLE_RATE, IPLCTRL_IME_P4_IMG_FMT, IPLCTRL_IME_P4_OUT_SIZE_H, IPLCTRL_IME_P4_OUT_SIZE_V, IPLCTRL_IME_P4_OUT_SIZE_HLOS, IPLCTRL_IME_P4_OUT_CROP_STR_X, IPLCTRL_IME_P4_OUT_CROP_STR_Y, IPLCTRL_IME_P4_OUT_CROP_H, IPLCTRL_IME_P4_OUT_CROP_V,					0},
	};

	if (Info == NULL) {
		DBG_ERR("id: %d, input fail\r\n", id);
		return;
	}

	switch (Pathid) {
	case IPL_IME_PATH1:
		Px = 0;
		//YCC Encode setting
		Info->out_ycc_compress_en = IPL_CtrlGetInfor(id, Pxidx[Px][11]);
		break;

	case IPL_IME_PATH2:
		Px = 1;
		break;

	case IPL_IME_PATH3:
		Px = 2;
		break;

	case IPL_IME_PATH4:
		Px = 3;
		break;

	default:
		DBG_ERR("id: %d, Pathid error %d\r\n", id, Pathid);
		return;
	}

	Info->output_en = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
	Info->md_evt_out_en = IPL_CtrlGetInfor(id, Pxidx[Px][1]);
	Info->sample_rate = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
	Info->scale_size_h = IPL_CtrlGetInfor(id, Pxidx[Px][4]);
	Info->scale_size_v = IPL_CtrlGetInfor(id, Pxidx[Px][5]);
	Info->out_img_lofs = IPL_CtrlGetInfor(id, Pxidx[Px][6]);
	Info->img_fmt = IPL_CtrlGetInfor(id, Pxidx[Px][3]);

	Info->out_crop_start_x = IPL_CtrlGetInfor(id, Pxidx[Px][7]);
	Info->out_crop_start_y = IPL_CtrlGetInfor(id, Pxidx[Px][8]);
	Info->out_crop_size_h = IPL_CtrlGetInfor(id, Pxidx[Px][9]);
	Info->out_crop_size_v = IPL_CtrlGetInfor(id, Pxidx[Px][10]);
}

void IPL_Setime_path_Info(UINT32 id, IPL_IME_PATH Pathid, IPL_IME_PATH_INFO *Info)
{
	USIZE ipl_in = {0};
	UINT32 Px;
	UINT32 Pxidx[4][12] = {
		{IPLCTRL_IME_P1_EN, IPLCTRL_IME_P1_MD_EVT_OUT_EN, IPLCTRL_IME_P1_SAMPLE_RATE, IPLCTRL_IME_P1_IMG_FMT, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLCTRL_IME_P1_OUT_CROP_STR_X, IPLCTRL_IME_P1_OUT_CROP_STR_Y, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V,  IPLCTRL_IME_P1_ENC_EN},
		{IPLCTRL_IME_P2_EN, IPLCTRL_IME_P2_MD_EVT_OUT_EN, IPLCTRL_IME_P2_SAMPLE_RATE, IPLCTRL_IME_P2_IMG_FMT, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLCTRL_IME_P2_OUT_CROP_STR_X, IPLCTRL_IME_P2_OUT_CROP_STR_Y, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V,					0},
		{IPLCTRL_IME_P3_EN, IPLCTRL_IME_P3_MD_EVT_OUT_EN, IPLCTRL_IME_P3_SAMPLE_RATE, IPLCTRL_IME_P3_IMG_FMT, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLCTRL_IME_P3_OUT_CROP_STR_X, IPLCTRL_IME_P3_OUT_CROP_STR_Y, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V,					0},
		{IPLCTRL_IME_P4_EN, IPLCTRL_IME_P4_MD_EVT_OUT_EN, IPLCTRL_IME_P4_SAMPLE_RATE, IPLCTRL_IME_P4_IMG_FMT, IPLCTRL_IME_P4_OUT_SIZE_H, IPLCTRL_IME_P4_OUT_SIZE_V, IPLCTRL_IME_P4_OUT_SIZE_HLOS, IPLCTRL_IME_P4_OUT_CROP_STR_X, IPLCTRL_IME_P4_OUT_CROP_STR_Y, IPLCTRL_IME_P4_OUT_CROP_H, IPLCTRL_IME_P4_OUT_CROP_V,					0},
	};

	if (Info == NULL) {
		DBG_ERR("id: %d, input fail\r\n", id);
		return;
	}

	switch (Pathid) {
	case IPL_IME_PATH1:
		Px = 0;
		//YCC Encode setting
		IPL_CtrlSetInfor(id, Pxidx[Px][11], Info->out_ycc_compress_en);
		break;

	case IPL_IME_PATH2:
		Px = 1;
		break;

	case IPL_IME_PATH3:
		Px = 2;
		break;

	case IPL_IME_PATH4:
		Px = 3;
		break;

	default:
		DBG_ERR("id: %d, Pathid error %d\r\n", id, Pathid);
		return;
	}

	ipl_in.w = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H);
	ipl_in.h = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V);
	if ((Info->output_en == ENABLE) && (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_DIRECT) && ((Info->scale_size_h > ipl_in.w) || (Info->scale_size_v > ipl_in.h))) {
		DBG_ERR("Direct Mode N.S. IME Scale up\r\n");
		DBG_ERR("IPL_ID_%d, IPL Input Size(%d, %d), IME Path_%d Scale Size(%d, %d)\r\n", id+1, ipl_in.w, ipl_in.h,
			(Pathid>>1)+1, Info->scale_size_h, Info->scale_size_v);

		//force set ime out = ipl in
		Info->scale_size_h = ipl_in.w;
		Info->scale_size_v = ipl_in.h;
		Info->out_img_lofs = ipl_in.w;
		Info->out_crop_start_x = 0;
		Info->out_crop_start_y = 0;
		Info->out_crop_size_h = ipl_in.w;
		Info->out_crop_size_v = ipl_in.h;
	}

	IPL_CtrlSetInfor(id, Pxidx[Px][0], Info->output_en);
	if (Info->output_en == ENABLE) {
		IPL_CtrlSetInfor(id, Pxidx[Px][1], Info->md_evt_out_en);
		IPL_CtrlSetInfor(id, Pxidx[Px][2], Info->sample_rate);
		IPL_CtrlSetInfor(id, Pxidx[Px][3], Info->img_fmt);
		IPL_CtrlSetInfor(id, Pxidx[Px][4], Info->scale_size_h);
		IPL_CtrlSetInfor(id, Pxidx[Px][5], Info->scale_size_v);
		IPL_CtrlSetInfor(id, Pxidx[Px][6], Info->out_img_lofs);
		IPL_CtrlSetInfor(id, Pxidx[Px][7], Info->out_crop_start_x);
		IPL_CtrlSetInfor(id, Pxidx[Px][8], Info->out_crop_start_y);
		IPL_CtrlSetInfor(id, Pxidx[Px][9], Info->out_crop_size_h);
		IPL_CtrlSetInfor(id, Pxidx[Px][10], Info->out_crop_size_v);
	}
}

void IPL_CtrlPrvSetDZoomInfo(UINT32 id)
{
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 width = 0, height = 0;
	UINT32 dz_idx;
	SR_WDR_SUBIMG_INFO wdr_sub_info = {0};
	SHDR_SUBIMG_INFO shdr_sub_info = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("IPL_ID_%d Get Sensor Status Fail, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)+1, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	dz_idx = IPL_DzoomGetIndex(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID));
	//SIE crop
	IPL_DzoomGetSIECropSize(id, dz_idx, FALSE, &width, &height);
	width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, width, 4);
	height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, height, 4);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V, height);
	//SIE output information
	if(sen_status.sensor_info.mode->mode_type == SENSOR_MODE_CCIR || sen_status.sensor_info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE){
		if(sen_status.sensor_info.mode->dvi != NULL) {
			if(sen_status.sensor_info.mode->dvi->out_split_en == TRUE)
			{
				width = width >> 1;
				//when MD or TMNE enable, width should be multiple of 8x
				width = IPL_ALIGN_ROUNDDOWN(width,8);
			}
		} else {
			DBG_WRN("DVI information is NULL\r\n");
		}
	}
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V, height);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_PRVBIT, IPL_AlgGetUIInfo(id, IPL_SEL_RAW_BITDEPTH_PRV));

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAWENC_EN) == ENABLE) {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_ALIGN_ROUNDUP(IPL_UTI_RAWH_CONV2_LOFS(width, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT))*IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAWENC_BUF_RATIO) / 100, 4));
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(width, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT)));
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_EN) == ENABLE && IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SRC) == CH3_YOUT_ACC) {
		if (IPL_CtrlGetInfor(id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) {
#if IPL_WDR_FUNC_SUPPORT
			SceneRender_WDR_GetSubImgInfo(IPL_UTI_CONV2_WDR_ID(id), &wdr_sub_info);
#endif
			if (wdr_sub_info.uiSubRatio == 1) {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), wdr_sub_info.uiSubWidth, 1)); //maximum y output window number 128
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), wdr_sub_info.uiSubHeight, 1)); //maximum y output window number 128
			} else {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, wdr_sub_info.uiSubWidth); //maximum y output window number 128
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, wdr_sub_info.uiSubHeight); //maximum y output window number 128
			}
		} else {
#if IPL_SHDR_FUNC_SUPPORT
			shdr_sub_info = SensorHDR_GetSubImgInfo();
#endif
			if (shdr_sub_info.uiSubRatio == 1) {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), shdr_sub_info.uiSubWidth, 1)); //maximum y output window number 128
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), shdr_sub_info.uiSubHeight, 1)); //maximum y output window number 128
			} else {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, shdr_sub_info.uiSubWidth); //maximum y output window number 128
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, shdr_sub_info.uiSubHeight); //maximum y output window number 128
			}
		}
	}

	//Set IPL IN for RHE IN
	//direct mode N.S. RHE Crop
	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_DIRECT) {
		IPL_DzoomGetRaw2YUVInSize(id, dz_idx, &width, &height);
		if (IPL_CtrlGetInfor(id, IPLCTRL_IME_TMNR_EN) == ENABLE) {
			width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, width, 8);	//tmnr limitation
		} else {
			width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, width, 4);
		}
		height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, height, 4);
	}
	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_BIT, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT));

	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_V, height);

	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_HLOS, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS));

	//Set flip type
	IPL_CtrlSetFlipType(id, IPL_CtrlGetInfor(id, IPLCTRL_FLIP_TYPE));
}

void IPL_CtrlPrvGetDZoomInfo(UINT32 id, IPC_Dzoom_Info *Info)
{
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 act_width = 0, act_height = 0;
	UINT32 i, frm_cnt = 0;
	ER rt = E_OK;
	INT32 crop_shift_x, crop_shift_y;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("IPL_ID_%d Get Sensor Status Fail, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)+1, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if (sen_status.sensor_info.mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
		for (i = IPL_ID_1 ; i < SEN_MFRAME_MAX_NUM ; i++) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT) & (1 << i)) {
				if (i == id) {
					break;
				}
				frm_cnt++;
			}
		}
		act_width = sen_status.sensor_info.mode->trans_hd[frm_cnt]->data_size;
	} else {
		act_width = sen_status.sensor_info.mode->trans_hd[0]->data_size;
	}

	if(sen_status.sensor_info.mode->dvi != NULL){
		if(sen_status.sensor_info.mode->dvi->mode != SENSOR_DVI_MODE_SD)
			act_width = act_width << 1;
	}
	act_height = sen_status.sensor_info.mode->trans_vd[0]->data_size;

	Info->Id = id;
	Info->Idx = IPL_CtrlGetHalSetId(id);
	Info->sieCropHini = (act_width - IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H)) >> 1;
	Info->sieCropVini = (act_height - IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V)) >> 1;
	Info->sieCropH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
	Info->sieCropV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);
	Info->sieOutH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H);   //no SIE scale, sie out = sie crop
	Info->sieOutV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V);   //no SIE scale, sie out = sie crop
	Info->sieOutLineOfs = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
	Info->sie_yout_winnum_x = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM);
	Info->sie_yout_winnum_y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM);
	Info->iplCropX = ((Info->sieCropH - IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H)) >> 1);
	Info->iplCropY = ((Info->sieCropV - IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V)) >> 1);
	Info->iplInH = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H);
	Info->iplInV = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V);
	Info->iplInLineOfs = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
	Info->bChgCAParam = (BOOL)IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_EN);
	Info->bChgLAParam = (BOOL)IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_EN);
	Info->bChgYOutParam = (BOOL)IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_EN);
	Info->flip = IPL_CtrlGetInfor(id, IPLCTRL_IPL_FLIP_TYPE);

	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_DIRECT || IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR) {
		crop_shift_x = IPL_Ctrl_cal_input_offset(act_width, Info->sieCropH, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), &Info->sieCropHini);
		crop_shift_y = IPL_Ctrl_cal_input_offset(act_height, Info->sieCropV, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), &Info->sieCropVini);
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_X, Info->sieCropHini);
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_Y, Info->sieCropVini);
		if (crop_shift_x != 0 || crop_shift_y != 0) {
			DBG_WRN("Shift Overflow, start: (%d, %d), dest_size: (%d, %d), src_size: (%d, %d)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), Info->sieCropH, Info->sieCropV, act_width, act_height);
		}
	} else {
		crop_shift_x = IPL_Ctrl_cal_input_offset(act_width, Info->sieCropH, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), &Info->sieCropHini);
		crop_shift_y = IPL_Ctrl_cal_input_offset(act_height, Info->sieCropV, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), &Info->sieCropVini);
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_X, Info->sieCropHini);
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_Y, Info->sieCropVini);

		crop_shift_x = IPL_Ctrl_cal_input_offset(Info->sieCropH, Info->iplInH, crop_shift_x, &Info->iplCropX);
		crop_shift_y = IPL_Ctrl_cal_input_offset(Info->sieCropV, Info->iplInV, crop_shift_y, &Info->iplCropY);
		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_START_X, Info->iplCropX);
		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_START_Y, Info->iplCropY);

		if (crop_shift_x != 0 || crop_shift_y != 0) {
			DBG_WRN("Shift Overflow, start: (%d, %d), dest_size: (%d, %d), src_size: (%d, %d)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), Info->sieCropH, Info->sieCropV, act_width, act_height);
		}
	}
}

void IPL_GetDceStrpInfo(UINT32 id, IPL_STRP_INFO *strp_info)
{
    strp_info->strp_mode = IPL_STRPMODE_AUTO_124ST;

    if (strp_info->strp_mode >= IPL_STRPMODE_MAX) {
        DBG_ERR("Wrong IPL stripe mode (%u). Force set to auto mode!\r\n", strp_info->strp_mode);
        strp_info->strp_mode = IPL_STRPMODE_AUTO_124ST;
    }
}

#if 0
#endif
void IPL_CtrlInitCapbuf_info(UINT32 id, IPL_MODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *buf_info)
{
	buf_info->id = ChgMode->proc_sen_id;
}

void IPL_CtrlInitPrvbuf_info(IPL_MODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *buf_info)
{
	if (IPL_CtrlGetInfor(ChgMode->proc_sen_id, IPLCTRL_PRVFLOW) == ICF_REV1 || IPL_CtrlGetInfor(ChgMode->proc_sen_id, IPLCTRL_PRVFLOW) >= ICF_MAX) {
		DBG_ERR("IPL_ID_%d, unsupport IPL flow %d\r\n", ChgMode->proc_sen_id+1, IPL_CtrlGetInfor(ChgMode->proc_sen_id, IPLCTRL_PRVFLOW));
	} else {
		IPL_CtrlInitbuf_infoFlowCommon(ChgMode->proc_sen_id, buf_info);
	}

	buf_info->id = ChgMode->proc_sen_id;
}

#if 0
#endif
void IPL_CtrlCapInit(UINT32 id, UINT32 event_id, IPLCTRL_INFO *ctrl_info)
{
	UINT32 width = 0, height = 0, BitNum = 0;
	awb_ca_info CAInfo = {0};
	BAYER_TYPE bayer_type = 0;

	switch (event_id) {
	case IPL_FLOW_NOR_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NORMAL);
		break;

	case IPL_FLOW_VID_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_INVID);
		break;

	case IPL_FLOW_HS_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_HS);
		break;

	case IPL_FLOW_3D_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_3D);
		break;

	case IPL_FLOW_EB_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_EBURST);
		break;

	case IPL_FLOW_AEB_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_AEB);
		break;

	case IPL_FLOW_AWB_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_AWB);
		break;

	case IPL_FLOW_MFANTISHAKE_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_MFANTISHAKE);
		break;

	case IPL_FLOW_DUALSBS_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_DUALSBS);
		break;

	case IPL_FLOW_RHDR_GETRAW :
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_RHDR);
		break;

	default:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NONE);
		DBG_ERR("IPL_ID_%d, unsupport event_id flow %d\r\n", id+1, event_id);
		break;
	}

	//set sie signal
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_VD_INV, ctrl_info->info->sig_info.vd_inv);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_HD_INV, ctrl_info->info->sig_info.hd_inv);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_VD_PHASE, ctrl_info->info->sig_info.vd_phase);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_HD_PHASE, ctrl_info->info->sig_info.hd_phase);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_DATA_PHASE, ctrl_info->info->sig_info.data_phase);

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_BASE_ID, IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID));
	IPL_CtrlSetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO, ctrl_info->chg_mode->crop_ratio);
	IPL_CtrlSetInfor(id, IPLCTRL_CAPAUTO_OB_EN, DISABLE);
	IPL_CtrlSetInfor(id, IPLCTRL_CAPDF_EN, DISABLE);

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CAPSTPIX, IPL_UTI_CONV_SEN_STPIX(ctrl_info->info->mode->stpix));
	IPL_CtrlSetInfor(id, IPLCTRL_IME_CAP_SUBRATIO, iq_get_ife2_lca_ratio(id, TRUE, FALSE));

	if (iq_check_ife2_lca(id, TRUE) == ENABLE) {
		IPL_CtrlSetInfor(id, IPLCTRL_IFE2_LCA_EN, ENABLE);
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_IFE2_LCA_EN, DISABLE);
	}

	if(ctrl_info->chg_mode->func_en & IPL_FUNC_WDR || ctrl_info->chg_mode->func_en & IPL_FUNC_SHDR) {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_EN, ENABLE);

		if (ctrl_info->chg_mode->func_en & IPL_FUNC_WDR) {
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_SRC, CH3_YOUT_ACC);
			IPL_CtrlSetInfor(id, IPLCTRL_WDR_CAP_EN, ENABLE);
		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_WDR_CAP_EN, DISABLE);
		}

		if (ctrl_info->chg_mode->func_en & IPL_FUNC_SHDR) {
			if (event_id == IPL_FLOW_RHDR_GETRAW) {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_SRC, CH3_YOUT_ACC);
				IPL_CtrlSetInfor(id, IPLCTRL_SHDR_CAP_EN, ENABLE);
			} else {
				DBG_WRN("SHDR can not open when event = %d\r\n", event_id);
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_EN, DISABLE);
			}
		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_SHDR_CAP_EN, DISABLE);
		}
	} else {
			IPL_CtrlSetInfor(id, IPLCTRL_SHDR_CAP_EN, DISABLE);
			IPL_CtrlSetInfor(id, IPLCTRL_WDR_CAP_EN, DISABLE);
	}

	if(ctrl_info->chg_mode->func_en & IPL_FUNC_DEFOG ) {
		IPL_CtrlSetInfor(id, IPLCTRL_DEFOG_CAP_EN, ENABLE);
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_DEFOG_CAP_EN, DISABLE);
	}

	IPL_DzoomGetSIECropSize(id, IPL_DzoomGetIndex(id), TRUE, &width, &height);

	width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), ctrl_info->info->mode->ratio.ratio_h_v, width, 8);
	height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), ctrl_info->info->mode->ratio.ratio_h_v, height, 4);

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CAPCROP_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CAPCROP_SIZE_V, height);

	//when ccir split enable, sie output >> 1
	if(ctrl_info->info->mode->dvi != NULL) {
		if(ctrl_info->info->mode->dvi->out_split_en == TRUE) {
			width = width >> 1;
		} else {
			//do nothing
		}
	} else {
		//do nothing
	}

	if (event_id == IPL_FLOW_EB_GETRAW) {
		BitNum = IPL_RAW_BIT_12;
	} else if (ctrl_info->info->mode->mode_type == SENSOR_MODE_CCIR || ctrl_info->info->mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
		BitNum = IPL_RAW_BIT_8;
	} else {
		BitNum = IPL_AlgGetUIInfo(id, IPL_SEL_RAW_BITDEPTH_CAP);
	}

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM, 1);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAPBIT, BitNum);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V, height);
	if (IPL_CtrlGetInfor(id, IPLCTRL_CAPFLOW) == IPLCTRL_CAP_FLOW_EBURST) {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS,  IPL_ALIGN_ROUNDUP(IPL_UTI_RAWH_CONV2_LOFS(width, BitNum)*IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAWENC_BUF_RATIO) / 100, 4));
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS,  IPL_UTI_RAWH_CONV2_LOFS(width, BitNum));
	}

	// CA information
	if(ctrl_info->info->mode->mode_type == SENSOR_MODE_LINEAR) {
		if(event_id == IPL_FLOW_NOR_GETRAW) {
			AWB_GetCAInfo(id, &CAInfo);
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_EN, ENABLE);
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_WINXNUM, CAInfo.WinNumX);
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_WINYNUM, CAInfo.WinNumY);
		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_EN, DISABLE);
		}
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_EN, DISABLE);
	}

	switch (ctrl_info->info->mode->stpix) {
	case SENSOR_STPIX_R:
	case SENSOR_STPIX_GR:
	case SENSOR_STPIX_GB:
	case SENSOR_STPIX_B:
		bayer_type = BAYER_FORMAT;
		break;

	case SENSOR_STPIX_RGBIR_RG:
	case SENSOR_STPIX_RGBIR_BG:
		bayer_type = RGBIR_FORMAT;
		break;

	case SENSOR_STPIX_RCCB_RC:
	case SENSOR_STPIX_RCCB_CR:
	case SENSOR_STPIX_RCCB_CB:
	case SENSOR_STPIX_RCCB_BC:
		bayer_type = RCCB_FORMAT;
		break;

	default:
		DBG_ERR("bayer type unkonw (start pix = %d)\r\n", ctrl_info->info->mode->stpix);
		break;
	}
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_DATAFMT, bayer_type);
	IPL_CtrlSetInfor(id, IPLCTRL_SENSOR_TYPE, ctrl_info->info->mode->mode_type);

}

void IPL_CtrlPrvInit(UINT32 id, IPLCTRL_INFO *ctrl_info)
{
	switch (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW)) {
	case ICF_FLOW_C:
		IPL_CtrlPrvInitFlowCommon(id, ctrl_info);
		break;

	case ICF_FLOW_CCIR:
		IPL_CtrlPrvInitFlowCCIR(id, ctrl_info);
		break;

	case ICF_FLOW_VIRTUAL:
		IPL_CtrlPrvInitFlowVirtual(id, ctrl_info);
		break;

	case ICF_FLOW_DIRECT:
		IPL_CtrlPrvInitFlowDirect(id, ctrl_info);
		break;

	default:
		DBG_ERR("IPL_ID_%d, unsupport IPL flow %d\r\n", id+1, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW));
		break;
	}
}

#if 0
#endif

IPL_HAL_PARASET IPL_CtrlGetHalSetId(UINT32 id)
{

	IPL_MODE mode;

	mode = IPL_CtrlGetInfor(id, IPLCTRL_CUR_MODE);

	if ((IPLCTRL_CAP_FLOW_INVID == IPL_CtrlGetInfor(id, IPLCTRL_CAPFLOW)) && (mode == IPL_MODE_CAP)) {
		if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_REV1 || IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) >= ICF_MAX) {
			DBG_ERR("IPL_ID_%d, unsupport IPL flow %d\r\n", id+1, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW));
			return IPL_HAL_PAR_CAP;
		} else {
			return IPL_CtrlGetHalSetIdFlow(id);
		}
	} else {
		if (mode == IPL_MODE_CAP) {
			return IPL_HAL_PAR_CAP;
		} else {
			if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_REV1 || IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) >= ICF_MAX) {
				DBG_ERR("IPL_ID_%d, unsupport IPL flow %d\r\n", id+1, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW));
				return IPL_HAL_PAR_CAP;
			} else {
				return IPL_CtrlGetHalSetIdFlow(id);
			}
		}
	}
}

ER IPL_Ctrl_Sen_ChgMode(UINT32 id, SEN_CHGMODE_DATA sen_mode_data)
{
	ER rt;
	rt = sensor_chg_mode(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), sen_mode_data);
	iq_init(id);
	return rt;
}

IPLBUF_MODE IPL_MapICF2BufMode(IPLCTRL_FLOW flow)
{
	if (flow == ICF_REV1 || flow >= ICF_MAX) {
		DBG_ERR("flow map fail %d\r\n", flow);
		return IPLBUF_MODE_NULL;
	} else {
		return IPLBUF_MODE_FLOW_STREAM;
	}
}

void IPL_CtrlSetFlipType(IPL_PROC_ID Id, FLIP_TYPE fliptype)
{
	FLIP_TYPE dsp_flip = FLIP_NONE;
	SENSOR_OPTION sen_flip = SENSOR_OPTION_NONE;
    SEN_STATUS_PARAM sensor_status = {0};
    ER rt = E_OK;

	if (IPL_AlgGetUIInfo(Id, IPL_SEL_FORCE_SEN_FLIP) == TRUE) {
		if (fliptype == FLIP_H) {
			sen_flip = SENSOR_OPTION_MIRROR;
		} else if (fliptype == FLIP_V) {
			sen_flip = SENSOR_OPTION_FLIP;
		} else if (fliptype == FLIP_H_V) {
			sen_flip = SENSOR_OPTION_ALL;
		} else {
			sen_flip = SENSOR_OPTION_NONE;
		}
	} else {
		//get sen flip_ability here
		if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_DIRECT) {
		    sensor_status.sensor_mode = SENSOR_MODE_UNKNOWN;
		    rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sensor_status);
			if (rt == E_OK) {
				switch (fliptype) {
				case FLIP_NONE:
					dsp_flip = FLIP_NONE;
					sen_flip = SENSOR_OPTION_NONE;
					break;

				case FLIP_H:
					if (sensor_status.sensor_info.property & SENSOR_SUPPORT_MIRROR) {
						sen_flip = SENSOR_OPTION_MIRROR;
					} else {
						dsp_flip = FLIP_H;
					}
					break;

				case FLIP_V:
					if (sensor_status.sensor_info.property & SENSOR_SUPPORT_FLIP) {
						sen_flip = SENSOR_OPTION_FLIP;
					} else {
						dsp_flip = FLIP_V;
					}
					break;

				case FLIP_H_V:
					if (sensor_status.sensor_info.property & (SENSOR_SUPPORT_MIRROR|SENSOR_SUPPORT_FLIP)) {
						sen_flip = SENSOR_OPTION_MIRROR|SENSOR_OPTION_FLIP;
					} else {
						dsp_flip = FLIP_H_V;
					}
					break;

				default:
					dsp_flip = FLIP_NONE;
					sen_flip = SENSOR_OPTION_NONE;
					DBG_ERR("IPL_ID_%d, Flip type error %d\r\n", Id+1, fliptype);
					break;
				}
			} else {
				DBG_ERR("IPL_ID_%d, Get Sensor Status fail, rt:%d, force use dsp mirror/flip\r\n", Id+1, rt);
				dsp_flip = fliptype;
			}
		} else {
			dsp_flip = fliptype;
		}
	}
	IPL_CtrlSetInfor(Id, IPLCTRL_IPL_FLIP_TYPE, dsp_flip);
	IPL_CtrlSetInfor(Id, IPLCTRL_SEN_FLIP_TYPE, sen_flip);
}

static BOOL IPL_CtrlChkOutEn(UINT32 id, IPL_IME_PATH_ID path_idx)
{
	UINT32 Pxidx[IPL_IME_PATH_ID_MAX] = {IPLCTRL_IME_P1_EN,	IPLCTRL_IME_P2_EN,	IPLCTRL_IME_P3_EN,	IPLCTRL_IME_P4_EN, 0};

	if (path_idx < IPL_IME_PATH_ID_Ref) {
		if (IPL_CtrlGetInfor(id, Pxidx[path_idx]) == ENABLE) {
			return TRUE;
		}
	} else {
		DBG_ERR("id: %d, path error %d\r\n", id, path_idx);
	}
	return FALSE;
}


static BOOL IPL_CtrlChkOutSizeValid(UINT32 id, IPL_IME_PATH_ID path_idx, BOOL compress_en, IPL_YCC_CH_INFO *size_info)
{
	UINT32 lofs = 0;
	UINT32 Pxidx[IPL_IME_PATH_ID_MAX][3] = {
		{IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS},
		{IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS},
		{IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS},
		{IPLCTRL_IME_P4_OUT_CROP_H, IPLCTRL_IME_P4_OUT_CROP_V, IPLCTRL_IME_P4_OUT_SIZE_HLOS},
		{0,0,0},
	};

	if (path_idx < IPL_IME_PATH_ID_Ref) {
		if (IPL_CtrlGetInfor(id, Pxidx[path_idx][2]) == IPL_CTRL_NO_USE) {
				lofs = 0;
		} else {
			if (path_idx == IPL_IME_PATH_ID_1 && compress_en == ENABLE) {
				lofs = ALIGN_CEIL((IPL_CtrlGetInfor(id, Pxidx[path_idx][2]) * 3) / 4, 16);
			} else {
				lofs = IPL_CtrlGetInfor(id, Pxidx[path_idx][2]);
			}
		}

		if (size_info->width == IPL_CtrlGetInfor(id, Pxidx[path_idx][0]) && size_info->height == IPL_CtrlGetInfor(id, Pxidx[path_idx][1]) && size_info->line_ofs == lofs) {
			return TRUE;
		}
	} else {
		DBG_ERR("id: %d, path error %d\r\n", id, path_idx);
	}
	return FALSE;
}

static int IPL_comp_path_size(const void *a, const void *b)
{
	if (((IPL_PATH_SORT_INFO *)a)->out_size.w * ((IPL_PATH_SORT_INFO *)a)->out_size.h > ((IPL_PATH_SORT_INFO *)b)->out_size.w * ((IPL_PATH_SORT_INFO *)b)->out_size.h) {
		return 1;
	} else if (((IPL_PATH_SORT_INFO *)a)->out_size.w * ((IPL_PATH_SORT_INFO *)a)->out_size.h < ((IPL_PATH_SORT_INFO *)b)->out_size.w * ((IPL_PATH_SORT_INFO *)b)->out_size.h) {
		return -1;
	}
	return 0;
}

void IPL_Ctrl_Out_Buf_Cfg(IPL_PROC_ID id, IPL_BUF_IO_CFG buf_io_cfg, IPL_YCC_ADDR_INFO *path[])
{
	IPL_YCC_IMG_INFO ycc_info;
	UINT32 i, start_addr = 0, img_size = 0, adas_buf_size = 0;
	IPL_IMG_BUF_INFO buf_info;
	IPL_IMG_OUT_INFO img_out_info = {0};
	HEADER_YUV_DATA *pyuv_header_out;
	static BOOL tmnr_ref_ppb[IPL_ID_MAX_NUM] = {FALSE};
	IPL_PATH_SORT_INFO ime_path_sort[IPL_IME_PATH_ID_MAX] = {0};

	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR:
		for (i = 0 ; i < IPL_IME_PATH_ID_MAX ; i++) {
			if (path[i] != NULL) {
				if (i == IPL_IME_PATH_ID_Ref) {
					if (IPL_CtrlGetInfor(id, IPLCTRL_IME_TMNR_EN) == ENABLE) {
						if (IPL_CtrlGetInfor(id, IPLCTRL_IME_TMNR_REF_BUF_NUM) == IPL_SINGLE_STRP_IME_TMNR_REF_PPB) {
							IPL_BufGet(id, IPLBUF_IME_TMNR_REF_OUT_1, &start_addr, &img_size);
						} else {
							if (tmnr_ref_ppb[id] == FALSE) {
								IPL_BufGet(id, IPLBUF_IME_TMNR_REF_OUT_1, &start_addr, &img_size);
							} else {
								IPL_BufGet(id, IPLBUF_IME_TMNR_REF_OUT_2, &start_addr, &img_size);
							}
							tmnr_ref_ppb[id] = !tmnr_ref_ppb[id];
						}
					} else {
						start_addr = 0;
					}
					img_out_info.buf_handle = start_addr;
				} else {
					if (IPL_CtrlChkOutEn(id, i) == TRUE) {
						//image buffer
						img_size = IPL_ALIGN_ROUNDUP(IPL_YUV_SIZE(path[i]->type, path[i]->size.line_ofs, path[i]->size.height), 4);
						if (i == IPL_IME_PATH_ID_4 && IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EXTOUT_EN) == ENABLE) {
							adas_buf_size = (IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EDGEMAP_BUFOFS) + IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_HISTO_BUFOFS));
						} else {
							adas_buf_size = 0;
						}
						img_out_info.Path_id = i;
						start_addr = IPL_BufIOCB_Proc(id, buf_io_cfg, img_size + path[i]->ext_buf_size + adas_buf_size, &img_out_info);
					} else {
						start_addr = 0;
					}
				}
				if (start_addr == 0) {
					//ext address
					path[i]->addr_start = 0;
					path[i]->ext_addr = 0;
					path[i]->buf_handle = 0;
					buf_info.Buf[0] = 0;
				} else {
					// do not clean buffer, in case single buffer get incorrect header information
					//memset((UINT32 *)start_addr, 0, total_dbg_data_size);   //total 1024 byte for debug information
					//yuv header address
					if (sizeof(HEADER_YUV_DATA) <= (total_dbg_data_size-sys_dbg_data_size)) {
						path[i]->pixel_yuv_header = start_addr + sys_dbg_data_size;
					} else {
						DBG_ERR("IPL_ID_%d, Path: %d, YUV Header buffer allocate fail, header size: %d > dbg_data_size %d \r\n", id+1, img_out_info.Path_id + 1, sizeof(HEADER_YUV_DATA), (total_dbg_data_size-sys_dbg_data_size));
					}
					//ext address
					path[i]->ext_addr = start_addr + total_dbg_data_size;
					buf_info.Buf[0] = start_addr + total_dbg_data_size + path[i]->ext_buf_size;

					//start address
					path[i]->addr_start = start_addr;
					path[i]->buf_handle = img_out_info.buf_handle;
				}

				//ycc address
				ycc_info.type = path[i]->type;
				ycc_info.ch[0] = path[i]->size;
				ycc_info.ch[1] = IPL_UTI_Y_INFO_CONV2(ycc_info.type, ycc_info.ch[0]);
				buf_info.Buf[1] = 0;
				buf_info.Buf[2] = 0;
				IPL_UTI_SET_CONT_BUFINFO(ycc_info.type, buf_info, &ycc_info);
				path[i]->pixel_addr[0] = ycc_info.pixel_addr[0];
				path[i]->pixel_addr[1] = ycc_info.pixel_addr[1];
				path[i]->pixel_addr[2] = ycc_info.pixel_addr[2];
				if (i == IPL_IME_PATH_ID_4 && IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EXTOUT_EN) == ENABLE) {
					path[i]->pixel_addr[1] = path[i]->pixel_addr[0] + img_size;	//Edge map
					path[i]->pixel_addr[2] = path[i]->pixel_addr[1] + IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EDGEMAP_BUFOFS);	//Histogram buffer
				}
				IPL_YUVBufStsDump(id, buf_io_cfg, path[i], &img_out_info);	//dump debug info
			}
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR:
		for (i = 0 ; i < IPL_IME_PATH_ID_MAX ; i++) {
			if (path[i] != NULL) {
				pyuv_header_out = (HEADER_YUV_DATA *) path[i]->pixel_yuv_header;
				ime_path_sort[i].path_idx = i;
				ime_path_sort[i].out_size.w = pyuv_header_out->img_info.ch[0].width;
				ime_path_sort[i].out_size.h = pyuv_header_out->img_info.ch[0].height;
			} else {
				ime_path_sort[i].path_idx = IPL_IME_PATH_ID_MAX;
			}
		}

		if (IPL_CtrlGetInfor(id, IPLCTRL_IME_PUSH_SORT_EN) == ENABLE) {
			qsort(ime_path_sort, IPL_IME_PATH_ID_MAX, sizeof(IPL_PATH_SORT_INFO), IPL_comp_path_size);
		}

		for (i = 0 ; i < IPL_IME_PATH_ID_MAX ; i++) {
			if (ime_path_sort[i].path_idx < IPL_IME_PATH_ID_MAX) {
				if (IPL_CtrlGetInfor(id, IPLCTRL_IME_TMNR_EN) == ENABLE && ime_path_sort[i].path_idx == IPL_IME_PATH_ID_Ref) {
				} else {
					memset(&img_out_info, 0, sizeof(IPL_IMG_OUT_INFO));   //total 1024 byte for debug information
					pyuv_header_out = (HEADER_YUV_DATA *) path[ime_path_sort[i].path_idx]->pixel_yuv_header;
					img_out_info.Path_id = ime_path_sort[i].path_idx;
					img_out_info.buf_handle = path[ime_path_sort[i].path_idx]->buf_handle;
					img_out_info.out_ycc_compress_en = pyuv_header_out->ycc_compress_en;
					img_out_info.raw_framecnt = pyuv_header_out->raw_info.framecnt[0];
					img_out_info.vd_timestamp = pyuv_header_out->raw_info.timestamp[0];
					img_out_info.dramend_timestamp = pyuv_header_out->raw_info.dramend_timestamp[0];
					img_out_info.ipl_timestamp_start = pyuv_header_out->ipl_timestamp_start;
					img_out_info.ipl_timestamp_end = pyuv_header_out->ipl_timestamp_end;
					img_out_info.ipl_framecnt = pyuv_header_out->ipl_framecnt;
					if (pyuv_header_out->status & (1 << IPL_HEADER_YUV_IMG_INFO)) {
						img_out_info.img = pyuv_header_out->img_info;
						img_out_info.bsize_valid = IPL_CtrlChkOutSizeValid(id, img_out_info.Path_id, pyuv_header_out->ycc_compress_en, &pyuv_header_out->img_info.ch[0]);
					}
					if (pyuv_header_out->status & (1 << IPL_HEADER_YUV_STRIPE_INFO)) {
						img_out_info.str_info_addr = (UINT32)&pyuv_header_out->stripe_info;
					}
					if (pyuv_header_out->status & (1 << IPL_HEADER_YUV_MD_INFO)) {
						img_out_info.ime_md_addr = (UINT32)&pyuv_header_out->ime_md_info;
					} else {
						img_out_info.ime_md_addr = 0xffffffff;	//set md not ready for default value
					}
					IPL_YUVBufStsDump(id, buf_io_cfg, path[ime_path_sort[i].path_idx], &img_out_info);	//dump debug info
					//push buffer out
					if ((pyuv_header_out->status & (1 << IPL_HEADER_YUV_IMG_INFO)) && (dma_isCacheAddr(img_out_info.img.pixel_addr[0]))) {
						DBG_WRN("IPL_ID_%d, ime path %d, buffer addr = %x, should be non_cacheable\r\n", id+1, ime_path_sort[i].path_idx, img_out_info.img.pixel_addr[0]);
					} else {
					    IPL_BufIOCB_Proc(id, buf_io_cfg, path[ime_path_sort[i].path_idx]->addr_start, &img_out_info);
					}
				}
			}
		}
		dis_push_time_stamp(img_out_info.ipl_timestamp_start,img_out_info.ipl_framecnt);

		break;

	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		for (i = 0 ; i < IPL_IME_PATH_ID_MAX ; i++) {
			if (path[i] != NULL) {
				if (IPL_CtrlGetInfor(id, IPLCTRL_IME_TMNR_EN) == ENABLE && i == IPL_IME_PATH_ID_Ref) {
				} else {
					img_out_info.Path_id = i;
					img_out_info.buf_handle = path[i]->buf_handle;

					IPL_YUVBufStsDump(id, buf_io_cfg, path[i], &img_out_info);	//debug

                    IPL_BufIOCB_Proc(id, buf_io_cfg, path[i]->addr_start, &img_out_info);
				}
			}
		}
		break;

	default:
		break;
	}
}

#if 0
#endif

void IPL_Stream2PauseFCB(IPL_MODE_DATA **ipl_mode_info, IPL_MODE_DATA *chg_mode, UINT32 *trig_flg)
{
	UINT32 id, i;
	SEN_STATUS_PARAM sen_status = {0};
	IPLCTRL_INFO ctrl_info = {0};
	IPC_CTRL_CMD trig_stop_cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	IPC_CTRL_CMD trig_pause_cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	static IPL_MODE prv_mode[IPL_ID_MAX_NUM] = {IPL_MODE_OFF}; //Previous IPL Mode
	IPL_PROC_BIT_ID ipl_stop_id_bit = 0, ipl_pause_id_bit = 0;
	ER rt = E_OK;

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("\r\nS2P-> PRV->Nxt(Act):\r\n%s->%s(%d),%s->%s(%d)\r\n",
				 ipl_dbg_mode_tbl[prv_mode[0]], ipl_dbg_mode_tbl[chg_mode[0].mode], chg_mode[0].act_flg,
				 ipl_dbg_mode_tbl[prv_mode[1]], ipl_dbg_mode_tbl[chg_mode[1].mode], chg_mode[1].act_flg);
	}

	for (i = 0 ; i < IPL_ID_MAX_NUM ; i++) {
		if (chg_mode[i].act_flg == TRUE) {
			switch (chg_mode[i].mode) {
			case IPL_MODE_OFF:
				trig_flg[i] = IPL_OFF;
				break;

			case IPL_MODE_PREVIEW:
			case IPL_MODE_VIDEO:
			case IPL_MODE_VIDEOREC:
				IPL_CtrlSetInfor(IPL_ID_1, IPLCTRL_IME_OUT_GROUP_BUF_ARRANGE, chg_mode->ime_path1_out_group.group_en);//Jarkko tmp, not ready
				if (prv_mode[i] == IPL_MODE_CAP) {
					trig_flg[i] = IPL_RESUME;
				} else if (prv_mode[i] == IPL_MODE_OFF || prv_mode[i] == IPL_MODE_BUF_QUERY) {
					trig_flg[i] = IPL_START;
				} else {
					trig_flg[i] = IPL_STOP;
				}
				break;

			case IPL_MODE_CAP:
				trig_flg[i] = IPL_PAUSE;
				gCapFlg = TRUE;
				break;

			default:
				trig_flg[i] = IPL_NONE;
				break;
			}
			prv_mode[i] = chg_mode[i].mode;
		}
	}

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("\r\nS2P-> Cur->Nxt(Trg):\r\n%s->%s(%s),%s->%s(%s)\r\n\r\n",
				 ipl_dbg_mode_tbl[prv_mode[0]], ipl_dbg_mode_tbl[chg_mode[0].mode], ipl_dbg_trigflg_tbl[trig_flg[0]],
				 ipl_dbg_mode_tbl[prv_mode[1]], ipl_dbg_mode_tbl[chg_mode[1].mode], ipl_dbg_trigflg_tbl[trig_flg[1]]);
	}

	//stop all other ipl when capture
	for (id = 0 ; id < IPL_ID_MAX_NUM ; id++) {
		if (gCapFlg == TRUE) {
			if (prv_mode[id] == IPL_MODE_PREVIEW || prv_mode[id] == IPL_MODE_VIDEO || prv_mode[id] == IPL_MODE_VIDEOREC) {
				trig_flg[id] = IPL_STOP;
			}
		}

		//RSC CB MSG
		IPL_RSC_CBMsgProc(id, IPL_RSC_CBMSG_CHGMODE_START, NULL);

		switch (trig_flg[id]) {
		case IPL_STOP:
		case IPL_PAUSE:
			//pause AE/AWB/AF/IQ/ICFFunc
			if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_C || IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_DIRECT) {
				if (IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) == id) {
					AE_Pause(IPL_UTI_CONV2_AE_ID(id), ENABLE);
					AWB_Pause(IPL_UTI_CONV2_AWB_ID(id), ENABLE);
#if IPL_AF_FUNC_SUPPORT
					AF_Pause(IPL_UTI_CONV2_AF_ID(id), ENABLE);
#endif
					IQS_Pause(IPL_UTI_CONV2_IQS_ID(id), ENABLE);
					ICFFunc_Pause(id, ENABLE);
					ExtFunc_UnInit(id);
				}
			} else if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR) {
#if IPL_AF_FUNC_SUPPORT
				AF_Pause(IPL_UTI_CONV2_AF_ID(id), ENABLE);
#endif
				IQS_Pause(IPL_UTI_CONV2_IQS_ID(id), ENABLE);
			}

			if (trig_flg[id] == IPL_STOP) {
				ipl_stop_id_bit |= 1 << id;
				trig_stop_cmd[id] = IPC_STOP_ALL;
				trig_flg[id] = IPL_RESTART;
			} else {
				if (chg_mode[id].event_id == IPL_FLOW_VID_GETRAW) {
					if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
						//initial capture parameter
						sen_status.sensor_mode = IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_SEN_MODE);
						rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
						if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
							DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_SEN_MODE));
						}
					}

					ctrl_info.info = &sen_status.sensor_info;
					ctrl_info.chg_mode = &chg_mode[id];
					IPL_CtrlCapInit(chg_mode[id].proc_sen_id, chg_mode[id].event_id, &ctrl_info);
					chg_mode[id].buf_size = 0;
				} else {
					if (IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) == id) {
						//stop IPL
						trig_pause_cmd[chg_mode[id].proc_sen_id] = IPC_STOP_YCC;
						ipl_pause_id_bit = IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT);
						IPL_Ctrl_Run(ipl_pause_id_bit, &trig_pause_cmd[IPL_ID_1]);

						if (IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_PRVFLOW) != ICF_FLOW_DIRECT) {
							IPL_Wait_Vd(ipl_pause_id_bit, FALSE, IPL_TIMEOUT_MS);
							trig_pause_cmd[chg_mode[id].proc_sen_id] = IPC_PAUSE_SIE;
							ipl_pause_id_bit = IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT);
							IPL_Ctrl_Run(ipl_pause_id_bit, &trig_pause_cmd[IPL_ID_1]);
						}

						trig_pause_cmd[chg_mode[id].proc_sen_id] = IPC_IGNORE;

						IPL_RSC_CBMsgProc(chg_mode[id].proc_sen_id, IPL_RSC_CBMSG_CHGMODE_IPLSTOP, NULL);
					}
				}
				trig_flg[id] = IPL_RESUME;
			}
			break;
		case IPL_OFF:
			ipl_stop_id_bit |= 1 << id;
			trig_stop_cmd[id] = IPC_STOP_ALL;

			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) == id) {
				if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_CCIR) {
					AE_Close(IPL_UTI_CONV2_AE_ID(id));
					AWB_Close(IPL_UTI_CONV2_AWB_ID(id));
					ICFFunc_Close(id);
					ExtFunc_UnInit(id);
				}
#if IPL_AF_FUNC_SUPPORT
				AF_Close(IPL_UTI_CONV2_AF_ID(id));
#endif
				IQS_Close(IPL_UTI_CONV2_IQS_ID(id));
			}
			break;

		default:
			ipl_stop_id_bit |= 1 << id;
			trig_stop_cmd[id] = IPC_IGNORE;
			break;
		}
	}
	IPL_Ctrl_Run(ipl_stop_id_bit, &trig_stop_cmd[IPL_ID_1]);
	//IPL STOP MSG
	for (id = 0 ; id < IPL_ID_MAX_NUM ; id++) {
		if (trig_stop_cmd[id] != IPC_IGNORE) {
			IPL_RSC_CBMsgProc(id, IPL_RSC_CBMSG_CHGMODE_IPLSTOP, NULL);
		}
	}
}

void IPL_Pause2StreamFCB(IPL_MODE_DATA **ipl_mode_info, IPL_MODE_DATA *chg_mode, UINT32 *trig_flg, IPC_CTRL_CMD *proc_cmd)
{
	UINT32 zoom_idx[IPL_ID_MAX_NUM] = {0};
	IPL_ISR_OBJ_INFO IsrObjInfo = {0};
	IPC_INFO IpcInfo = {0};
	IPLCTRL_INFO ctrl_info = {0};
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 *zoom_tab, zoom_tab_max = 0;
	UINT32 id;
	IPL_PROC_BIT_ID ipl_proc_id_bit = 0;
	UINT32 Group = 0, Order = 0;
	SEN_CHGMODE_DATA sen_chg_mode = {0};
	IPL_BUF_OBJ_INFO buf_obj;
	IPLBUF_ENG2DRAMINFO buf_info;
	IPLBUF_RST buf_rst = {0};
	ER rt = E_OK;

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("\r\nP2S-> Cur->Nxt(Act):\r\n%s->%s(%s),%s->%s(%s)\r\n\r\n",
				 ipl_dbg_mode_tbl[ipl_mode_info[0]->mode], ipl_dbg_mode_tbl[chg_mode[0].mode], ipl_dbg_trigflg_tbl[trig_flg[0]],
				 ipl_dbg_mode_tbl[ipl_mode_info[1]->mode], ipl_dbg_mode_tbl[chg_mode[1].mode], ipl_dbg_trigflg_tbl[trig_flg[1]]);
	}

	if (gCapFlg == TRUE) {
		gCapFlg = FALSE;
		return;
	}

	for (id = 0 ; id < IPL_ID_MAX_NUM ; id++) {
		switch (trig_flg[id]) {
		case IPL_START:
		case IPL_RESTART:
			if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
				//change sensor mode
				sen_chg_mode.mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
				sen_chg_mode.multi_id = IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT);
				sen_chg_mode.frame_rate = IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_FPS);
				IPL_Ctrl_Sen_ChgMode(id, sen_chg_mode);

				//get mode status
				sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
				rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
				if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
					DBG_ERR("IPL_ID_%d Get Sensor Status Fail, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)+1, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
					return;
				}
				zoom_tab = ipl_ext_senmode2tbl(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), &zoom_tab_max);
				IPL_DZoomSetInfor(id, zoom_tab, zoom_tab_max);
			}

			ctrl_info.info = &sen_status.sensor_info;
			chg_mode[id] = *ipl_mode_info[id];
			ctrl_info.chg_mode = &chg_mode[id];
			IPL_CtrlPrvInit(id, &ctrl_info);

			//allocate buffer & initial buffer size
			buf_obj.id = ctrl_info.chg_mode->proc_sen_id;
			buf_obj.buf_addr = ctrl_info.chg_mode->buf_addr;
			buf_obj.buf_size = ctrl_info.chg_mode->buf_size;

			IPL_BufInit(&buf_obj);
			IPL_CtrlInitPrvbuf_info(&chg_mode[id], &buf_info);
			buf_rst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW)), &buf_info);

			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("IPL_ID_%d_FUNC_EN = 0x%x\r\n", chg_mode->proc_sen_id+1, chg_mode->func_en);
				IPL_BufDump(buf_obj.id); //DEBUG Using, use for dump buffer information
			}
			ctrl_info.chg_mode->buf_size = buf_rst.buf_size;

			if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
				//initial digital zoom
				zoom_idx[id] = IPL_DzoomGetIndex(id);
				IPL_DzoomSetIndex(id, zoom_idx[id]);
				IPL_CtrlPrvSetDZoomInfo(id);
			}
			IPL_CtrlInitPrvIpcInfo(id, &IpcInfo, &IsrObjInfo, &chg_mode[id]);

			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_NUM) > 1) {
				Order = IPL_UTI_CONV2_HDR_ORDER(id, IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT));
				Group = IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID);
			} else {
				Order = IPL_CtrlGetInfor(id, IPLCTRL_ORDER_IDX);
				Group = IPL_CtrlGetInfor(id, IPLCTRL_GROUP_IDX);
			}

			IPL_Ctrl_Cfg(Group, id, Order, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_Ctrl_Out_Buf_Cfg);
			ipl_proc_id_bit |= 1 << id;
			proc_cmd[id] = IPC_START_RUN;
			break;

		case IPL_RESUME:
			IPL_CtrlInitPrvIpcInfo(id, &IpcInfo, &IsrObjInfo, &chg_mode[id]);
			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_NUM) > 1) {
				Order = IPL_UTI_CONV2_HDR_ORDER(id, IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT));
				Group = IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID);
			} else {
				Order = IPL_CtrlGetInfor(id, IPLCTRL_ORDER_IDX);
				Group = IPL_CtrlGetInfor(id, IPLCTRL_GROUP_IDX);
			}

			IPL_Ctrl_Cfg(Group, id, Order, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_Ctrl_Out_Buf_Cfg);
			ipl_proc_id_bit |= 1 << id;
			proc_cmd[id] = IPC_START_RUN;
			break;

		case IPL_OFF:
			proc_cmd[id] = IPC_STOP_ALL;
			break;

		default:
			break;
		}
	}

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("\r\nP2S-> Trig Bit 0x%x, Cmd %d ,%d\r\n",
				 ipl_proc_id_bit,
				 proc_cmd[IPL_ID_1], proc_cmd[IPL_ID_2]);
	}

	IPL_Ctrl_Run(ipl_proc_id_bit, &proc_cmd[IPL_ID_1]);

	for (id = IPL_ID_1; id < IPL_ID_MAX_NUM ; id++) {
		if (proc_cmd[id] == IPC_START_RUN) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
				if (IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) == id) {
					ipl_proc_id_bit = 1 << id;
					IPL_Wait_Ime_Fmd(ipl_proc_id_bit, TRUE, IPL_TIMEOUT_MS);

					IPL_CBMsgProc(id, IPL_CBMSG_PREVIEW, NULL);
					//Trigger RSC
					IPL_RSC_CBMsgProc(id, IPL_RSC_CBMSG_CHGMODE_END, NULL);

					if (trig_flg[id] != IPL_START) {
						if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_CCIR) {
							AE_Resume(IPL_UTI_CONV2_AE_ID(id));
							AWB_Resume(IPL_UTI_CONV2_AWB_ID(id));
							ICFFunc_Resume(id);
						}
#if IPL_AF_FUNC_SUPPORT
						AF_Resume(IPL_UTI_CONV2_AF_ID(id));
#endif
						IQS_Resume(IPL_UTI_CONV2_IQS_ID(id));
					}
				}
			}
		}
		trig_flg[id] = IPL_NONE;
	}
}

void IPL_YUVBufStsDump(IPL_PROC_ID id, IPL_BUF_IO_CFG buf_io_cfg, IPL_YCC_ADDR_INFO *path, IPL_IMG_OUT_INFO *img_out_info)
{
	static UINT8 dump_buf_cnt[IPL_ID_MAX_NUM] = {0};

	if (ipl_yuv_buf_dump_id == id) {
		switch (buf_io_cfg) {
		case IPL_BUF_IO_GET_NEXT_OUT_ADDR:
			dump_buf_cnt[id]++;
			DBG_DUMP("[G] Id:%d, p:%d, comp:%d, addr:%#x, lofs:%d\r\n",
				id,
				img_out_info->Path_id + 1,
				img_out_info->out_ycc_compress_en,
				path->addr_start,
				path->size.line_ofs);
			break;

		case IPL_BUF_IO_PUSH_READY_ADDR:
			DBG_DUMP("[P] Id:%d, p:%d, comp:%d, addr:%#x, y_addr:%#x, lofs:%d, fmt:%d, md:%x, valid: %d\r\n",
				id,
				img_out_info->Path_id + 1,
				img_out_info->out_ycc_compress_en,
				path->addr_start,
				path->pixel_addr[0],
				path->size.line_ofs,
				img_out_info->img.type,
				img_out_info->ime_md_addr,
				img_out_info->bsize_valid);
			break;

		case IPL_BUF_IO_LOCK_BUF:
			DBG_DUMP("[L] id:%d,p:%d,adr=%x\r\n", id, img_out_info->Path_id+1, path->addr_start);
			break;
		case IPL_BUF_IO_UNLOCK_BUF:
			DBG_DUMP("[UnL] id:%d,p:%d,adr=%x\r\n", id, img_out_info->Path_id+1, path->addr_start);
			break;

		case IPL_BUF_IO_START:
			DBG_DUMP("[Start] id:%d,p:%d\r\n", id, img_out_info->Path_id+1);
			break;
		case IPL_BUF_IO_STOP:
			DBG_DUMP("[Stop] id:%d,p:%d\r\n", id, img_out_info->Path_id+1);
			break;

		default:
			break;
		}
	}
	if (dump_buf_cnt[id] >= ipl_yuv_buf_dump_cnt) {
		ipl_yuv_buf_dump_id = IPL_ID_MAX_NUM;
		dump_buf_cnt[id] = 0;
	}
}
