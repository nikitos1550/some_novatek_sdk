/*
    IPL mode API

    This file is IPL mode control.

    @file       ipl_mode_sim_ff.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "ipl_sim_ff_int.h"
#include "iq_sim.h"

static ER IPL_GetSensorInfoFp(UINT32 id, UINT32 ipl_mode, SENSOR_INFO *sen_info);
static UINT32 *IPL_GetModeFp(UINT32 cur_ipl_mode, UINT32 next_ipl_mode);

static UINT32 ipl_id_map_table[IPL_ID_MAX_NUM][SEN_MFRAME_MAX_NUM + 1] = {
	{IPL_BIT_ID_NONE,   IPL_BIT_ID_1, (IPL_BIT_ID_1 | IPL_BIT_ID_2)},
	{IPL_BIT_ID_NONE,   IPL_BIT_ID_2, (IPL_BIT_ID_1 | IPL_BIT_ID_2)},
};

static const IPL_ID_GROP_INFO ipl_id_group_info[IPL_ID_MAX_NUM] = {
	//	group,  order
	{   0,      0},
	{   1,      0},
};

static IPL_SEN_MODE_SEL_INFO sensor_mode_sel_table[] = {
#if 0	//special case for manual select sensor mode
	{1920,   1080, 	IPL_UTI_CONV2_UINT32(30, 1),	SEL_SHDR_FRAME_NONE,	SENSOR_MODE_1},
	{1920,   1080, 	IPL_UTI_CONV2_UINT32(60, 1),	SEL_SHDR_FRAME_NONE,	SENSOR_MODE_2},
	{1280,   720,	IPL_UTI_CONV2_UINT32(30, 1),	SEL_SHDR_FRAME_NONE, 	SENSOR_MODE_1},
#endif
};

static IPL_EXT_LIB_TAB m_p_ipl_ext_lib = {
	IPL_GetSensorInfoFp,
	IPL_GetModeFp,
	SenMode2TblFp_sim_ff,
};

static void IPL_SetSensorMode(IPL_PROC_ID id, IPL_MODE mode)
{
	UINT32 img_size_h, img_size_v;
	UINT32 img_fps_src, img_fps_div, img_fps, ui_frm_num = SEL_SHDR_FRAME_NONE;
	UINT32 tbl_size, i;

	//special case for manual select sensor mode
	img_fps_src = (IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_FPS) & 0xffff0000) >> 16;
	img_fps_div = (IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_FPS) & 0x0000ffff);
	if (img_fps_div == 0 || img_fps_src == 0) {
		img_fps_src = 30;
		img_fps_div = 1;
		DBG_ERR("IPL_SEL_VIDEO_FPS %d Error\r\n", IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_FPS));
	}

	img_fps = IPL_UTI_CONV2_UINT32(img_fps_src,img_fps_div);

	if (IPL_CtrlGetInfor(id, IPLCTRL_FUNC_EN) & IPL_FUNC_SHDR) {
		ui_frm_num = IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_SHDR_FRAME_NUM);
	}

	img_size_h = (IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_SIZE) & 0xffff0000) >> 16;
	img_size_v = (IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_SIZE) & 0x0000ffff);

	tbl_size = sizeof(sensor_mode_sel_table) / sizeof(IPL_SEN_MODE_SEL_INFO);
	//set streaming sensor mode
	for (i = 0; i < tbl_size; i ++) {
		if (sensor_mode_sel_table[i].img_size_h == img_size_h &&
			sensor_mode_sel_table[i].img_size_v == img_size_v &&
			sensor_mode_sel_table[i].img_fps == img_fps &&
			sensor_mode_sel_table[i].hdr_frm_num == ui_frm_num) {
			IPL_AlgSetUIInfo(id, IPL_SEL_PRVSENMODE, sensor_mode_sel_table[i].sen_mode);
			return;
		}
	}

	DBG_IND("ui size = %d %d, fps = %d/%d, ui_frm_num = %d\r\n", img_size_h, img_size_v, img_fps_src, img_fps_div, ui_frm_num);

	//auto select sensor mode
	IPL_ChkSensorMode(id, mode);
}


static ER IPL_GetSensorInfoFp(UINT32 id, UINT32 ipl_mode, SENSOR_INFO *sen_info)
{
	ER rt = E_OK;
	SENSOR_MODE sen_mode;
	SEN_STATUS_PARAM sen_status = {0};

	//decide sensor mode
	IPL_SetSensorMode(id, ipl_mode);
	//get sensor status
	sen_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);

	if (sen_mode == IPL_CTRL_NO_USE) {
		DBG_ERR("Get Sensor mode fail, Force Set Sensor Mode to SENSOR_MODE_1\r\n");
		sen_mode = SENSOR_MODE_1;
	}

	sen_status.sensor_mode = sen_mode;
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	*sen_info = sen_status.sensor_info;

	if (rt != E_OK || sen_info->mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), sen_mode);
		return E_SYS;
	} else if (sen_info->mode->multi_frame_info == NULL) {
		DBG_WRN("Get Sensor multi_frame_info Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), sen_mode);
		IPL_CtrlSetInfor(id, IPLCTRL_MF_TOTAL_FRM_NUM, 1);
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_MF_TOTAL_FRM_NUM, sen_info->mode->multi_frame_info->total_frm);
	}

	//set gropu/order idx
	IPL_CtrlSetInfor(id, IPLCTRL_GROUP_IDX, ipl_id_group_info[IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)].group);
	IPL_CtrlSetInfor(id, IPLCTRL_ORDER_IDX, ipl_id_group_info[IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)].order);

	//set multi frame info for sensor lib and flow control
	IPL_CtrlSetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT, ipl_id_map_table[IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)][IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_NUM)]);

	if (ipl_mode == IPL_MODE_CAP) {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_TOTAL_FRM, IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_NUM));
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_FRM_BIT, ipl_id_map_table[IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)][IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_NUM)]);
	} else {
	}

	return E_OK;
}

static ER IPL_Query_Buffer(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode)
{
	UINT32 *zoom_tab, zoom_tab_max = 0;
	SENSOR_INFO info = {0};
	IPL_BUF_OBJ_INFO buf_obj;
	IPLBUF_ENG2DRAMINFO buf_info;
	IPLBUF_RST buf_rst = {0};
	IPLCTRL_INFO ctrl_info;
	UINT32 width = 0, height = 0;
	ER rt = E_OK;
	UINT32 old_dzoom_idx;

	IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_SIE_CH0_RAWENC_BUF_RATIO, RAW_ENC_BUF_RATIO);
	old_dzoom_idx = IPL_DzoomGetIndex(chg_mode->proc_sen_id);

	if (chg_mode->func_en & IPL_FUNC_RAW2YUV_FLOW) {
		IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_PRVFLOW, ICF_FLOW_VIRTUAL);
 	} else {
		rt = IPL_GetSensorInfoFp(chg_mode->proc_sen_id, chg_mode->mode, &info);
		if (rt != E_OK) {
			return E_SYS;
		}
		if (IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID) == chg_mode->proc_sen_id) {
			iq_init(chg_mode->proc_sen_id);
		}

		//dzoom table init
		IPL_DzoomSetIndex(chg_mode->proc_sen_id, 0);
		zoom_tab = ipl_ext_senmode2tbl(IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID),&zoom_tab_max);
		IPL_DZoomSetInfor(chg_mode->proc_sen_id, zoom_tab, zoom_tab_max);

		//decide ipl control flow
		if (info.mode->mode_type == SENSOR_MODE_CCIR || info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
			IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_PRVFLOW, ICF_FLOW_CCIR);
		} else {
			if (chg_mode->ext_ctrl_en & IPL_EXT_CTRL_RAW_DIRECT) {
				IPL_DzoomGetSIECropSize(chg_mode->proc_sen_id, 0, FALSE, &width, &height);
				if (width > DIR_SIE_MAX_WIDTH) {
					DBG_WRN("Direct mode not support SIE Crop Size over than %d, force Disable Direct Mode\r\n", DIR_SIE_MAX_WIDTH);
					IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_PRVFLOW, ICF_FLOW_C);
				} else {
					IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_PRVFLOW, ICF_FLOW_DIRECT);
				}
			} else {
				IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_PRVFLOW, ICF_FLOW_C);
			}
		}
	}
	//prv parameter init
	ctrl_info.info = &info;
	ctrl_info.chg_mode = chg_mode;
	IPL_CtrlPrvInit(chg_mode->proc_sen_id, &ctrl_info);

	//allocate buffer & initial buffer size
	buf_obj.id = chg_mode->proc_sen_id;
	buf_obj.buf_addr = chg_mode->buf_addr;
	buf_obj.buf_size = chg_mode->buf_size;
	IPL_BufInit(&buf_obj);

	IPL_CtrlInitPrvbuf_info(chg_mode, &buf_info);
	buf_rst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_PRVFLOW)), &buf_info);
	chg_mode->buf_size = buf_rst.buf_size;

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("IPL_ID_%d_Query FUNC_EN = 0x%x\r\n", chg_mode->proc_sen_id+1, chg_mode->func_en);
		IPL_BufDump(buf_obj.id); //DEBUG Using, use for dump buffer information
	}
	IPL_DzoomSetIndex(chg_mode->proc_sen_id, old_dzoom_idx);

	return E_OK;
}

static ER IPL_Off2Stream(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode)
{
	ICFFUNC_OBJ_INFO ICFFuncObj = {0};
	SENSOR_INFO info = {0};
	AE_OBJ_INFO AeObj = {0};
	AWB_OBJ_INFO AwbObj = {0};
	AF_OBJ_INFO AfObj = {0};
	IQS_OBJ_INFO IqsObj = {0};
	ER rt = E_OK;

	//decide ipl control flow
	if (chg_mode->func_en & IPL_FUNC_RAW2YUV_FLOW) {
		IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_PRVFLOW, ICF_FLOW_VIRTUAL);
		return E_OK;
	}

	rt = IPL_GetSensorInfoFp(chg_mode->proc_sen_id, chg_mode->mode, &info);
	if (rt != E_OK) {
		return E_SYS;
	}


	if (info.mode->mode_type == SENSOR_MODE_CCIR || info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
		IPL_CBMsgInit(chg_mode->proc_sen_id, chg_mode->cb_fp);
		IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_PRVFLOW, ICF_FLOW_CCIR);

#if IPL_AF_FUNC_SUPPORT
		AfObj.EventTab.Start = AF_Init;
		AfObj.EventTab.Process = AF_Process;
		AfObj.EventTab.Pause = NULL;
		AF_Open(IPL_UTI_CONV2_AF_ID(chg_mode->proc_sen_id), &AfObj);
		sensor_preset_lock(IPL_UTI_CONV2_SEN_ID(chg_mode->proc_sen_id), ENABLE);
		sensor_preset_mode(IPL_UTI_CONV2_SEN_ID(chg_mode->proc_sen_id), IPL_AlgGetUIInfo(chg_mode->proc_sen_id, IPL_SEL_PRVSENMODE),IPL_AlgGetUIInfo(chg_mode->proc_sen_id, IPL_SEL_VIDEO_FPS));
		AF_Start(IPL_UTI_CONV2_AF_ID(chg_mode->proc_sen_id), FALSE);
		sensor_preset_lock(IPL_UTI_CONV2_SEN_ID(chg_mode->proc_sen_id), DISABLE);
#endif

		IqsObj.EventTab.Start = IQS_Ctrl_Init;
		IqsObj.EventTab.Process = IQS_Ctrl_Process;
		IqsObj.EventTab.Pause = NULL;
		IQS_Open(IPL_UTI_CONV2_IQS_ID(chg_mode->proc_sen_id), &IqsObj);
		IQS_Start(IPL_UTI_CONV2_IQS_ID(chg_mode->proc_sen_id), TRUE);
	} else {
		IPL_AlgSetEventFP(chg_mode->proc_sen_id, IPL_SEL_IMAGEFLIP_IMM, IPL_FCB_ImageFlip);
		IPL_AlgSetEventFP(chg_mode->proc_sen_id, IPL_SEL_SHDR, IPL_FCB_AlgDR);
		IPL_AlgSetEventFP(chg_mode->proc_sen_id, IPL_SEL_WDR_IMM, IPL_FCB_AlgDR);
		IPL_AlgSetEventFP(chg_mode->proc_sen_id, IPL_SEL_DEFOG_IMM, IPL_FCB_AlgDR);

		IPL_CBMsgInit(chg_mode->proc_sen_id, chg_mode->cb_fp);

		if (IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID) == chg_mode->proc_sen_id) {
			AeObj.EventTab.Start = AE_Init;
			AeObj.EventTab.Process = AE_Process;
			AeObj.EventTab.Pause = NULL;
			AeObj.EventTab.HdrNotify = NULL;
			AeObj.EventTab.HdrNotify = NULL;//ICFFunc_TrigEvent;

			AwbObj.EventTab.Start = AWB_Init;
			AwbObj.EventTab.Process = AWB_Process;
			AwbObj.EventTab.Pause = NULL;

#if IPL_AF_FUNC_SUPPORT
			AfObj.EventTab.Start = AF_Init;
			AfObj.EventTab.Process = AF_Process;
			AfObj.EventTab.Pause = NULL;
#endif

			AE_Open(IPL_UTI_CONV2_AE_ID(chg_mode->proc_sen_id), &AeObj);
			AWB_Open(IPL_UTI_CONV2_AWB_ID(chg_mode->proc_sen_id), &AwbObj);
#if IPL_AF_FUNC_SUPPORT
			AF_Open(IPL_UTI_CONV2_AF_ID(chg_mode->proc_sen_id), &AfObj);
#endif

			sensor_preset_lock(IPL_UTI_CONV2_SEN_ID(chg_mode->proc_sen_id), ENABLE);
			sensor_preset_mode(IPL_UTI_CONV2_SEN_ID(chg_mode->proc_sen_id), IPL_AlgGetUIInfo(chg_mode->proc_sen_id, IPL_SEL_PRVSENMODE),IPL_AlgGetUIInfo(chg_mode->proc_sen_id, IPL_SEL_VIDEO_FPS));
			AE_Start(IPL_UTI_CONV2_AE_ID(chg_mode->proc_sen_id), TRUE);
			AWB_Start(IPL_UTI_CONV2_AWB_ID(chg_mode->proc_sen_id), FALSE);
#if IPL_AF_FUNC_SUPPORT
			AF_Start(IPL_UTI_CONV2_AF_ID(chg_mode->proc_sen_id), FALSE);
#endif
			sensor_preset_lock(IPL_UTI_CONV2_SEN_ID(chg_mode->proc_sen_id), DISABLE);

			IqsObj.EventTab.Start = IQS_Ctrl_Init;
			IqsObj.EventTab.Process = IQS_Ctrl_Process;
			IqsObj.EventTab.Pause = NULL;
			IQS_Open(IPL_UTI_CONV2_IQS_ID(chg_mode->proc_sen_id), &IqsObj);

			IQS_Start(IPL_UTI_CONV2_IQS_ID(chg_mode->proc_sen_id), TRUE);

			ICFFuncObj.EventTab.Process = ExtFunc_Process;
			ICFFunc_Open(chg_mode->proc_sen_id, &ICFFuncObj);
		}
	}

	return E_OK;
}

static ER IPL_Stream2Cap(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode)
{
	UINT32 *zoom_tab, zoom_tab_max = 0;
	SENSOR_INFO info = {0};
	IPL_BUF_OBJ_INFO buf_obj;
	IPLBUF_RST buf_rst;
	IPLCTRL_INFO ctrl_info;
	IPLBUF_ENG2DRAMINFO buf_info;
	ER rt = E_OK;

	if (chg_mode->event_id != IPL_FLOW_VID_GETRAW) {
		rt = IPL_GetSensorInfoFp(chg_mode->proc_sen_id, chg_mode->mode, &info);
		if (rt != E_OK) {
			return E_SYS;
		}
		zoom_tab = ipl_ext_senmode2tbl(IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID),&zoom_tab_max);
		IPL_DZoomSetInfor(chg_mode->proc_sen_id, zoom_tab, zoom_tab_max);

		IPL_Wait_Vd((IPL_PROC_BIT_ID)(1 << chg_mode->proc_sen_id), FALSE, IPL_TIMEOUT_MS);
		//initial capture parameter
		ctrl_info.info = &info;
		ctrl_info.chg_mode = chg_mode;
		IPL_CtrlCapInit(chg_mode->proc_sen_id, chg_mode->event_id, &ctrl_info);

		//conver to cap buffer
		buf_obj.id = chg_mode->proc_sen_id;
		buf_obj.buf_addr = chg_mode->buf_addr;
		buf_obj.buf_size = chg_mode->buf_size;
		IPL_BufInit(&buf_obj);
		IPL_CtrlInitCapbuf_info(buf_obj.id, chg_mode, &buf_info);
		buf_rst = IPL_BufAssign(IPLBUF_MODE_CAP, &buf_info);
		chg_mode->buf_size = buf_rst.buf_size;
	}

	return E_OK;
}

static ER IPL_Cap2Stream(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode)
{
	UINT32 zoom_idx[IPL_ID_MAX_NUM] = {0};
	IPC_CTRL_CMD proc_cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	IPL_PROC_BIT_ID ipl_proc_id_bit = 0;
	UINT32 *zoom_tab, zoom_tab_max = 0;
	SENSOR_INFO info = {0};
	ER rt = E_OK;
	IPLCTRL_INFO ctrl_info;
	SEN_CHGMODE_DATA sen_chg_mode = {0};
	IPL_IME_PATH_INFO PathInfo[IPL_IME_PATH_ID_MAX];
	UINT32 crop_ratio;

	//keep runtime update information
	crop_ratio = IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_SEN_PRV_CROP_RATIO);
	IPL_Getime_path_Info(chg_mode->proc_sen_id, IPL_IME_PATH1, &PathInfo[IPL_IME_PATH_ID_1]);
	IPL_Getime_path_Info(chg_mode->proc_sen_id, IPL_IME_PATH2, &PathInfo[IPL_IME_PATH_ID_2]);
	IPL_Getime_path_Info(chg_mode->proc_sen_id, IPL_IME_PATH3, &PathInfo[IPL_IME_PATH_ID_3]);
	IPL_Getime_path_Info(chg_mode->proc_sen_id, IPL_IME_PATH4, &PathInfo[IPL_IME_PATH_ID_4]);

	rt = IPL_GetSensorInfoFp(chg_mode->proc_sen_id, chg_mode->mode, &info);
	if (rt != E_OK) {
		return E_SYS;
	}

	if (info.mode->mode_type == SENSOR_MODE_CCIR || info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
		proc_cmd[chg_mode->proc_sen_id] = IPC_STOP_ALL;
	} else {
		proc_cmd[chg_mode->proc_sen_id] = IPC_STOP_RAW;
	}

	//initial sensor mode
	if (IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID) == chg_mode->proc_sen_id) {
		ipl_proc_id_bit = IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_TOTAL_FRM_BIT);
		IPL_Ctrl_Run(ipl_proc_id_bit, &proc_cmd[IPL_ID_1]);

		sen_chg_mode.mode = IPL_AlgGetUIInfo(chg_mode->proc_sen_id, IPL_SEL_PRVSENMODE);
		sen_chg_mode.multi_id = IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_TOTAL_FRM_BIT);
		sen_chg_mode.frame_rate = IPL_AlgGetUIInfo(chg_mode->proc_sen_id, IPL_SEL_VIDEO_FPS);
		IPL_Ctrl_Sen_ChgMode(chg_mode->proc_sen_id, sen_chg_mode);
	}
	IPL_Query_Buffer(chg_mode, prv_mode);

	zoom_tab = ipl_ext_senmode2tbl(IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID),&zoom_tab_max);
	IPL_DZoomSetInfor(chg_mode->proc_sen_id, zoom_tab, zoom_tab_max);

	rt = IPL_GetSensorInfoFp(chg_mode->proc_sen_id, chg_mode->mode, &info);
	if (rt != E_OK) {
		return E_SYS;
	}

	//decide preivew flow
	ctrl_info.info = &info;
	ctrl_info.chg_mode = chg_mode;
	IPL_CtrlPrvInit(chg_mode->proc_sen_id, &ctrl_info);

	IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_SEN_PRV_CROP_RATIO, crop_ratio);
	IPL_Setime_path_Info(chg_mode->proc_sen_id, IPL_IME_PATH1, &PathInfo[IPL_IME_PATH_ID_1]);
	IPL_Setime_path_Info(chg_mode->proc_sen_id, IPL_IME_PATH2, &PathInfo[IPL_IME_PATH_ID_2]);
	IPL_Setime_path_Info(chg_mode->proc_sen_id, IPL_IME_PATH3, &PathInfo[IPL_IME_PATH_ID_3]);
	IPL_Setime_path_Info(chg_mode->proc_sen_id, IPL_IME_PATH4, &PathInfo[IPL_IME_PATH_ID_4]);

	//initial digital zoom
	zoom_idx[chg_mode->proc_sen_id] = IPL_DzoomGetIndex(chg_mode->proc_sen_id);
	IPL_DzoomSetIndex(chg_mode->proc_sen_id, zoom_idx[chg_mode->proc_sen_id]);
	IPL_CtrlPrvSetDZoomInfo(chg_mode->proc_sen_id);

	return E_OK;
}

static ER IPL_Cap2Off(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode)
{
	DBG_IND("do nothing\r\n");
	return E_OK;
}

static ER IPL_Off2Cap(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode)
{
    SENSOR_INFO info = {0};
    IPL_BUF_OBJ_INFO buf_obj;
    IPLBUF_RST buf_rst;
    IPLBUF_ENG2DRAMINFO buf_info;
    //UINT32 Addr=0, Size=0;
	ER rt = E_OK;

    DBG_ERR("\r\n\r\n--->off2cap<---\r\n\r\n");
    //need assigned raw 0 & raw 2 data information
    UINT32 Width0, Height0, BitNum0;
    UINT32 Id;
    IQSIM_APPINFO *AppInfo = IQSIM_GetAppInfo();
    Id = chg_mode->proc_sen_id;
    //IPL_CtrlSetInfor(Id, IPLCTRL_CAPBURSTLENGTH, _BURST_128WORD);

    IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NORMAL);

	rt = IPL_GetSensorInfoFp(chg_mode->proc_sen_id, chg_mode->mode, &info);
	if (rt != E_OK) {
		return E_SYS;
	}

	//set multi frame information from sensor lib and Libext
	IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_TOTAL_FRM_NUM, info.mode->multi_frame_info->total_frm);
	IPL_CtrlSetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_TOTAL_FRM_BIT, ipl_id_map_table[IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID)][IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_TOTAL_FRM_NUM)]);

	DBG_DUMP("IPL_Off2Cap_sampl-->id: %d, base id: %d, total frm: %d, total id bit: %x\r\n",
			 chg_mode->proc_sen_id,
			 IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID),
			 IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_TOTAL_FRM_NUM),
			 IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_TOTAL_FRM_BIT));

    IPL_CtrlSetInfor(Id, IPLCTRL_SEN_CAP_CROP_RATIO, 0x00040003);

    IPL_CtrlSetInfor(Id, IPLCTRL_CAPAUTO_OB_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_CAPDF_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_IME_CAP_SUBRATIO, 25/*iq_get_ife2_lca_ratio(Id)*/);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPSTPIX, AppInfo->IQSimPriInfo.RawInfo.cfa);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_DATAFMT, AppInfo->IQSimPriInfo.RawInfo.format);
    //IPL_CtrlSetInfor(Id, IPLCTRL_IPE_CAP_SUBRATIO, 100);

    //if (iq_check_ife2_lca(Id, TRUE) == ENABLE)
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_LCA_EN, ENABLE);
    //else
    //    IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_LCA_EN, DISABLE);

    Width0  = AppInfo->IQSimPriInfo.RawInfo.Size.w;
    Height0 = AppInfo->IQSimPriInfo.RawInfo.Size.h;
    BitNum0 = AppInfo->IQSimPriInfo.RawInfo.BitNum;

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_H, Width0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_V, Height0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM, 1);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H, Width0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAPBIT, BitNum0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width0, BitNum0));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V, Height0);

    IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_SIM);

    UINT32 *zoom_tab, zoom_tab_max = 0;
    zoom_tab = ipl_ext_senmode2tbl(IPL_CtrlGetInfor(chg_mode->proc_sen_id, IPLCTRL_MF_BASE_ID),&zoom_tab_max);
    zoom_tab[0] = zoom_tab[2] = zoom_tab[4] = Width0;
    zoom_tab[1] = zoom_tab[3] = zoom_tab[5] = Height0;
    IPL_DZoomSetInfor(Id, zoom_tab, zoom_tab_max);

	//conver to cap buffer
	buf_obj.id = chg_mode->proc_sen_id;
	buf_obj.buf_addr = chg_mode->buf_addr;
	buf_obj.buf_size = chg_mode->buf_size;
	IPL_BufInit(&buf_obj);
	IPL_CtrlInitCapbuf_info(buf_obj.id, chg_mode, &buf_info);
	buf_rst = IPL_BufAssign(IPLBUF_MODE_CAP, &buf_info);
	chg_mode->buf_size = buf_rst.buf_size;


#if IQS_DUMP_ENGREG
    IPL_BufGet(buf_obj.Id, (IPLBUF_DRE_DBGBUF), &Addr, &Size);
    //IQS_SetEngDumpBuffer(IPL_HAL_DRE, Addr, Size);
    DBG_ERR("dre info 0x%08x   0x%08x\r\n", Addr, Size);
#endif
    return E_OK;
}

static ER IPL_Stream2Off(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode)
{
	IPL_AlgSetEventFP(chg_mode->proc_sen_id, IPL_SEL_IMAGEFLIP_IMM, NULL);
	IPL_AlgSetEventFP(chg_mode->proc_sen_id, IPL_SEL_SHDR, NULL);
	IPL_AlgSetEventFP(chg_mode->proc_sen_id, IPL_SEL_WDR_IMM, NULL);
	IPL_AlgSetEventFP(chg_mode->proc_sen_id, IPL_SEL_DEFOG_IMM, NULL);

	IPL_RSC_CBMsgProc(chg_mode->proc_sen_id, IPL_RSC_CBMSG_CHGMODE_IPLSTOP, NULL);

	//uninitial interface/CB module
	IPL_CBMsgUnInit(chg_mode->proc_sen_id);
	IPL_BufUnInit(chg_mode->proc_sen_id);
	IPL_DzoomSetIndex(chg_mode->proc_sen_id, 0);
	return E_OK;
}

static ER IPL_Stream2Stream(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode)
{
	IPL_CBMsgInit(chg_mode->proc_sen_id, chg_mode->cb_fp);
	//decide sensor mode
	IPL_SetSensorMode(chg_mode->proc_sen_id, chg_mode->mode);
	return E_OK;
}

static IPL_CMD_CHGMODE_FP ChgModeAct[IPL_MODE_NUM][IPL_MODE_NUM] = {
	//next mode
	/*IPL_MODE_OFF,     IPL_MODE_PREVIEW,   IPL_MODE_VIDEO,     IPL_MODE_VIDEOREC,  IPL_MODE_CAP*/
	{NULL,              IPL_Off2Stream,     IPL_Off2Stream,     IPL_Off2Stream,     IPL_Off2Cap},       /*IPL_MODE_OFF*/
	{IPL_Stream2Off,    IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Cap},    /*IPL_MODE_PREVIEW*/
	{IPL_Stream2Off,    IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Cap},    /*IPL_MODE_VIDEO*/
	{IPL_Stream2Off,    IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Cap},    /*IPL_MODE_VIDEOREC*/
	{IPL_Cap2Off,       IPL_Cap2Stream,     IPL_Cap2Stream,     IPL_Cap2Stream,     NULL},              /*IPL_MODE_CAP*/
};

static UINT32 *IPL_GetModeFp(UINT32 cur_ipl_mode, UINT32 next_ipl_mode)
{
	if (next_ipl_mode == IPL_MODE_BUF_QUERY) {   //buffer query
		return (UINT32 *)IPL_Query_Buffer;
	} else if ((cur_ipl_mode >= IPL_MODE_NUM) || (next_ipl_mode >= IPL_MODE_NUM)) {
		return NULL;
	} else {
		return (UINT32 *)ChgModeAct[cur_ipl_mode][next_ipl_mode];
	}
}

IPL_EXT_LIB_TAB *IPL_SIM_FF_EXT_LIB_TAB(void)
{
	return &m_p_ipl_ext_lib;
}
