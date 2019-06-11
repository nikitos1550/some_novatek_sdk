/**
    Calibration item

    @file       CalibrationItem.c
    @ingroup    calibration
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#include "PrjCfg.h"
#if (CALIBRATION_FUNC == ENABLE)
// system
#include "stdio.h"
#include "Type.h"
#include "GxVideo.h"
#include "SysCfg.h"
#include "SysKer.h"
#include "PStore.h"
#include "FileSysTsk.h"
#include "GxImage.h"
#include "EngineerMode.h"
#include "KeyDef.h"
// cal
#include "ipl_calibration.h"
#include "ipl_cal_api.h"
#include "ipl_cal_obj.h"
#include "ipl_cal_obj_drv.h"
#include "ipl_cal_buf.h"
#include "ipl_cal_rw.h"
#include "ipl_cal_alg.h"
#include "ipl_cal_debug.h"
#include "cal_header_int.h"
#include "calibration_item.h"
#include "cal_ui_sample.h"
// ipl
#include "ipl_utility.h"
#include "ipl_api.h"
#include "ipl_ini.h"
#include "ipl_header_infor_raw.h"

// user setting
CAL_UI_STYLE g_cal_ui_style = CAL_UI_STYLE_NORMAL;
UINT32 g_cal_id[IPL_ID_MAX_NUM] = {IPL_ID_1, CAL_END}; // set the IPL ID you want to do calibration
BOOL g_dp_bright_ony = FALSE;
BOOL g_b_saveraw = FALSE;
#define PRV_SHOW_VA DISABLE // TODO:only IPE support
#define CAL_UI_MAX_STR 100

// for internal flow use
UINT32 g_current_cal_id = IPL_ID_MAX_NUM;
BOOL g_b_cal_id[IPL_ID_MAX_NUM] = {FALSE, FALSE};
CAL_GETRAW_RST getraw_rst = {0};
CAL_ALG_PARAM alg_param = {0};
CAL_PRV_DATA prv_data_rst = {0};
CAL_PRV_DATA prv_data[IPL_MAX_SEN_NUM] = {0};

/**
    rw function point
*/
UINT32 cal_rw_store_op_open(char *p_sec_name, CAL_RW_OPEN_SEL open_sel);
ER cal_rw_store_op_close(UINT32 p_section_handle);
ER cal_rw_store_op_read(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle);
ER cal_rw_store_op_write(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle);
ER cal_rw_store_op_delete(char *p_sec_name);
UINT32 cal_rw_store_op_get_sec_size(char *p_sec_name);
CAL_RW_STORE_OP cal_rw_store_op = {
	cal_rw_store_op_open,
	cal_rw_store_op_close,
	cal_rw_store_op_read,
	cal_rw_store_op_write,
	cal_rw_store_op_delete,
	cal_rw_store_op_get_sec_size,
};


#if PRV_SHOW_VA
/**
    show VA for focus adjust
*/
#define CAL_VA_HW_WIN 16
#define CAL_VA_HW_IND_WIN 5
#define CAL_VA_WIN(x, y) (x + (y * CAL_VA_HW_WIN))
#define CAL_VA_SHOW_BLOCK_NUM 5
IPL_CUR_INPUT_INFO ipl_cur_info = {0};
IPL_SET_VA_DATA va_data = {0};
IPL_VA_INFOR va_info = {0};
IPE_HAL_INDEP_VA_WIN_RSLT cal_va_indep_win_rst = {0};
IPL_VA_GET_RST cal_va_rst = {0};
UINT32 cal_va_win_g1_h[CAL_VA_HW_WIN * CAL_VA_HW_WIN] = {0};
UINT32 cal_va_win_g1_v[CAL_VA_HW_WIN * CAL_VA_HW_WIN] = {0};
UINT32 cal_va_win_g2_h[CAL_VA_HW_WIN * CAL_VA_HW_WIN] = {0};
UINT32 cal_va_win_g2_v[CAL_VA_HW_WIN * CAL_VA_HW_WIN] = {0};
UINT32 cal_va_win_g1_hcnt[CAL_VA_HW_WIN * CAL_VA_HW_WIN] = {0};
UINT32 cal_va_win_g1_vcnt[CAL_VA_HW_WIN * CAL_VA_HW_WIN] = {0};
UINT32 cal_va_win_g2_hcnt[CAL_VA_HW_WIN * CAL_VA_HW_WIN] = {0};
UINT32 cal_va_win_g2_vcnt[CAL_VA_HW_WIN * CAL_VA_HW_WIN] = {0};
static void cal_focus_getva(void)
{
	UINT32 show_win_cnt;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];
	URECT show_rect_info;

	IPL_GetCmd(IPL_GET_VA_CUR_INFOR, &va_info);
#if 1 // 16x16 window
	IPOINT va_win_block[CAL_VA_SHOW_BLOCK_NUM] = {{2, 2}, {13, 2}, {8, 8}, {2, 13}, {13, 13}};
	for (show_win_cnt = 0; show_win_cnt < CAL_VA_SHOW_BLOCK_NUM; show_win_cnt++) {
		snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%d"
				 , (cal_va_win_g1_h[CAL_VA_WIN(va_win_block[show_win_cnt].x, va_win_block[show_win_cnt].y)] +
				 cal_va_win_g1_v[CAL_VA_WIN(va_win_block[show_win_cnt].x, va_win_block[show_win_cnt].y)])/2);
		show_rect_info.x = (cal_ui()->get_dispsize().w / CAL_VA_HW_WIN) * (va_win_block[show_win_cnt].x);
		show_rect_info.y = (cal_ui()->get_dispsize().h / CAL_VA_HW_WIN) * (va_win_block[show_win_cnt].y);
		show_rect_info.w = (cal_ui()->get_dispsize().w / CAL_VA_HW_WIN);
		show_rect_info.h = (cal_ui()->get_dispsize().h / CAL_VA_HW_WIN);
		cal_ui()->ui_showstring(show_win_cnt, cal_ui_msg, CAL_UICOL_RED, &show_rect_info, TRUE, TRUE);
	}
#else // independent
	for (show_win_cnt = 0; show_win_cnt < CAL_VA_HW_IND_WIN; show_win_cnt++) {
		if (va_data.va_indep_win[show_win_cnt].bEn) {
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%d"
					 , (cal_va_indep_win_rst.indep_win_rst[show_win_cnt].uiVaG1H +
					 cal_va_indep_win_rst.indep_win_rst[show_win_cnt].uiVaG1V) / 2);
			show_rect_info.x = (cal_ui()->get_dispsize().w * va_data.va_indep_win[show_win_cnt].uiWinStX) / (ipl_cur_info.in_size_h);
			show_rect_info.y = (cal_ui()->get_dispsize().h * va_data.va_indep_win[show_win_cnt].uiWinStY) / (ipl_cur_info.in_size_v);
			show_rect_info.w = (cal_ui()->get_dispsize().w * va_data.va_indep_win[show_win_cnt].uiWinSzX) / (ipl_cur_info.in_size_h);
			show_rect_info.h = (cal_ui()->get_dispsize().h * va_data.va_indep_win[show_win_cnt].uiWinSzY) / (ipl_cur_info.in_size_v);
			cal_ui()->ui_showstring(show_win_cnt, cal_ui_msg, CAL_UICOL_RED, &show_rect_info, TRUE, TRUE);
		}
	}
#endif
}

static void cal_focus(IPL_PROC_ID id)
{
	/**
	    IPL_SET_VA
	*/
	// get ipl input size
	ipl_cur_info.id = id;
	IPL_GetCmd(IPL_GET_IPL_CUR_INPUT_INFOR, &ipl_cur_info);

	// set IPL_SET_VA parameters
	va_data.id = id;
	va_data.va_out_en = ENABLE;
	va_data.VAOutSel = IPE_VA_OUT_GROUP1;
	va_data.VaFltrG1.FltrH.uiTapA = 2;
	va_data.VaFltrG1.FltrH.iTapB = 0;
	va_data.VaFltrG1.FltrH.iTapC = -1;
	va_data.VaFltrG1.FltrH.iTapD = 0;
	va_data.VaFltrG1.FltrH.FltrSymm = IPE_VA_FLTR_MIRROR;
	va_data.VaFltrG1.FltrH.FltrSize = IPE_VA_FLTR_SIZE_7;
	va_data.VaFltrG1.FltrH.uiDiv = 2;
//	va_data.VaFltrG1.FltrH.uiThL = 0x80;
	va_data.VaFltrG1.FltrH.uiThL = 0x05;
	va_data.VaFltrG1.FltrH.uiThH = 0xc0;
	va_data.VaFltrG1.FltrV.uiTapA = 2;
	va_data.VaFltrG1.FltrV.iTapB = 0;
	va_data.VaFltrG1.FltrV.iTapC = -1;
	va_data.VaFltrG1.FltrV.iTapD = 0;
	va_data.VaFltrG1.FltrV.FltrSymm = IPE_VA_FLTR_MIRROR;
	va_data.VaFltrG1.FltrV.FltrSize = IPE_VA_FLTR_SIZE_7;
	va_data.VaFltrG1.FltrV.uiDiv = 2;
//	va_data.VaFltrG1.FltrV.uiThL = 0x80;
	va_data.VaFltrG1.FltrV.uiThL = 0x05;
	va_data.VaFltrG1.FltrV.uiThH = 0xc0;
	va_data.VaFltrG1.bLineMax = DISABLE;
	va_data.VaFltrG1.bCnt = DISABLE;
	if (va_data.VAOutSel == IPE_VA_OUT_BOTH) {
		va_data.VaFltrG2.FltrH.uiTapA = 2;
		va_data.VaFltrG2.FltrH.iTapB = 0;
		va_data.VaFltrG2.FltrH.iTapC = -1;
		va_data.VaFltrG2.FltrH.iTapD = 0;
		va_data.VaFltrG2.FltrH.FltrSymm = IPE_VA_FLTR_MIRROR;
		va_data.VaFltrG2.FltrH.FltrSize = IPE_VA_FLTR_SIZE_7;
		va_data.VaFltrG2.FltrH.uiDiv = 2;
		va_data.VaFltrG2.FltrH.uiThL = 0x05;
		va_data.VaFltrG2.FltrH.uiThH = 0xc0;
		va_data.VaFltrG2.FltrV.uiTapA = 2;
		va_data.VaFltrG2.FltrV.iTapB = 0;
		va_data.VaFltrG2.FltrV.iTapC = -1;
		va_data.VaFltrG2.FltrV.iTapD = 0;
		va_data.VaFltrG2.FltrV.FltrSymm = IPE_VA_FLTR_MIRROR;
		va_data.VaFltrG2.FltrV.FltrSize = IPE_VA_FLTR_SIZE_7;
		va_data.VaFltrG2.FltrV.uiDiv = 2;
		va_data.VaFltrG2.FltrV.uiThL = 0x05;
		va_data.VaFltrG2.FltrV.uiThH = 0xc0;
		va_data.VaFltrG2.bLineMax = DISABLE;
		va_data.VaFltrG2.bCnt = DISABLE;
	}

	va_data.va_win.uiWinStX = 0;
	va_data.va_win.uiWinStY = 0;
	va_data.va_win.uiWinNmX = CAL_VA_HW_WIN;
	va_data.va_win.uiWinNmY = CAL_VA_HW_WIN;
	va_data.va_win.uiWinSzX = ipl_cur_info.in_size_h / va_data.va_win.uiWinNmX;
	va_data.va_win.uiWinSzY = ipl_cur_info.in_size_v / va_data.va_win.uiWinNmY;
	va_data.va_win.uiWinSpX = 0;
	va_data.va_win.uiWinSpY = 0;


	va_data.va_indep_win[0].bEn = ENABLE;
	va_data.va_indep_win[0].uiWinSzX = va_data.va_win.uiWinSzX * 4;
	va_data.va_indep_win[0].uiWinSzY = va_data.va_win.uiWinSzY * 4;
	va_data.va_indep_win[0].uiWinStX = (ipl_cur_info.in_size_h - va_data.va_indep_win[0].uiWinSzX) / 2;
	va_data.va_indep_win[0].uiWinStY = (ipl_cur_info.in_size_v - va_data.va_indep_win[0].uiWinSzY) / 2;
	va_data.va_indep_win[0].bLineMaxG1 = DISABLE;
	va_data.va_indep_win[0].bLineMaxG2 = DISABLE;

#if 1
	va_data.va_indep_win[1].bEn = FALSE;
	va_data.va_indep_win[2].bEn = FALSE;
	va_data.va_indep_win[3].bEn = FALSE;
	va_data.va_indep_win[4].bEn = FALSE;
#else
	va_data.va_indep_win[1] = va_data.va_indep_win[0];
	va_data.va_indep_win[2] = va_data.va_indep_win[0];
	va_data.va_indep_win[3] = va_data.va_indep_win[0];
	va_data.va_indep_win[4] = va_data.va_indep_win[0];
#endif
	// set va
	IPL_SetCmd(IPL_SET_VA, (void *)&va_data);
	IPL_WaitCmdFinish();

	/**
	    set IPL_GET_VA_CUR_INFOR parameters
	*/
	cal_va_rst.p_g1h = cal_va_win_g1_h;
	cal_va_rst.p_g1v = cal_va_win_g1_v;
	cal_va_rst.p_g1hcnt = cal_va_win_g1_hcnt;
	cal_va_rst.p_g1vcnt = cal_va_win_g1_vcnt;
	cal_va_rst.p_g2h = cal_va_win_g2_h;
	cal_va_rst.p_g2v = cal_va_win_g2_v;
	cal_va_rst.p_g2hcnt = cal_va_win_g2_hcnt;
	cal_va_rst.p_g2vcnt = cal_va_win_g2_vcnt;

	va_info.id = id;
	va_info.va_indep_win_rst = &cal_va_indep_win_rst;
	va_info.va_rst = &cal_va_rst;

	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, cal_focus_getva);

}
#endif

static void cal_get_prv_param(CAL_PRV_DATA *prv_data)
{
	ISIZE  disp_size;

	disp_size = GxVideo_GetDeviceSize(DOUT1);

	prv_data[0].id = g_current_cal_id;
	prv_data[0].crop_ratio = IPL_UTI_CONV2_UINT32(16, 9);
	prv_data[0].raw_out_buf_num = 3;
	prv_data[0].ime_path_en = CAL_IME_PATH_EN_2;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].output_en = ENABLE;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].sample_rate = IPL_SAMPLE_RATE_STREAM;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].scale_size_h = disp_size.w;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].scale_size_v = disp_size.h;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_crop_size_h = disp_size.w;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_crop_size_v = disp_size.h;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_img_lofs = disp_size.w;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_crop_start_x = 0;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_crop_start_y = 0;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].img_fmt = IPL_IMG_Y_PACK_UV420;
	prv_data[0].ime_ppb_num[CAL_IME_PATH_2] = 3;
	prv_data[0].ime_out_group[CAL_IME_PATH_2].group_en = DISABLE;
	prv_data[0].func_en = IPL_FUNC_NONE;

#if PRV_SHOW_VA
	prv_data[0].func_en |= IPL_FUNC_VA;
#endif

#if 1
	prv_data[0].pnext = NULL;
#else
	prv_data[1] = prv_data[0];
	prv_data[1].id = id + 1;
	prv_data[1].pnext = NULL;
#endif
}

void cal_item_awb(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_AWB;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_AWB_RST cal_awb_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_AWB;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_awb_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}
			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

		}
	}


	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}


void cal_item_dpb(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_DP;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			if (g_dp_bright_ony) {
				ipl_cal_set_info(g_current_cal_id, CAL_SEL_ALG_DP_TAG, CAL_ITEM_DP_TAG_BRIGHT_ONLY);
			} else {
				ipl_cal_set_info(g_current_cal_id, CAL_SEL_ALG_DP_TAG, CAL_ITEM_DP_TAG_BRIGHT);
			}

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_DP_RST cal_dp_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_DP;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_dp_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}


			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else if (rw_header_info.value.cal_status == CAL_STATUS_DP_B_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK (bright pixel)!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

		}
	}

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_dpd(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_DP;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			ipl_cal_set_info(g_current_cal_id, CAL_SEL_ALG_DP_TAG, CAL_ITEM_DP_TAG_DARK);
			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_DP_RST cal_dp_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_DP;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_dp_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}


			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else if (rw_header_info.value.cal_status == CAL_STATUS_DP_D_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK (dark pixel)!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

		}
	}

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_iso(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_ISO;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_ISO_RST cal_iso_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_ISO;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_iso_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}


			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

		}
	}

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_ecs(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_ECS;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_ECS_RST cal_ecs_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_ECS;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_ecs_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}

			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_MAX_NUM; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}
		}
	}
	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_lenscen(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_LENSCEN;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_LENSCEN_RST cal_lenscen_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_LENSCEN;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_lenscen_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}


			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

		}
	}

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_focus(void)
{
	DBG_IND("----------------------->\r\n");
	UINT32 cnt, cnt_id;

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			cal_ui()->init(CAL_UI_STYLE_NORMAL);
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_BIT_NONE;
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

#if PRV_SHOW_VA
			cal_focus(g_current_cal_id);
#endif


			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

		}
	}


	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();

}

void cal_item_test(void)
{
	DBG_IND("----------------------->\r\n");
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "start to test (press any key to exit)");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

	cal_ui()->uninit();
}

UINT32 cal_rw_store_op_open(char *p_sec_name, CAL_RW_OPEN_SEL open_sel)
{
	UINT32 p_section_handle = 0;
#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	UINT32 RWOperation = 0;
	RWOperation |= (open_sel & CAL_RD_ONLY) ? PS_RDONLY : 0;
	RWOperation |= (open_sel & CAL_WR_ONLY) ? PS_WRONLY : 0;
	RWOperation |= (open_sel & CAL_CREATE) ? PS_CREATE : 0;
	RWOperation |= (open_sel & CAL_UPDATE) ? PS_UPDATE : 0;
	p_section_handle = (UINT32)PStore_OpenSection(p_sec_name, RWOperation);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	UINT32 rw_operation = 0;
	rw_operation |= (open_sel & CAL_RD_ONLY) ? FST_OPEN_READ : 0;
	rw_operation |= (open_sel & CAL_WR_ONLY) ? FST_OPEN_WRITE : 0;
	rw_operation |= (open_sel & CAL_CREATE) ? FST_CREATE_ALWAYS : 0;
	p_section_handle = (UINT32)FileSys_OpenFile(p_sec_name, rw_operation);
#else
#endif

	return p_section_handle;
}

ER cal_rw_store_op_close(UINT32 p_section_handle)
{
	ER rt;
#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	rt = PStore_CloseSection((PSTORE_SECTION_HANDLE *) p_section_handle);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	rt = FileSys_CloseFile((FST_FILE) p_section_handle);
#else
#endif
	return rt;
}

// p_buffer: read out the information to which buffer addr to put
// data_length: read out the information size
ER cal_rw_store_op_read(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle)
{
	ER rt;

#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	rt = PStore_ReadSection(p_buffer, start_addr, data_length, (PSTORE_SECTION_HANDLE *)p_section_handle);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	rt = FileSys_ReadFile((FST_FILE)p_section_handle, p_buffer, &data_length, 0, NULL);
#else
#endif
	return rt;
}

ER cal_rw_store_op_write(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle)
{
	ER rt;
#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	rt = PStore_WriteSection(p_buffer, start_addr, data_length, (PSTORE_SECTION_HANDLE *)p_section_handle);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	rt = FileSys_WriteFile((FST_FILE)p_section_handle, p_buffer, &data_length, 0, NULL);
#else
#endif
	return rt;
}

ER cal_rw_store_op_delete(char *p_sec_name)
{
	ER rt;

#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	rt = PStore_DeleteSection(p_sec_name);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	rt = FileSys_DeleteFile(p_sec_name);
#else
#endif

	return rt;
}

UINT32 cal_rw_store_op_get_sec_size(char *p_sec_name)
{

#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	UINT32 size = 0;
	BOOL b_next = FALSE;
	BOOL b_find = FALSE;
	PSTORE_SEARCH_HANDLE search_hdl;

	// search section name
	b_next = PStore_SearchSectionOpen(&search_hdl);
	while (b_next) {
		DBG_IND("%15s 0x%08x  %d  0x%08x\r\n", search_hdl.pSectionName, search_hdl.uiSectionSize, search_hdl.uiReadOnly, search_hdl.uiDescCheckSum);
		if (!strncmp(p_sec_name, search_hdl.pSectionName, CAL_HEADER_SEC_LEN)) {
			size = search_hdl.uiSectionSize;
			b_find = TRUE;
			break;
		}
		b_next = PStore_SearchSection(&search_hdl);
	}
	PStore_SearchSectionClose();

	if (b_find == FALSE) {
		DBG_IND("section %s does not exist\r\n", p_sec_name);
	}
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	INT32 size = 0;
	size = FileSys_GetFileLen(p_sec_name);
	if (size < 0) {
		size = 0;
	}
#else
#endif

	return size;
}

void cal_system_init(void)
{
	ER rt = E_OK;
	UINT32 cnt_id;
	CAL_INIT_DATA init_data = {0};

	if (EngineerMode_CheckEng()) {
		init_data.buf_addr = OS_GetMempoolAddr(POOL_ID_APP);
		init_data.buf_size = POOL_SIZE_APP / 2;
		init_data.cal_rw_fp = &cal_rw_store_op;
	} else {
		init_data.buf_addr = OS_GetMempoolAddr(POOL_ID_CAL);
		init_data.buf_size = POOL_SIZE_CAL / 2;
		init_data.cal_rw_fp = &cal_rw_store_op;
	}

	DBG_IND("init_data buf_addr 0x%.8x buf_size 0x%.8x EngineerMode_CheckEng %d\r\n"
			, init_data.buf_addr, init_data.buf_size, EngineerMode_CheckEng());

	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_cal_id[cnt_id] == CAL_END) {
			break;
		}
		g_b_cal_id[cnt_id] = TRUE;
		rt = ipl_cal_init(&init_data);
		rt |= ipl_cal_ext_init(cnt_id);

		if (rt != E_OK) {
			DBG_ERR("cal init fail\r\n");
		}
	}
}

void cal_system_uninit(void)
{
	ER rt = E_OK;
	UINT32 cnt_id;

	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_cal_id[cnt_id] == CAL_END) {
			break;
		}
		rt = ipl_cal_ext_uninit(cnt_id);
		rt = ipl_cal_uninit();

		if (rt != E_OK) {
			DBG_ERR("cal uninit fail\r\n");
		}
	}

}

#endif

