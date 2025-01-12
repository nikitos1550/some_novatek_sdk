#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_IQSim.h"
#include <string.h>
#include "ipl_cmd.h"
#include "ipl_alg_infor.h"
#include "ipl_ctrl_infor.h"
#include "SysCfg.h"
#include "FileSysTsk.h"
#include "imgcaptureapi.h"
#include "imgcapinfor.h"
#include "iq_sim.h"
#include "iq_api.h"
#include "ae_task.h"
#include "awb_task.h"
#include "af_task.h"
#include "ae_api.h"
#include "awb_api.h"
#include "af_api.h"
#include "ae_alg.h"
#include "awb_alg.h"
#include "af_alg.h"
#include "Lens.h"
#include "DxSensor.h"
#include "ImageUnit_UserProc.h"
#include "ImageApp_IPCam.h"
#include "ImageApp_MovieMulti.h"
#include "UIAppIPCam_Param.h"
#include "pll.h"
#include "Delay.h"
#include "sensor.h"
#include "ipl_ctrl_infor.h"
#include "SysSensor.h"
#include "NMediaRecVdoEncAPI.h"

#define THIS_DBGLVL         6 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          MsdcIQSim
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define NEW_CAPTURE				DISABLE


#define IQCMD_SET                   0x9a00ff00
#define IQCMD_SCENELOCK             0x9a00ff01
#define IQCMD_SCENEUNLOCK           0x9a00ff02
#define IQCMD_INITPARAM             0x9a00ff03
#define IQCMD_DUMPIQSETTING         0x9a00ff04
#define IQCMD_DUMPIQSETTING_SIQT    0x9a00ff05

#define IQCMD_SAVEPRVRAW_START 0x9a00f002
#define IQCMD_SAVEPRVRAW_END   0x9a00f003
#define IQCMD_PRVRAWINFO_GET   0x9a00f004
#define IQCMD_RUNPARAM         0x9a00f005
#define IQCMD_READFROMMEMORY   0x9a00f006
#define IQCMD_READFROMSTORAGE  0x9a00f006
#define IQCMD_WRITETOSTORAGE   0x9a00f007
#define IQCMD_GETMCTFOUT       0x9a00f008
#define IQCMD_GETIMEINFO       0x9a00f009

#define AECMD_ENABLE           0x9a00f101
//#define AECMD_SET              0x9a00f102
//#define AECMD_GET              0x9a00f103
//#define AECMD_LUMINANCE        0x9a00f104

#define AWBCMD_ENABLE          0x9a00f201
//#define AWBCMD_SET             0x9a00f202
//#define AWBCMD_GET             0x9a00f203

#define IQSIM_MAX_BIN_SIZE 0x100000
#define IQSIM_MAX_CH0_SIZE 0x1800000
#define IQSIM_MAX_CH2_SIZE 0x1800000

typedef struct{
	UINT32 ipcam_path_id;
    UINT32 size;

    IMG_BUF yuv_buf;
} sISP_CODED_YUV;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

	IPCAM_STREAM_INFO ipcam_stream_info[UIAPPIPCAM_PATH_MAX_ID];
    UIAPPIPCAM_SENSOR_INFO ipcam_sensor_info[UIAPPIPCAM_SENSOR_MAX];
    UIAPPIPCAM_CROP_INFO ipcam_crop_info[UIAPPIPCAM_SENSOR_MAX];
    UIAPPIPCAM_ENCODE_CONFIG ipcam_encode_config[UIAPPIPCAM_PATH_MAX_ID];
    INT32 alg_path;
    INT32 display_path;
} sISP_CODEC_INFO;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

    UIAPPIPCAM_AQ_CONFIG ipcam_aq_info[UIAPPIPCAM_PATH_MAX_ID];
} sISP_CODEC_AQ_INFO;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

	MP_VDOENC_AQ_INFO aq;
} sISP_CODEC_AQ;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

    UIAPPIPCAM_CBR_CONFIG ipcam_cbr_info[UIAPPIPCAM_PATH_MAX_ID];
} sISP_CODEC_CBR_INFO;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

	MP_VDOENC_CBR_INFO cbr;
} sISP_CODEC_CBR;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

    UIAPPIPCAM_EVBR_CONFIG ipcam_evbr_info[UIAPPIPCAM_PATH_MAX_ID];
} sISP_CODEC_EVBR_INFO;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

	MP_VDOENC_EVBR_INFO evbr;
} sISP_CODEC_EVBR;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

    UIAPPIPCAM_FIXQP_CONFIG ipcam_fix_qp_info[UIAPPIPCAM_PATH_MAX_ID];
} sISP_CODEC_FIX_QP_INFO;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

	MP_VDOENC_FIXQP_INFO fix_qp;
} sISP_CODEC_FIX_QP;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

    UIAPPIPCAM_VBR_CONFIG ipcam_vbr_info[UIAPPIPCAM_PATH_MAX_ID];
} sISP_CODEC_VBR_INFO;

typedef struct{
	UINT32 ipcam_path_id;
	UINT32 size;

	MP_VDOENC_VBR_INFO vbr;
} sISP_CODEC_VBR;
typedef struct{
	UINT32 id;
	UINT32 size;

	UINT32 ui_info_element;
	UINT32 value;
	UINT32 reserved_2;
} sISP_IQ_UI_INFO;
////////////////////////////////
//            IQSIM            /
////////////////////////////////
#ifdef IQS_SIMT_ENABLE
void IQSim_GetBinAddr(void);
void IQSim_GetRawCh0Addr(void);
void IQSim_GetRawCh2Addr(void);
void IQSim_GetJpgAddr(void);
void IQSim_GetJpgOk(void);
void IQSim_GetJpgSize(void);
void IQSim_GetQVAddr(void);
void IQSim_GetQVSize(void);
void IQSim_GetQVFmt(void);
void IQSim_GetPRIAddr(void);
void IQSim_GetPRISize(void);
void IQSim_GetPRIFmt(void);
#endif
void IQSim_GetReserved(void);
#ifdef IQS_SIMT_ENABLE
void IQSim_SetConvertInit(void);
void IQSim_SetConvertProc(void);
void IQSim_SetConvertEnd(void);
#endif
void IQSim_SetReserved(void);

///////////////////////////////////
//         ISP Setting AE         /
///////////////////////////////////
void IQSAE_get_ver(void);
void IQSAE_get_status(void);
void IQSAE_get_expect_lum(void);
void IQSAE_get_histogram_adj(void);
void IQSAE_get_over_exp(void);
void IQSAE_get_boundary(void);
void IQSAE_get_converge(void);
void IQSAE_get_manual(void);
void IQSAE_get_laclamp(void);
void IQSAE_get_curvegen(void);
void IQSAE_get_window(void);
void IQSAE_get_la(void);
void IQSAE_get_lum_gamma(void);
void IQSAE_get_shdr(void);
void IQSAE_get_uicmd(void);
void IQSAE_get_iris_cfg(void);
void IQSAE_get_curvegen_photo(void);
void IQSAE_get_la_va(void);
void IQSAE_get_reserved(void);

void IQSAE_set_expect_lum(void);
void IQSAE_set_histogram_adj(void);
void IQSAE_set_over_exp(void);
void IQSAE_set_boundary(void);
void IQSAE_set_converge(void);
void IQSAE_set_manual(void);
void IQSAE_set_laclamp(void);
void IQSAE_set_curvegen(void);
void IQSAE_set_window(void);
void IQSAE_set_lum_gamma(void);
void IQSAE_set_shdr(void);
void IQSAE_set_uicmd(void);
void IQSAE_set_iris_cfg(void);
void IQSAE_set_curvegen_photo(void);
void IQSAE_set_reserved(void);

///////////////////////////////////
//         ISP Setting AWB        /
///////////////////////////////////
void IQSAWB_get_ver(void);
void IQSAWB_get_status(void);
void IQSAWB_get_ca(void);
void IQSAWB_get_flag(void);
void IQSAWB_get_manual(void);
void IQSAWB_get_param(void);
void IQSAWB_get_converge(void);
void IQSAWB_get_dbg_msg(void);
void IQSAWB_get_reserved(void);

void IQSAWB_set_manual(void);
void IQSAWB_set_param(void);
void IQSAWB_set_converge(void);
void IQSAWB_set_dbg_msg(void);
void IQSAWB_set_reserved(void);

///////////////////////////////////
//         ISP Setting AF         /
///////////////////////////////////
void IQSAF_get_ver(void);
void IQSAF_get_enable(void);
void IQSAF_get_method(void);
void IQSAF_get_shot_mode(void);
void IQSAF_get_search_direction(void);
void IQSAF_get_thres(void);
void IQSAF_get_step_size(void);
void IQSAF_get_max_search_cnt(void);
void IQSAF_get_va(void);
void IQSAF_get_status(void);
void IQSAF_get_win_weight(void);
void IQSAF_get_dbg_msg(void);
void IQSAF_get_va_param(void);
void IQSAF_get_reserved(void);

void IQSAF_set_enable(void);
void IQSAF_set_method(void);
void IQSAF_set_shot_mode(void);
void IQSAF_set_search_direction(void);
void IQSAF_set_thres(void);
void IQSAF_set_step_size(void);
void IQSAF_set_max_search_cnt(void);
void IQSAF_set_retrigger(void);
void IQSAF_set_win_weight(void);
void IQSAF_set_dbg_msg(void);
void IQSAF_set_va_param(void);
void IQSAF_set_reserved(void);

///////////////////////////////////
//         ISP Setting IQ         /
///////////////////////////////////
void IQSISP_get_ver(void);
void IQSISP_get_ime_info(void);
void IQSISP_get_yuv_lock(void);
void IQSISP_get_raw_info(void);
void IQSISP_get_isp_info(void);
void IQSISP_get_model_type(void);
void IQSISP_get_frame_num(void);
void IQSISP_get_iq_status(void);
void IQSISP_get_iq_ui_info(void);
void IQSISP_get_dr_mode(void);
void IQSISP_get_hdr_raw_info(void);
void IQSISP_get_info_reserved(void);

//**  Get Parameter
void IQSISP_get_iso_map(void);
void IQSISP_get_ob(void);
void IQSISP_get_ecs(void);
void IQSISP_get_wdr(void);
void IQSISP_get_outlier(void);
void IQSISP_get_defog(void);
void IQSISP_get_spatial(void);
void IQSISP_get_range(void);
void IQSISP_get_rbfill(void);
void IQSISP_get_vig(void);
void IQSISP_get_gbal(void);
void IQSISP_get_ldc(void);
void IQSISP_get_edge(void);
void IQSISP_get_rgblpf(void);
void IQSISP_get_cc(void);
void IQSISP_get_y_curve(void);
void IQSISP_get_gamma(void);
void IQSISP_get_3dcc(void);
void IQSISP_get_cctrl(void);
void IQSISP_get_cst(void);
void IQSISP_get_ife2filter(void);
void IQSISP_get_lca(void);
void IQSISP_get_chroma_supp(void);
void IQSISP_get_tmnr(void);
void IQSISP_get_tmnr_debug(void);
void IQSISP_get_tmnr_auto(void);
void IQSISP_get_tnr(void);
void IQSISP_get_tnr_cfg(void);
void IQSISP_get_ce(void);
void IQSISP_get_hdr_info(void);
void IQSISP_get_ctrl(void);
void IQSISP_get_size_check(void);
void IQSISP_get_8iso_param(void);
void IQSISP_get_reserved(void);

void IQSISP_set_sensor_id(void);
void IQSISP_set_ime_path_id(void);
void IQSISP_set_iso_idx(void);
void IQSISP_set_sie_pause(void);
void IQSISP_set_mem_map(void);
void IQSISP_set_yuv_unlock(void);
void IQSISP_set_sie_resume(void);
void IQSISP_set_info_reserved(void);

//**  Set Parameter
void IQSISP_set_iso_map(void);
void IQSISP_set_ob(void);
void IQSISP_set_ecs(void);
void IQSISP_set_wdr(void);
void IQSISP_set_defog(void);
void IQSISP_set_outlier(void);
void IQSISP_set_spatial(void);
void IQSISP_set_range(void);
void IQSISP_set_rbfill(void);
void IQSISP_set_vig(void);
void IQSISP_set_gbal(void);
void IQSISP_set_ldc(void);
void IQSISP_set_edge(void);
void IQSISP_set_rgblpf(void);
void IQSISP_set_cc(void);
void IQSISP_set_y_curve(void);
void IQSISP_set_gamma(void);
void IQSISP_set_3dcc(void);
void IQSISP_set_cctrl(void);
void IQSISP_set_cst(void);
void IQSISP_set_ife2filter(void);
void IQSISP_set_lca(void);
void IQSISP_set_chroma_supp(void);
void IQSISP_set_tmnr(void);
void IQSISP_set_tmnr_debug(void);
void IQSISP_set_tmnr_auto(void);
void IQSISP_set_tnr(void);
void IQSISP_set_tnr_cfg(void);
void IQSISP_set_ce(void);
void IQSISP_set_ctrl(void);
void IQSISP_set_hdr_info(void);
void IQSISP_set_8iso_param(void);
void IQSISP_set_iq_ui_info(void);
void IQSISP_set_dr_mode(void);
void IQSISP_set_reserved(void);

///////////////////////////////////
//       ISP Setting Codec        /
///////////////////////////////////
void IQSCODEC_get_ver(void);
void IQSCODEC_get_info(void);
void IQSCODEC_get_aq_all(void);
void IQSCODEC_get_cbr_all(void);
void IQSCODEC_get_fix_qp_all(void);
void IQSCODEC_get_vbr_all(void);
void IQSCODEC_get_aq(void);
void IQSCODEC_get_cbr(void);
void IQSCODEC_get_fix_qp(void);
void IQSCODEC_get_vbr(void);
void IQSCODEC_get_evbr_all(void);
void IQSCODEC_get_evbr(void);
void IQSCODEC_get_reserved(void);

void IQSCODEC_set_ipcam_path(void);
void IQSCODEC_set_aq_all(void);
void IQSCODEC_set_cbr_all(void);
void IQSCODEC_set_fix_qp_all(void);
void IQSCODEC_set_vbr_all(void);
void IQSCODEC_set_aq(void);
void IQSCODEC_set_cbr(void);
void IQSCODEC_set_fix_qp(void);
void IQSCODEC_set_vbr(void);
void IQSCODEC_set_evbr_all(void);
void IQSCODEC_set_evbr(void);
void IQSCODEC_set_reserved(void);

///////////////////////////////////
//       ISP Setting Inter        /
///////////////////////////////////
void IQSINTER_get_direct_mode(void);
void IQSINTER_get_color_range(void);
void IQSINTER_get_sie_crop_pos(void);
void IQSINTER_get_uitron_ready(void);
void IQSINTER_get_dis_enable(void);
void IQSINTER_get_reserved(void);
void IQSINTER_set_color_range(void);
void IQSINTER_set_sie_crop_pos(void);
void IQSINTER_set_dis_enable(void);
void IQSINTER_set_reserved(void);

//**  Set sensor
void IQSSEN_get_ver(void);
void IQSSEN_get_reg(void);
void IQSSEN_get_mode(void);
void IQSSEN_get_fps(void);
void IQSSEN_get_mirror_flip(void);
void IQSSEN_get_status(void);
void IQSSEN_get_resloution(void);
void IQSSEN_get_reserved(void);

void IQSSEN_set_addr(void);
void IQSSEN_set_reg(void);
void IQSSEN_set_mode(void);
void IQSSEN_set_fps(void);
void IQSSEN_set_mirror_flip(void);
void IQSSEN_set_reserved(void);

//**  Set lens
void IQSLENS_get_ver(void);
void IQSLENS_get_focal_length(void);
void IQSLENS_get_zoom_speed(void);
void IQSLENS_get_zoom_section(void);
void IQSLENS_get_zoom_pos(void);
void IQSLENS_get_focus_speed(void);
void IQSLENS_get_focus_range(void);
void IQSLENS_get_focus_pos(void);
void IQSLENS_get_aperture_pos(void);
void IQSLENS_get_cap_info(void);
void IQSLENS_get_state(void);
void IQSLENS_get_zoom_focus_item(void);
void IQSLENS_get_zoom_focus_tbl(void);
void IQSLENS_get_reserved(void);

void IQSLENS_set_argu(void);
void IQSLENS_set_focal_length(void);
void IQSLENS_set_zoom_init(void);
void IQSLENS_set_zoom_speed(void);
void IQSLENS_set_zoom_section(void);
void IQSLENS_set_zoom_pos(void);
void IQSLENS_set_zoom_define_pos(void);
void IQSLENS_set_zoom_move(void);
void IQSLENS_set_zoom_stop(void);
void IQSLENS_set_focus_init(void);
void IQSLENS_set_focus_speed(void);
void IQSLENS_set_focus_pos(void);
void IQSLENS_set_focus_define_pos(void);
void IQSLENS_set_focus_move(void);
void IQSLENS_set_focus_stop(void);
void IQSLENS_set_aperture_pos(void);
void IQSLENS_set_shutter_state(void);
void IQSLENS_set_ircut_state(void);
void IQSLENS_set_zoom_focus_item(void);
void IQSLENS_set_zoom_focus_tbl(void);
void IQSLENS_set_reserved(void);


//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCIQSimGet[])(void) = {
	//**  IQSIM
    #ifndef IQS_SIMT_ENABLE
	IQSim_GetReserved,      // 0
	IQSim_GetReserved,      // 1
	IQSim_GetReserved,      // 2
	IQSim_GetReserved,      // 3
	IQSim_GetReserved,      // 4
	IQSim_GetReserved,      // 5
	IQSim_GetReserved,      // 6
	IQSim_GetReserved,      // 7
	IQSim_GetReserved,      // 8
	IQSim_GetReserved,      // 9
	IQSim_GetReserved,      // 10
	IQSim_GetReserved,      // 11
    #else
	IQSim_GetBinAddr,       // 0 - get input bin address
	IQSim_GetRawCh0Addr,    // 1 - get input raw address
	IQSim_GetRawCh2Addr,    // 2 - get input raw address
	IQSim_GetJpgAddr,       // 3 - get output JPG address
	IQSim_GetJpgOk,         // 4 - get output JPG status
	IQSim_GetJpgSize,       // 5 - get output JPG size
	IQSim_GetQVAddr,        // 6 - get output Quickview address
	IQSim_GetQVSize,        // 7 - get output Quickview size
	IQSim_GetQVFmt,         // 8 - get output Quickview size
	IQSim_GetPRIAddr,       // 9 - get output Primary Image address
	IQSim_GetPRISize,       // 10- get output Primary Image size
	IQSim_GetPRIFmt,        // 11- get output Primary Image size
	#endif
 	IQSim_GetReserved,      // 12
	IQSim_GetReserved,      // 13
	IQSim_GetReserved,      // 14
	IQSim_GetReserved,      // 15

	//**  ISP Setting AE
	IQSAE_get_ver,          // 16
	IQSAE_get_status,       // 17
	IQSAE_get_expect_lum,   // 18
	IQSAE_get_histogram_adj,// 19
	IQSAE_get_over_exp,     // 20
	IQSAE_get_boundary,     // 21
	IQSAE_get_converge,     // 22
	IQSAE_get_manual,       // 23
	IQSAE_get_curvegen,     // 24
	IQSAE_get_laclamp,      // 25
	IQSAE_get_window,       // 26
	IQSAE_get_la,           // 27
	IQSAE_get_lum_gamma,    // 28
	IQSAE_get_shdr,         // 29
	IQSAE_get_uicmd,        // 30

	//**  ISP Setting AWB
	IQSAWB_get_ver,         // 31
	IQSAWB_get_status,      // 32
	IQSAWB_get_param,       // 33
	IQSAWB_get_manual,      // 34
	IQSAWB_get_ca,          // 35
	IQSAWB_get_flag,        // 36
	IQSAWB_get_converge,    // 37
	IQSAWB_get_dbg_msg,     // 38
	IQSAWB_get_reserved,    // 39
	IQSAWB_get_reserved,    // 40

	//**  ISP Setting AF
	IQSAF_get_ver,             // 41
	IQSAF_get_enable,          // 42
	IQSAF_get_method,          // 43
	IQSAF_get_shot_mode,       // 44
	IQSAF_get_search_direction,// 45
	IQSAF_get_thres,           // 46
	IQSAF_get_step_size,       // 47
	IQSAF_get_max_search_cnt,  // 48
	IQSAF_get_va,              // 49
	IQSAF_get_status,          // 50
	IQSAF_get_win_weight,      // 51
	IQSAF_get_dbg_msg,         // 52
	IQSAF_get_va_param,        // 53
	IQSAF_get_reserved,        // 54
	IQSAF_get_reserved,        // 55

	//**  ISP Setting IQ
	IQSISP_get_ver,         // 56
	IQSISP_get_ime_info,    // 57
	IQSISP_get_yuv_lock,    // 58
	IQSISP_get_raw_info,    // 59
	IQSISP_get_isp_info,    // 60
	IQSISP_get_model_type,  // 61
	IQSISP_get_frame_num,   // 62
	IQSISP_get_iq_status,   // 63
	IQSISP_get_iq_ui_info,  // 64
	IQSISP_get_dr_mode,     // 65

	IQSISP_get_iso_map,     // 66
	IQSISP_get_ob,          // 67
	IQSISP_get_ecs,         // 68
	IQSISP_get_wdr,         // 69
	IQSISP_get_outlier,     // 70
	IQSISP_get_spatial,     // 71
	IQSISP_get_range,       // 72
	IQSISP_get_vig,         // 73
	IQSISP_get_gbal,        // 74
	IQSISP_get_ldc,         // 75
	IQSISP_get_edge,        // 76
	IQSISP_get_rgblpf,      // 77
	IQSISP_get_cc,          // 78
	IQSISP_get_y_curve,     // 79
	IQSISP_get_gamma,       // 80
	IQSISP_get_3dcc,        // 81
	IQSISP_get_cctrl,       // 82
	IQSISP_get_cst,         // 83
	IQSISP_get_ife2filter,  // 84
	IQSISP_get_lca,         // 85
	IQSISP_get_chroma_supp, // 86
	IQSISP_get_rbfill,      // 87
	IQSISP_get_tmnr,        // 88
	IQSISP_get_tnr,         // 89
	IQSISP_get_tnr_cfg,     // 90
	IQSISP_get_ce,          // 91
	IQSISP_get_ctrl,        // 92
	IQSISP_get_defog,       // 93
	IQSISP_get_hdr_info,    // 94
	IQSISP_get_tmnr_debug,  // 95
	IQSISP_get_tmnr_auto,   // 96
	IQSISP_get_size_check,  // 97
	IQSISP_get_8iso_param,  // 98
	IQSISP_get_reserved,    // 99
	IQSISP_get_reserved,    // 100

	//**  ISP Setting Codec
	IQSCODEC_get_ver,       // 101
	IQSCODEC_get_info,      // 102
	IQSCODEC_get_aq_all,    // 103
	IQSCODEC_get_cbr_all,   // 104
	IQSCODEC_get_fix_qp_all,// 105
	IQSCODEC_get_vbr_all,   // 106
	IQSCODEC_get_aq,        // 107
	IQSCODEC_get_cbr,       // 108
	IQSCODEC_get_fix_qp,    // 109
	IQSCODEC_get_vbr,       // 110
	IQSCODEC_get_evbr_all,  // 111
	IQSCODEC_get_evbr,      // 112
	IQSCODEC_get_reserved,  // 113
	IQSCODEC_get_reserved,  // 114
	IQSCODEC_get_reserved,  // 115
	IQSCODEC_get_reserved,  // 116
	IQSCODEC_get_reserved,  // 117
	IQSCODEC_get_reserved,  // 118
	IQSCODEC_get_reserved,  // 119
	IQSCODEC_get_reserved,  // 120

	//**  ISP Setting Internal
	IQSINTER_get_direct_mode, // 121
	IQSINTER_get_color_range, // 122
	IQSINTER_get_reserved,    // 123
	IQSINTER_get_sie_crop_pos,// 124
	IQSISP_get_hdr_raw_info,  // 125
	IQSINTER_get_uitron_ready,// 126
	IQSINTER_get_dis_enable,  // 127
	IQSINTER_get_reserved,    // 128
	IQSINTER_get_reserved,    // 129
	IQSINTER_get_reserved,    // 130

	//**  ISP Setting sensor
	IQSSEN_get_ver,         // 131
	IQSSEN_get_reg,         // 132
	IQSSEN_get_mode,        // 133
	IQSSEN_get_fps,         // 134
	IQSSEN_get_mirror_flip, // 135
	IQSSEN_get_status,      // 136
	IQSSEN_get_resloution,  // 137
	IQSSEN_get_reserved,    // 138
	IQSSEN_get_reserved,    // 139
	IQSSEN_get_reserved,    // 140
	IQSSEN_get_reserved,    // 141
	IQSSEN_get_reserved,    // 142
	IQSSEN_get_reserved,    // 143
	IQSSEN_get_reserved,    // 144
	IQSSEN_get_reserved,    // 145

	//**  ISP Setting lens
	IQSLENS_get_ver,            // 146
	IQSLENS_get_focal_length,   // 147
	IQSLENS_get_reserved,       // 148
	IQSLENS_get_zoom_speed,     // 149
	IQSLENS_get_zoom_section,   // 150
	IQSLENS_get_zoom_pos,       // 151
	IQSLENS_get_reserved,       // 152
	IQSLENS_get_reserved,       // 153
	IQSLENS_get_reserved,       // 154
	IQSLENS_get_reserved,       // 155
	IQSLENS_get_focus_speed,    // 156
	IQSLENS_get_focus_range,    // 157
	IQSLENS_get_focus_pos,      // 158
	IQSLENS_get_reserved,       // 159
	IQSLENS_get_reserved,       // 160
	IQSLENS_get_reserved,       // 161
	IQSLENS_get_aperture_pos,   // 162
	IQSLENS_get_cap_info,       // 163
	IQSLENS_get_state,          // 164
	IQSLENS_get_zoom_focus_item,// 165
	IQSLENS_get_zoom_focus_tbl, // 166
	IQSLENS_get_reserved,       // 167
	IQSLENS_get_reserved,       // 168
	IQSLENS_get_reserved,       // 169
	IQSLENS_get_reserved,       // 170

	// ISP misc
	IQSAE_get_iris_cfg,         // 171
	IQSAE_get_curvegen_photo,   // 172
	IQSAE_get_la_va,            // 173
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCIQSimSet[])(void) = {
	//**  IQSIM
	#ifndef IQS_SIMT_ENABLE
	IQSim_SetReserved,      // 0
	IQSim_SetReserved,      // 1
	IQSim_SetReserved,      // 2
    #else
	IQSim_SetConvertInit,   // 0
	IQSim_SetConvertProc,   // 1
	IQSim_SetConvertEnd,    // 2
	#endif
	IQSim_SetReserved,      // 3
	IQSim_SetReserved,      // 4
	IQSim_SetReserved,      // 5
	IQSim_SetReserved,      // 6
	IQSim_SetReserved,      // 7
	IQSim_SetReserved,      // 8
	IQSim_SetReserved,      // 9
	IQSim_SetReserved,      // 10
	IQSim_SetReserved,      // 11
	IQSim_SetReserved,      // 12
	IQSim_SetReserved,      // 13
	IQSim_SetReserved,      // 14
	IQSim_SetReserved,      // 15

	//**  ISP Setting AE
	IQSAE_set_reserved,     // 16
	IQSAE_set_reserved,     // 17
	IQSAE_set_expect_lum,   // 18
	IQSAE_set_histogram_adj,// 19
	IQSAE_set_over_exp,     // 20
	IQSAE_set_boundary,     // 21
	IQSAE_set_converge,     // 22
	IQSAE_set_manual,       // 23
	IQSAE_set_curvegen,     // 24
	IQSAE_set_laclamp,      // 25
	IQSAE_set_window,       // 26
	IQSAE_set_reserved,     // 27 // for get la reserved
	IQSAE_set_lum_gamma,    // 28
	IQSAE_set_shdr,         // 29
	IQSAE_set_uicmd,        // 30

	//**  ISP Setting AWB
	IQSAWB_set_reserved,    // 31
	IQSAWB_set_reserved,    // 32
	IQSAWB_set_param,       // 33
	IQSAWB_set_manual,      // 34
	IQSAWB_set_reserved,    // 35
	IQSAWB_set_reserved,    // 36
	IQSAWB_set_converge,    // 37
	IQSAWB_set_dbg_msg,     // 38
	IQSAWB_set_reserved,    // 39
	IQSAWB_set_reserved,    // 40

	//**  ISP Setting AF
	IQSAF_set_reserved,         // 41
	IQSAF_set_enable,           // 42
	IQSAF_set_method,           // 43
	IQSAF_set_shot_mode,        // 44
	IQSAF_set_search_direction, // 45
	IQSAF_set_thres,            // 46
	IQSAF_set_step_size,        // 47
	IQSAF_set_max_search_cnt,   // 48
	IQSAF_set_retrigger,        // 49
	IQSAF_set_reserved,         // 50
	IQSAF_set_win_weight,       // 51
	IQSAF_set_dbg_msg,          // 52
	IQSAF_set_va_param,         // 53
	IQSAF_set_reserved,         // 54
	IQSAF_set_reserved,         // 55

	//**  ISP Setting IQ
	IQSISP_set_sensor_id,   // 56
	IQSISP_set_ime_path_id, // 57
	IQSISP_set_iso_idx,     // 58
	IQSISP_set_sie_pause,   // 59
	IQSISP_set_mem_map,     // 60
	IQSISP_set_yuv_unlock,  // 61
	IQSISP_set_sie_resume,  // 62
	IQSISP_set_info_reserved,// 63
	IQSISP_set_iq_ui_info,   // 64
	IQSISP_set_dr_mode,      // 65

	IQSISP_set_iso_map,     // 66
	IQSISP_set_ob,          // 67
	IQSISP_set_ecs,         // 68
	IQSISP_set_wdr,         // 69
	IQSISP_set_outlier,     // 70
	IQSISP_set_spatial,     // 71
	IQSISP_set_range,       // 72
	IQSISP_set_vig,         // 73
	IQSISP_set_gbal,        // 74
	IQSISP_set_ldc,         // 75
	IQSISP_set_edge,        // 76
	IQSISP_set_rgblpf,      // 77
	IQSISP_set_cc,          // 78
	IQSISP_set_y_curve,     // 79
	IQSISP_set_gamma,       // 80
	IQSISP_set_3dcc,        // 81
	IQSISP_set_cctrl,       // 82
	IQSISP_set_cst,         // 83
	IQSISP_set_ife2filter,  // 84
	IQSISP_set_lca,         // 85
	IQSISP_set_chroma_supp, // 86
	IQSISP_set_rbfill,      // 87
	IQSISP_set_tmnr,        // 88
	IQSISP_set_tnr,         // 89
	IQSISP_set_tnr_cfg,     // 90
	IQSISP_set_ce,          // 91
	IQSISP_set_ctrl,        // 92
	IQSISP_set_defog,       // 93
	IQSISP_set_hdr_info,    // 94
	IQSISP_set_tmnr_debug,  // 95
	IQSISP_set_tmnr_auto,   // 96
	IQSISP_set_reserved,    // 97
	IQSISP_set_8iso_param,  // 98
	IQSISP_set_reserved,    // 99
	IQSISP_set_reserved,    // 100

	//**  ISP Setting Codec
	IQSCODEC_set_reserved,  // 101
	IQSCODEC_set_ipcam_path,// 102
	IQSCODEC_set_aq_all,    // 103
	IQSCODEC_set_cbr_all,   // 104
	IQSCODEC_set_fix_qp_all,// 105
	IQSCODEC_set_vbr_all,   // 106
	IQSCODEC_set_aq,        // 107
	IQSCODEC_set_cbr,       // 108
	IQSCODEC_set_fix_qp,    // 109
	IQSCODEC_set_vbr,       // 110
	IQSCODEC_set_evbr_all,  // 111
	IQSCODEC_set_evbr,      // 112
	IQSCODEC_set_reserved,  // 113
	IQSCODEC_set_reserved,  // 114
	IQSCODEC_set_reserved,  // 115
	IQSCODEC_set_reserved,  // 116
	IQSCODEC_set_reserved,  // 117
	IQSCODEC_set_reserved,  // 118
	IQSCODEC_set_reserved,  // 119
	IQSCODEC_set_reserved,  // 120

	//**  ISP Setting Internal
	IQSINTER_set_reserved,    // 121
	IQSINTER_set_color_range, // 122
	IQSINTER_set_reserved,    // 123
	IQSINTER_set_sie_crop_pos,// 124
	IQSINTER_set_reserved,    // 125
	IQSINTER_set_reserved,    // 126
	IQSINTER_set_dis_enable,  // 127
	IQSINTER_set_reserved,    // 128
	IQSINTER_set_reserved,    // 129
	IQSINTER_set_reserved,    // 130

	//**  ISP Setting sensor
	IQSSEN_set_addr,        // 131
	IQSSEN_set_reg,         // 132
	IQSSEN_set_mode,        // 133
	IQSSEN_set_fps,         // 134
	IQSSEN_set_mirror_flip, // 135
	IQSSEN_set_reserved,    // 136
	IQSSEN_set_reserved,    // 137
	IQSSEN_set_reserved,    // 138
	IQSSEN_set_reserved,    // 139
	IQSSEN_set_reserved,    // 140
	IQSSEN_set_reserved,    // 141
	IQSSEN_set_reserved,    // 142
	IQSSEN_set_reserved,    // 143
	IQSSEN_set_reserved,    // 144
	IQSSEN_set_reserved,    // 145

	//**  ISP Setting lens
	IQSLENS_set_argu,            // 146
	IQSLENS_set_focal_length,    // 147
	IQSLENS_set_zoom_init,       // 148
	IQSLENS_set_zoom_speed,      // 149
	IQSLENS_set_zoom_section,    // 150
	IQSLENS_set_zoom_pos,        // 151
	IQSLENS_set_zoom_define_pos, // 152
	IQSLENS_set_zoom_move,       // 153
	IQSLENS_set_zoom_stop,       // 154
	IQSLENS_set_focus_init,      // 155
	IQSLENS_set_focus_speed,     // 156
	IQSLENS_set_reserved,        // 157
	IQSLENS_set_focus_pos,       // 158
	IQSLENS_set_focus_define_pos,// 159
	IQSLENS_set_focus_move,      // 160
	IQSLENS_set_focus_stop,      // 161
	IQSLENS_set_aperture_pos,    // 162
	IQSLENS_set_shutter_state,   // 163
	IQSLENS_set_ircut_state,     // 164
	IQSLENS_set_zoom_focus_item, // 165
	IQSLENS_set_zoom_focus_tbl,  // 166
	IQSLENS_set_reserved,        // 167
	IQSLENS_set_reserved,        // 168
	IQSLENS_set_reserved,        // 169
	IQSLENS_set_reserved,        // 170

	// ISP misc
	IQSAE_set_iris_cfg,          // 171
	IQSAE_set_curvegen_photo,    // 172
};

BOOL MsdcNvtRegSi_IQSim(void)
{
	return MsdcNvt_AddCallback_Si(MSDCIQSimGet, sizeof(MSDCIQSimGet) / sizeof(MSDCIQSimGet[0]), MSDCIQSimSet, sizeof(MSDCIQSimSet) / sizeof(MSDCIQSimSet[0]));
}

////////////////////////////////
//            IQSIM            /
////////////////////////////////
#ifdef IQS_SIMT_ENABLE
extern ER ImgCap_RawIsIdle(void);
extern ER ImgProc_IsIdle(void);
extern ER ImgEnc_IsIdle(void);
extern ER ImgProc2_IsIdle(void);

void IQSim_GetBinAddr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}
	pData[0] = (UINT32)IQSIM_GetAppInfo();
}

void IQSim_GetRawCh0Addr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}
	UINT32 ch0, ch2;
	IQSIM_GetRawAddr(&ch0, &ch2);
	pData[0] = ch0;
}

void IQSim_GetRawCh2Addr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}
	UINT32 ch0, ch2;
	IQSIM_GetRawAddr(&ch0, &ch2);
	pData[0] = ch2;
}

void IQSim_GetJpgAddr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	UINT32 Addr, Size;
	ImgCap_GetCurJpgInfo(&Addr, &Size);
	pData[0] = Addr;
	DBG_ERR("IQSim_GetJpgAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetJpgOk(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}
	if (ImgCap_RawIsIdle() == E_OK && ImgProc_IsIdle() == E_OK && ImgProc2_IsIdle() == E_OK && ImgEnc_IsIdle() == E_OK) {
		pData[0] = TRUE;
	} else {
		pData[0] = FALSE;
	}
	DBG_ERR("IQSim_GetJpgOk return %d\r\n", pData[0]);
}

void IQSim_GetJpgSize(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	UINT32 Addr, Size;
	ImgCap_GetCurJpgInfo(&Addr, &Size);
	pData[0] = Size;
	DBG_ERR("IQSim_GetJpgSize return %d\r\n", pData[0]);
}

void IQSim_GetQVAddr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	UINT32 Addr, Size, Fmt;
	ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
	pData[0] = Addr;
	DBG_ERR("IQSim_GetQVAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetQVSize(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	UINT32 Addr, Size, Fmt;
	ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
	pData[0] = Size;
	DBG_ERR("IQSim_GetQVSize return %d\r\n", pData[0]);
}

void IQSim_GetQVFmt(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	UINT32 Addr, Size, Fmt;
	ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
	pData[0] = Fmt;
	DBG_ERR("IQSim_GetQVFmt return %d\r\n", pData[0]);
}

void IQSim_GetPRIAddr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	UINT32 Addr, Size, Fmt;
	ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
	pData[0] = Addr;
	DBG_ERR("IQSim_GetPRIAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetPRISize(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	UINT32 Addr, Size, Fmt;
	ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
	pData[0] = Size;
	DBG_ERR("IQSim_GetPRISize return %d\r\n", pData[0]);
}

void IQSim_GetPRIFmt(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	UINT32 Addr, Size, Fmt;
	ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
	pData[0] = Fmt;
	DBG_ERR("IQSim_GetPRIFmt return %d\r\n", pData[0]);
}
#endif

void IQSim_GetReserved(void)
{
}

#ifdef IQS_SIMT_ENABLE
void IQSim_SetConvertInit(void)
{
	//***************************************
	//* Open Sensor in Playback only for IQSIM flow
	//* Add by Wendy Liao 2016/06/17
	//***************************************
	//SENSOR_INIT_OBJ SenObj;
	//SENSOR_DRV_TAB *pSenTab;
	SENSOR_HANDLE sen_handle = {0};

	drv_sensor_power_turn_on(IPL_PATH(0));
	drv_sensor_init(IPL_PATH(0), &sen_handle);
    if (sen_handle.drv_tab != NULL) {
	    sensor_open(IPL_PATH(0), &sen_handle);
    }
	//***************************************

	UINT32 uiPoolAddr;
	CAP_OBJ_INFO CapObj = {0};
	uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);

#if 0
	UINT32 BinAddr = uiPoolAddr;
	UINT32 Ch0Addr = BinAddr + IQSIM_MAX_BIN_SIZE;
	UINT32 Ch2Addr = Ch0Addr + IQSIM_MAX_CH0_SIZE;
	UINT32 uiBufferSize = IQSIM_MAX_BIN_SIZE + IQSIM_MAX_CH0_SIZE + IQSIM_MAX_CH2_SIZE;
#else
	UINT32 BinAddr = uiPoolAddr;
	UINT32 Ch2Addr = uiPoolAddr + POOL_SIZE_APP - IQSIM_MAX_CH2_SIZE;
	UINT32 Ch0Addr = Ch2Addr - IQSIM_MAX_CH0_SIZE;
	UINT32 uiBufferSize = IQSIM_MAX_BIN_SIZE;
#endif

	DBG_ERR("BinAddr = %X\r\n", BinAddr);
	DBG_ERR("Ch0Addr = %X\r\n", Ch0Addr);
	DBG_ERR("Ch2Addr = %X\r\n", Ch2Addr);
	DBG_ERR("uiBufferSize = %X\r\n", uiBufferSize);


	pll_setPLLEn(PLL_ID_5, ENABLE);

	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_ACTFLAG, TRUE);

	//enter
	IPL_Open();
	CapObj.uiBufAddr = uiPoolAddr + uiBufferSize;
	CapObj.uiBufSize = POOL_SIZE_APP - uiBufferSize;
	CapObj.S2DetFP = NULL;
	CapObj.CBMsgFP = NULL;
	CapObj.WriteFileFP = NULL;
	ImgCap_Open(&CapObj);

	IQSIM_SetAppInfo(BinAddr);
	IQSIM_SetRawAddr(Ch0Addr, Ch2Addr);
}

void IQSim_SetConvertProc(void)
{
	IQSIM_APPINFO *AppInfo;

	IQSIM_ReorgAppInfo();

	AppInfo = (IQSIM_APPINFO *)IQSIM_GetAppInfo();
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SIM);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_PICNUM, 1);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_RAW_BUF_NUM, 1);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_JPG_BUF_NUM, 1);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_JPGFMT, SEL_JPGFMT_422);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_ENCRATIO, 100);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_JPG_IMG_H_SIZE, AppInfo->IQSimJpgInfo.Size.w);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_JPG_IMG_V_SIZE, AppInfo->IQSimJpgInfo.Size.h);

	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_QV_IMG_H_SIZE, 640);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_QV_IMG_V_SIZE, 480);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_REENCTYPE, SEL_REENCTYPE_FIXQ);
	ImgCap_SetUIInfo(IPL_PATH(0), CAP_SEL_QUALITY, AppInfo->IQSimJpgInfo.BitRate);
	DBG_ERR("JPG BitRate = %d\r\n", AppInfo->IQSimJpgInfo.BitRate);

	ImgCap_StartCapture();
}

void IQSim_SetConvertEnd(void)
{
	DBG_ERR("IQSim_SetConvertEnd\r\n");

	ImgCap_Close();
	IPL_Close();
}
#endif

void IQSim_SetReserved(void)
{
}

///////////////////////////////////
//         ISP Setting AE         /
///////////////////////////////////
UINT32 sensor_id = 0;
UINT32 ime_path_id = 0;

void IQSAE_get_ver(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("size mismatch");
		return;
	}
	pData[0] = 0x00010007;//v.1.00.07
}

void IQSAE_get_status(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_STATUS, (UINT32)pData, uiLength);
}

void IQSAE_get_expect_lum(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_EXPECTLUM, (UINT32)pData, uiLength);
}

void IQSAE_get_histogram_adj(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_HISTOGRAMADJ, (UINT32)pData, uiLength);
}

void IQSAE_get_over_exp(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_OVEREXP, (UINT32)pData, uiLength);
}

void IQSAE_get_boundary(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_BOUNDARY, (UINT32)pData, uiLength);
}

void IQSAE_get_converge(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_CONVERGE, (UINT32)pData, uiLength);
}

void IQSAE_get_manual(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_MANUAL, (UINT32)pData, uiLength);
}

void IQSAE_get_curvegen(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_CURVEGEN, (UINT32)pData, uiLength);
}

void IQSAE_get_laclamp(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_LACLAMP, (UINT32)pData, uiLength);
}

void IQSAE_get_window(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_WINDOW, (UINT32)pData, uiLength);
}

void IQSAE_get_la(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_LADATA, (UINT32)pData, uiLength);
}

void IQSAE_get_lum_gamma(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_LUMGAMMA, (UINT32)pData, uiLength);
}

void IQSAE_get_shdr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_SHDR, (UINT32)pData, uiLength);
}

void IQSAE_get_uicmd(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_UICMD, (UINT32)pData, uiLength);
}

void IQSAE_get_iris_cfg(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_IRIS_CFG, (UINT32)pData, uiLength);
}

void IQSAE_get_curvegen_photo(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_CURVEGEN_PHOTO, (UINT32)pData, uiLength);
}

void IQSAE_get_la_va(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	//UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Get_Cmd(sensor_id, AE_GET_LADATA, (UINT32)pData, sizeof(AE_TUNING_LA));
	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_VA, ((UINT32)pData + sizeof(AE_TUNING_LA)), sizeof(AF_TUNING_VA));
}

void IQSAE_set_expect_lum(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_EXPECTLUM, (UINT32)pData, uiLength);
}

void IQSAE_set_histogram_adj(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_HISTOGRAMADJ, (UINT32)pData, uiLength);
}

void IQSAE_set_over_exp(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_OVEREXP, (UINT32)pData, uiLength);
}

void IQSAE_set_boundary(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_BOUNDARY, (UINT32)pData, uiLength);
}

void IQSAE_set_converge(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_CONVERGE, (UINT32)pData, uiLength);
}

void IQSAE_set_manual(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_MANUAL, (UINT32)pData, uiLength);
}

void IQSAE_set_curvegen(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_CURVEGEN, (UINT32)pData, uiLength);
}

void IQSAE_set_laclamp(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_LACLAMP, (UINT32)pData, uiLength);
}

void IQSAE_set_window(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_WINDOW, (UINT32)pData, uiLength);
}

void IQSAE_set_lum_gamma(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_LUMGAMMA, (UINT32)pData, uiLength);
}

void IQSAE_set_shdr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_SHDR, (UINT32)pData, uiLength);
}

void IQSAE_set_uicmd(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_UICMD, (UINT32)pData, uiLength);
}

void IQSAE_set_iris_cfg(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_IRIS_CFG, (UINT32)pData, uiLength);
}

void IQSAE_set_curvegen_photo(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AETuning_Set_Cmd(sensor_id, AE_SET_CURVEGEN_PHOTO, (UINT32)pData, uiLength);
}

void IQSAE_set_reserved(void)
{
}

///////////////////////////////////
//         ISP Setting AWB        /
///////////////////////////////////
void IQSAWB_get_ver(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("size mismatch");
		return;
	}
	pData[0] = 0x00010005;//v.1.00.05
}

void IQSAWB_get_status(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Get_Cmd(sensor_id, AWB_TUNING_GET_STATUS, (UINT32)pData, uiLength);
}

void IQSAWB_get_ca(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Get_Cmd(sensor_id, AWB_TUNING_GET_CA, (UINT32)pData, uiLength);
}

void IQSAWB_get_flag(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Get_Cmd(sensor_id, AWB_TUNING_GET_FLAG, (UINT32)pData, uiLength);
}

void IQSAWB_get_manual(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Get_Cmd(sensor_id, AWB_TUNING_GET_MANUAL, (UINT32)pData, uiLength);
}

void IQSAWB_get_param(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Get_Cmd(sensor_id, AWB_TUNING_GET_PARAM, (UINT32)pData, uiLength);
}

void IQSAWB_get_converge(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Get_Cmd(sensor_id, AWB_TUNING_GET_CONVERGE, (UINT32)pData, uiLength);
}

void IQSAWB_get_dbg_msg(void)
{
#if 0
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Get_Cmd(sensor_id, AWB_TUNING_GET_DBG_MSG, (UINT32)pData, uiLength);
#endif
}

void IQSAWB_get_reserved(void)
{
}

void IQSAWB_set_manual(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Set_Cmd(sensor_id, AWB_TUNING_SET_MANUAL, (UINT32)pData, uiLength);
}

void IQSAWB_set_param(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Set_Cmd(sensor_id, AWB_TUNING_SET_PARAM, (UINT32)pData, uiLength);
}

void IQSAWB_set_converge(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Set_Cmd(sensor_id, AWB_TUNING_SET_CONVERGE, (UINT32)pData, uiLength);
}

void IQSAWB_set_dbg_msg(void)
{
#if 0
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AWBTuning_Set_Cmd(sensor_id, AWB_TUNING_SET_DBG_MSG, (UINT32)pData, uiLength);
#endif
}

void IQSAWB_set_reserved(void)
{
}

///////////////////////////////////
//         ISP Setting AF         /
///////////////////////////////////
void IQSAF_get_ver(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("size mismatch");
		return;
	}
	pData[0] = 0x00010000;//v.1.00.00
}

void IQSAF_get_enable(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_ENABLE, (UINT32)pData, uiLength);
}

void IQSAF_get_method(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_METHOD, (UINT32)pData, uiLength);
}

void IQSAF_get_shot_mode(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_SHOT_MODE, (UINT32)pData, uiLength);
}

void IQSAF_get_search_direction(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_SEARCH_DIR, (UINT32)pData, uiLength);
}

void IQSAF_get_thres(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_THRES, (UINT32)pData, uiLength);
}

void IQSAF_get_step_size(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_STEP_SIZE, (UINT32)pData, uiLength);
}

void IQSAF_get_max_search_cnt(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_MAX_SEARCH_CNT, (UINT32)pData, uiLength);
}

void IQSAF_get_va(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_VA, (UINT32)pData, uiLength);
}

void IQSAF_get_status(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_STATUS, (UINT32)pData, uiLength);
}

void IQSAF_get_win_weight(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_WIN_WEIGHT, (UINT32)pData, uiLength);
}

void IQSAF_get_dbg_msg(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_DBG_MSG, (UINT32)pData, uiLength);
}

void IQSAF_get_va_param(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	AFTuning_Get_Cmd(sensor_id, AF_TUNING_GET_VA_PARAM, (UINT32)pData, uiLength);
}

void IQSAF_get_reserved(void)
{
}

void IQSAF_set_enable(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_ENABLE, (UINT32)pData, uiLength);
}

void IQSAF_set_method(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_METHOD, (UINT32)pData, uiLength);
}

void IQSAF_set_shot_mode(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_SHOT_MODE, (UINT32)pData, uiLength);
}

void IQSAF_set_search_direction(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_SEARCH_DIR, (UINT32)pData, uiLength);
}

void IQSAF_set_thres(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_THRES, (UINT32)pData, uiLength);
}

void IQSAF_set_step_size(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_STEP_SIZE, (UINT32)pData, uiLength);
}

void IQSAF_set_max_search_cnt(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_MAX_SEARCH_CNT, (UINT32)pData, uiLength);
}

void IQSAF_set_retrigger(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_RETRIGGER, (UINT32)pData, uiLength);
}

void IQSAF_set_win_weight(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_WIN_WEIGHT, (UINT32)pData, uiLength);
}

void IQSAF_set_dbg_msg(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_DBG_MSG, (UINT32)pData, uiLength);
}

void IQSAF_set_va_param(void)
{
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AFTuning_Set_Cmd(sensor_id, AF_TUNING_SET_VA_PARAM, (UINT32)pData, uiLength);
}

void IQSAF_set_reserved(void)
{
}


///////////////////////////////////
//         ISP Setting IQ         /
///////////////////////////////////
UINT16 fake_iso_idx = 0xffff;
ISF_DATA    isf_data;
ISF_PORT    *pSrc;

void IQSISP_get_ver(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("size mismatch");
		return;
	}
	pData[0] = 0x00010102;//v.1.01.02
}

void IQSISP_get_ime_info(void)
{
	//IPCAM_STREAM_INFO *pStreamInfo = (IPCAM_STREAM_INFO *)UIAppIPCam_get_streamInfo();

	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_IME_INFO *sPtr = (sISP_IME_INFO *)pData;
	//#NT#2017/09/21#Photon Lin -begin
	//#NT#Check IME path1 if compress
	//UIAPPIPCAM_SENSOR_INFO *pSensorInfo = (UIAPPIPCAM_SENSOR_INFO *)UIAppIPCam_get_SensorInfo();

	//#NT#2017/09/21#Photon Lin -end

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

	sPtr->Id = sensor_id;
	sPtr->size = sizeof(*sPtr);
	sPtr->ime_info.id = sensor_id;
	sPtr->ime_info.pnext = NULL;

	if (IPL_GetCmd(IPL_GET_IME_INFOR, (void *)&(sPtr->ime_info)) != E_OK) {
		DBG_ERR("ipl get ime command fail\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
    UINT32 display_codec_path = UIAppIPCam_get_DisplayPath();
    INT32 display_ime_path = -1;
	IPCAM_STREAM_INFO *pStreamInfo = (IPCAM_STREAM_INFO *)UIAppIPCam_get_streamInfo();
    if((pStreamInfo[display_codec_path].enable == ENABLE)&&(pStreamInfo[display_codec_path].sensor == sensor_id)) {
        display_ime_path = pStreamInfo[display_codec_path].ime;
    }
    ISF_PORT_STATE DispStatus = 0;
    DispStatus = UIAppIPCam_Get_Unit_Status(UIAPPIPCAM_VDOOUT_UNIT);
    if(DispStatus != ISF_PORT_STATE_RUN){
        if (display_ime_path == 0)      sPtr->ime_info.p1_enable = 0;
        else if(display_ime_path == 1)    sPtr->ime_info.p2_enable = 0;
        else if(display_ime_path == 2)    sPtr->ime_info.p3_enable = 0;
        else if(display_ime_path == 3)    sPtr->ime_info.p4_enable = 0;
        else if(display_ime_path == 5)    sPtr->ime_info.p5_enable = 0;
        DBG_DUMP("display path is disabled\r\n");
    }
#endif

#if(IPCAM_FUNC!=ENABLE)
	//DBG_DUMP("Get IME Info at DVCAM interface!!");
	sPtr->ime_info.p1_enable = ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_1);
	sPtr->ime_info.p2_enable = 0;
	sPtr->ime_info.p3_enable = 0;
	sPtr->ime_info.p4_enable = 0;
	sPtr->ime_info.p5_enable = 0;
#endif
}

void IQSISP_get_yuv_lock(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODED_YUV *yuv = (sISP_CODED_YUV *)pData;

	if (uiLength != sizeof(*yuv)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

	UINT32 isf_out = ISF_OUT1;
	ISF_UNIT    *pUnit;

#if(IPCAM_FUNC==ENABLE)
	//DBG_DUMP("Get YUV at IPCAM interface!!");
	IPCAM_STREAM_INFO *pStreamInfo = (IPCAM_STREAM_INFO *)UIAppIPCam_get_streamInfo();
	UINT32 path_index = 0;
	for (path_index = UIAPPIPCAM_PATH_ID_0; path_index < UIAPPIPCAM_PATH_MAX_ID; path_index++) {
        if((pStreamInfo[path_index].enable == ENABLE)&&(pStreamInfo[path_index].sensor == sensor_id)&&(pStreamInfo[path_index].ime == ime_path_id)) {
            isf_out = pStreamInfo[path_index].path;
            break;
        }
        if(path_index + 1 == UIAPPIPCAM_PATH_MAX_ID){
            DBG_ERR("IPCAM path match error!!");
        }
	}
#else
	//DBG_DUMP("Get YUV at DVCAM interface!!");
#endif

	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(isf_out, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, isf_out);
#if (NEW_CAPTURE == ENABLE)
	// pause sie
	sISP_SIE_PAUSE SIE_Pause;

	SIE_Pause.Id = sensor_id;
	SIE_Pause.size = sizeof(sISP_SIE_PAUSE);
	SIE_Pause.sie_pause.id = sensor_id;
	SIE_Pause.sie_pause.pnext = NULL;

	if (IPL_SetCmd(IPL_SET_PAUSEDMA, (void *)&(SIE_Pause.sie_pause)) != E_OK) {
		DBG_ERR("ipl set sie pause command fail\r\n");
		return;
	}
	IPL_WaitCmdFinish();
#endif
	// pull data
	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		yuv->ipcam_path_id = isf_out;
		yuv->size = sizeof(*yuv);
		yuv->yuv_buf = *((IMG_BUF*)&isf_data.Desc[0]);

		//release UserProc's PULL queue
		ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(isf_out, USERPROC_PARAM_ALLOW_PULL_IMM, FALSE);
		ImageUnit_End();
	} else {
		DBG_ERR("get ime rdy info fail\r\n");
	}
}

void IQSISP_get_raw_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_RAW_Info *sPtr = (sISP_RAW_Info *)pData;

	if (IPL_CtrlGetInfor(0, IPLCTRL_PRVFLOW) == ICF_FLOW_DIRECT) {
		DBG_WRN("DIRECT mode. \r\n");
		return;
	}

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

    #if 1
	IPL_PROC_ID ipl_id = IPL_ID_1;

	if(IPL_AlgGetUIInfo(IPL_ID_1, IPL_SEL_SHDR) == SEL_SHDR_ON) {
		ipl_id = IPL_ID_2;
	} else {
		ipl_id = IPL_ID_1;
	}

	sPtr->Id = ipl_id;
	sPtr->size = sizeof(*sPtr);

	if(IPL_HeaderRawGetRdyInfo(ipl_id, &(sPtr->raw_info)) != E_OK) {
		DBG_ERR("get raw info fail\r\n");
	}
    #else
	sPtr->Id = sensor_id;
	sPtr->size = sizeof(*sPtr);

	if(IPL_HeaderRawGetRdyInfo(sensor_id, &(sPtr->raw_info)) != E_OK) {
		DBG_ERR("get raw info fail\r\n");
	}
    #endif
}

void IQSISP_get_hdr_raw_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	UINT32 ipl_mf_bit = IPL_CtrlGetInfor(sensor_id, IPLCTRL_MF_TOTAL_FRM_BIT);
	UINT32 i;

	sISP_HDR_RAW_Info *sPtr = (sISP_HDR_RAW_Info *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

	sPtr->Id = sensor_id;
	sPtr->size = sizeof(*sPtr);

	sPtr->mf_bit = ipl_mf_bit;
	for (i = 0; i < IPL_CtrlGetInfor(sensor_id, IPLCTRL_MF_TOTAL_FRM_NUM); i++){
		if (((ipl_mf_bit >> i) & 0x01) != 0x0) {
			if(IPL_HeaderRawGetRdyInfo(i, &(sPtr->raw_info[i])) != E_OK) {
				DBG_ERR("get raw info fail\r\n");
			}
		}
	}
}

void IQSISP_get_isp_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_isp_info *sPtr = (sISP_isp_info *)pData;

	if(uiLength != sizeof(*sPtr)) {
		DBG_WRN("size mismatch \r\n");
		return;
	}

	sPtr->id = sensor_id;
	iqs_get_isp_info(sPtr);
}

void IQSISP_get_iq_ui_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_IQ_UI_INFO *sPtr = (sISP_IQ_UI_INFO *)pData;

	if(uiLength != sizeof(*sPtr)) {
		DBG_WRN("size mismatch \r\n");
		return;
	}

	sPtr->id = sensor_id;
	sPtr->ui_info_element = IQ_UI_IRCUT;
	sPtr->value = iq_get_ui_info(sPtr->id, sPtr->ui_info_element);
	//iqs_get_isp_info(sPtr);
}

void IQSISP_get_model_type(void)
{
	CHAR *pData = (CHAR *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 16) {
		DBG_ERR("size mismatch");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	CHAR model_name[16] = "IPCAM";
	memcpy(pData, model_name, sizeof(CHAR)*16);
#else
	CHAR model_name[16] = "DVCAM";
	memcpy(pData, model_name, sizeof(CHAR)*16);
#endif
}

void IQSISP_get_frame_num(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("get frame number fail\r\n");
		return;
	}

	iqt_get_param(ISP_FRAME_NUM, (UINT32)pData, uiLength);
}

void IQSISP_get_iq_status(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_IQ_STATUS *sPtr = (sISP_IQ_STATUS *)pData;

	if(uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch \r\n");
		return;
	}

	sPtr->id = sensor_id;
	iq_get_status(sPtr);
}

void IQSISP_get_dr_mode(void)
{
    #if(IPCAM_FUNC==ENABLE)
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	//UINT32 uiLength = MsdcNvt_GetTransSize();
    UIAPPIPCAM_IPL_FUNC *pIPLFunc = UIAppIPCam_get_IPLFuncInfo();
    pData[0] = 0;

    if (pIPLFunc[sensor_id].sDefog.eDeFogMode == SEL_DEFOG_ON) {
        pData[0] |= IQ_FEATURE_DEFOG;
    }

    if (pIPLFunc[sensor_id].sHDR.eHDREnable == SEL_SHDR_ON) {
        pData[0] |= IQ_FEATURE_HDR;
    }

    if (pIPLFunc[sensor_id].sWDR.uiWDRMode == SEL_WDR_AUTO) {
        pData[0] |= IQ_FEATURE_WDR;
    }
    #endif
}

void IQSISP_get_info_reserved(void)
{
}

void IQSISP_get_iso_map(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_ISOMAP *sPtr = (sISP_ISOMAP *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_ISOMAP, (UINT32)pData, uiLength);
}

void IQSISP_get_ob(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_OB *sPtr = (sISP_OB *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_OB, (UINT32)pData, uiLength);
}

void IQSISP_get_ecs(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_ECS *sPtr = (sISP_ECS *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_ECS, (UINT32)pData, uiLength);
}

void IQSISP_get_wdr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_WDR *sPtr = (sISP_WDR *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_WDR, (UINT32)pData, uiLength);
	sPtr->WDR_mode = IPL_AlgGetUIInfo(sPtr->Id, IPL_SEL_WDR_IMM);
	sPtr->WDR_manual_level = IPL_AlgGetUIInfo(sPtr->Id, IPL_SEL_WDR_M_LEVEL_IMM);
}

void IQSISP_get_defog(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_DEFOG_CONFIG *sPtr = (sISP_DEFOG_CONFIG *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_DEFOG_CONFIG, (UINT32)pData, uiLength);
	sPtr->defog_mode = IPL_AlgGetUIInfo(sPtr->Id, IPL_SEL_DEFOG_IMM);
	sPtr->defog_level = IPL_AlgGetUIInfo(sPtr->Id, IPL_SEL_DEFOG_LEVEL_IMM);
}

void IQSISP_get_outlier(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_OUTLIER *sPtr = (sISP_OUTLIER *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_OUTLIER, (UINT32)pData, uiLength);
}

void IQSISP_get_spatial(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_SPATIAL *sPtr = (sISP_SPATIAL *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_SPATIAL, (UINT32)pData, uiLength);
}

void IQSISP_get_range(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_RANGE *sPtr = (sISP_RANGE *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RANGE, (UINT32)pData, uiLength);
}

void IQSISP_get_rbfill(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_RBFILL *sPtr = (sISP_RBFILL *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RBFILL, (UINT32)pData, uiLength);
}

void IQSISP_get_vig(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_VIG *sPtr = (sISP_VIG *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_VIG, (UINT32)pData, uiLength);
}

void IQSISP_get_gbal(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_GBAL *sPtr = (sISP_GBAL *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_GBAL, (UINT32)pData, uiLength);
}

void IQSISP_get_ldc(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_LDC *sPtr = (sISP_LDC *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_LDC, (UINT32)pData, uiLength);
}

void IQSISP_get_edge(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_EDGE *sPtr = (sISP_EDGE *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_EDGE, (UINT32)pData, uiLength);
}

void IQSISP_get_rgblpf(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_RGBLPF *sPtr = (sISP_RGBLPF *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RGBLPF, (UINT32)pData, uiLength);
}

void IQSISP_get_cc(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_CC *sPtr = (sISP_CC *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_CC, (UINT32)pData, uiLength);
}

void IQSISP_get_y_curve(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_Y_CURVE *sPtr = (sISP_Y_CURVE *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_Y_CURVE, (UINT32)pData, uiLength);
}

void IQSISP_get_gamma(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_GAMMA *sPtr = (sISP_GAMMA *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_GAMMA, (UINT32)pData, uiLength);
}

void IQSISP_get_3dcc(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_3DCC *sPtr = (sISP_3DCC *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_3DCC, (UINT32)pData, uiLength);
}

void IQSISP_get_cctrl(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_CCTRL *sPtr = (sISP_CCTRL *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CCTRL, (UINT32)pData, uiLength);
}

void IQSISP_get_cst(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_CST *sPtr = (sISP_CST *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_CST, (UINT32)pData, uiLength);
}

void IQSISP_get_ife2filter(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_IFE2FILTER *sPtr = (sISP_IFE2FILTER *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_IFE2FILTER, (UINT32)pData, uiLength);
}

void IQSISP_get_lca(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_LCA *sPtr = (sISP_LCA *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_LCA, (UINT32)pData, uiLength);
}

void IQSISP_get_chroma_supp(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_CHROMA_SUPP *sPtr = (sISP_CHROMA_SUPP *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CHROMA_SUPP, (UINT32)pData, uiLength);
}

void IQSISP_get_tmnr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_TMNR *sPtr = (sISP_TMNR *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_TMNR, (UINT32)pData, uiLength);
}

void IQSISP_get_tmnr_debug(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_TMNR_DEBUG *sPtr = (sISP_TMNR_DEBUG *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_TMNR_DEBUG, (UINT32)pData, uiLength);
}

void IQSISP_get_tmnr_auto(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_TMNR_AUTO *sPtr = (sISP_TMNR_AUTO *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_TMNR_AUTO, (UINT32)pData, uiLength);
}

void IQSISP_get_tnr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_TNR *sPtr = (sISP_TNR *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_TNR, (UINT32)pData, uiLength);
}

void IQSISP_get_tnr_cfg(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_TNR_CONFIG *sPtr = (sISP_TNR_CONFIG *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_TNR_CONFIG, (UINT32)pData, uiLength);
}

void IQSISP_get_ce(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_CE *sPtr = (sISP_CE *)pData;
	sPtr->Id = sensor_id;
	sPtr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CE, (UINT32)pData, uiLength);
}

void IQSISP_get_ctrl(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_ENGINE_CTRL *sPtr = (sISP_ENGINE_CTRL *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_ENGINE_CTRL, (UINT32)pData, uiLength);
}

void IQSISP_get_hdr_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_HDR_CONFIG *sPtr = (sISP_HDR_CONFIG *)pData;
	sPtr->Id = sensor_id;

	iqt_get_param(ISP_HDR_CONFIG, (UINT32)pData, uiLength);

	sPtr->test_param = SensorHDR_GetTestMode();
	sPtr->hdr_mode = IPL_AlgGetUIInfo(sPtr->Id, IPL_SEL_SHDR);
	sPtr->hdr_level = IPL_AlgGetUIInfo(sPtr->Id, IPL_SEL_SHDR_LEVEL_IMM);
}

void IQSISP_get_size_check(void)
{
	UINT8 *des_addr;
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 data_size = 0;

	// 1
	sISP_ENGINE_CTRL *sPtr_ec = (sISP_ENGINE_CTRL *)pData;
	sPtr_ec->Id = sensor_id;

	iqt_get_param(ISP_ENGINE_CTRL, (UINT32)pData, sizeof(sISP_ENGINE_CTRL));

	des_addr = (UINT8 *)pData + 16 * 1024;    // 16KB ~
	data_size += sizeof(sISP_ENGINE_CTRL);

	memcpy(des_addr, pData, sizeof(sISP_ENGINE_CTRL));

	// 2
	sISP_OB *sPtr_ob = (sISP_OB *)pData;
	sPtr_ob->Id = sensor_id;
	sPtr_ob->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_OB, (UINT32)pData, sizeof(sISP_OB));

	des_addr += sizeof(sISP_ENGINE_CTRL);
	data_size += sizeof(sISP_OB);

	memcpy(des_addr, pData, sizeof(sISP_OB));

	// 3
	sISP_WDR *sPtr_wdr = (sISP_WDR *)pData;
	sPtr_wdr->Id = sensor_id;

	iqt_get_param(ISP_WDR, (UINT32)pData, sizeof(sISP_WDR));
	sPtr_wdr->WDR_mode = IPL_AlgGetUIInfo(sPtr_wdr->Id, IPL_SEL_WDR_IMM);
	sPtr_wdr->WDR_manual_level = IPL_AlgGetUIInfo(sPtr_wdr->Id, IPL_SEL_WDR_M_LEVEL_IMM);

	des_addr += sizeof(sISP_OB);
	data_size += sizeof(sISP_WDR);

	memcpy(des_addr, pData, sizeof(sISP_WDR));

	// 4
	sISP_DEFOG_CONFIG *sPtr_defog = (sISP_DEFOG_CONFIG *)pData;
	sPtr_defog->Id = sensor_id;

	iqt_get_param(ISP_DEFOG_CONFIG, (UINT32)pData, sizeof(sISP_DEFOG_CONFIG));
	sPtr_defog->defog_mode = IPL_AlgGetUIInfo(sPtr_defog->Id, IPL_SEL_DEFOG_IMM);
	sPtr_defog->defog_level = IPL_AlgGetUIInfo(sPtr_defog->Id, IPL_SEL_DEFOG_LEVEL_IMM);

	des_addr += sizeof(sISP_WDR);
	data_size += sizeof(sISP_DEFOG_CONFIG);

	memcpy(des_addr, pData, sizeof(sISP_DEFOG_CONFIG));

	// 5
	sISP_HDR_CONFIG *sPtr_hdr = (sISP_HDR_CONFIG *)pData;
	sPtr_hdr->Id = sensor_id;

	iqt_get_param(ISP_HDR_CONFIG, (UINT32)pData, sizeof(sISP_HDR_CONFIG));
	sPtr_hdr->test_param = SensorHDR_GetTestMode();
	sPtr_hdr->hdr_mode = IPL_AlgGetUIInfo(sPtr_hdr->Id, IPL_SEL_SHDR);
	sPtr_hdr->hdr_level = IPL_AlgGetUIInfo(sPtr_hdr->Id, IPL_SEL_SHDR_LEVEL_IMM);

	des_addr += sizeof(sISP_DEFOG_CONFIG);
	data_size += sizeof(sISP_HDR_CONFIG);

	memcpy(des_addr, pData, sizeof(sISP_HDR_CONFIG));

	// 6
	sISP_CE *sPtr_ce = (sISP_CE *)pData;
	sPtr_ce->Id = sensor_id;
	sPtr_ce->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CE, (UINT32)pData, sizeof(sISP_CE));

	des_addr += sizeof(sISP_HDR_CONFIG);
	data_size += sizeof(sISP_CE);

	memcpy(des_addr, pData, sizeof(sISP_CE));

	// 7
	sISP_OUTLIER *sPtr_outl = (sISP_OUTLIER *)pData;
	sPtr_outl->Id = sensor_id;
	sPtr_outl->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_OUTLIER, (UINT32)pData, sizeof(sISP_OUTLIER));

	des_addr += sizeof(sISP_CE);
	data_size += sizeof(sISP_OUTLIER);

	memcpy(des_addr, pData, sizeof(sISP_OUTLIER));

	// 8
	sISP_SPATIAL *sPtr_spa = (sISP_SPATIAL *)pData;
	sPtr_spa->Id = sensor_id;
	sPtr_spa->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_SPATIAL, (UINT32)pData, sizeof(sISP_SPATIAL));

	des_addr += sizeof(sISP_OUTLIER);
	data_size += sizeof(sISP_SPATIAL);

	memcpy(des_addr, pData, sizeof(sISP_SPATIAL));

	// 9
	sISP_RANGE *sPtr_range = (sISP_RANGE *)pData;
	sPtr_range->Id = sensor_id;
	sPtr_range->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RANGE, (UINT32)pData, sizeof(sISP_RANGE));

	des_addr += sizeof(sISP_SPATIAL);
	data_size += sizeof(sISP_RANGE);

	memcpy(des_addr, pData, sizeof(sISP_RANGE));

	// 10
	sISP_VIG *sPtr_vig = (sISP_VIG *)pData;
	sPtr_vig->Id = sensor_id;

	iqt_get_param(ISP_VIG, (UINT32)pData, sizeof(sISP_VIG));

	des_addr += sizeof(sISP_RANGE);
	data_size += sizeof(sISP_VIG);

	memcpy(des_addr, pData, sizeof(sISP_VIG));

	// 11
	sISP_GBAL *sPtr_gbal = (sISP_GBAL *)pData;
	sPtr_gbal->Id = sensor_id;
	sPtr_gbal->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_GBAL, (UINT32)pData, sizeof(sISP_GBAL));

	des_addr += sizeof(sISP_VIG);
	data_size += sizeof(sISP_GBAL);

	memcpy(des_addr, pData, sizeof(sISP_GBAL));

	// 12
	sISP_RBFILL *sPtr_rbfill = (sISP_RBFILL *)pData;
	sPtr_rbfill->Id = sensor_id;
	sPtr_rbfill->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RBFILL, (UINT32)pData, sizeof(sISP_RBFILL));

	des_addr += sizeof(sISP_GBAL);
	data_size += sizeof(sISP_RBFILL);

	memcpy(des_addr, pData, sizeof(sISP_RBFILL));

	// 13
	sISP_LDC *sPtr_ldc = (sISP_LDC *)pData;
	sPtr_ldc->Id = sensor_id;

	iqt_get_param(ISP_LDC, (UINT32)pData, sizeof(sISP_LDC));

	des_addr += sizeof(sISP_RBFILL);
	data_size += sizeof(sISP_LDC);

	memcpy(des_addr, pData, sizeof(sISP_LDC));

	// 14
	sISP_EDGE *sPtr_edge = (sISP_EDGE *)pData;
	sPtr_edge->Id = sensor_id;
	sPtr_edge->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_EDGE, (UINT32)pData, sizeof(sISP_EDGE));

	des_addr += sizeof(sISP_LDC);
	data_size += sizeof(sISP_EDGE);

	memcpy(des_addr, pData, sizeof(sISP_EDGE));

	// 15
	sISP_RGBLPF *sPtr_lpf = (sISP_RGBLPF *)pData;
	sPtr_lpf->Id = sensor_id;
	sPtr_lpf->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RGBLPF, (UINT32)pData, sizeof(sISP_RGBLPF));

	des_addr += sizeof(sISP_EDGE);
	data_size += sizeof(sISP_RGBLPF);

	memcpy(des_addr, pData, sizeof(sISP_RGBLPF));

	// 16
	sISP_CC *sPtr_cc = (sISP_CC *)pData;
	sPtr_cc->Id = sensor_id;

	iqt_get_param(ISP_CC, (UINT32)pData, sizeof(sISP_CC));

	des_addr += sizeof(sISP_RGBLPF);
	data_size += sizeof(sISP_CC);

	memcpy(des_addr, pData, sizeof(sISP_CC));

	// 17
	sISP_Y_CURVE *sPtr_ycrv = (sISP_Y_CURVE *)pData;
	sPtr_ycrv->Id = sensor_id;

	iqt_get_param(ISP_Y_CURVE, (UINT32)pData, sizeof(sISP_Y_CURVE));

	des_addr += sizeof(sISP_CC);
	data_size += sizeof(sISP_Y_CURVE);

	memcpy(des_addr, pData, sizeof(sISP_Y_CURVE));

	// 18
	sISP_GAMMA *sPtr_gm = (sISP_GAMMA *)pData;
	sPtr_gm->Id = sensor_id;

	iqt_get_param(ISP_GAMMA, (UINT32)pData, sizeof(sISP_GAMMA));

	des_addr += sizeof(sISP_Y_CURVE);
	data_size += sizeof(sISP_GAMMA);

	memcpy(des_addr, pData, sizeof(sISP_GAMMA));

	// 19
	sISP_CCTRL *sPtr_cctrl = (sISP_CCTRL *)pData;
	sPtr_cctrl->Id = sensor_id;
	sPtr_cctrl->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CCTRL, (UINT32)pData, sizeof(sISP_CCTRL));

	des_addr += sizeof(sISP_GAMMA);
	data_size += sizeof(sISP_CCTRL);

	memcpy(des_addr, pData, sizeof(sISP_CCTRL));

	// 20
	sISP_CST *sPtr_cst = (sISP_CST *)pData;
	sPtr_cst->Id = sensor_id;

	iqt_get_param(ISP_CST, (UINT32)pData, sizeof(sISP_CST));

	des_addr += sizeof(sISP_CCTRL);
	data_size += sizeof(sISP_CST);

	memcpy(des_addr, pData, sizeof(sISP_CST));

	// 21
	sISP_IFE2FILTER *sPtr_ife2 = (sISP_IFE2FILTER *)pData;
	sPtr_ife2->Id = sensor_id;
	sPtr_ife2->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_IFE2FILTER, (UINT32)pData, sizeof(sISP_IFE2FILTER));

	des_addr += sizeof(sISP_CST);
	data_size += sizeof(sISP_IFE2FILTER);

	memcpy(des_addr, pData, sizeof(sISP_IFE2FILTER));

	// 22
	sISP_LCA *sPtr_lca = (sISP_LCA *)pData;
	sPtr_lca->Id = sensor_id;
	sPtr_lca->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_LCA, (UINT32)pData, sizeof(sISP_LCA));

	des_addr += sizeof(sISP_IFE2FILTER);
	data_size += sizeof(sISP_LCA);

	memcpy(des_addr, pData, sizeof(sISP_LCA));

	// 23
	sISP_CHROMA_SUPP *sPtr_csupp = (sISP_CHROMA_SUPP *)pData;
	sPtr_csupp->Id = sensor_id;
	sPtr_csupp->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CHROMA_SUPP, (UINT32)pData, sizeof(sISP_CHROMA_SUPP));

	des_addr += sizeof(sISP_LCA);
	data_size += sizeof(sISP_CHROMA_SUPP);

	memcpy(des_addr, pData, sizeof(sISP_CHROMA_SUPP));

	// 24
	sISP_TMNR *sPtr_tmnr = (sISP_TMNR *)pData;
	sPtr_tmnr->Id = sensor_id;
	sPtr_tmnr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_TMNR, (UINT32)pData, sizeof(sISP_TMNR));

	des_addr += sizeof(sISP_CHROMA_SUPP);
	data_size += sizeof(sISP_TMNR);

	memcpy(des_addr, pData, sizeof(sISP_TMNR));

	// 25
	sISP_TMNR_AUTO *sPtr_tmnr_auto = (sISP_TMNR_AUTO *)pData;
	sPtr_tmnr_auto->Id = sensor_id;

	iqt_get_param(ISP_TMNR_AUTO, (UINT32)pData, sizeof(sISP_TMNR_AUTO));

	des_addr += sizeof(sISP_TMNR);
	data_size += sizeof(sISP_TMNR_AUTO);

	memcpy(des_addr, pData, sizeof(sISP_TMNR_AUTO));

	// 26
	sISP_TNR_CONFIG *sPtr_tnr = (sISP_TNR_CONFIG *)pData;
	sPtr_tnr->Id = sensor_id;
	sPtr_tnr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_TNR_CONFIG, (UINT32)pData, sizeof(sISP_TNR_CONFIG));

	des_addr += sizeof(sISP_TMNR_AUTO);
	data_size += sizeof(sISP_TNR_CONFIG);

	memcpy(des_addr, pData, sizeof(sISP_TNR_CONFIG));

	// copy all data
	memcpy(pData, ((UINT8 *)pData + 16 * 1024), data_size);
}

void IQSISP_get_8iso_param(void)
{
	UINT8 *des_addr;
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 data_size = 0;

	// 1
	sISP_OB *sPtr_ob = (sISP_OB *)pData;
	sPtr_ob->Id = sensor_id;
	sPtr_ob->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_OB, (UINT32)pData, sizeof(sISP_OB));

	des_addr = (UINT8 *)pData + 16 * 1024;    // 16KB ~
	data_size += sizeof(sISP_OB);

	memcpy(des_addr, pData, sizeof(sISP_OB));

	// 2
	sISP_CE *sPtr_ce = (sISP_CE *)pData;
	sPtr_ce->Id = sensor_id;
	sPtr_ce->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CE, (UINT32)pData, sizeof(sISP_CE));

	des_addr += sizeof(sISP_OB);
	data_size += sizeof(sISP_CE);

	memcpy(des_addr, pData, sizeof(sISP_CE));

	// 3
	sISP_OUTLIER *sPtr_outl = (sISP_OUTLIER *)pData;
	sPtr_outl->Id = sensor_id;
	sPtr_outl->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_OUTLIER, (UINT32)pData, sizeof(sISP_OUTLIER));

	des_addr += sizeof(sISP_CE);
	data_size += sizeof(sISP_OUTLIER);

	memcpy(des_addr, pData, sizeof(sISP_OUTLIER));

	// 4
	sISP_SPATIAL *sPtr_spa = (sISP_SPATIAL *)pData;
	sPtr_spa->Id = sensor_id;
	sPtr_spa->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_SPATIAL, (UINT32)pData, sizeof(sISP_SPATIAL));

	des_addr += sizeof(sISP_OUTLIER);
	data_size += sizeof(sISP_SPATIAL);

	memcpy(des_addr, pData, sizeof(sISP_SPATIAL));

	// 5
	sISP_RANGE *sPtr_range = (sISP_RANGE *)pData;
	sPtr_range->Id = sensor_id;
	sPtr_range->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RANGE, (UINT32)pData, sizeof(sISP_RANGE));

	des_addr += sizeof(sISP_SPATIAL);
	data_size += sizeof(sISP_RANGE);

	memcpy(des_addr, pData, sizeof(sISP_RANGE));

	// 6
	sISP_GBAL *sPtr_gbal = (sISP_GBAL *)pData;
	sPtr_gbal->Id = sensor_id;
	sPtr_gbal->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_GBAL, (UINT32)pData, sizeof(sISP_GBAL));

	des_addr += sizeof(sISP_RANGE);
	data_size += sizeof(sISP_GBAL);

	memcpy(des_addr, pData, sizeof(sISP_GBAL));

	// 7
	sISP_RBFILL *sPtr_rbfill = (sISP_RBFILL *)pData;
	sPtr_rbfill->Id = sensor_id;
	sPtr_rbfill->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RBFILL, (UINT32)pData, sizeof(sISP_RBFILL));

	des_addr += sizeof(sISP_GBAL);
	data_size += sizeof(sISP_RBFILL);

	memcpy(des_addr, pData, sizeof(sISP_RBFILL));

	// 8
	sISP_EDGE *sPtr_edge = (sISP_EDGE *)pData;
	sPtr_edge->Id = sensor_id;
	sPtr_edge->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_EDGE, (UINT32)pData, sizeof(sISP_EDGE));

	des_addr += sizeof(sISP_RBFILL);
	data_size += sizeof(sISP_EDGE);

	memcpy(des_addr, pData, sizeof(sISP_EDGE));

	// 9
	sISP_RGBLPF *sPtr_lpf = (sISP_RGBLPF *)pData;
	sPtr_lpf->Id = sensor_id;
	sPtr_lpf->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_RGBLPF, (UINT32)pData, sizeof(sISP_RGBLPF));

	des_addr += sizeof(sISP_EDGE);
	data_size += sizeof(sISP_RGBLPF);

	memcpy(des_addr, pData, sizeof(sISP_RGBLPF));

	// 10
	sISP_CCTRL *sPtr_cctrl = (sISP_CCTRL *)pData;
	sPtr_cctrl->Id = sensor_id;
	sPtr_cctrl->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CCTRL, (UINT32)pData, sizeof(sISP_CCTRL));

	des_addr += sizeof(sISP_RGBLPF);
	data_size += sizeof(sISP_CCTRL);

	memcpy(des_addr, pData, sizeof(sISP_CCTRL));

	// 11
	sISP_IFE2FILTER *sPtr_ife2 = (sISP_IFE2FILTER *)pData;
	sPtr_ife2->Id = sensor_id;
	sPtr_ife2->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_IFE2FILTER, (UINT32)pData, sizeof(sISP_IFE2FILTER));

	des_addr += sizeof(sISP_CCTRL);
	data_size += sizeof(sISP_IFE2FILTER);

	memcpy(des_addr, pData, sizeof(sISP_IFE2FILTER));

	// 12
	sISP_LCA *sPtr_lca = (sISP_LCA *)pData;
	sPtr_lca->Id = sensor_id;
	sPtr_lca->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_LCA, (UINT32)pData, sizeof(sISP_LCA));

	des_addr += sizeof(sISP_IFE2FILTER);
	data_size += sizeof(sISP_LCA);

	memcpy(des_addr, pData, sizeof(sISP_LCA));

	// 13
	sISP_CHROMA_SUPP *sPtr_csupp = (sISP_CHROMA_SUPP *)pData;
	sPtr_csupp->Id = sensor_id;
	sPtr_csupp->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_CHROMA_SUPP, (UINT32)pData, sizeof(sISP_CHROMA_SUPP));

	des_addr += sizeof(sISP_LCA);
	data_size += sizeof(sISP_CHROMA_SUPP);

	memcpy(des_addr, pData, sizeof(sISP_CHROMA_SUPP));

	// 14
	sISP_TMNR *sPtr_tmnr = (sISP_TMNR *)pData;
	sPtr_tmnr->Id = sensor_id;
	sPtr_tmnr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_TMNR, (UINT32)pData, sizeof(sISP_TMNR));

	des_addr += sizeof(sISP_CHROMA_SUPP);
	data_size += sizeof(sISP_TMNR);

	memcpy(des_addr, pData, sizeof(sISP_TMNR));

	// 15
	sISP_TNR_CONFIG *sPtr_tnr = (sISP_TNR_CONFIG *)pData;
	sPtr_tnr->Id = sensor_id;
	sPtr_tnr->ISO_index = fake_iso_idx;

	iqt_get_param(ISP_TNR_CONFIG, (UINT32)pData, sizeof(sISP_TNR_CONFIG));

	des_addr += sizeof(sISP_TMNR);
	data_size += sizeof(sISP_TNR_CONFIG);

	memcpy(des_addr, pData, sizeof(sISP_TNR_CONFIG));

	// copy all data
	memcpy(pData, ((UINT8 *)pData + 16 * 1024), data_size);
}

void IQSISP_get_reserved(void)
{
}

void IQSISP_set_sensor_id(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("set sensor id fail\r\n");
		return;
	}
	sensor_id = IPL_PATH(pData[0]);
}

void IQSISP_set_ime_path_id(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("set path id fail\r\n");
		return;
	}
	ime_path_id = pData[0];
}

void IQSISP_set_iso_idx(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_ISOINDEX, (UINT32)pData, uiLength);
}

UINT32 IQSISP_VirtualMemMap(UINT32 MemOffset, UINT32 MemSize)
{
	return MemOffset;
}

void IQSISP_set_mem_map(void)
{
#define REG_TAG(ch0, ch1, ch2, ch3) ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) | ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))

	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	if (pData[0] == REG_TAG('V', 'M', 'A', 'P')) {
		MsdcNvt_RegHandler(MSDCNVT_HANDLER_TYPE_VIRTUAL_MEM_MAP, IQSISP_VirtualMemMap);
	} else {
		MsdcNvt_RegHandler(MSDCNVT_HANDLER_TYPE_VIRTUAL_MEM_MAP, NULL);
	}
#undef REG_TAG
}

void IQSISP_set_yuv_unlock(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("set yuv ok fail\r\n");
		return;
	}

	if(pData[0] == 1){
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

#if (NEW_CAPTURE == ENABLE)
	// resume sie
	sISP_SIE_RESUME SIE_resume;

	SIE_resume.Id = sensor_id;
	SIE_resume.size = sizeof(sISP_SIE_RESUME);
	SIE_resume.sie_resume.id = sensor_id;
	SIE_resume.sie_resume.pnext = NULL;

	if (IPL_SetCmd(IPL_SET_RESUMEDMA, (void *)&(SIE_resume.sie_resume)) != E_OK) {
		DBG_ERR("ipl set sie resume command fail\r\n");
		return;
	}
	IPL_WaitCmdFinish();
#endif
}

void IQSISP_set_sie_pause(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_SIE_PAUSE *sPtr = (sISP_SIE_PAUSE *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("set sie pause fail\r\n");
		return;
	}

	sPtr->Id = sensor_id;
	sPtr->size = sizeof(*sPtr);
	sPtr->sie_pause.id = sensor_id;
	sPtr->sie_pause.pnext = NULL;

	if (IPL_SetCmd(IPL_SET_PAUSEDMA, (void *)&(sPtr->sie_pause)) != E_OK) {
		DBG_ERR("ipl set sie pause command fail\r\n");
		return;
	}
	IPL_WaitCmdFinish();
}

void IQSISP_set_iso_map(void)
{

	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_ISOMAP, (UINT32)pData, uiLength);
}

void IQSISP_set_sie_resume(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_SIE_RESUME *sPtr = (sISP_SIE_RESUME *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("set sie resume fail\r\n");
		return;
	}

	sPtr->Id = sensor_id;
	sPtr->size = sizeof(*sPtr);
	sPtr->sie_resume.id = sensor_id;
	sPtr->sie_resume.pnext = NULL;

	if (IPL_SetCmd(IPL_SET_RESUMEDMA, (void *)&(sPtr->sie_resume)) != E_OK) {
		DBG_ERR("ipl set sie resume command fail\r\n");
		return;
	}
	IPL_WaitCmdFinish();
}

void IQSISP_set_info_reserved(void)
{
}

void IQSISP_set_ob(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_OB, (UINT32)pData, uiLength);
}

void IQSISP_set_ecs(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_ECS, (UINT32)pData, uiLength);
}

void IQSISP_set_wdr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_WDR *sPtr = (sISP_WDR *)pData;
	UINT32 pre_mode = IPL_AlgGetUIInfo(sPtr->Id, IPL_SEL_WDR_IMM);
	UINT32 pre_level = IPL_AlgGetUIInfo(sPtr->Id, IPL_SEL_WDR_M_LEVEL_IMM);

	iqt_set_param(ISP_WDR, (UINT32)pData, uiLength);

	if (pre_mode != sPtr->WDR_mode) {
		IPL_AlgSetUIInfo(sPtr->Id, IPL_SEL_WDR_IMM, sPtr->WDR_mode);
	}
	if (pre_level != sPtr->WDR_manual_level) {
		IPL_AlgSetUIInfo(sPtr->Id, IPL_SEL_WDR_M_LEVEL_IMM, sPtr->WDR_manual_level);
	}
}

void IQSISP_set_defog(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_DEFOG_CONFIG, (UINT32)pData, uiLength);

	sISP_DEFOG_CONFIG *sPtr = (sISP_DEFOG_CONFIG *)pData;
	IPL_AlgSetUIInfo(sPtr->Id, IPL_SEL_DEFOG_IMM, sPtr->defog_mode);
	IPL_AlgSetUIInfo(sPtr->Id, IPL_SEL_DEFOG_LEVEL_IMM, sPtr->defog_level);
}

void IQSISP_set_outlier(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_OUTLIER, (UINT32)pData, uiLength);
}

void IQSISP_set_spatial(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_SPATIAL, (UINT32)pData, uiLength);
}

void IQSISP_set_range(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_RANGE, (UINT32)pData, uiLength);
}

void IQSISP_set_rbfill(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_RBFILL, (UINT32)pData, uiLength);
}

void IQSISP_set_vig(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_VIG, (UINT32)pData, uiLength);
}

void IQSISP_set_gbal(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_GBAL, (UINT32)pData, uiLength);
}

void IQSISP_set_ldc(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_LDC, (UINT32)pData, uiLength);
}

void IQSISP_set_edge(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_EDGE, (UINT32)pData, uiLength);
}

void IQSISP_set_rgblpf(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_RGBLPF, (UINT32)pData, uiLength);
}

void IQSISP_set_cc(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_CC, (UINT32)pData, uiLength);
}

void IQSISP_set_y_curve(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_Y_CURVE, (UINT32)pData, uiLength);
}

void IQSISP_set_gamma(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_GAMMA, (UINT32)pData, uiLength);
}

void IQSISP_set_3dcc(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_3DCC, (UINT32)pData, uiLength);
}

void IQSISP_set_cctrl(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_CCTRL, (UINT32)pData, uiLength);
}

void IQSISP_set_cst(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_CST, (UINT32)pData, uiLength);
}

void IQSISP_set_ife2filter(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_IFE2FILTER, (UINT32)pData, uiLength);
}

void IQSISP_set_lca(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_LCA, (UINT32)pData, uiLength);
}

void IQSISP_set_chroma_supp(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_CHROMA_SUPP, (UINT32)pData, uiLength);
}

void IQSISP_set_tmnr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_TMNR, (UINT32)pData, uiLength);
}

void IQSISP_set_tmnr_debug(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_TMNR_DEBUG, (UINT32)pData, uiLength);
}

void IQSISP_set_tmnr_auto(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_TMNR_AUTO, (UINT32)pData, uiLength);
}

void IQSISP_set_tnr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_TNR, (UINT32)pData, uiLength);
}

void IQSISP_set_tnr_cfg(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_TNR_CONFIG, (UINT32)pData, uiLength);
}

void IQSISP_set_ce(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_CE, (UINT32)pData, uiLength);
}

void IQSISP_set_ctrl(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_ENGINE_CTRL, (UINT32)pData, uiLength);
}

void IQSISP_set_hdr_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	iqt_set_param(ISP_HDR_CONFIG, (UINT32)pData, uiLength);

	sISP_HDR_CONFIG *sPtr = (sISP_HDR_CONFIG *)pData;
	SensorHDR_SetTestMode(sPtr->test_param);
	IPL_AlgSetUIInfo(sPtr->Id, IPL_SEL_SHDR, sPtr->hdr_mode);
	IPL_AlgSetUIInfo(sPtr->Id, IPL_SEL_SHDR_LEVEL_IMM, sPtr->hdr_level);
}

void IQSISP_set_8iso_param(void)
{
	UINT8 *des_addr;
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 data_size;

	data_size = sizeof(sISP_OB) + sizeof(sISP_CE) + sizeof(sISP_OUTLIER) + sizeof(sISP_SPATIAL) +
			sizeof(sISP_RANGE) + sizeof(sISP_GBAL) + sizeof(sISP_RBFILL) + sizeof(sISP_EDGE) +
			sizeof(sISP_RGBLPF) + sizeof(sISP_CCTRL) + sizeof(sISP_IFE2FILTER) + sizeof(sISP_LCA) +
			sizeof(sISP_CHROMA_SUPP) + sizeof(sISP_TMNR) + sizeof(sISP_TNR_CONFIG);

	// 1
	des_addr = (UINT8 *)pData + 16 * 1024;    // 16KB ~
	memcpy(des_addr, pData, data_size);

	iqt_set_param(ISP_OB, (UINT32)pData, sizeof(sISP_OB));

	// 2
	des_addr = (UINT8 *)pData + sizeof(sISP_OB);
	memcpy(pData, des_addr, sizeof(sISP_CE));

	iqt_set_param(ISP_CE, (UINT32)pData, sizeof(sISP_CE));

	// 3
	des_addr += sizeof(sISP_CE);
	memcpy(pData, des_addr, sizeof(sISP_OUTLIER));

	iqt_set_param(ISP_OUTLIER, (UINT32)pData, sizeof(sISP_OUTLIER));

	// 4
	des_addr += sizeof(sISP_OUTLIER);
	memcpy(pData, des_addr, sizeof(sISP_SPATIAL));

	iqt_set_param(ISP_SPATIAL, (UINT32)pData, sizeof(sISP_SPATIAL));

	// 5
	des_addr += sizeof(sISP_SPATIAL);
	memcpy(pData, des_addr, sizeof(sISP_RANGE));

	iqt_set_param(ISP_RANGE, (UINT32)pData, sizeof(sISP_RANGE));

	// 6
	des_addr += sizeof(sISP_RANGE);
	memcpy(pData, des_addr, sizeof(sISP_GBAL));

	iqt_set_param(ISP_GBAL, (UINT32)pData, sizeof(sISP_GBAL));

	// 7
	des_addr += sizeof(sISP_GBAL);
	memcpy(pData, des_addr, sizeof(sISP_RBFILL));

	iqt_set_param(ISP_RBFILL, (UINT32)pData, sizeof(sISP_RBFILL));

	// 8
	des_addr += sizeof(sISP_RBFILL);
	memcpy(pData, des_addr, sizeof(sISP_EDGE));

	iqt_set_param(ISP_EDGE, (UINT32)pData, sizeof(sISP_EDGE));

	// 9
	des_addr += sizeof(sISP_EDGE);
	memcpy(pData, des_addr, sizeof(sISP_RGBLPF));

	iqt_set_param(ISP_RGBLPF, (UINT32)pData, sizeof(sISP_RGBLPF));

	// 10
	des_addr += sizeof(sISP_RGBLPF);
	memcpy(pData, des_addr, sizeof(sISP_CCTRL));

	iqt_set_param(ISP_CCTRL, (UINT32)pData, sizeof(sISP_CCTRL));

	// 11
	des_addr += sizeof(sISP_CCTRL);
	memcpy(pData, des_addr, sizeof(sISP_IFE2FILTER));

	iqt_set_param(ISP_IFE2FILTER, (UINT32)pData, sizeof(sISP_IFE2FILTER));

	// 12
	des_addr += sizeof(sISP_IFE2FILTER);
	memcpy(pData, des_addr, sizeof(sISP_LCA));

	iqt_set_param(ISP_LCA, (UINT32)pData, sizeof(sISP_LCA));

	// 13
	des_addr += sizeof(sISP_LCA);
	memcpy(pData, des_addr, sizeof(sISP_CHROMA_SUPP));

	iqt_set_param(ISP_CHROMA_SUPP, (UINT32)pData, sizeof(sISP_CHROMA_SUPP));

	// 14
	des_addr += sizeof(sISP_CHROMA_SUPP);
	memcpy(pData, des_addr, sizeof(sISP_TMNR));

	iqt_set_param(ISP_TMNR, (UINT32)pData, sizeof(sISP_TMNR));

	// 15
	des_addr += sizeof(sISP_TMNR);
	memcpy(pData, des_addr, sizeof(sISP_TNR_CONFIG));

	iqt_set_param(ISP_TNR_CONFIG, (UINT32)pData, sizeof(sISP_TNR_CONFIG));
}

void IQSISP_set_iq_ui_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_IQ_UI_INFO *sPtr = (sISP_IQ_UI_INFO *)pData;

	if(uiLength != sizeof(*sPtr)) {
		DBG_WRN("size mismatch \r\n");
		return;
	}

	sPtr->id = sensor_id;
	iq_set_ui_info(sPtr->id, sPtr->ui_info_element, sPtr->value);
}

void IQSISP_set_dr_mode(void)
{
    #if(IPCAM_FUNC==ENABLE)
    IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    //UINT32 uiLength = MsdcNvt_GetTransSize();
    UIAPPIPCAM_IPL_FUNC *pIPLFunc = UIAppIPCam_get_IPLFuncInfo();
    UINT32 uiIPLFunc = ImageUnit_GetParam(ISF_IPL(sensor_id),ISF_CTRL,IMAGEPIPE_PARAM_FUNC);
    //UINT32 mode = pData[0];  //0: linear, 1: WDR, 2: HDR
    UINT32 mode = pData[0];  //0x0: linear, 0x1: HDR, 0x2: WDR, 0x4: Defog
    UINT32 i=0;

    #if 1 //With IPL change mode.
    debug_msg("~G IQSISP_set_wdr_shdr_mode mode = 0x%X\r\n",mode);

    if (mode & IQ_FEATURE_DEFOG) {
        uiIPLFunc |= IPL_FUNC_DEFOG;
        pIPLFunc[sensor_id].sDefog.eDeFogMode = SEL_DEFOG_ON;
        debug_msg("~G Defog ON\r\n");
    } else {
        uiIPLFunc &= ~IPL_FUNC_DEFOG;
        pIPLFunc[sensor_id].sDefog.eDeFogMode = SEL_DEFOG_OFF;
        debug_msg("~M Defog OFF\r\n");
    }

    if (mode & IQ_FEATURE_HDR) {
        uiIPLFunc |= IPL_FUNC_SHDR;
        mode &= ~IQ_FEATURE_WDR;  //Force to disable WDR
        pIPLFunc[sensor_id].sHDR.eHDREnable = SEL_SHDR_ON;
        debug_msg("~G HDR ON\r\n");
    } else {
        uiIPLFunc &= ~IPL_FUNC_SHDR;
        pIPLFunc[sensor_id].sHDR.eHDREnable = SEL_SHDR_OFF;
        debug_msg("~M HDR OFF\r\n");
    }

    if (mode & IQ_FEATURE_WDR) {
        uiIPLFunc |= IPL_FUNC_WDR;
        pIPLFunc[sensor_id].sWDR.uiWDRMode = SEL_WDR_AUTO;
        debug_msg("~G WDR ON\r\n");
    } else {
        if (mode & IQ_FEATURE_HDR) {
            uiIPLFunc &= ~IPL_FUNC_WDR;
            debug_msg("~M WDR true OFF\r\n");
        } else {
            uiIPLFunc |= IPL_FUNC_WDR;
            debug_msg("~M WDR fake OFF\r\n");
        }
        pIPLFunc[sensor_id].sWDR.uiWDRMode = SEL_WDR_OFF;
    }

    for(i=0; i<IPCAM_PATH_MAX_ID; i++) {
        if(pStreamInfo[i].sensor == sensor_id && pStreamInfo[i].enable== IPCAM_STATE_ENABLE) {
            ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY);
            break;
        }
    }
    ImageUnit_Begin(ISF_IPL(sensor_id), 0);
    ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR, pIPLFunc[sensor_id].sHDR.eHDREnable);
    ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, pIPLFunc[sensor_id].sWDR.uiWDRMode);
    ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DEFOG_IMM, pIPLFunc[sensor_id].sDefog.eDeFogMode);
    ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_OFF);
    ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, uiIPLFunc);
    ImageUnit_End();

    for(i=0; i<IPCAM_PATH_MAX_ID; i++) {
        if(pStreamInfo[i].sensor == sensor_id && pStreamInfo[i].enable== IPCAM_STATE_ENABLE) {
            ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN);
            ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY);
            break;
        }
    }

    ImageUnit_Begin(ISF_IPL(sensor_id), 0);
    ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_VIDEO);
    ImageUnit_End();

    for(i=0; i<IPCAM_PATH_MAX_ID; i++) {
        if(pStreamInfo[i].sensor == sensor_id && pStreamInfo[i].enable== IPCAM_STATE_ENABLE) {
            ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN);
            break;
        }
    }
    #else  //Without IPL change mode.
    debug_msg("~G IQSISP_set_wdr_shdr_mode mode = 0x%X\r\n",mode);

    BOOL ipl_chg_mode_flg = FALSE;

    if (mode & IQ_FEATURE_DEFOG) {
        if (pIPLFunc[sensor_id].sDefog.eDeFogMode == SEL_DEFOG_OFF) {
            ipl_chg_mode_flg = TRUE;
            uiIPLFunc |= IPL_FUNC_DEFOG;
            pIPLFunc[sensor_id].sDefog.eDeFogMode = SEL_DEFOG_ON;
            debug_msg("~G Defog ON\r\n");
        }
    } else {
        if (pIPLFunc[sensor_id].sDefog.eDeFogMode == SEL_DEFOG_ON) {
            ipl_chg_mode_flg = TRUE;
            uiIPLFunc &= ~IPL_FUNC_DEFOG;
            pIPLFunc[sensor_id].sDefog.eDeFogMode = SEL_DEFOG_OFF;
            debug_msg("~M Defog OFF\r\n");
        }
    }

    if (mode & IQ_FEATURE_HDR) {
        uiIPLFunc |= IPL_FUNC_SHDR;
        uiIPLFunc &= ~IPL_FUNC_WDR;
        pIPLFunc[sensor_id].sHDR.eHDREnable = SEL_SHDR_ON;
        pIPLFunc[sensor_id].sWDR.uiWDRMode = SEL_WDR_OFF;
        debug_msg("~G HDR ON\r\n");
    } else if (mode & IQ_FEATURE_WDR) {
        uiIPLFunc |= IPL_FUNC_SHDR;
        uiIPLFunc &= ~IPL_FUNC_WDR;
        pIPLFunc[sensor_id].sHDR.eHDREnable = SEL_SHDR_WDR;
        pIPLFunc[sensor_id].sWDR.uiWDRMode = SEL_WDR_AUTO;
        debug_msg("~G WDR ON\r\n");
    } else {
        uiIPLFunc |= IPL_FUNC_SHDR;
        uiIPLFunc &= ~IPL_FUNC_WDR;
        pIPLFunc[sensor_id].sHDR.eHDREnable = SEL_SHDR_OFF;
        pIPLFunc[sensor_id].sWDR.uiWDRMode = SEL_WDR_OFF;
        debug_msg("~M WDR/HDR OFF\r\n");
    }

    if (ipl_chg_mode_flg == TRUE) {
        for(i=0; i<IPCAM_PATH_MAX_ID; i++) {
            if(pStreamInfo[i].sensor == sensor_id && pStreamInfo[i].enable== IPCAM_STATE_ENABLE) {
                ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY);
                break;
            }
        }
        ImageUnit_Begin(ISF_IPL(sensor_id), 0);
        ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR, pIPLFunc[sensor_id].sHDR.eHDREnable);
        ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, pIPLFunc[sensor_id].sWDR.uiWDRMode);
        ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DEFOG_IMM, pIPLFunc[sensor_id].sDefog.eDeFogMode);
        ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_OFF);
        ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, uiIPLFunc);
        ImageUnit_End();

        for(i=0; i<IPCAM_PATH_MAX_ID; i++) {
            if(pStreamInfo[i].sensor == sensor_id && pStreamInfo[i].enable== IPCAM_STATE_ENABLE) {
                ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN);
                ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY);
                break;
            }
        }

        ImageUnit_Begin(ISF_IPL(sensor_id), 0);
        ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_VIDEO);
        ImageUnit_End();

        for(i=0; i<IPCAM_PATH_MAX_ID; i++) {
            if(pStreamInfo[i].sensor == sensor_id && pStreamInfo[i].enable== IPCAM_STATE_ENABLE) {
                ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN);
                break;
            }
        }
    }else {
        for(i=0; i<IPCAM_PATH_MAX_ID; i++) {
            if(pStreamInfo[i].sensor == sensor_id && pStreamInfo[i].enable== IPCAM_STATE_ENABLE) {
                ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY);
                break;
            }
        }
        ImageUnit_Begin(ISF_IPL(sensor_id), 0);
        ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR, pIPLFunc[sensor_id].sHDR.eHDREnable);
        ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, pIPLFunc[sensor_id].sWDR.uiWDRMode);
        debug_msg("~R checkpoint, ui hdr = %d, ui wdr = %d \r\n",pIPLFunc[sensor_id].sHDR.eHDREnable,pIPLFunc[sensor_id].sWDR.uiWDRMode);
        ImageUnit_End();
        for(i=0; i<IPCAM_PATH_MAX_ID; i++) {
            if(pStreamInfo[i].sensor == sensor_id && pStreamInfo[i].enable== IPCAM_STATE_ENABLE) {
                ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN);
                break;
            }
        }
    }
    #endif
    #endif
}

void IQSISP_set_reserved(void)
{
}

///////////////////////////////////
//       ISP Setting Codec        /
///////////////////////////////////
UINT32 ipcam_path_id = 0;

void IQSCODEC_get_ver(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("size mismatch");
		return;
	}
	pData[0] = 0x00010007;//v.1.0.7
}

void IQSCODEC_get_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_INFO *sPtr = (sISP_CODEC_INFO *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	IPCAM_STREAM_INFO* p_stream = (IPCAM_STREAM_INFO *)UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_SENSOR_INFO* p_sensor = (UIAPPIPCAM_SENSOR_INFO *)UIAppIPCam_get_SensorInfo();
	UIAPPIPCAM_CROP_INFO* p_crop = (UIAPPIPCAM_CROP_INFO *)UIAppIPCam_get_CropInfo();
	UIAPPIPCAM_ENCODE_CONFIG* p_encode = (UIAPPIPCAM_ENCODE_CONFIG *)UIAppIPCam_get_EncodeConfig();

	memcpy(sPtr->ipcam_stream_info, p_stream, sizeof(IPCAM_STREAM_INFO)*UIAPPIPCAM_PATH_MAX_ID);
	memcpy(sPtr->ipcam_sensor_info, p_sensor, sizeof(UIAPPIPCAM_SENSOR_INFO)*UIAPPIPCAM_SENSOR_MAX);
	memcpy(sPtr->ipcam_crop_info, p_crop, sizeof(UIAPPIPCAM_CROP_INFO)*UIAPPIPCAM_SENSOR_MAX);
	memcpy(sPtr->ipcam_encode_config, p_encode, sizeof(UIAPPIPCAM_ENCODE_CONFIG)*UIAPPIPCAM_PATH_MAX_ID);

	sPtr->alg_path = UIAppIPCam_get_ALGPath();
	sPtr->display_path = UIAppIPCam_get_DisplayPath();
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_aq_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_AQ_INFO *sPtr = (sISP_CODEC_AQ_INFO *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAPPIPCAM_AQ_CONFIG* p_aq = (UIAPPIPCAM_AQ_CONFIG *)UIAppIPCam_get_AQInfo();
	memcpy(sPtr->ipcam_aq_info, p_aq, sizeof(UIAPPIPCAM_AQ_CONFIG)*UIAPPIPCAM_PATH_MAX_ID);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_cbr_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_CBR_INFO *sPtr = (sISP_CODEC_CBR_INFO *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAPPIPCAM_CBR_CONFIG* p_cbr = (UIAPPIPCAM_CBR_CONFIG *)UIAppIPCam_get_CBRInfo();
	memcpy(sPtr->ipcam_cbr_info, p_cbr, sizeof(UIAPPIPCAM_CBR_CONFIG)*UIAPPIPCAM_PATH_MAX_ID);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_evbr_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_EVBR_INFO *sPtr = (sISP_CODEC_EVBR_INFO *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	sISP_CODEC_EVBR_INFO* p_evbr = (sISP_CODEC_EVBR_INFO *)UIAppIPCam_get_EVBRInfo();
	memcpy(sPtr->ipcam_evbr_info, p_evbr, sizeof(sISP_CODEC_EVBR_INFO)*UIAPPIPCAM_PATH_MAX_ID);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_fix_qp_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_FIX_QP_INFO *sPtr = (sISP_CODEC_FIX_QP_INFO *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAPPIPCAM_FIXQP_CONFIG* p_fix_qp = (UIAPPIPCAM_FIXQP_CONFIG *)UIAppIPCam_get_FixQPInfo();
	memcpy(sPtr->ipcam_fix_qp_info, p_fix_qp, sizeof(UIAPPIPCAM_FIXQP_CONFIG)*UIAPPIPCAM_PATH_MAX_ID);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_vbr_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_VBR_INFO *sPtr = (sISP_CODEC_VBR_INFO *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAPPIPCAM_VBR_CONFIG* p_vbr = (UIAPPIPCAM_VBR_CONFIG *)UIAppIPCam_get_VBRInfo();
	memcpy(sPtr->ipcam_vbr_info, p_vbr, sizeof(UIAPPIPCAM_VBR_CONFIG)*UIAPPIPCAM_PATH_MAX_ID);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_aq(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_AQ *sPtr = (sISP_CODEC_AQ *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_GetAQ(sPtr->ipcam_path_id, &(sPtr->aq));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_cbr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_CBR *sPtr = (sISP_CODEC_CBR *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_GetCBR(sPtr->ipcam_path_id, &(sPtr->cbr));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_evbr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_EVBR *sPtr = (sISP_CODEC_EVBR *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_GetEVBR(sPtr->ipcam_path_id, &(sPtr->evbr));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_fix_qp(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_FIX_QP *sPtr = (sISP_CODEC_FIX_QP *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_GetFixQP(sPtr->ipcam_path_id, &(sPtr->fix_qp));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_vbr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_VBR *sPtr = (sISP_CODEC_VBR *)pData;
	sPtr->ipcam_path_id = ipcam_path_id;
	sPtr->size = sizeof(*sPtr);

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_GetVBR(sPtr->ipcam_path_id, &(sPtr->vbr));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_get_reserved(void)
{
}

void IQSCODEC_set_ipcam_path(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("set ipcam path id fail\r\n");
		return;
	}
	ipcam_path_id = pData[0];
}

void IQSCODEC_set_aq_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_AQ_INFO *sPtr = (sISP_CODEC_AQ_INFO *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAppIPCam_set_AQInfo(sPtr->ipcam_aq_info);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_cbr_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_CBR_INFO *sPtr = (sISP_CODEC_CBR_INFO *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAppIPCam_set_CBRInfo(sPtr->ipcam_cbr_info);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_evbr_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_EVBR_INFO *sPtr = (sISP_CODEC_EVBR_INFO *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAppIPCam_set_EVBRInfo(sPtr->ipcam_evbr_info);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_fix_qp_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_FIX_QP_INFO *sPtr = (sISP_CODEC_FIX_QP_INFO *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAppIPCam_set_FixQPInfo(sPtr->ipcam_fix_qp_info);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_vbr_all(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_VBR_INFO *sPtr = (sISP_CODEC_VBR_INFO *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	UIAppIPCam_set_VBRInfo(sPtr->ipcam_vbr_info);
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_aq(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_AQ *sPtr = (sISP_CODEC_AQ *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_SetAQ(sPtr->ipcam_path_id, &(sPtr->aq));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_cbr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_CBR *sPtr = (sISP_CODEC_CBR *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_SetCBR(sPtr->ipcam_path_id, &(sPtr->cbr));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_evbr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_EVBR *sPtr = (sISP_CODEC_EVBR *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_SetEVBR(sPtr->ipcam_path_id, &(sPtr->evbr));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_fix_qp(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_FIX_QP *sPtr = (sISP_CODEC_FIX_QP *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_SetFixQP(sPtr->ipcam_path_id, &(sPtr->fix_qp));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_vbr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_CODEC_VBR *sPtr = (sISP_CODEC_VBR *)pData;

	if (uiLength != sizeof(*sPtr)) {
		DBG_ERR("size mismatch\r\n");
		return;
	}

#if(IPCAM_FUNC==ENABLE)
	NMR_VdoEnc_SetVBR(sPtr->ipcam_path_id, &(sPtr->vbr));
#else
	DBG_ERR("NOT IPCAM MODEL!!\r\n");
#endif
}

void IQSCODEC_set_reserved(void)
{
}

///////////////////////////////////
//       ISP Setting Inter        /
///////////////////////////////////
void IQSINTER_get_direct_mode(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	IPL_SIE_CUR_FRAME_INFO sie_info = {0};

	sie_info.id = sensor_id;

	if (IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &sie_info) != E_OK) {
		DBG_ERR("ipl get sie info fail\r\n");
		return;
	}

	pData[0] = 0x5AA50000 | sie_info.out_direct_en;
}

void IQSINTER_get_color_range(void)
{
	CHAR *pData = (CHAR *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 16) {
		DBG_ERR("size mismatch");
		return;
	}

#if (YUV_TV_RANGE == ENABLE)
	CHAR color_range[16] = "TV_RANGE";
	memcpy(pData, color_range, sizeof(CHAR)*16);
#else
	CHAR color_range[16] = "FULL_RANGE";
	memcpy(pData, color_range, sizeof(CHAR)*16);
#endif
}

void IQSINTER_get_sie_crop_pos(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	IPL_IN_SIZE_INFO *sPtr = (IPL_IN_SIZE_INFO *)pData;

	sPtr->id = (IPL_PROC_ID)sensor_id;

	//IPL_GetCmd(IPL_GET_CAL_IPL_IN_SIZE, (void *)sPtr);
}

void IQSINTER_get_uitron_ready(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	IPL_MODE_INFOR ipl_info = { 0 };

	ipl_info.id = (IPL_PROC_ID)sensor_id;

	if (IPL_IsOpen() == TRUE) {
		IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&ipl_info);

		pData[0] = 0x5AA50000 | ipl_info.mode;
	} else {
		pData[0] = 0x5AA50000;
	}
}

void IQSINTER_get_dis_enable(void)
{
#if(IPCAM_FUNC==ENABLE)
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UIAPPIPCAM_IPL_FUNC *pIPLFunc = UIAppIPCam_get_IPLFuncInfo();

	if (pIPLFunc[sensor_id].uiDIS == ENABLE) {
		pData[0] = 0x5AA50000 | ENABLE;
	} else {
		pData[0] = 0x5AA50000 | DISABLE;
	}
#endif
}

void IQSINTER_get_reserved(void)
{
}

void IQSINTER_set_color_range(void)
{
}

void IQSINTER_set_sie_crop_pos(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	IPL_IN_IMG_OFS *sPtr = (IPL_IN_IMG_OFS *)pData;

	IPL_SetCmd(IPL_SET_IN_IMG_OFFSET, (void *)sPtr);
}

void IQSINTER_set_dis_enable(void)
{
#if(IPCAM_FUNC==ENABLE)
    IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
    UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
    //UINT32 uiLength = MsdcNvt_GetTransSize();
    UIAPPIPCAM_IPL_FUNC *pIPLFunc = UIAppIPCam_get_IPLFuncInfo();
    ///UINT32 uiIPLFunc2 = ImageUnit_GetParam(ISF_IPL(sensor_id), ISF_CTRL, IMAGEPIPE_PARAM_FUNC2);
    UINT32 i, dis_en = pData[1];

    if (dis_en != 0) {
        ///uiIPLFunc2 |= IMAGEPIPE_FUNC2_VIEWTRACKING4 | IMAGEPIPE_FUNC2_VIEWTRACKING3 | IMAGEPIPE_FUNC2_VIEWTRACKING2 | IMAGEPIPE_FUNC2_VIEWTRACKING1;
        pIPLFunc[sensor_id].uiDIS = ENABLE;
    } else {
        ///uiIPLFunc2 &= ~(IMAGEPIPE_FUNC2_VIEWTRACKING4 | IMAGEPIPE_FUNC2_VIEWTRACKING3 | IMAGEPIPE_FUNC2_VIEWTRACKING2 | IMAGEPIPE_FUNC2_VIEWTRACKING1);
        pIPLFunc[sensor_id].uiDIS = DISABLE;
    }

    for (i = 0; i < IPCAM_PATH_MAX_ID; i++) {
        if ((pStreamInfo[i].sensor == sensor_id) && (pStreamInfo[i].enable == IPCAM_STATE_ENABLE)) {
            ImageApp_IPCam_ImagePipe_set_status(i, ISF_PORT_STATE_READY);
            break;
        }
    }
    ImageUnit_Begin(ISF_IPL(sensor_id), 0);
    ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, pIPLFunc[sensor_id].uiDIS);
    ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_OFF);
    ///ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MAX_FUNC2, uiIPLFunc2);
    ///ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC2, uiIPLFunc2);
    ImageUnit_End();

    for (i = 0; i < IPCAM_PATH_MAX_ID; i++) {
        if ((pStreamInfo[i].sensor == sensor_id) && (pStreamInfo[i].enable== IPCAM_STATE_ENABLE)) {
            ImageApp_IPCam_ImagePipe_set_status(i, ISF_PORT_STATE_RUN);
            ImageApp_IPCam_ImagePipe_set_status(i, ISF_PORT_STATE_READY);
            break;
        }
    }

    ImageUnit_Begin(ISF_IPL(sensor_id), 0);
    ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_VIDEO);
    ImageUnit_End();

    for (i = 0; i < IPCAM_PATH_MAX_ID; i++) {
        if ((pStreamInfo[i].sensor == sensor_id) && (pStreamInfo[i].enable== IPCAM_STATE_ENABLE)) {
            ImageApp_IPCam_ImagePipe_set_status(i, ISF_PORT_STATE_RUN);
            break;
        }
    }
#endif
}

void IQSINTER_set_reserved(void)
{
}

///////////////////////////////////
//       ISP Setting sensor       /
///////////////////////////////////
static UINT32 sensor_addr = 0x0000;
static UINT32 sensor_mode = SENSOR_MODE_1;

void IQSSEN_get_ver(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("size mismatch");
		return;
	}
	pData[0] = 0x00010001;//v.1.0.1
}

void IQSSEN_get_reg(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	SENSOR_CMD cmd = {0, 1, {0xFFFFFFFF, 0xFFFFFFFF}};

	cmd.ui_addr = sensor_addr;

	if (sensor_read_reg((SENSOR_ID)sensor_id, &cmd) == E_OK) {
		pData[0] = cmd.ui_data[0];
		pData[1] = cmd.ui_data[1];
	} else {
		pData[0] = 0xFFFFFFFF;
	}
}

void IQSSEN_get_mode(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 curr_mode = SENSOR_MODE_1, max_mode_num = 1;

	if (sensor_get_info((SENSOR_ID)sensor_id, SEN_GET_CUR_MODE, &curr_mode) == E_OK) {
		sensor_mode = curr_mode;
	}

	if (sensor_get_info((SENSOR_ID)sensor_id, SEN_GET_MAX_MODE, &max_mode_num) == E_OK) {
		pData[0] = (max_mode_num << 16) + sensor_mode;
	} else {
		pData[0] = (1 << 16) + sensor_mode;
		DBG_ERR("sensor get max_mode fail\r\n");
	}
}

void IQSSEN_get_fps(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	FPS_PARAM fps_info = {0};
	ER rt = E_OK;

	fps_info.sensor_mode = sensor_mode;
	rt = sensor_get_info((SENSOR_ID)sensor_id, SEN_GET_FPS, (void *)&fps_info);
	if (rt != E_OK) {
		return;
	}

	fps_info.cur_fps = SEN_FPS_FMT_CONV2_REAL(fps_info.cur_fps) / 1000;
	fps_info.chg_fps = SEN_FPS_FMT_CONV2_REAL(fps_info.chg_fps) / 1000;
	fps_info.dft_fps = SEN_FPS_FMT_CONV2_REAL(fps_info.dft_fps) / 1000;

	memcpy(pData, &fps_info, sizeof(FPS_PARAM));
}

void IQSSEN_get_mirror_flip(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 mirror_flip = 0;

	ImageUnit_Begin(ISF_IPL(sensor_id), 0);
	ImageUnit_GetVdoDirection(ISF_IPL(sensor_id),ISF_IN1, &mirror_flip);
	ImageUnit_End();

	pData[0] = (mirror_flip >> 28) & (SENSOR_OPTION_FLIP | SENSOR_OPTION_MIRROR);
}

void IQSSEN_get_status(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	SEN_STATUS_PARAM *sPtr = (SEN_STATUS_PARAM *)pData;

	sPtr->sensor_mode = sensor_mode;

	if (sensor_get_info((SENSOR_ID)sensor_id, SEN_GET_STATUS, sPtr) == E_OK) {
		sPtr->sensor_info.cell_width = sPtr->sensor_info.mode->valid_width;
		sPtr->sensor_info.cell_height = sPtr->sensor_info.mode->valid_height;
	} else {
		DBG_ERR("sensor get status fail\r\n");
	}
}

void IQSSEN_get_resloution(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	SEN_STATUS_PARAM sen_param;

	sen_param.sensor_mode = sensor_mode;

	if (sensor_get_info((SENSOR_ID)sensor_id, SEN_GET_STATUS, &sen_param) != E_OK) {
		pData[0] = 0;
		pData[1] = 0;

		DBG_ERR("sensor get status fail\r\n");

		return;
	}

	pData[0] = sen_param.sensor_info.mode->valid_width;
	pData[1] = sen_param.sensor_info.mode->valid_height;
}

void IQSSEN_get_reserved(void)
{
}

void IQSSEN_set_addr(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("set sensor address fail\r\n");
		return;
	}
	sensor_addr = pData[0];
}

void IQSSEN_set_reg(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	SENSOR_CMD cmd = {0, 0, {0, 0}};

	cmd.ui_addr     = pData[0];
	cmd.ui_data[0]  = pData[1];
	cmd.ui_data[1]  = pData[2];
	cmd.ui_data_len = 2;

	sensor_write_reg((SENSOR_ID)sensor_id, &cmd);
}

void IQSSEN_set_mode(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	SEN_CHGMODE_DATA sen_mode = {0};

	sen_mode.mode = sensor_mode = pData[0];
	sen_mode.multi_id = SENSOR_MULTI_ID_NONE;
	sen_mode.frame_rate = SEN_FPS(pData[1], 1);

	sensor_chg_mode((SENSOR_ID)sensor_id, sen_mode);
}

void IQSSEN_set_fps(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 fps, default_fps;
	FPS_PARAM fps_info = {0};
	ER rt = E_OK;

	fps_info.sensor_mode = sensor_mode;
	rt = sensor_get_info((SENSOR_ID)sensor_id, SEN_GET_FPS, (void *)&fps_info);
	if (rt != E_OK) {
		return;
	}

	default_fps = SEN_FPS_FMT_CONV2_REAL(fps_info.dft_fps) / 1000;

	if ((pData[0] > default_fps) || (pData[0] < 5)) {
		fps = default_fps;
	} else {
		fps = pData[0];
	}

	fps = SEN_FPS(fps, 1);
	sensor_set_info((SENSOR_ID)sensor_id, SEN_SET_FPS, (void *)&fps);
}

void IQSSEN_set_mirror_flip(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 mirror_flip = 0, img_dir = 0;

	mirror_flip = pData[0] & (SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP);

	if (mirror_flip & SENSOR_OPTION_MIRROR) {
		img_dir |= ISF_VDO_DIR_MIRRORX;
	}

	if (mirror_flip & SENSOR_OPTION_FLIP) {
		img_dir |= ISF_VDO_DIR_MIRRORY;
	}

	ImageUnit_Begin(ISF_IPL(sensor_id), 0);
	ImageUnit_SetVdoDirection(ISF_IN1, img_dir);
	ImageUnit_End();

	ImageApp_IPCam_reset_path(IPCAM_PATH_ID_0);
}

void IQSSEN_set_reserved(void)
{
}

///////////////////////////////////
//       ISP Setting lens         /
///////////////////////////////////
UINT32 lens_argu = 0;

void IQSLENS_get_ver(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4) {
		DBG_ERR("size mismatch");
		return;
	}
	pData[0] = 0x00010001;//v.1.0.1
}

void IQSLENS_get_focal_length(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	pData[0] = Lens_FocusGetFocalLength(lens_argu);    // ZOOM_SECTION_WIDE ~ ZOOM_SECTION_TELE
}

void IQSLENS_get_zoom_speed(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	pData[0] = Lens_ZoomGetSpeed();
}

void IQSLENS_get_zoom_section(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	ZOOM_SECTION_CATEGORY zoom_sec;

	zoom_sec = (ZOOM_SECTION_CATEGORY)((lens_argu > ZOOM_CURR_SECTION) ? ZOOM_CURR_SECTION : lens_argu);

	pData[0] = Lens_ZoomGetSection(zoom_sec);    // ZOOM_MIN_SECTION, ZOOM_MAX_SECTION, ZOOM_CURR_SECTION
}

void IQSLENS_get_zoom_pos(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();
	ZOOM_POS_CATEGORY pos_type;

	pos_type = (ZOOM_POS_CATEGORY)((lens_argu > ZOOM_CURR_SECTION_POS) ? ZOOM_CURR_SECTION_POS : lens_argu);

	pData[0] = Lens_ZoomGetPosition(pos_type);    // ZOOM_CURR_POS or ZOOM_CURR_SECTION_POS
}

void IQSLENS_get_focus_speed(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	pData[0] = Lens_FocusGetSpeed();
}

void IQSLENS_get_focus_range(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	FOCUS_RANGE range_type;

	range_type = (FOCUS_RANGE)((lens_argu > FOCUS_RANGE_END) ? FOCUS_RANGE_END : lens_argu);

	pData[0] = Lens_FocusGetRange(range_type);    // FOCUS_RANGE_START or FOCUS_RANGE_END
}

void IQSLENS_get_focus_pos(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();

	pData[0] = Lens_FocusGetPosition();
}

void IQSLENS_get_aperture_pos(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	pData[0] = (UINT32)Lens_ApertureGetPosition();
}

void IQSLENS_get_cap_info(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	pData[0] = Lens_GetCapabilityInfo();
}

void IQSLENS_get_state(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	pData[0] = Lens_GetBusyState();
}

void IQSLENS_get_zoom_focus_item(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();
	LENS_ZOOM_FOCUS_ITEM *sPtr = (LENS_ZOOM_FOCUS_ITEM *)pData;

	sPtr->uiTabIdx = lens_argu;

	Lens_GetZoomFocusTable(sPtr->uiTabIdx, &sPtr->ZoomFocusTab);
}

void IQSLENS_get_zoom_focus_tbl(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();
	LENS_ZOOM_FOCUS_TAB *sPtr = (LENS_ZOOM_FOCUS_TAB *)pData;

	Lens_GetZoomFocusTable(0xFFFF, sPtr);
}

void IQSLENS_get_reserved(void)
{
}


void IQSLENS_set_argu(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	lens_argu = pData[0];
}

void IQSLENS_set_focal_length(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	Lens_FocusSetFocalLength(pData[0], pData[1]);    // pData[0]: ZOOM_SECTION_WIDE or ZOOM_SECTION_TELE
}

void IQSLENS_set_zoom_init(void)
{
	Lens_ZoomSetFunc(LENS_ZOOM_INIT, 0);
}

void IQSLENS_set_zoom_speed(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 zoom_spd;

	zoom_spd = 20 * (pData[0] / 20);
	zoom_spd = (zoom_spd > 100) ? 100 : (zoom_spd < 20 ? 20 : zoom_spd);

	Lens_ZoomSetSpeed(zoom_spd);
}

void IQSLENS_set_zoom_section(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	Lens_ZoomSetFunc(LENS_ZOOM_GO2_SECTION, pData[0]);
}

void IQSLENS_set_zoom_pos(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();

	Lens_ZoomSetFunc(LENS_ZOOM_GO2_POSITION, pData[0]);
}

void IQSLENS_set_zoom_define_pos(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();
	INT32 def_dir;

	def_dir = (pData[1] >= 1) ? 1 : ((pData[1] <= -1) ? -1 : pData[1]);

	Lens_ZoomSetPosition(pData[0], def_dir);
}

void IQSLENS_set_zoom_move(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 move_dir;

	move_dir = (pData[0] >= 1) ? 1 : 0;

	if (move_dir == 1) {
		Lens_ZoomSetFunc(LENS_ZOOM_IN, 0);
	} else {
		Lens_ZoomSetFunc(LENS_ZOOM_OUT, 0);
	}
}

void IQSLENS_set_zoom_stop(void)
{
	Lens_ZoomStop();
}

void IQSLENS_set_focus_init(void)
{
	Lens_FocusSetFunc(LENS_FOCUS_INIT, 0);
}

void IQSLENS_set_focus_speed(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 focus_spd;

	focus_spd = 20 * (pData[0] / 20);
	focus_spd = (focus_spd > 100) ? 100 : (focus_spd < 20 ? 20 : focus_spd);

	Lens_FocusSetSpeed(focus_spd);
}

void IQSLENS_set_focus_pos(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();

	Lens_FocusSetFunc(LENS_FOCUS_GO2_POSITION, pData[0]);
}

void IQSLENS_set_focus_define_pos(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();
	INT32 def_dir;

	def_dir = (pData[1] >= 1) ? 1 : ((pData[1] <= -1) ? -1 : pData[1]);

	Lens_FocusSetPosition(pData[0], def_dir);
}

void IQSLENS_set_focus_move(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32 move_dir;

	move_dir = (pData[0] >= 1) ? 1 : 0;

	if (move_dir == 1) {
		Lens_FocusSetFunc(LENS_FOCUS_TELE, 0);
	} else {
		Lens_FocusSetFunc(LENS_FOCUS_WIDE, 0);
	}
}

void IQSLENS_set_focus_stop(void)
{
	Lens_FocusStop();
}

void IQSLENS_set_aperture_pos(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	Lens_ApertureSetAct(MOTOR_APERTURE_NORMAL, pData[0]);
}

void IQSLENS_set_shutter_state(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	Lens_ShutterSetAct(MOTOR_SHUTTER_NORMAL, pData[0]);
}

void IQSLENS_set_ircut_state(void)
{
	UINT32 *pData = (UINT32 *)MsdcNvt_GetDataBufferAddress();

	Lens_IRCUTSetAct((MOTOR_IRCUT_ACT)pData[0], pData[1]);
}

void IQSLENS_set_zoom_focus_item(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();
	LENS_ZOOM_FOCUS_ITEM *sPtr = (LENS_ZOOM_FOCUS_ITEM *)pData;

	Lens_SetZoomFocusTable(sPtr->uiTabIdx, &sPtr->ZoomFocusTab);
}

void IQSLENS_set_zoom_focus_tbl(void)
{
	INT32 *pData = (INT32 *)MsdcNvt_GetDataBufferAddress();
	LENS_ZOOM_FOCUS_TAB *sPtr = (LENS_ZOOM_FOCUS_TAB *)pData;

	Lens_SetZoomFocusTable(0xFFFF, sPtr);
}

void IQSLENS_set_reserved(void)
{
}

