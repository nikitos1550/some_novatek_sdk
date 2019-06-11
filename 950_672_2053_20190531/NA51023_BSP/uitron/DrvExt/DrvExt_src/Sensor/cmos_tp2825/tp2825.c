/**
    @file       tp2825.c

    @brief      tp2825

    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
//----------------------------------------------------------------------------------------------------
#include "pll.h"
#include "Utility.h"
#include "tp2825_param_int.h"
#include "sensor_cmd_if.h"
#include "NvtVerInfo.h"

NVTVER_VERSION_ENTRY(CMOS_tp2825, 1, 00, 000, 00)

#define __MODULE__ tp2825
#define __DBGLVL__          1 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" // *=All, [mark]=CustomClass
#define __DBGFLT__ "*"      // *=All
#include "DebugModule.h"

#define MIN_EXPOSURE_LINE    2

//Function prototype
#if 0
#endif

static UINT32 sensor_option[SENSOR_ID_MAX_NUM] = {0};
static BOOL is_sensor_init[SENSOR_ID_MAX_NUM] = {0};

static ER init_tp2825(SENSOR_ID id, SEN_DRV_INIT_OBJ *drv_init_obj);
static ER uninit_tp2825(SENSOR_ID id);
static ER sleep_tp2825(SENSOR_ID id);
static ER wakeup_tp2825(SENSOR_ID id);
static ER write_reg_tp2825(SENSOR_ID id, SENSOR_CMD *cmd);
static ER read_reg_tp2825(SENSOR_ID id, SENSOR_CMD *cmd);
static ER chg_mode_tp2825(SENSOR_ID id, SEN_CHGMODE_DATA chg_mode_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt);
static ER chg_fps_tp2825(SENSOR_ID id, SEN_CHGFPS_DATA chg_fps_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt);
static ER set_info_tp2825(SENSOR_ID id, SEN_SET_DRV_ITEM set_drv_item, void *data, SEN_COM_INFO com_info);
static ER get_info_tp2825(SENSOR_ID id, SEN_GET_DRV_ITEM get_drv_item, void *data);

//Internal API
static ER set_exptime_tp2825(SENSOR_ID id, EXPOSURE_SETTING *expt_setting, SENSOR_MODE mode);
static ER set_gain_tp2825(SENSOR_ID id, GAIN_SETTING *gain, SENSOR_MODE mode);
static ER set_vd_period_register_tp2825(SENSOR_ID id, SENSOR_MODE mode, UINT32 frame_rate);
static ER set_grs_mode_tp2825(SENSOR_ID id, void *param);
static ER set_hdr_info_tp2825(SENSOR_ID id, HDR_SET_INFO *hdr_info);
static ER set_user_option_tp2825(SENSOR_ID id, UINT32 option);
static ER get_expo_setting_tp2825(SENSOR_ID id, SENSOR_MODE mode, UINT32 *exp_time, EXPOSURE_SETTING *expt_setting, UINT32 *compensate_ratio);
static ER get_gain_setting_tp2825(SENSOR_ID id, SENSOR_MODE mode, UINT32 *gain_ratio, GAIN_SETTING *gain);
static ER set_init_reg_tp2825(SENSOR_ID id);

static SENSOR_DRV_TAB m_p_tp2825_drv = {
	init_tp2825,
	uninit_tp2825,
	sleep_tp2825,
	wakeup_tp2825,
	write_reg_tp2825,
	read_reg_tp2825,
	chg_mode_tp2825,
	chg_fps_tp2825,
	set_info_tp2825,
	get_info_tp2825,
	get_sensor_drv_info_tp2825,
	get_mode_info_tp2825,
	get_sensor_cmd_info_tp2825,
	get_sensor_if_info_tp2825,
};

#if 0
-Intern api
#endif

static SENSOR_CMD set_cmd_info_tp2825(UINT32 addr, UINT32 data_length, UINT32 data0, UINT32 data1)
{
	SENSOR_CMD cmd;
	cmd.ui_addr = addr;
	cmd.ui_data_len = data_length;
	cmd.ui_data[0] = data0;
	cmd.ui_data[1] = data1;
	return cmd;
}

static ER init_tp2825(SENSOR_ID id, SEN_DRV_INIT_OBJ *drv_init_obj)
{
	return E_OK;
}

static ER uninit_tp2825(SENSOR_ID id)
{
	UINT32 cnt;
	for (cnt = 0; cnt < MODE_MAX; cnt++) {
		set_sensor_chgmode_frame_rate_tp2825(id, cnt, 0);
	}
	is_sensor_init[id] = FALSE;
	return E_OK;
}

static ER sleep_tp2825(SENSOR_ID id)
{
	return E_OK;
}

static ER wakeup_tp2825(SENSOR_ID id)
{
	return E_OK;
}

static ER write_reg_tp2825(SENSOR_ID id, SENSOR_CMD *cmd)
{
	//Use command interface (sif/i2c/io/...) write register to sensor
	ER rt = E_OK;
	rt = sensor_cmd_get_i2c_ctrl_obj()->write(id, cmd->ui_addr, cmd->ui_data[0], I2CFMT_1B1B);

    //re try
    if (rt != E_OK) {
        rt = sensor_cmd_get_i2c_ctrl_obj()->write(id, cmd->ui_addr, cmd->ui_data[0], I2CFMT_1B1B);
    }

    return rt;
}

static ER read_reg_tp2825(SENSOR_ID id, SENSOR_CMD *cmd)
{
	//Use command interface (i2c/io/...) read register from sensor
	ER rt = E_OK;
	rt = sensor_cmd_get_i2c_ctrl_obj()->read(id, cmd->ui_addr, &(cmd->ui_data[0]), I2CFMT_1B1B);
	return rt;
}

static ER chg_mode_tp2825(SENSOR_ID id, SEN_CHGMODE_DATA chg_mode_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt)
{
	SENSOR_CMD *p_cmd_list = NULL, cmd;
	UINT32 idx, cmd_num = 0;
	ER rt = E_OK;
	DBG_ERR("chg_mode_tp2825\r\n");

	//Set sensor frame rate
	set_sensor_chgmode_frame_rate_tp2825(id, chg_mode_data.mode, chg_mode_data.frame_rate);
	rt |= set_vd_period_register_tp2825(id, chg_mode_data.mode, chg_mode_data.frame_rate); // set register to change VD period
	if (rt != E_OK) {
		DBG_ERR("set_vd_period_register_tp2825 error %d\r\n", rt);
	}


	//Get & set sensor cmd table
	cmd_num = get_cmd_tab_tp2825(chg_mode_data.mode, &p_cmd_list);
	if (p_cmd_list == NULL) {
		DBG_ERR("%s: SenMode(%d) out of range!!!\r\n", __func__, chg_mode_data.mode);
		return E_SYS;
	}

	for (idx = 0; idx < cmd_num; idx ++) {
		if (p_cmd_list[idx].ui_addr == CMD_DELAY) {
            Delay_DelayUs((p_cmd_list[idx].ui_data[0] & 0xff) << 10);
		} else {
			cmd = set_cmd_info_tp2825(p_cmd_list[idx].ui_addr, p_cmd_list[idx].ui_data_len, p_cmd_list[idx].ui_data[0], p_cmd_list[idx].ui_data[1]);
			rt |= write_reg_tp2825(id, &cmd);
		}
    }
/*
    cmd = set_cmd_info_tp2825(0x01, 1, 0xFFFFFFFF, 0xFFFFFFFF);
    
    sensor_read_reg(id, &cmd);
    DBG_ERR("chg_mode_tp2825(): data = 0x%02x\r\n", cmd.ui_data[0]);
*/

	
    if (rt != E_OK) {
        DBG_ERR("write register error %d\r\n", rt);
        return rt;
    }

	return E_OK;
}

static ER chg_fps_tp2825(SENSOR_ID id, SEN_CHGFPS_DATA chg_fps_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt)
{
	ER rt = E_OK;

	set_sensor_chgmode_frame_rate_tp2825(id, chg_fps_data.mode, chg_fps_data.frame_rate);
	set_vd_period_register_tp2825(id, chg_fps_data.mode, chg_fps_data.frame_rate);

    rt |= set_exptime_tp2825(id, preset_expt, chg_fps_data.mode);
    rt |= set_gain_tp2825(id, preset_gain, chg_fps_data.mode);

	if (rt != E_OK) {
		DBG_ERR("chg fps error\r\n");
	}

	return rt;
}

static ER set_info_tp2825(SENSOR_ID id, SEN_SET_DRV_ITEM set_drv_item, void *data, SEN_COM_INFO com_info)
{
	ER rt = E_OK;
	SENSOR_MODE_INFO *mode_info;
	GAIN_SETTING *gain_setting;
    SEN_PRESET_MODE *sen_preset_mode;

	if (set_drv_item >= SEN_SET_DRV_MAX_NUM) {
		DBG_ERR("%s: sensor set item(%d) out of range!!!\r\n", __func__, set_drv_item);
		return E_SYS;
	}

	switch (set_drv_item) {
	case SEN_SET_DRV_EXPT:
		rt |= set_exptime_tp2825(id, (EXPOSURE_SETTING *)(data), com_info.cur_sensor_mode);
		break;
	case SEN_SET_DRV_GAIN:
		gain_setting = (GAIN_SETTING *)data;
		rt |= set_gain_tp2825(id, gain_setting, com_info.cur_sensor_mode);
		mode_info = get_mode_info_tp2825(com_info.cur_sensor_mode);
		break;
	case SEN_SET_DRV_USER_OPTION:
		rt |= set_user_option_tp2825(id, *((UINT32 *)data));
		break;
	case SEN_SET_DRV_GRS:
		rt |= set_grs_mode_tp2825(id, (void *)(data));
		break;
	case SEN_SET_DRV_HDR_INFO:
		rt |= set_hdr_info_tp2825(id, (HDR_SET_INFO *)data);
		break;
	case SEN_SET_DRV_TRIGGER_EVENT:
		break;
    case SEN_SET_DRV_PRESET_MODE:
        sen_preset_mode = (SEN_PRESET_MODE *)data;
        set_sensor_chgmode_frame_rate_tp2825(id, com_info.cur_sensor_mode, sen_preset_mode->frame_rate);
        break;
    case SEN_SET_DRV_PRESET_LOCK:
        break;
	default:
		DBG_ERR("sensor set drv item error %d\r\n", set_drv_item);
		rt = E_PAR;
		break;
	}

	if (rt != E_OK) {
		DBG_ERR("sensor set drv item %d information error %d\r\n", set_drv_item, rt);
	}

	return rt;
}

static ER det_plug_sts_tp2825(SENSOR_ID id, BOOL *is_plug_in)
{
	static BOOL detect = FALSE;
	SENSOR_CMD cmd = {0x01, 1, {0xFFFFFFFF, 0xFFFFFFFF}};


	DBG_ERR(">>>>>det plug \r\n");

	if (is_sensor_init[id] == FALSE) {
		DBG_ERR("id%d, need init decoder reg before detect plug sts\r\n");
		if (set_init_reg_tp2825(id) != E_OK) {
			DBG_ERR("id%d, init decoder reg fail\r\n");
			return E_SYS;
		}
	}

	if (sensor_read_reg(id, &cmd) == E_OK) {
		DBG_ERR("^C det_plug_sts_tp2825(): data = 0x%02x\r\n", cmd.ui_data[0]);

		#if 1
		// bit 7: Video Loss (0: video present, 1: video loss)
		// bit 3: Video Detect (0: no video, 1: video detected)
		switch (cmd.ui_data[0]) {
		case 0x0f:
		//case 0x4a:
		case 0x7a:
			detect = TRUE;
 			break;

		default:
			detect = FALSE;
 			break;
		}
		#else
		// add debounce mechanism
		static UINT32 detect_count = 0;
		static UINT32 loss_count = 0;

		// bit 7: Video Loss (0: video present, 1: video loss)
		// bit 3: Video Detect (0: no video, 1: video detected)
		switch (cmd.ui_data[0] & 0x88) {
		case 0x08: // video present, video detected
			detect_count++;
			loss_count = 0;
			if (detect_count >= 3) {
				if (detect == FALSE) {
					detect = TRUE;
					DBG_DUMP("^Y video detected!\r\n");
				}
			}
			break;

		default:
			loss_count++;
			detect_count = 0;
			if (loss_count >= 3) {
				if (detect == TRUE) {
					detect = FALSE;
					DBG_DUMP("^Y video loss!\r\n");
				}
			}
			break;
		}
		#endif

		*is_plug_in = detect;

	} else {
		DBG_ERR("read reg error\r\n");
		*is_plug_in = FALSE;
	}

	return E_OK;
}

static ER get_info_tp2825(SENSOR_ID id, SEN_GET_DRV_ITEM get_drv_item, void *data)
{
	ER rt = E_OK;
	EXPOSURE_PARAM *exp_param;
	GAIN_PARAM *gain_param;
	GAIN_INFO *gain_info;
	UINT32 *value;
	CHAR *name;
	FPS_PARAM *fps_param;
	UINT32 buf_cnt = 0;
	SENSOR_MODE_INFO *mode_info;
	BOOL *is_plug_in;

	switch (get_drv_item) {
	case SEN_GET_DRV_EXPO_SETTING:
		exp_param = (EXPOSURE_PARAM *)data;
		rt |= get_expo_setting_tp2825(id, exp_param->sensor_mode, &exp_param->exp_time[0], &exp_param->exp_setting, &exp_param->compensate_ratio[0]);
		break;
	case SEN_GET_DRV_GAIN_SETTING:
		gain_param = (GAIN_PARAM *)data;
		gain_param->gain_setting.rev[0] = gain_param->gain_ratio[0];
		gain_param->gain_setting.rev[1] = gain_param->gain_ratio[1];
		rt |= get_gain_setting_tp2825(id, gain_param->sensor_mode, &gain_param->gain_ratio[0], &gain_param->gain_setting);
		break;
	case SEN_GET_DRV_GAIN_INFO:
		gain_info = (GAIN_INFO *)data;
		gain_info->gain = get_mode_info_tp2825(gain_info->sensor_mode)->gain;
		break;
	case SEN_GET_DRV_NAME:
		name = (CHAR *)data;
		buf_cnt = snprintf(NULL, 0, "%s", __xstring(__section_name__));
		snprintf((char *)name, buf_cnt + 1, "%s", __xstring(__section_name__));
		break;
	case SEN_GET_DRV_FPS:
		fps_param = (FPS_PARAM *)data;
		mode_info = get_mode_info_tp2825(fps_param->sensor_mode);
		fps_param->cur_fps = get_sensor_cur_frame_rate_tp2825(id, fps_param->sensor_mode);
		fps_param->chg_fps = get_sensor_chgmode_frame_rate_tp2825(id, fps_param->sensor_mode);
		fps_param->dft_fps = mode_info->frame_rate;
		break;
	case SEN_GET_DRV_MAX_MODE:
		value = (UINT32 *)data;
		*value = MODE_MAX;
		break;
	case SEN_GET_DRV_USER_OPTION:
		value = (UINT32 *)data;
		*value = (UINT32)(sensor_option[id]);
		break;
	case SEN_GET_DRV_TEMP:
		DBG_WRN("not suppor get sensor temperature\r\n");
		value = (UINT32 *)data;
		*value = (UINT32)(0);
		break;
	case SEN_GET_DRV_HDR_STATUS:
		DBG_WRN("not suppor get sensor hdr status\r\n");
		value = (UINT32 *)data;
		*value = (UINT32)(0);
		break;
	case SEN_GET_DRV_DET_PLUG:
		is_plug_in = (BOOL*)data;
		rt = det_plug_sts_tp2825(id, is_plug_in);
		break;
	default:
		DBG_ERR("sensor get drv item error %d\r\n", get_drv_item);
		rt = E_PAR;
		break;
	}

	if (rt != E_OK) {
		DBG_ERR("sensor get drv item %d information error %d\r\n", get_drv_item, rt);
	}

	return rt;
}

/**
 * Sensor internal AE driver API
*/

static ER get_expo_setting_tp2825(SENSOR_ID id, SENSOR_MODE mode, UINT32 *exp_time, EXPOSURE_SETTING *expt_setting, UINT32 *compensate_ratio)
{
	return E_OK;
}

static ER get_gain_setting_tp2825(SENSOR_ID id, SENSOR_MODE mode, UINT32 *gain_ratio, GAIN_SETTING *gain)
{
	return E_OK;
}

static ER set_exptime_tp2825(SENSOR_ID id, EXPOSURE_SETTING *expt_setting, SENSOR_MODE mode)
{
	return E_OK;
}

static ER set_gain_tp2825(SENSOR_ID id, GAIN_SETTING *gain, SENSOR_MODE mode)
{
	return E_OK;
}

/**
 * Sensor internal API
*/
static ER set_vd_period_register_tp2825(SENSOR_ID id, SENSOR_MODE mode, UINT32 frame_rate)
{
	return E_OK;
}

static ER set_grs_mode_tp2825(SENSOR_ID id, void *param)
{
	return E_OK;
}

static ER set_hdr_info_tp2825(SENSOR_ID id, HDR_SET_INFO *hdr_info)
{
	return E_OK;
}

static ER set_user_option_tp2825(SENSOR_ID id, UINT32 option)
{
	return E_OK;
}

static ER set_init_reg_tp2825(SENSOR_ID id)
{
	SENSOR_CMD *p_cmd_list = NULL, cmd;
	UINT32 idx, cmd_num = 0;
	ER rt = E_OK;

	cmd_num = get_cmd_tab_tp2825(SENSOR_MODE_1, &p_cmd_list);

	for (idx = 0; idx < cmd_num; idx ++) {
		if (p_cmd_list[idx].ui_addr == CMD_DELAY) {
            Delay_DelayUs((p_cmd_list[idx].ui_data[0] & 0xff) << 10);
		} else {
			cmd = set_cmd_info_tp2825(p_cmd_list[idx].ui_addr, p_cmd_list[idx].ui_data_len, p_cmd_list[idx].ui_data[0], p_cmd_list[idx].ui_data[1]);
			rt |= write_reg_tp2825(id, &cmd);
		}
    }
    if (rt != E_OK) {
        DBG_ERR("write register error %d\r\n", rt);
        return rt;
    }

	is_sensor_init[id] = TRUE;
	return E_OK;
}

#if 0
-Extern api
#endif
SENSOR_DRV_TAB *sensor_get_drv_tab_tp2825(void)
{
	return &m_p_tp2825_drv;
}
//@@ EOF
