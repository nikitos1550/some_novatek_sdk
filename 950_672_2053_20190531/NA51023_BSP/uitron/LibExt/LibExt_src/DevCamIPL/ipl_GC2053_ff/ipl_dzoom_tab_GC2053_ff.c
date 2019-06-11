/**
    IPL dzoom table GC2053.

    @file       ipl_dzoom_tab_GC2053_ff.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "ipl_GC2053_ff_int.h"

static IPL_DZOOM_SEL_INFO sensor_mode_dzoom_table[] = {
//special case for sensor auto gen
	//{SENSOR_MODE_1,61,8,0,0,IPL_CROP},
};
const static UINT32 VDOZOOM_INFOR_MODE_1_TABLE[1][DZOOM_ITEM_MAX] = {
#if _FPGA_EMULATION_
	{1280, 720, 1280, 720, 1280, 720}, //1
#else
	{1920, 1080, 1920, 1080, 1920, 1080}, //1
#endif
};

const static UINT32 VDOZOOM_INFOR_MODE_2_TABLE[1][DZOOM_ITEM_MAX] = {
#if _FPGA_EMULATION_
	{960, 240, 960, 240, 960, 240}, //1
#else
	{1920, 1080, 1920, 1080, 1920, 1080}, //1
#endif
};

UINT32 *SenMode2TblFp_GC2053_ff(UINT32 id,UINT32 *DzMaxidx)
{
	UINT32 *Ptr = NULL;
	UINT32 tbl_size,i;
	ER rt = E_OK;
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 sen_mode;

	sen_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return NULL;
	}
	//special case for manual select dzoom table
	switch (sen_mode) {
	case SENSOR_MODE_1:
	case SENSOR_MODE_2:
	default:
#if _FPGA_EMULATION_
#if DIRECT_MODE_ENABLE
		Ptr = (UINT32 *)&VDOZOOM_INFOR_MODE_2_TABLE[0][0];
		*DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_2_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
#else
		Ptr = (UINT32 *)&VDOZOOM_INFOR_MODE_1_TABLE[0][0];
		*DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
#endif
#else
        Ptr = NULL;
        *DzMaxidx = 0;
#endif
		break;
	}
	Ptr = NULL;
	tbl_size = sizeof(sensor_mode_dzoom_table) / sizeof(IPL_DZOOM_SEL_INFO);
	//auto gen dzoom table
	if(tbl_size == 0){
		//use default setting
		Ptr = ChkSenModeTbl(id, &sen_status.sensor_info, Ptr, DzMaxidx, NULL);
	} else {
		for(i=0;i<tbl_size;i++){
			if(sensor_mode_dzoom_table[i].mode == sen_mode){
				Ptr = ChkSenModeTbl(id, &sen_status.sensor_info, Ptr, DzMaxidx, &sensor_mode_dzoom_table[i]);
			} else {
				//use default setting
				Ptr = ChkSenModeTbl(id, &sen_status.sensor_info, Ptr, DzMaxidx, NULL);
			}
		}
	}

	return Ptr;
}
