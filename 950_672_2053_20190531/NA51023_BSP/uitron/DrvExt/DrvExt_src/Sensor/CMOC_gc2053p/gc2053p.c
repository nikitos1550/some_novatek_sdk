/**
    @file       gc2053p.c

    @brief      gc2053p

    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
//----------------------------------------------------------------------------------------------------
#include "pll.h"
#include "Utility.h"
#include "gc2053p_param_int.h"
#include "sensor_cmd_if.h"
#include "NvtVerInfo.h"

NVTVER_VERSION_ENTRY(CMOS_GC2053P, 1, 00, 000, 00)

#define __MODULE__ GC2053P
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" // *=All, [mark]=CustomClass
#define __DBGFLT__ "*"      // *=All
#include "DebugModule.h"

#define MAX_VD_PERIOD        0x3FFF
#define MIN_EXPOSURE_LINE    2
#define NON_EXPOSURE_LINE    1 
#define MAX_EXPOSURE_LINE    0x3FFF
#define MAX_GAIN_RATIO       110000
#define MIN_GAIN_RATIO       1000

//Function prototype
#if 0
#endif

static UINT32 sensor_option_init[SENSOR_ID_MAX_NUM] = {0};
static UINT32 sensor_option[SENSOR_ID_MAX_NUM] = {0};
static UINT32 vd_period[SENSOR_ID_MAX_NUM] = {0};

static ER init_gc2053p(SENSOR_ID id, SEN_DRV_INIT_OBJ *drv_init_obj);
static ER uninit_gc2053p(SENSOR_ID id);
static ER sleep_gc2053p(SENSOR_ID id);
static ER wakeup_gc2053p(SENSOR_ID id);
static ER write_reg_gc2053p(SENSOR_ID id, SENSOR_CMD *cmd);
static ER read_reg_gc2053p(SENSOR_ID id, SENSOR_CMD *cmd);
static ER chg_mode_gc2053p(SENSOR_ID id, SEN_CHGMODE_DATA chg_mode_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt);
static ER chg_fps_gc2053p(SENSOR_ID id, SEN_CHGFPS_DATA chg_fps_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt);
static ER set_info_gc2053p(SENSOR_ID id, SEN_SET_DRV_ITEM set_drv_item, void *data, SEN_COM_INFO com_info);
static ER get_info_gc2053p(SENSOR_ID id, SEN_GET_DRV_ITEM get_drv_item, void *data);

//Internal API
static ER set_exptime_gc2053p(SENSOR_ID id, EXPOSURE_SETTING *expt_setting, SENSOR_MODE mode);
static ER set_gain_gc2053p(SENSOR_ID id, GAIN_SETTING *gain, SENSOR_MODE mode);
static ER set_vd_period_register_gc2053p(SENSOR_ID id, SENSOR_MODE mode, UINT32 frame_rate);
static ER set_grs_mode_gc2053p(SENSOR_ID id, void *param);
static ER set_hdr_info_gc2053p(SENSOR_ID id, HDR_SET_INFO *hdr_info);
static ER set_user_option_gc2053p(SENSOR_ID id, UINT32 option);
static ER get_expo_setting_gc2053p(SENSOR_ID id, SENSOR_MODE mode, UINT32 *exp_time, EXPOSURE_SETTING *expt_setting, UINT32 *compensate_ratio);
static ER get_gain_setting_gc2053p(SENSOR_ID id, SENSOR_MODE mode, UINT32 *gain_ratio, GAIN_SETTING *gain);

static SENSOR_DRV_TAB m_p_gc2053p_drv = {
    init_gc2053p,
    uninit_gc2053p,
    sleep_gc2053p,
    wakeup_gc2053p,
    write_reg_gc2053p,
    read_reg_gc2053p,
    chg_mode_gc2053p,
    chg_fps_gc2053p,
    set_info_gc2053p,
    get_info_gc2053p,
    get_sensor_drv_info_gc2053p,
    get_mode_info_gc2053p,
    get_sensor_cmd_info_gc2053p,
    get_sensor_if_info_gc2053p,
};

#if 0
-Intern api
#endif

typedef struct gain_set {
    UINT32  gain_reg1;
    UINT32  gain_reg2;    
    UINT32  gain_reg3;
    UINT32  gain_reg4;
    UINT32  total_gain;
} gain_set_t;

static gain_set_t gain_table[] = // gain*1000
{
    {0x00, 0x00, 0x01, 0x00, 1000  }, {0x00, 0x10, 0x01, 0x0c, 1188  }, {0x00, 0x20, 0x01, 0x1b, 1422  },
    {0x00, 0x30, 0x01, 0x2c, 1688  }, {0x00, 0x40, 0x01, 0x3f, 1984  }, {0x00, 0x50, 0x02, 0x16, 2344  },
    {0x00, 0x60, 0x02, 0x35, 2828  }, {0x00, 0x70, 0x03, 0x16, 3344  }, {0x00, 0x80, 0x04, 0x02, 4031  },
    {0x00, 0x90, 0x04, 0x31, 4766  }, {0x00, 0xa0, 0x05, 0x32, 5781  }, {0x00, 0xb0, 0x06, 0x35, 6828  },
    {0x00, 0xc0, 0x08, 0x04, 8063  }, {0x00, 0x5a, 0x09, 0x19, 9391  }, {0x00, 0x83, 0x0b, 0x0f, 11234 },
    {0x00, 0x93, 0x0d, 0x12, 13281 }, {0x00, 0x84, 0x10, 0x00, 16000 }, {0x00, 0x94, 0x12, 0x3a, 18906 },
    {0x01, 0x2c, 0x1a, 0x02, 26031 }, {0x01, 0x3c, 0x1b, 0x20, 27500 }, {0x00, 0x8c, 0x20, 0x0f, 32234 },
    {0x00, 0x9c, 0x26, 0x07, 38109 }, {0x02, 0x64, 0x36, 0x21, 54516 }, {0x02, 0x74, 0x37, 0x3a, 55906 },
    {0x00, 0xc6, 0x3d, 0x02, 61031 }, {0x00, 0xdc, 0x3f, 0x3f, 81016 }, {0x02, 0x85, 0x3f, 0x3f, 86063 },
    {0x02, 0x95, 0x3f, 0x3f, 105375}, {0x00, 0xce, 0x3f, 0x3f, 110516}, 
};

#define NUM_OF_GAINSET (sizeof(gain_table) / sizeof(gain_set_t))

static SENSOR_CMD set_cmd_info_gc2053p(UINT32 addr, UINT32 data_length, UINT32 data0, UINT32 data1)
{
    SENSOR_CMD cmd;
    cmd.ui_addr = addr;
    cmd.ui_data_len = data_length;
    cmd.ui_data[0] = data0;
    cmd.ui_data[1] = data1;
    return cmd;
}

static ER init_gc2053p(SENSOR_ID id, SEN_DRV_INIT_OBJ *drv_init_obj)
{
    sensor_option_init[id] = drv_init_obj->option;
    sensor_option[id] = 0;
    
    return E_OK;
}

static ER uninit_gc2053p(SENSOR_ID id)
{
    UINT32 cnt;
    for (cnt = 0; cnt < MODE_MAX; cnt++) {
        set_sensor_chgmode_frame_rate_gc2053p(id, cnt, 0);
    }
    return E_OK;
}

static ER sleep_gc2053p(SENSOR_ID id)
{
    return E_OK;
}

static ER wakeup_gc2053p(SENSOR_ID id)
{
    return E_OK;
}

static ER write_reg_gc2053p(SENSOR_ID id, SENSOR_CMD *cmd)
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

static ER read_reg_gc2053p(SENSOR_ID id, SENSOR_CMD *cmd)
{
    //Use command interface (i2c/io/...) read register from sensor
    ER rt = E_OK;
    rt = sensor_cmd_get_i2c_ctrl_obj()->read(id, cmd->ui_addr, &(cmd->ui_data[0]), I2CFMT_1B1B);
    return rt;
}

static ER chg_mode_gc2053p(SENSOR_ID id, SEN_CHGMODE_DATA chg_mode_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt)
{
    SENSOR_CMD *p_cmd_list = NULL, cmd;
    UINT32 idx, cmd_num = 0;
    ER rt = E_OK;
    
    //Set sensor frame rate
    set_sensor_chgmode_frame_rate_gc2053p(id, chg_mode_data.mode, chg_mode_data.frame_rate);
    rt |= set_vd_period_register_gc2053p(id, chg_mode_data.mode, chg_mode_data.frame_rate); // set register to change VD period
    if (rt != E_OK) {
        DBG_ERR("set_vd_period_register_gc2053p error %d\r\n", rt);
    }
    
    //Get & set sensor cmd table
    cmd_num = get_cmd_tab_gc2053p(chg_mode_data.mode, &p_cmd_list);
    if (p_cmd_list == NULL) {
        DBG_ERR("%s: SenMode(%d) out of range!!!\r\n", __func__, chg_mode_data.mode);
        return E_SYS;
    }

    for (idx = 0; idx < cmd_num; idx ++) {
        if (p_cmd_list[idx].ui_addr == CMD_DELAY) {
            Delay_DelayUs((p_cmd_list[idx].ui_data[0] & 0xFF) << 10);   
        } else if (p_cmd_list[idx].ui_addr == CMD_SETAE) {
            if ((preset_gain != NULL) && (preset_expt != NULL)) {
                ER rt_prset = E_OK;
                rt_prset |= set_exptime_gc2053p(id, preset_expt, chg_mode_data.mode);
                rt_prset |= set_gain_gc2053p(id, preset_gain, chg_mode_data.mode);

                if (rt_prset != E_OK) {
                    DBG_WRN("preset error %d\r\n", rt_prset);
                }
            }
        } else if (p_cmd_list[idx].ui_addr == CMD_SETVD) {
            cmd = set_cmd_info_gc2053p(0x41, 1, (vd_period[id] >> 8) & 0x3F, 0x0);//[5:0]
            rt |= write_reg_gc2053p(id, &cmd);
            cmd = set_cmd_info_gc2053p(0x42, 1, vd_period[id] & 0xFF, 0x0);//[7:0]
            rt |= write_reg_gc2053p(id, &cmd);
        } else {
            cmd = set_cmd_info_gc2053p(p_cmd_list[idx].ui_addr, p_cmd_list[idx].ui_data_len, p_cmd_list[idx].ui_data[0], p_cmd_list[idx].ui_data[1]);
            rt |= write_reg_gc2053p(id, &cmd);   
            //DBG_ERR("write addr=0x%x, data0=0x%x, data1=0x%x\r\n", p_cmd_list[idx].ui_addr, p_cmd_list[idx].ui_data[0], p_cmd_list[idx].ui_data[1]);         
        }
    }
    if (rt != E_OK) {
        DBG_ERR("write register error %d\r\n", rt);
        return rt;
    }

    //Set user option
    rt = set_user_option_gc2053p(id, sensor_option[id]);
    if (rt != E_OK) {
		DBG_ERR("set user option error %d\r\n", rt);
        return rt;
    }

    return E_OK;
}

static ER chg_fps_gc2053p(SENSOR_ID id, SEN_CHGFPS_DATA chg_fps_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt)
{
    ER rt = E_OK;
    SENSOR_CMD cmd;
        
    set_sensor_chgmode_frame_rate_gc2053p(id, chg_fps_data.mode, chg_fps_data.frame_rate);
    set_vd_period_register_gc2053p(id, chg_fps_data.mode, chg_fps_data.frame_rate);

    if ( preset_expt != NULL ) {
        rt |= set_exptime_gc2053p(id, preset_expt, chg_fps_data.mode); 		
    } else {
        cmd = set_cmd_info_gc2053p(0x41, 1, (vd_period[id] >> 8)& 0x3F, 0x0);//[5:0]
        rt |= write_reg_gc2053p(id, &cmd);
        cmd = set_cmd_info_gc2053p(0x42, 1, vd_period[id] & 0xFF, 0x0);//[7:0]
        rt |= write_reg_gc2053p(id, &cmd);		
    }
    
    if ( preset_gain != NULL ) {     
        rt |= set_gain_gc2053p(id, preset_gain, chg_fps_data.mode);	 
    }
        
    if (rt != E_OK) {
        DBG_ERR("chg fps error\r\n");
    }
    
    return rt;
}

static ER set_info_gc2053p(SENSOR_ID id, SEN_SET_DRV_ITEM set_drv_item, void *data, SEN_COM_INFO com_info)
{
    ER rt = E_OK;
    GAIN_SETTING *gain_setting;
    SEN_PRESET_MODE *sen_preset_mode;
    
    if (set_drv_item >= SEN_SET_DRV_MAX_NUM) {
        DBG_ERR("%s: sensor set item(%d) out of range!!!\r\n", __func__, set_drv_item);
        return E_SYS;
    }
    
    switch (set_drv_item) {
    case SEN_SET_DRV_EXPT:
        rt |= set_exptime_gc2053p(id, (EXPOSURE_SETTING *)(data), com_info.cur_sensor_mode);
        break;
    case SEN_SET_DRV_GAIN:
        gain_setting = (GAIN_SETTING *)data;
        rt |= set_gain_gc2053p(id, gain_setting, com_info.cur_sensor_mode);
        break;
    case SEN_SET_DRV_USER_OPTION:
        rt |= set_user_option_gc2053p(id, *((UINT32 *)data));
        break;
    case SEN_SET_DRV_GRS:
        rt |= set_grs_mode_gc2053p(id, (void *)(data));
        break;
    case SEN_SET_DRV_HDR_INFO:
        rt |= set_hdr_info_gc2053p(id, (HDR_SET_INFO *)data);
        break;
    case SEN_SET_DRV_TRIGGER_EVENT:
        break;       
    case SEN_SET_DRV_PRESET_MODE:
        sen_preset_mode = (SEN_PRESET_MODE *)data;
        set_sensor_chgmode_frame_rate_gc2053p(id, com_info.cur_sensor_mode, sen_preset_mode->frame_rate);
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

static ER get_info_gc2053p(SENSOR_ID id, SEN_GET_DRV_ITEM get_drv_item, void *data)
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
    
    switch (get_drv_item) {
    case SEN_GET_DRV_EXPO_SETTING:
        exp_param = (EXPOSURE_PARAM *)data;
        rt |= get_expo_setting_gc2053p(id, exp_param->sensor_mode, &exp_param->exp_time[0], &exp_param->exp_setting, &exp_param->compensate_ratio[0]);
        break;
    case SEN_GET_DRV_GAIN_SETTING:
        gain_param = (GAIN_PARAM *)data;
        gain_param->gain_setting.rev[0] = gain_param->gain_ratio[0];
        gain_param->gain_setting.rev[1] = gain_param->gain_ratio[1];
        rt |= get_gain_setting_gc2053p(id, gain_param->sensor_mode, &gain_param->gain_ratio[0], &gain_param->gain_setting);
        break;
    case SEN_GET_DRV_GAIN_INFO:
        gain_info = (GAIN_INFO *)data;
        gain_info->gain = get_mode_info_gc2053p(gain_info->sensor_mode)->gain;
        break;
    case SEN_GET_DRV_NAME:
        name = (CHAR *)data;
        buf_cnt = snprintf(NULL, 0, "%s", __xstring(__section_name__));
        snprintf((char *)name, buf_cnt + 1, "%s", __xstring(__section_name__));
        break;
    case SEN_GET_DRV_FPS:
        fps_param = (FPS_PARAM *)data;
        mode_info = get_mode_info_gc2053p(fps_param->sensor_mode);
        fps_param->cur_fps = get_sensor_cur_frame_rate_gc2053p(id, fps_param->sensor_mode);
        fps_param->chg_fps = get_sensor_chgmode_frame_rate_gc2053p(id, fps_param->sensor_mode);
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
static UINT32 calc_expo_setting_gc2053p(SENSOR_ID id, UINT32 exp_time, EXPOSURE_SETTING *expt_setting, SENSOR_MODE mode, UINT32 multi_frame_num)
{
    SENSOR_MODE_INFO *mode_info;
    UINT32 compensate_ratio;
    UINT32 t_row, exp_line, real_exp_time;

    mode_info = get_mode_info_gc2053p(mode);

    //Calculates the exposure setting
    t_row = 100 * (mode_info->sen_hd[multi_frame_num]->period) / ( mode_info->pclk / 100000 );
    exp_line = exp_time * 10 / t_row ;
    
    //DBG_ERR("t_row= %d\r\n", t_row);
    
    //Limit minimun exposure line
    if (exp_line < MIN_EXPOSURE_LINE) {
        exp_line = MIN_EXPOSURE_LINE;
    }

    //Collect the exposure setting
    expt_setting->ui_frame[0] = 0;
    expt_setting->ui_line[0] = exp_line;
    expt_setting->ui_pixel[0] = 0;

    //Calculates compensation ratio
    real_exp_time = exp_line * t_row / 10;

    if (real_exp_time < exp_time) {
        compensate_ratio = (exp_time * 100) / real_exp_time;
    } else {
        compensate_ratio = 100;
    }
    
    return compensate_ratio;
}

static ER get_expo_setting_gc2053p(SENSOR_ID id, SENSOR_MODE mode, UINT32 *exp_time, EXPOSURE_SETTING *expt_setting, UINT32 *compensate_ratio)
{
    SENSOR_MODE_INFO *mode_info;
    UINT32 frame_cnt, total_frame;
    
    mode_info = get_mode_info_gc2053p(mode);
    
    //Get total frame number
    if (mode_info->multi_frame_info != NULL) {
        total_frame = mode_info->multi_frame_info->total_frm;
        if (total_frame == 0) {
            DBG_WRN("total_frame = 0, force to 1 \r\n");
            total_frame = 1;
        }        
    } else {
        total_frame = 1;
    }
    
    //Calculates and set the exposure setting
    for (frame_cnt = 0; frame_cnt < total_frame; frame_cnt++) {
        EXPOSURE_SETTING exp_time_set = {0};
        compensate_ratio[frame_cnt] = calc_expo_setting_gc2053p(id, exp_time[frame_cnt], &(exp_time_set), mode, frame_cnt);
        expt_setting->ui_frame[frame_cnt] = exp_time_set.ui_frame[0];
        expt_setting->ui_line[frame_cnt] = exp_time_set.ui_line[0];
        expt_setting->ui_pixel[frame_cnt] = exp_time_set.ui_pixel[0];
        DBG_IND("Time = %d %d %d %d\r\n", exp_time[frame_cnt], expt_setting->ui_frame[frame_cnt], expt_setting->ui_line[frame_cnt], expt_setting->ui_pixel[frame_cnt]);
    }
    return E_OK;
}

static ER calc_gain_setting_gc2053p(SENSOR_ID id, UINT32 gain_ratio, GAIN_SETTING *gain)
{
    UINT32 i = 0, step = 0, digital_gain = 0;

    if ( MIN_GAIN_RATIO > gain_ratio ) {
        gain_ratio = MIN_GAIN_RATIO;
    } else if ( MAX_GAIN_RATIO < gain_ratio ) {
        gain_ratio = MAX_GAIN_RATIO;
    }
			
    if (gain_ratio > gain_table[NUM_OF_GAINSET - 1].total_gain) {
        gain_ratio = gain_table[NUM_OF_GAINSET - 1].total_gain;
    } else if(gain_ratio < gain_table[0].total_gain) {
        gain_ratio = gain_table[0].total_gain;
    }
    //Search most suitable gain into gain table
    for (i=0; i<NUM_OF_GAINSET; i++) {
        step = i;
        if (gain_table[i].total_gain > gain_ratio) {
            break;
        }
    }

    //Collect the gain setting	
    if ( 1 > step ) {
        digital_gain = gain_ratio * 1000 / (gain_table[0].total_gain);
        gain->data1[0] = ((gain_table[0].gain_reg1) << 8) | (gain_table[0].gain_reg2);
        gain->data2[0] = ((gain_table[0].gain_reg3) << 8) | (gain_table[0].gain_reg4);
        gain->data3[0] = digital_gain;	
    } else {
    	digital_gain = gain_ratio * 1000 / (gain_table[step - 1].total_gain);
        gain->data1[0] = ((gain_table[step-1].gain_reg1) << 8) | (gain_table[step-1].gain_reg2);
        gain->data2[0] = ((gain_table[step-1].gain_reg3) << 8) | (gain_table[step-1].gain_reg4);      
        gain->data3[0] = digital_gain;	  
    }    
							
    return E_OK;
}

static ER get_gain_setting_gc2053p(SENSOR_ID id, SENSOR_MODE mode, UINT32 *gain_ratio, GAIN_SETTING *gain)
{
    SENSOR_MODE_INFO *mode_info;
    UINT32 frame_cnt, total_frame;
    
    mode_info = get_mode_info_gc2053p(mode);
    
    if (mode_info->multi_frame_info != NULL) {
        total_frame = mode_info->multi_frame_info->total_frm;
    } else {
        total_frame = 1;
    }
    
    for (frame_cnt = 0; frame_cnt < total_frame; frame_cnt++) {
        GAIN_SETTING analog_gain = {0};
        calc_gain_setting_gc2053p(id, gain_ratio[frame_cnt], &(analog_gain));
        gain->data1[frame_cnt] = analog_gain.data1[0];
        gain->data2[frame_cnt] = analog_gain.data2[0];
        gain->data3[frame_cnt] = analog_gain.data3[0];
        DBG_IND("gain_ratio = %d 0x%.8x 0x%.8x 0x%.8x\r\n", gain_ratio[frame_cnt], gain->data1[frame_cnt], gain->data2[frame_cnt], gain->data3[frame_cnt]);
    }
    return E_OK;
}

static ER set_exptime_gc2053p(SENSOR_ID id, EXPOSURE_SETTING *expt_setting, SENSOR_MODE mode)
{
    SENSOR_CMD cmd;
    SENSOR_DRV_INFO *sensor_drv_info;
    SENSOR_MODE_INFO *mode_info;
    UINT32 exp_time = 0, sen_vd_period = 0, cur_fps = 0, new_fps = 0, sen_temp_vd = 0;
    UINT32 t_row = 0;
    ER rt = E_OK;

    //Get information
    mode_info = get_mode_info_gc2053p(mode);
    if (mode_info == NULL) {
        DBG_ERR("Sensor mode(%d) overflow\r\n", mode);
        return E_SYS;
    }
	
    sensor_drv_info = get_sensor_drv_info_gc2053p();
    if (sensor_drv_info == NULL) {
        DBG_ERR("get sensor info error\r\n");
        return E_SYS;
    }

    // set frame rate
    if (get_sensor_chgmode_frame_rate_gc2053p(id, mode) != 0) {
        SEN_FPS_CONV_PARAM(mode_info->sen_vd[0]->period, &sen_vd_period
        , mode_info->frame_rate, get_sensor_chgmode_frame_rate_gc2053p(id, mode));
    } else {
        sen_vd_period = mode_info->sen_vd[0]->period;
    }
    
    t_row = 100 * (mode_info->sen_hd[0]->period) / ( mode_info->pclk / 100000 );
    exp_time = t_row * (expt_setting->ui_line[0]) / 10;
    
    //DBG_ERR("t_row= %d\r\n", t_row);		

    SEN_FPS_EXPT_CONV2_FMT(exp_time, &new_fps);
    cur_fps = (SEN_FPS_FMT_CONV2_REAL(new_fps) < (SEN_FPS_FMT_CONV2_REAL(get_sensor_chgmode_frame_rate_gc2053p(id, mode)))) ? new_fps : get_sensor_chgmode_frame_rate_gc2053p(id, mode);
    SEN_FPS_CONV_PARAM(mode_info->sen_vd[0]->period, &sen_temp_vd, mode_info->frame_rate, cur_fps);		
	
    set_sensor_cur_frame_rate_gc2053p(id, mode, cur_fps);

    if ( sen_temp_vd > sen_vd_period ) {
        sen_vd_period = sen_temp_vd;
    }
	
    //Check max vts
    if (sen_vd_period > MAX_VD_PERIOD) {
        DBG_ERR("max vts overflow\r\n");
        sen_vd_period = MAX_VD_PERIOD;
    }

    //Check max exp time	
    //step 1.
    if ((expt_setting->ui_line[0]) > MAX_EXPOSURE_LINE) {    
        expt_setting->ui_line[0] = MAX_EXPOSURE_LINE;
    }
    //step 2.
    if ((expt_setting->ui_line[0]) > (sen_vd_period - NON_EXPOSURE_LINE)) {    
        expt_setting->ui_line[0] = (sen_vd_period - NON_EXPOSURE_LINE);
    }

    // Set vts to sensor
    cmd = set_cmd_info_gc2053p(0x41, 1, ( sen_vd_period >> 8)& 0x3F, 0x0);
    rt |= write_reg_gc2053p(id, &cmd);
    cmd = set_cmd_info_gc2053p(0x42, 1, sen_vd_period & 0xFF, 0x0);
    rt |= write_reg_gc2053p(id, &cmd);				
    //DBG_ERR("vts %d (0x41 0x%x 0x42 0x%x )\r\n"
    //, sen_vd_period , (sen_vd_period >> 8 )& 0xFF, (sen_vd_period & 0xFF));		
    	
    //Set exposure line to sensor
    cmd = set_cmd_info_gc2053p(0x03, 1, ( expt_setting->ui_line[0] >> 8 ) & 0x3F, 0);//[13:8]
    rt |= write_reg_gc2053p(id, &cmd);    
    cmd = set_cmd_info_gc2053p(0x04, 1, ( expt_setting->ui_line[0] & 0xFF ), 0);//[7:0]
    rt |= write_reg_gc2053p(id, &cmd);
    //DBG_ERR("exp_line %d (0x03 0x%x 0x04 0x%x)\r\n"
    //, expt_setting->ui_line[0], ( expt_setting->ui_line[0] >> 8 ) & 0x3F,( expt_setting->ui_line[0] & 0xFF));
		
    if (rt != E_OK) {
        DBG_ERR("write register error %d\r\n", rt);
    }

    return rt;
}

static ER set_gain_gc2053p(SENSOR_ID id, GAIN_SETTING *gain, SENSOR_MODE mode)
{
    SENSOR_MODE_INFO *mode_info;
    SENSOR_CMD cmd;
    UINT32 decimal = 0;
    ER rt = E_OK;

    //Get information
    mode_info = get_mode_info_gc2053p(mode);
	
    if (mode_info == NULL) {
        DBG_ERR("Sensor mode(%d) overflow\r\n", mode);
        return E_SYS;
    }

    decimal = (gain->data3[0] & 0xFFFF) * 64 / 1000;	
 
    cmd = set_cmd_info_gc2053p(0xfe, 1, 0x00, 0);
    rt |= write_reg_gc2053p(id, &cmd);
    cmd = set_cmd_info_gc2053p(0xb4, 1, (gain->data1[0] >> 8) & 0xFF , 0x0);
    rt |= write_reg_gc2053p(id, &cmd);
    cmd = set_cmd_info_gc2053p(0xb3, 1, gain->data1[0] & 0xFF, 0x0);
    rt |= write_reg_gc2053p(id, &cmd);
    cmd = set_cmd_info_gc2053p(0xb8, 1, (gain->data2[0] >> 8) & 0xFF, 0x0);
    rt |= write_reg_gc2053p(id, &cmd);
    cmd = set_cmd_info_gc2053p(0xb9, 1, gain->data2[0] & 0xFF, 0x0);
    rt |= write_reg_gc2053p(id, &cmd);		
    cmd = set_cmd_info_gc2053p(0xb1, 1, (decimal >> 6) & 0xFF , 0);
    rt |= write_reg_gc2053p(id, &cmd);
    cmd = set_cmd_info_gc2053p(0xb2, 1, ((decimal & 0x3f) << 2 ), 0);
    rt |= write_reg_gc2053p(id, &cmd);
   
    if (rt != E_OK) {
        DBG_ERR("write register error%d\r\n", rt);
    }

    return rt;
}

/**
 * Sensor internal API
*/
static ER set_vd_period_register_gc2053p(SENSOR_ID id, SENSOR_MODE mode, UINT32 frame_rate)
{
    ER rt = E_OK;
    SENSOR_MODE_INFO *mode_info;
    SENSOR_DRV_INFO *sensor_drv_info;
    UINT32 new_vd_period, update_input_frame_rate, target_fps_src = 30, target_fps_div = 1, ori_fps_src = 30, ori_fps_div = 1;
    
    sensor_drv_info = get_sensor_drv_info_gc2053p();
    if (!sensor_drv_info->fps_info.support_chg) {
        DBG_ERR("sensor driver not support frame rate adjust\r\n");
        return E_SYS;
    }
    
    //Get previous information
    mode_info = get_mode_info_gc2053p(mode);
    
    //Convert frame rate format
    SEN_FPS_CONV2_DIV(frame_rate, &target_fps_src, &target_fps_div);
    SEN_FPS_CONV2_DIV(mode_info->frame_rate, &ori_fps_src, &ori_fps_div);
    if ( target_fps_div < 1 ) target_fps_div = 1;
    if ( ori_fps_div < 1 ) ori_fps_div = 1;
    if (( target_fps_src << 10 / target_fps_div ) > ( ori_fps_src << 10 / ori_fps_div )) {
        frame_rate = mode_info->frame_rate;
    }
    SEN_FPS_CONV_PARAM(mode_info->sen_vd[0]->period, &new_vd_period, mode_info->frame_rate, frame_rate);
    
    if (new_vd_period > MAX_VD_PERIOD) {
        new_vd_period = MAX_VD_PERIOD;
        update_input_frame_rate = (mode_info->sen_vd[0]->period * (ori_fps_src * target_fps_div)) / new_vd_period;
        DBG_ERR("frame rate %d/%d out of sensor driver range, force to sensor driver min support fps %d\r\n"
        , target_fps_src, target_fps_div, update_input_frame_rate);
    }
    
    //Set new period
    vd_period[id] = new_vd_period;
    
    return rt;
}

static ER set_grs_mode_gc2053p(SENSOR_ID id, void *param)
{
    return E_OK;
}

static ER set_hdr_info_gc2053p(SENSOR_ID id, HDR_SET_INFO *hdr_info)
{
    return E_OK;
}

static ER set_user_option_gc2053p(SENSOR_ID id, UINT32 option)
{
    ER rt = E_OK;
    SENSOR_CMD cmd;
    UINT32 cmd_value, init_option = sensor_option_init[id];
    
    //Set sensor option
    if (option & SENSOR_OPTION_MIRROR) {
        sensor_option[id] |= SENSOR_OPTION_MIRROR;
    } else {
        sensor_option[id] &= ~SENSOR_OPTION_MIRROR;
    }
      
    if (option & SENSOR_OPTION_FLIP) {
        sensor_option[id] |= SENSOR_OPTION_FLIP;
    } else {
        sensor_option[id] &= ~SENSOR_OPTION_FLIP;
    }
    
    //Set sensor register
    cmd = set_cmd_info_gc2053p(0x17, 1, 0x0, 0x0);
    rt |= read_reg_gc2053p(id, &cmd);
    cmd_value = cmd.ui_data[0];
      
    if ((option & SENSOR_OPTION_MIRROR) != (init_option & SENSOR_OPTION_MIRROR)) {
        cmd_value = cmd_value | 0x01;
    } else {
        cmd_value = cmd_value & (~0x01);
    }
    
    if ((option & SENSOR_OPTION_FLIP) != (init_option & SENSOR_OPTION_FLIP)) {
        cmd_value = cmd_value | 0x02;
    } else {
        cmd_value = cmd_value & (~0x02);
    }
      
    cmd = set_cmd_info_gc2053p(0x17, 1, cmd_value, 0x0);
    rt |= write_reg_gc2053p(id, &cmd);
	
    if (rt != E_OK) {
        DBG_ERR("set sensor option error %d\r\n", rt);
    }
    
    return E_OK;
}

#if 0
-Extern api
#endif
SENSOR_DRV_TAB *sensor_get_drv_tab_gc2053p(void)
{
    return &m_p_gc2053p_drv;
}
//@@ EOF
