#ifndef _IPL_EXT_COMMON_H_
#define _IPL_EXT_COMMON_H_

//other
#include "Type.h"
#include "DxSensor.h"
#include "ae_api.h"
#include "ae_task.h"
#include "awb_api.h"
#include "awb_task.h"
#include "af_api.h"
#include "af_task.h"
#include "iq_api.h"
#include "iq_task.h"
#include "iq_ctrl.h"
#include "md_module.h"
#include "DxSys.h"
#include "shdr_ctrl.h"
#include "SceneRenderLib.h"
#include "DefogLib.h"
#include "SensorHDRLib.h"
#include "dma.h"
#include "adas_Apps.h"

//ipl common
#include "ipl_api.h"
#include "ipl_utility.h"
#include "ipl_buffer.h"
#include "ipl_header_infor_raw.h"
#include "ipl_header_infor_yuv.h"
#include "ipl_icf_func.h"
#include "ipl_cb_msg.h"
#include "ipl_dzoom.h"
#include "ipl_alg_infor.h"
#include "ipl_cmd.h"
#include "ipl_ctrl_par.h"
#include "ipl_ctrl.h"
#include "ipl_debug_infor.h"
#include "ipl_ctrl_infor.h"
#include "ipl_cal_api.h"

//ext
#include "ipl_ext_cmd_int.h"
#include "ipl_ext_buffer_int.h"
#include "ipl_ext_ctrl_int.h"
#include "ipl_ext_ctrl_flow_common_int.h"
#include "ipl_ext_isr_int.h"
#include "ipl_ext_flow_cb_int.h"
#include "ipl_ext_extend_func_int.h"

#define __MODULE__ IPL
#define __DBGLVL__ 4          // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

/////////IPL FUNCTION SUPPORT ENABLE//////////
#define IPL_SHDR_FUNC_SUPPORT 	ENABLE
#define IPL_WDR_FUNC_SUPPORT 	ENABLE
#define IPL_DEFOG_FUNC_SUPPORT 	ENABLE
#define IPL_AF_FUNC_SUPPORT 	ENABLE
#define IPL_IMEMD_FUNC_SUPPORT 	ENABLE
//////////////////////////////////////////////

#define IPL_HDR_RINGBUF_RESERVE_SIZE  		(8)
#define total_dbg_data_size 				(0x400)	//total reserve size for debug data
#define sys_dbg_data_size 					(0x80)	//sys dbg data size
#define DIR_SIE_MAX_WIDTH 					(2592)	//direct mode maximum width
#define IPL_TIMEOUT_MS    					(500)  	//ms
#define IPL_DIRECT_SIE_PPB  				(1)		//direct mode sie2 ppb force set to 1
#define IPL_MULTI_STRP_IME_TMNR_REF_PPB   	(2)	//multi stripe ime tmnr need 2 ref ppb
#define IPL_SINGLE_STRP_IME_TMNR_REF_PPB  	(1)	//single stripe ime tmnr need 1 ref ppb

extern BOOL b_ipl_ctrl_dbg_en;  //ipl control debug enable/disable
extern UINT32 ipl_yuv_buf_dump_id;  //ipl yuv buffer status
extern UINT32 ipl_yuv_buf_dump_cnt;

typedef enum _IPL_DZOOM_CROP_SEL {
	SIE_CROP = 0,
	IPL_CROP,
	ENUM_DUMMY4WORD(IPL_DZOOM_CROP_SEL)
} IPL_DZOOM_CROP_SEL;

typedef struct _IPL_SIE_SIG_INFO {
	UINT32 vd_inv;
	UINT32 hd_inv;
	SIG_PHASE vd_phase;
	SIG_PHASE hd_phase;
	SIG_PHASE data_phase;
} IPL_SIE_SIG_INFO;

typedef struct _IPL_ID_GROP_INFO
{
    UINT32 group;
    UINT32 order;
} IPL_ID_GROP_INFO;

typedef struct _IPL_SEN_MODE_SEL_INFO
{
    UINT32 img_size_h;
    UINT32 img_size_v;
    UINT32 img_fps;
    SEL_SHDR_FRM_NUM hdr_frm_num;
	SENSOR_MODE sen_mode;
} IPL_SEN_MODE_SEL_INFO;

typedef struct _IPL_DZOOM_SEL_INFO
{
	UINT32 mode;
    UINT32 max_index;
	UINT32 factor;
	UINT32 img_size_h;  //ipl input horizontal size(0 = default)
	UINT32 img_size_v;  //ipl input vertical size(0 = default)
	IPL_DZOOM_CROP_SEL crop_p; //crop position
} IPL_DZOOM_SEL_INFO;

extern void IPL_ChkSensorMode(IPL_PROC_ID id, IPL_MODE mode);
extern UINT32 *ChkSenModeTbl(UINT32 Id, SENSOR_INFO *SenInfo, UINT32 *TabPtr, UINT32 *DzMaxidx, IPL_DZOOM_SEL_INFO *DzInfo);
extern ER IPL_Ctrl_Sen_ChgMode(UINT32 id, SEN_CHGMODE_DATA sen_mode_data);
#endif //_IPL_EXT_COMMON_H_
