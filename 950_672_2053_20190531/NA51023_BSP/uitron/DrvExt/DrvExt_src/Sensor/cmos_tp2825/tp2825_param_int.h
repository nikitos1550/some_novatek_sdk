/**
    tp2825 sensor parameter .


    @file       tp2825_param_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _TP2825_PARAM_INT_H_
#define _TP2825_PARAM_INT_H_

#include "sensor.h"

/**
     Sensor mode
     plz map to enum: SENSOR_MODE sequenitally and continuously
*/

#define MODE_MAX    1               //total senMode supported by this sensor

// get sensor information
extern SENSOR_DRV_TAB *sensor_get_drv_tab_tp2825(void);
extern SENSOR_DRV_INFO *get_sensor_drv_info_tp2825(void);
extern SENSOR_MODE_INFO *get_mode_info_tp2825(SENSOR_MODE mode);
extern UINT32 get_cmd_tab_tp2825(SENSOR_MODE mode, SENSOR_CMD **cmd_tab);
extern SEN_CMD_INFO *get_sensor_cmd_info_tp2825(void);
extern SEN_IF_INFO *get_sensor_if_info_tp2825(void);
// frame rate
extern void set_sensor_cur_frame_rate_tp2825(SENSOR_ID id, SENSOR_MODE mode, UINT32 frame_rate);
extern UINT32 get_sensor_cur_frame_rate_tp2825(SENSOR_ID id, SENSOR_MODE mode);
extern void set_sensor_chgmode_frame_rate_tp2825(SENSOR_ID id, SENSOR_MODE mode, UINT32 frame_rate);
extern UINT32 get_sensor_chgmode_frame_rate_tp2825(SENSOR_ID id, SENSOR_MODE mode);
#endif
