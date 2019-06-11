/**
    Public header file for IPL command

    This file is the header file that define the API and data type for IPL command.

    @file       ipl_cmd.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_CMD_H_
#define _IPL_CMD_H_
#include "Type.h"
#include "ErrorNo.h"
#include "ipl_utility.h"
#include "ipl_cb_msg.h"
#include "ipl_isr_event.h"
#include "ipl_hal_sie_com_info.h"
#include "ipl_ctrl_flow.h"
#include "ipl_header_infor_yuv.h"
#include "sensor.h"

/**
    IPL ACT
*/
typedef enum _IPL_ACT {
	IPL_NONE = 0,
	IPL_START,
	IPL_STOP,
	IPL_PAUSE,
	IPL_RESTART,
	IPL_RESUME,
	IPL_OFF,
	ENUM_DUMMY4WORD(IPL_ACT)
} IPL_ACT;

typedef enum {
	IPL_CUR_ST_IDX = 0, ///<current  mode Information
	IPL_PRE_ST_IDX,     ///<previous mode Information
	IPL_MAX_ST_IDX,
	ENUM_DUMMY4WORD(IPL_MODE_IDX)
} IPL_MODE_IDX;

/***********************************************************/
/*                  IPL set command                        */
/***********************************************************/

/** \addtogroup mIIPL */
//@{

/**
    IPL SET Command(IPL_SetCmd())
*/
typedef enum _IPL_SET_CMD {
	IPL_SET_CHGMODE = 0,       ///< change mode command, data type IPL_MODE_DATA
	IPL_SET_BUF_QUERY,         ///< query ipl buffer size, data type IPL_BUF_QUERY_DATA
	IPL_SET_IMGINOUT,          ///< change IMG IN/OUT settings command, data type IPL_SET_IMGINOUT_DATA
	IPL_SET_DZOOM,             ///< change digital idx command, data type IPL_DZOOM
	IPL_SET_DZOOM_WAIT_END,    ///< wait digital zoom finish, data type IPL_DZOOM
	IPL_SET_SLEEP,             ///< enter sleep mode command, data type IPL_SLEEP_INFO
	IPL_SET_WAKEUP,            ///< exit sleep mode command, data type IPL_WAKEUP_INFO
	IPL_SET_GET_CAP_RAW,       ///< get cap raw command, data type IPL_GET_CAP_RAW_DATA
	IPL_SET_PAUSEDMA,          ///< pause all IPL dma, for system before change IC frequency, data type IPL_PROC_ID
	IPL_SET_RESUMEDMA,         ///< resume all IPL dma, for system after change IC frequency, data type IPL_PROC_ID
	IPL_SET_VA,                ///< set VA settings command, data type IPL_SET_VA_DATA
	IPL_SET_TRIG_VIRTUAL,      ///< set virtual ipl trigger command, data type IPL_TRIG_VIRTUAL_DATA
	IPL_SET_TRIG_IME_OUT,      ///< set trigger ime dma out command, data type IPL_TRIG_IME_DMA
	IPL_SET_IN_IMG_OFFSET,     ///< set input image offset, data type IPL_IN_IMG_OFS
	IPL_SET_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(IPL_SET_CMD)
} IPL_SET_CMD;

#define IPL_SET_SWITCH_ALL  0xffffffff
#define IPL_SET_CMD_SWITCH_TAB  32      ///< maxinum value of IPL SET command

//check IPL_SET_CMD range
STATIC_ASSERT((IPL_SET_MAX_CNT_DEF <= IPL_SET_CMD_SWITCH_TAB));


/********* IPL_CMD_CHGMODE *********/
/**
    IPL MODE
*/
typedef enum _IPL_MODE {
	IPL_MODE_OFF = 0,       ///< IPL off mode.
	IPL_MODE_PREVIEW,       ///< IPL photo mode.
	IPL_MODE_VIDEO,         ///< IPL video mode.
	IPL_MODE_VIDEOREC,      ///< IPL video recording mode.
	IPL_MODE_CAP,           ///< IPL capture mode.
	IPL_MODE_NUM,
	IPL_MODE_BUF_QUERY = IPL_MODE_NUM + 1,
	IPL_MODE_NONE = 0xffffffff,
	ENUM_DUMMY4WORD(IPL_MODE)
} IPL_MODE;

/**
    IPL ext ctrl flag, IPL_SET_CHGMODE command
*/
typedef enum _IPL_EXT_CTRL_EN {
	IPL_EXT_CTRL_NONE           = 0x00000000,       ///< none.
	IPL_EXT_CTRL_RAW_DIRECT		= 0x00000001,       ///< set raw direct to ipl(flow2), reduce raw buffer
	IPL_EXT_CTRL_TMNR_SHARE_BUF	= 0x00000002,       ///< set tmnr reference buffer share with ime path1
	ENUM_DUMMY4WORD(IPL_EXT_CTRL_EN)
} IPL_EXT_CTRL_EN;

/**
    IPL post process function, IPL_SET_CHGMODE command
*/
typedef enum _IPL_POST_PROC_FUNC_EN {
	IPL_FUNC_NONE           = 0x00000000,       ///< disable post process function.
	IPL_FUNC_VA             = 0x00000001,       ///< allocate IPE va output buffer. for AF using
	IPL_FUNC_ETH            = 0x00000002,       ///< allocate IPE Eth output buffer. for DIS or ADAS using
	IPL_FUNC_WDR            = 0x00000004,       ///< prepare WDR function.  conflict with IPL_FUNC_SHDR and IPL_FUNC_STITCH
	IPL_FUNC_SHDR           = 0x00000008,       ///< prepare Sensor HDR function. conflict with IPL_FUNC_WDR and IPL_FUNC_STITCH
	IPL_FUNC_STITCH         = 0x00000010,       ///< prepare Image Stich buffer.  conflict with IPL_FUNC_WDR and IPL_FUNC_SHDR
	IPL_FUNC_ADAS           = 0x00000020,       ///< prepare Lane Departure Warning System/Foward Collision Warning System working buffer
	IPL_FUNC_DEFOG          = 0x00000040,       ///< prepare defog buffer.  conflict with IPL_FUNC_STITCH
	IPL_FUNC_PRIMACY_MASK   = 0x00000080,       ///< prepare Primacy mask buffer.
	IPL_FUNC_IME_TMNR       = 0x00000100,       ///< prepare IME SQUARE function output buffer.
	IPL_FUNC_MD             = 0x00000200,       ///< prepare motion detection output buffer
	IPL_FUNC_RAW2YUV_FLOW   = 0x00000400,       ///< prepare RAW2YUV flow
	ENUM_DUMMY4WORD(IPL_POST_PROC_FUNC_EN)
} IPL_POST_PROC_FUNC_EN;

/**
    IPL ime path information
    For IPL MODE_PREVIEW/IPL_MODE_VIDEO/IPL_MODE_VIDEOREC/IPL_MODE_OFF.

*/
typedef struct _IPL_IME_PATH_INFO {
	UINT32 output_en;             ///<path output enable/disable
	UINT32 sample_rate;           ///< sample rate of each ime path(DMA Enable control)(dst_fps:bit[31:16], src_fps:bit[15:0])
	///< sample rate (range: 1~IPL_SAMPLE_RATE_LIMIT)
	///< IPL_SAMPLE_RATE_ZERO:    never enable
	///< IPL_SAMPLE_RATE_SINGLE:  enable once(for single mode)
	///< IPL_SAMPLE_RATE_STREAM:  trigger for each frame
	UINT32 scale_size_h;          ///< Scale H size
	UINT32 scale_size_v;          ///< Scale V size
	IPL_YUV_IMG_TYPE img_fmt;     ///< ime output format
	UINT32 out_crop_start_x;      ///< ime output crop start x, out_crop_start_x+out_crop_size_h must be <= scale_size_h
	UINT32 out_crop_start_y;      ///< ime output crop start y, out_crop_start_y+out_crop_size_v must be <= scale_size_v
	UINT32 out_crop_size_h;       ///< ime output size H, out_crop_size_h x out_crop_size_v must be <= buf_size_h x buf_size_v
	UINT32 out_crop_size_v;       ///< ime output size V, out_crop_size_h x out_crop_size_v must be <= buf_size_h x buf_size_v
	UINT32 out_img_lofs;          ///< output image line offset
	BOOL out_ycc_compress_en;     ///< ycc compress enable/disable, only for ime path1
	BOOL md_evt_out_en;           ///< motion detection event output enable/disable
} IPL_IME_PATH_INFO;


/**
    IPL IME out image position arrange parameter

    For IPL_CMD use to set output postion

*/
typedef struct _IPL_IME_OUT_GROUP {
	BOOL group_en;
	UINT32 group_idx;   //group index
	UINT32 win_num;     //group window number NXM (win_num_h:bit[31:16], win_num_v:bit[15:0])
	UINT32 out_img_pos; //output image position (position horizontal:bit[31:16], position vertical:bit[15:0])
} IPL_IME_OUT_GROUP;

/**
    Query IPL buffer size parameter

    Data structure for IPL CMD!
*/
typedef struct _IPL_BUF_QUERY_DATA {
	UINT32 proc_sen_id;					///< assign ipl Id
	UINT32 res;
	UINT32 raw_out_buf_num;             ///< sie ch0 raw output buffer number, minimum 2 in normal mode
	IPL_POST_PROC_FUNC_EN func_en;      ///< notify IPL post process enable & prepare buffer
	IPL_EXT_CTRL_EN ext_ctrl_en;		///< ipl external control enable
	struct _IPL_BUF_QUERY_DATA *pnext;  ///< Link to next property description. (Fill NULL if this is last item)
} IPL_BUF_QUERY_DATA;

/**
    IPL change mode parameter

    Data structure for IPL CMD use!
*/
typedef struct _IPL_MODE_DATA {
	UINT32 proc_sen_id;             ///< assign ipl Id
	IPL_MODE mode;                  ///< next ipl mode

	UINT32 buf_addr;                ///< input buffer start address
	UINT32 buf_size;                ///< input buffer size
	UINT32 crop_ratio;              ///< sie crop ratio (H_ratio:bit[31:16], V_ratio:bit[15:0])
	UINT32 raw_out_buf_num;         ///< sie ch0 raw output buffer number, minimum 2 in normal mode
	FLIP_TYPE flip_info;			///< flip setting

	IPL_IME_YUV_TYPE  ime_yuv_type;				   ///< ime output image yuv type
	UINT32 			  ime_scale_ratio_thr;		   ///< ime scale ratio threshold [31:16]/[15:0](src:bit[31:16], div:bit[15:0])
	IPL_IME_SCALER 	  ime_scaler_over;   		   ///< force use this scale method when ime in/out ratio over scale_ratio_thr
	IPL_IME_SCALER 	  ime_scaler_under;  		   ///< force use this scale method when ime in/out ratio equal or under scale_ratio_thr
	IPL_IME_PATH_INFO ime_path_1;                  ///< ime path1  information
	IPL_IME_OUT_GROUP ime_path1_out_group;         ///< ime path1 output gropu infor
	IPL_IME_PATH_INFO ime_path_2;                  ///< ime path2  information
	IPL_IME_OUT_GROUP ime_path2_out_group;         ///< ime path2 output gropu infor
	IPL_IME_PATH_INFO ime_path_3;                  ///< ime path3  information
	IPL_IME_OUT_GROUP ime_path3_out_group;         ///< ime path3 output gropu infor
	IPL_IME_PATH_INFO ime_path_4;                  ///< ime path4  information, Y only
	IPL_IME_OUT_GROUP ime_path4_out_group;         ///< ime path4 output gropu infor

	IPL_CBMSG_FP cb_fp;                             ///< flow callback function
	SIE_RAW_POSTPROC_CB raw_postproc_cb_fp;         ///< raw post process callback function
	IPL_DATASTAMP_CB    datastamp_cb_fp;            ///< data stamp callback function
	IPL_PM_CB pm_cb_fp;                           	///< privacy mask callback function
	IPL_MD_CB md_cb_fp;								///< motion detection module callback function
	IPL_OUT_BUF_CFG_CB out_buf_cfg_cb_fp;           ///< get ipl output address callback function
	IPL_ISR_EVENT_FP isr_cb_fp[IPL_ISR_MAX_CNT];    ///< ISR CB for photo mode

	IPL_POST_PROC_FUNC_EN func_en;                  ///< notify IPL post process enable & prepare buffer
	IPL_EXT_CTRL_EN ext_ctrl_en;					///< ipl external control enable
	UINT32 event_id;                                ///< only valid when in capture mode, notify IPL current still event.  BOOL act_flg;                                    ///<
	BOOL act_flg;                                   ///< reserved
	struct _IPL_MODE_DATA *pnext;                   ///< Link to next property description. (Fill NULL if this is last item)
} IPL_MODE_DATA;

/**
    IPL mode parameter

    Data structure for change to IPL_MODE_CAP.
*/
typedef struct _IPL_MODE_DATA_CAP {
	UINT32 proc_sen_id;                             ///< assign sensor Id
	IPL_MODE mode;                                  ///< next mode

	UINT32 buf_addr;
	UINT32 buf_size;

	UINT32 crop_ratio;                              ///< sie crop ratio (H:bit[31:16], V:bit[15:0])
	IPL_POST_PROC_FUNC_EN func_en;                  ///< notify IPL post process enable & prepare buffer
	UINT32 event_id;                                ///< notify IPL current still event.

	struct _IPL_MODE_DATA_CAP *pnext;                ///< Link to next property description. (Fill NULL if this is last item)
} IPL_MODE_DATA_CAP;

/********* IPL_SET_SETDZOOM *********/

/**
    IPL digital zoom parameter

    Data structure for IPL_SET_SETDZOOM command operation.
*/
typedef struct _IPL_DZOOM {
	IPL_PROC_ID id;             ///< ipl process id
	UINT32 idx;                 ///< zoom index
	struct _IPL_DZOOM *pnext;    ///< Link to next property description. (Fill NULL if this is last item)
} IPL_DZOOM;

/********* IPL_SET_DZOOM_WAIT_END *********/

/**
    IPL digital zoom wait end parameter

    Data structure for IPL_SET_DZOOM_WAIT_END command operation.
*/
typedef struct _IPL_DZOOM_WAIT_END {
	IPL_PROC_ID id;                     ///< ipl process id
	struct _IPL_DZOOM_WAIT_END *pnext;   ///< Link to next property description. (Fill NULL if this is last item)
} IPL_DZOOM_WAIT_END;

/********* IPL_SET_PAUSEDMA *********/
/**
    IPL pause dma command parameter

    Data structure for IPL_SET_PAUSEDMA command operation.
*/
typedef struct _IPL_PAUSEDMA_INFO {
	IPL_PROC_ID id;                     ///< ipl process id
	struct _IPL_PAUSEDMA_INFO *pnext;    ///< Link to next property description. (Fill NULL if this is last item)
} IPL_PAUSEDMA_INFO;


/********* IPL_SET_RESUMEDMA *********/
/**
    IPL Resume dma command parameter

    Data structure for IPL_SET_RESUMEDMA command operation.
*/
typedef struct _IPL_RESUMEDMA_INFO {
	IPL_PROC_ID id;                     ///< ipl process id
	struct _IPL_RESUMEDMA_INFO *pnext;   ///< Link to next property description. (Fill NULL if this is last item)
} IPL_RESUMEDMA_INFO;

/********* IPL_SET_SLEEP *********/
/**
    IPL sleep command parameter

    Data structure for IPL_SET_SLEEP command operation.
*/
typedef struct _IPL_SLEEP_INFO {
	IPL_PROC_ID id;                 ///< ipl process id
	struct _IPL_SLEEP_INFO *pnext;   ///< Link to next property description. (Fill NULL if this is last item)
} IPL_SLEEP_INFO;

/********* IPL_SET_WAKEUP *********/
/**
    IPL WAKEUP command parameter

    Data structure for IPL_SET_WAKEUP command operation.
*/
typedef struct _IPL_WAKEUP_INFO {
	IPL_PROC_ID id;                 ///< ipl process id
	struct _IPL_WAKEUP_INFO *pnext;  ///< Link to next property description. (Fill NULL if this is last item)
} IPL_WAKEUP_INFO;

/********* IPL_SET_IME *********/

/**
    IPL ime path id
*/
typedef enum _IPL_IME_PATH {
	IPL_IME_PATH_NONE = 0x00000000,     ///< path null
	IPL_IME_PATH1 = 0x00000001,         ///< path id 1
	IPL_IME_PATH2 = 0x00000002,         ///< path id 2
	IPL_IME_PATH3 = 0x00000004,         ///< path id 3
	IPL_IME_PATH4 = 0x00000008,         ///< path id 4
	IPL_IME_PATH5 = 0x00000010,         ///< path id 5
	ENUM_DUMMY4WORD(IPL_IME_PATH)
} IPL_IME_PATH;


/********* IPL_SET_GET_CAP_RAW *********/
/**
    IPL get raw yout information
*/
typedef struct _IPL_GET_CAP_RAW_YOUT_INFO {
	UINT32 yout_x_winnum;         ///< yout x window number
	UINT32 yout_y_winnum;         ///< yout y window number
	UINT32 win_size_x;            ///< yout window size_x, calculated by sie driver
	UINT32 win_size_y;            ///< yout window size_y, calculated by sie driver
} IPL_GET_CAP_RAW_YOUT_INFO;
/********* IPL_SET_GET_CAP_RAW *********/
/**
    IPL get raw information
*/
typedef struct _IPL_GET_CAP_RAW_INFO {
	IPL_GET_CAP_RAW_YOUT_INFO yout_info;  ///< yout information
} IPL_GET_CAP_RAW_INFO;
/********* IPL_SET_GET_CAP_RAW *********/
/**
    IPL get raw information
    Data structure for IPL_SET_GET_CAP_RAW command
*/
typedef struct _IPL_GET_CAP_RAW_DATA {
	IPL_PROC_ID id;               ///< ipl process id
	FLIP_TYPE flip_info;          ///< flip information
	UINT32 out_ch0_en;            ///< sie channel 0 output enable/disable
	UINT32 buf_addr_ch0;          ///< sie channel 0 output buffer address
	UINT32 out_ch3_subraw_en;     ///< sie channel 3 outout enable/disable
	UINT32 out_ch3_yout_en;       ///< sie channel 3 outout enable/disable
	UINT32 buf_addr_ch3;          ///< sie channel 3 output buffer address
	UINT32 out_ch5_en;            ///< sie channel 5 output enable/disable
	UINT32 buf_addr_ch5;          ///< sie channel 5 output buffer address
	UINT32 is_dark_frame;         ///< notify get raw flow
	UINT32 buf_addr_ch0_1;        ///< sie channel 0 output buffer address
	UINT32 buf_addr_ch3_1;        ///< sie channel 2 output buffer address
	UINT32 out_ca_en;             ///< sie ca output enable/disable
	UINT32 buf_addr_ca;           ///< sie ca output buffer address
	UINT32 res[10];
	UINT32 frm_ctrl;			  ///< sie frame rate ctrl,if value=0, no use
	IPL_GET_CAP_RAW_INFO *praw_info; ///< raw information
	struct _IPL_GET_CAP_RAW_DATA *pnext;    ///< Link to next property description. (Fill NULL if this is last item)
} IPL_GET_CAP_RAW_DATA;

/********* IPL_SET_IMGINOUT *********/

/**
    IPL set image input/output information
    Data structure for IPL_SET_IMGINOUT command
*/
typedef struct _IPL_SET_IMGINOUT_DATA {
	IPL_PROC_ID id;                         ///< ipl process id
	UINT32 crop_ratio;                      ///< sie crop ratio (H:bit[31:16], V:bit[15:0]), IGNORE it crop_ratio = 0
	FLIP_TYPE flip_info;                    ///< flip information
	IPL_IME_PATH path_id;                   ///< select ime path
	IPL_IME_PATH_INFO ime_path_1;           ///< ime path1 information, only valid when path_id & path1 == 1
	IPL_IME_PATH_INFO ime_path_2;           ///< ime path2 information, only valid when path_id & path2 == 1
	IPL_IME_PATH_INFO ime_path_3;           ///< ime path3 information, only valid when path_id & path3 == 1
	IPL_IME_PATH_INFO ime_path_4;           ///< ime path4 information, only valid when path_id & path4 == 1
	IPL_IME_PATH_INFO ime_path_5;           ///< ime path5 information, only valid when path_id & path5 == 1
	struct _IPL_SET_IMGINOUT_DATA *pnext;   ///< Link to next property description. (Fill NULL if this is last item)
} IPL_SET_IMGINOUT_DATA;

/********* IPL_SET_VA *********/

/**
    IPL set va information
    For IPL_SET_VA command
*/
typedef struct _IPL_SET_VA_DATA{
	IPL_PROC_ID id;                 		///< ipl process id
	BOOL va_out_en;             			///< va output enable/disable
	IPE_HAL_VA_OUTSEL            VAOutSel;  ///< VA output selection
	IPE_HAL_VA_FLTR_GROUP_PARAM  VaFltrG1; ///< VA Group 1 filter parameters
	IPE_HAL_VA_FLTR_GROUP_PARAM  VaFltrG2; ///< VA Group 2 filter parameters
	IPE_HAL_VA_WIN_PARAM         va_win;    ///< VA window parameters
	IPE_HAL_INDEP_VA_PARAM       va_indep_win[5];   ///< Independent VA window parameters
} IPL_SET_VA_DATA;

/********* IPL_SET_TRIG_VIRTUAL *********/

/**
    IPL trigger virtual ipl data
    For IPL_SET_TRIG_VIRTUAL command
*/
typedef struct {
	IPL_PROC_ID id;                 ///< ipl process id
	IPL_RAW_IMG raw_info;           ///< input raw image information
	UINT32 header_addr;             ///< raw header buffer, get by IPL_HeaderRawGetBufSize
} IPL_TRIG_VIRTUAL_DATA;

/********* IPL_SET_TRIG_IME_OUT *********/

/**
    IPL trigger ime dma out
    For IPL_SET_TRIG_IME_OUT command
*/
typedef struct {
	IPL_PROC_ID id;                 ///< ipl process id
	IPL_IME_PATH_ID path_id;        ///< ime path id
} IPL_TRIG_IME_DMA;

/**
    Input Image offset
    For IPL_SET_IN_IMG_OFFSET command
*/
typedef struct {
	IPL_PROC_ID id;			///< ipl process id
	IPOINT   	in_ofs;		///< input image offset (unit: pixel)
} IPL_IN_IMG_OFS;

/********* IPL_SET_CMD *********/
typedef union {
	IPL_MODE_DATA 			chg_mode;
	IPL_BUF_QUERY_DATA 		buf_query_data;
	IPL_MODE_DATA_CAP 		cap_info;
	IPL_SET_IMGINOUT_DATA 	img_in_out_data;
	IPL_DZOOM 				dzoom_info;
	IPL_DZOOM_WAIT_END 		dzoom_wait_end;
	IPL_SLEEP_INFO 			sleep_info;
	IPL_WAKEUP_INFO 		wake_info;
	IPL_GET_CAP_RAW_DATA 	get_raw_data;
	IPL_PAUSEDMA_INFO 		pause_info;
	IPL_RESUMEDMA_INFO 		resume_info;
	IPL_SET_VA_DATA 		va_info;
	IPL_TRIG_VIRTUAL_DATA 	trig_virtual_data;
	IPL_TRIG_IME_DMA 		trig_ime_dma_out_data;
	IPL_IN_IMG_OFS 			in_img_ofs;
} IPL_SET_CMD_DATA;

/**
     IPL SET command

     @note only one command can be executed each time

     @param[in] cmd       ipl command (IPL_SET_CMD and user define command)
     @param[in] data      ipl command data

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_SetCmd(UINT32 cmd, void *data);

/**
     wait ipl SET command finish

     @note only valid for SET command
*/
extern void IPL_WaitCmdFinish(void);
/***********************************************************/
/*                  IPL get command                        */
/***********************************************************/

/**
    IPL GET Command
*/
typedef enum _IPL_GET_CMD {
	IPL_GET_CUR_MODE = 0,               ///< get current mode command, data type IPL_MODE_INFOR
	IPL_GET_CUR_BUF_INFOR,              ///< get current buffer information, data type IPL_BUF_INFOR
	IPL_GET_IME_INFOR,                  ///< get IME information, data IPL_IME_INFOR
	IPL_GET_SIE_CUR_CAP_CH0_RAW_INFOR,  ///< get SIE channel 0 current output size, data type IPL_SIE_RAW_INFO
	IPL_GET_SIE_CUR_CAP_CH3_RAW_INFOR,  ///< get SIE channel 3 current output size, data type IPL_SIE_RAW_INFO
	IPL_GET_CAP_INFOR,                  ///< get capture information data type IPL_CAP_INFOR
	IPL_GET_CAP_ENC_RST_INFOR,          ///< get image raw encode information, data type IPL_ENC_RST_INFOR
	IPL_GET_DZ_CUR_INFOR,               ///< get current digital zoom information, data type IPL_DZOOM_INFOR
	IPL_GET_CA_CUR_INFOR,               ///< get current image CA information, data type IPL_CA_INFOR
	IPL_GET_CA_SET_INFOR,               ///< get CA setting information, data type IPL_CA_SET_INFOR
	IPL_GET_LA_CUR_INFOR,               ///< get current image LA information, data type IPL_LA_INFOR
	IPL_GET_LA_SET_INFOR,               ///< get LA setting information, data type IPL_LA_SET_INFOR
	IPL_GET_VA_CUR_INFOR,               ///< get current image VA information, data type IPL_VA_INFOR
	IPL_GET_VA_SET_INFOR,               ///< get VA setting information, data type IPL_VA_SET_INFOR
	IPL_GET_SIE_CUR_FRAME_INFOR,        ///< get SIE current frame information, data type IPL_SIE_CUR_FRAME_INFO
	IPL_GET_CAL_IPL_IN_SIZE,      		///< cal IPL input size inforamtion, only can call this cmd when ipl is off mode, data type IPL_IN_SIZE_INFO
	IPL_GET_IPL_CUR_INPUT_INFOR,        ///< get IPL current input frame information, data type IPL_CUR_INPUT_INFO
	IPL_GET_SIE_CUR_CAP_CA_INFOR,   	///< get SIE channel 1 CA INFORMATION, data type IPL_SIE_RAW_INFO
	IPL_GET_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(IPL_GET_CMD)
} IPL_GET_CMD;

#define IPL_GET_SWITCH_ALL  0xffffffff
#define IPL_GET_CMD_SWITCH_TAB  32      ///< maxinum value of IPL GET command

//check IPL_GET_CMD range
STATIC_ASSERT((IPL_GET_MAX_CNT_DEF <= IPL_GET_CMD_SWITCH_TAB));

/********* IPL_VA_INFOR *********/
/**
    IPL va window id
*/
typedef enum _VA_WINIDX {
	VA_WIN_16X16 = 0,               ///< va 16 * 16 window
	VA_WIN_INDP0,                   ///< va independent window 0
	VA_WIN_INDP1,                   ///< va independent window 1
	VA_WIN_INDP2,                   ///< va independent window 2
	VA_WIN_INDP3,                   ///< va independent window 3
	VA_WIN_INDP4,                   ///< va independent window 4
	ENUM_DUMMY4WORD(VA_WINIDX)
} VA_WINIDX;

typedef struct {
	UINT32* p_g1h;			///< output, uiWinNmX*uiWinNmY@16bits
	UINT32* p_g1v;			///< output, uiWinNmX*uiWinNmY@16bits
	UINT32* p_g1hcnt;		///< output, uiWinNmX*uiWinNmY@16bits
	UINT32* p_g1vcnt;		///< output, uiWinNmX*uiWinNmY@16bits
	UINT32* p_g2h;			///< output, uiWinNmX*uiWinNmY@16bits
	UINT32* p_g2v;			///< output, uiWinNmX*uiWinNmY@16bits
	UINT32* p_g2hcnt;		///< output, uiWinNmX*uiWinNmY@16bits
	UINT32* p_g2vcnt;		///< output, uiWinNmX*uiWinNmY@16bits
} IPL_VA_GET_RST;


/**
    IPL current va information
*/
typedef struct _IPL_VA_SET_INFOR {
	IPL_PROC_ID id;                 			///< input parameter, ipl process id
	IPE_HAL_VA_WIN_PARAM         *va_window;    ///< VA window parameters
	IPE_HAL_INDEP_VA_PARAM       *va_indep_win; ///< Independent VA window parameters
} IPL_VA_SET_INFOR;

/**
    IPL current va information
*/
typedef struct _IPL_VA_INFOR {
	IPL_PROC_ID id;                 ///< input parameter, ipl process id
	IPE_HAL_INDEP_VA_WIN_RSLT *va_indep_win_rst;
	IPL_VA_GET_RST *va_rst;
} IPL_VA_INFOR;

/********* IPL_CA_INFOR *********/
/**
    IPL current ca information
*/
typedef struct _IPL_CA_INFOR {
	IPL_PROC_ID id;             ///< input parameter, ipl process id
	UINT16 *r;                  ///< output parameter, need assign memory, ca_win_x*ca_win_y@16bits
	UINT16 *g;                  ///< output parameter, need assign memory, ca_win_x*ca_win_y@16bits
	UINT16 *b;                  ///< output parameter, need assign memory, ca_win_x*ca_win_y@16bits
	UINT16 *ir;                 ///< output parameter, need assign memory, ca_win_x*ca_win_y@16bits
	UINT16 *r_th;               ///< output parameter, need assign memory, ca_win_x*ca_win_y@16bits
	UINT16 *g_th;               ///< output parameter, need assign memory, ca_win_x*ca_win_y@16bits
	UINT16 *b_th;               ///< output parameter, need assign memory, ca_win_x*ca_win_y@16bits
	UINT16 *acc_cnt;            ///< output parameter, need assign memory, ca_win_x*ca_win_y@16bits
	UINT32 length;              ///< input array length for *R, *G, *B
	struct IPL_CA_INFOR *pnext; ///< Link to next property description. (Fill NULL if this is last item)
} IPL_CA_INFOR;

/********* IPL_CA_SET_INFOR *********/

/**
    IPL ca setting information
*/
typedef struct _IPL_CA_SET_INFOR {
	IPL_PROC_ID id;                 ///<input parameter, ipl process id
	UINT32 win_x_num;                 ///<output parameter, the number of the horizontal window
	UINT32 win_y_num;                 ///<output parameter, the number of the vertical window
	UINT32 crop_size_h;               ///<output parameter, CA crop H size
	UINT32 crop_size_v;               ///<output parameter, CA corp V size
	struct IPL_CA_SET_INFOR *pnext; ///< Link to next property description. (Fill NULL if this is last item)
} IPL_CA_SET_INFOR;

/********* IPL_LA_INFOR *********/
/**
    IPL current lca information
*/
typedef struct _IPL_LA_INFOR {
	IPL_PROC_ID id;             ///< input parameter, ipl process id
	UINT16 *lum1;               ///< 12 bit Pre  Gamma luminance, LA_WIN_X*LA_WIN_Y@12bits
	UINT16 *lum2;               ///< 10 bit Post Gamma luminance, LA_WIN_X*LA_WIN_Y@10bits
	UINT16 *histo;              ///< output: historgram, 64bin@12bits
	struct IPL_LA_INFOR *pnext; ///< Link to next property description. (Fill NULL if this is last item)
} IPL_LA_INFOR;

/********* IPL_LA_SET_INFOR *********/

/**
    IPL la setting information
*/
typedef struct _IPL_LA_SET_INFOR {
	IPL_PROC_ID id;                 ///<input parameter, ipl process id
	UINT32 win_x_num;                 ///<output parameter, the number of the horizontal window
	UINT32 win_y_num;                 ///<output parameter, the number of the vertical window
	UINT32 crop_size_h;               ///<output parameter, LA crop H size
	UINT32 crop_size_v;               ///<output parameter, LA corp V size
	struct IPL_LA_SET_INFOR *pnext; ///< Link to next property description. (Fill NULL if this is last item)
} IPL_LA_SET_INFOR;

/********* IPL_GET_CUR_DZ_INFOR *********/

/**
    IPL current digital zoom information
*/
typedef struct _IPL_DZOOM_INFOR {
	IPL_PROC_ID id;                 ///<input parameter, ipl process id
	UINT32 cur_idx;                  ///<output parameter, digital zoom current index
	UINT32 max_idx;                  ///<output parameter, digital zoom maxinum index
	UINT32 ratio;                   ///<output parameter, digital zoom ratio
	struct IPL_DZOOM_INFOR *pnext;  ///< Link to next property description. (Fill NULL if this is last item)
} IPL_DZOOM_INFOR;

/********* IPL_GET_CAP_ENC_RST_INFOR *********/

/**
    IPL rde information(only valid when in capture mode)
*/
typedef struct _IPL_ENC_RST_INFOR {
	IPL_PROC_ID id;                         ///<input parameter, ipl process id
	struct {
		UINT32            ui_output_size;     ///< Raw encoder - accumulated output byte count
		UINT32            ui_ovfl_line;
		UINT16            ui_brc_lncnt_lfn[8];
	} sie_enc_rslt;
	struct IPL_ENC_RST_INFOR *pnext;        ///< Link to next property description. (Fill NULL if this is last item)
} IPL_ENC_RST_INFOR;

/********* IPL_CAP_INFOR *********/

/**
    IPL capture information(only valid when in capture mode)
*/
typedef struct _IPL_CAP_INFOR {
	IPL_PROC_ID id;             ///<input parameter, ipl process id
	UINT32 ime_sub_ratio;         ///<output parameter, ipe subout ratio(%)
	UINT32 df_en;                ///<output parameter, dark frame function enable/disable
	UINT32 lca_en;               ///<output parameter, LCA function enable/disable
	struct IPL_CAP_INFOR *pnext;///< Link to next property description. (Fill NULL if this is last item)
} IPL_CAP_INFOR;

/**
    IPL capture CA information(only valid when in capture mode)
*/
typedef struct _IPL_CAP_CA_INFOR {
	IPL_PROC_ID id;              ///<input parameter, ipl process id
	BOOL func_en;				 ///<ca function enable
	UINT32 win_x_num;			 ///<ca x window number
	UINT32 win_y_num; 			 ///<ca y window number
	struct IPL_CAP_INFOR *pnext;///< Link to next property description. (Fill NULL if this is last item)
} IPL_CAP_CA_INFOR;

/********* IPL_GET_CUR_MODE *********/

/**
    IPL current mode information
*/
typedef struct _IPL_MODE_INFOR {
	IPL_PROC_ID id;              ///<input parameter, ipl process id
	IPL_MODE mode;               ///<output parameter, current mode
	struct _IPL_MODE_INFOR *pnext;///< Link to next property description. (Fill NULL if this is last item)
} IPL_MODE_INFOR;


/********* IPL_GET_CUR_BUF_INFO *********/

/**
    IPL current buffer information
*/
typedef struct _IPL_BUF_INFOR {
	IPL_PROC_ID id;             ///<input parameter, ipl process id
	UINT32 buf_addr;             ///<output parameter, current ipl buffer address
	UINT32 buf_size;             ///<output parameter, current ipl buffer size
} IPL_BUF_INFOR;


/********* IPL_GET_IME_INFOR *********/

/**
    IPL ime information
*/
typedef struct _IPL_IME_INFOR {
	IPL_PROC_ID id;                 ///<input parameter, ipl process id
	UINT32 p1_num;                   ///<output parameter, path1 buffer number
	UINT32 p1_enable;                ///<output parameter, path1 enable/disable
	IPL_YCC_IMG_INFO ime_path_1;     ///<output parameter, path1 image setting(format, buffer address, size)
	struct {
		Coordinate   start;                 ///< crop start
		UINT32       size_h;                 ///< crop size H
		UINT32       size_v;                 ///< crop size V
	} ime_path_1_crop;

	UINT32 p2_num;                   ///<output parameter, path2 buffer number
	UINT32 p2_enable;                ///<output parameter, path2 enable/disable
	IPL_YCC_IMG_INFO ime_path_2;     ///<output parameter, path2 image setting(format, buffer address, size)
	struct {
		Coordinate   start;                 ///< crop start
		UINT32       size_h;                 ///< crop size H
		UINT32       size_v;                 ///< crop size V
	} ime_path_2_crop;

	UINT32 p3_num;                   ///<output parameter, path3 buffer number
	UINT32 p3_enable;                ///<output parameter, path3 enable/disable
	IPL_YCC_IMG_INFO ime_path_3;     ///<output parameter, path3 image setting(format, buffer address, size)
	struct {
		Coordinate   start;                 ///< crop start
		UINT32       size_h;                 ///< crop size H
		UINT32       size_v;                 ///< crop size V
	} ime_path_3_crop;

	UINT32 p4_num;                   ///<output parameter, path4 buffer number
	UINT32 p4_enable;                ///<output parameter, path4 enable/disable
	IPL_YCC_IMG_INFO ime_path_4;     ///<output parameter, path4 image setting(format, buffer address, size)
	struct {
		Coordinate   start;                 ///< crop start
		UINT32       size_h;                 ///< crop size H
		UINT32       size_v;                 ///< crop size V
	} ime_path_4_crop;

	UINT32 p5_num;                   ///<output parameter, path4 buffer number
	UINT32 p5_enable;                ///<output parameter, path4 enable/disable
	IPL_YCC_IMG_INFO ime_path_5;     ///<output parameter, path4 image setting(format, buffer address, size)
	struct {
		Coordinate   start;                 ///< crop start
		UINT32       size_h;                 ///< crop size H
		UINT32       size_v;                 ///< crop size V
	} ime_path_5_crop;
	struct IPL_IME_INFOR *pnext;            ///< Link to next property description. (Fill NULL if this is last item)
} IPL_IME_INFOR;

/********* IPL_SIE_RAW_SIZE *********/

/**
    IPL raw information
*/
typedef struct _IPL_SIE_RAW_INFO {
	IPL_PROC_ID id;                  ///<input parameter, ipl process id
	UINT32 img_width;                ///<output parameter, raw width
	UINT32 img_height;               ///<output parameter, raw height
	UINT32 img_line_ofs;             ///<output parameter, raw line offset
	IPL_RAW_BIT img_bit;             ///<output parameter, raw bit
	IPL_RAW_PIX img_pix;             ///<output parameter, raw start pixel
	UINT32 frm_bit;                  ///<output HDR prod id
	UINT32 sensor_type;              ///<output parameter, raw start pixel
	UINT32 sensor_data_fmt;          ///<output sensor datafmt(BAYER_FORMAT,RGBIR_FORMAT,RCCB_FORMAT)
	UINT32 sensor_total_frm;         ///<output sensor HDR total frame
	struct IPL_SIE_RAW_INFO *pnext;  ///< Link to next property description. (Fill NULL if this is last item)
} IPL_SIE_RAW_INFO;

/********* IPL_SIE_RAW_SIZE *********/

/**
    IPL SIE frame cnt infomation
*/
typedef struct _IPL_SIE_CUR_FRAME_INFO {
	IPL_PROC_ID id;                         ///<input parameter, ipl process id
	BOOL   out_direct_en;					///<output parameter, ch0 output direct mode enable
	UINT32 frame_cnt;                       ///<output parameter, frame cnt
	UINT32 out_size_h;                      ///<output parameter, sie out size H
	UINT32 out_size_v;                      ///<output parameter, sie out size V
	UINT32 out_size_lofs;                   ///<output parameter, sie out size line ofs
	struct IPL_SIE_CUR_FRAME_INFO *pnext;   ///< Link to next property description. (Fill NULL if this is last item)
} IPL_SIE_CUR_FRAME_INFO;

/********* IPL_INPUT_SIZE *********/

/**
    IPL frame size infomation
*/
typedef struct _IPL_CUR_INPUT_INFO {
	IPL_PROC_ID id;			///<input parameter, ipl process id
	UINT32 in_size_h;       ///<output parameter, ipl in size H
	UINT32 in_size_v;       ///<output parameter, ipl in size V
	UINT32 in_size_lofs;	///<output parameter, ipl in line offset
	struct IPL_CUR_INPUT_INFO *pnext;   ///< Link to next property description. (Fill NULL if this is last item)
} IPL_CUR_INPUT_INFO;



/********* CAL IPL input size information *********/

/**
    CAL IPL input size information
*/
typedef struct _IPL_IN_SIZE_INFO {
	IPL_PROC_ID id;                  ///<input parameter, ipl process id
	IPL_MODE ipl_mode;               ///<input parameter, ipl mode
	IPL_EXT_CTRL_EN ext_ctrl_en;	 ///< ipl external control enable
	UINT32 crop_ratio;               ///< sie crop ratio (H_ratio:bit[31:16], V_ratio:bit[15:0])
	UINT32 video_width;              ///<input parameter, video out size width
	UINT32 video_height;             ///<input parameter, video out size height
	UINT32 video_fps;                ///<input parameter, video fps*10
	UINT32 shdr_func_en;             ///<input parameter, shdr function enable/disable
	UINT32 ipl_in_width;             ///<output parameter, ipl input width
	UINT32 ipl_in_height;            ///<output parameter, ipl input height
	struct IPL_IN_SIZE_INFO *pnext;  ///< Link to next property description. (Fill NULL if this is last item)
} IPL_IN_SIZE_INFO;


/********* IPL_GET_CMD *********/
typedef union {
	IPL_MODE_INFOR mode_info;
	IPL_BUF_INFOR buf_info;
	IPL_IME_INFOR ime_info;
	IPL_CAP_INFOR cap_info;
	IPL_ENC_RST_INFOR enc_rst_info;
	IPL_DZOOM_INFOR dz_info;
	IPL_CA_INFOR ca_info;
	IPL_CA_SET_INFOR ca_set_info;
	IPL_LA_INFOR la_info;
	IPL_LA_SET_INFOR la_set_info;
	IPL_VA_INFOR va_info;
	IPL_SIE_CUR_FRAME_INFO sie_cur_frame_info;
	IPL_IN_SIZE_INFO ipl_input_size_info;
	IPL_CUR_INPUT_INFO ipl_cur_input_info;
} IPL_GET_CMD_DATA;

#define IPL_CMD_VAR2STR(x)   (#x)
#define IPL_CMD_DUMPD(x)      dump("%s = %d\r\n", IPL_CMD_VAR2STR(x), x)
#define IPL_CMD_DUMPH(x)      dump("%s = 0x%.8x\r\n", IPL_CMD_VAR2STR(x), x)

/**
     Get IPL command

     @param[in] cmd       ipl command (IPL_GET_CMD)
     @param[in,out] data  ipl command data

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_GetCmd(IPL_GET_CMD cmd, void *data);

/**
     IPL open
     initial ipl module

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_Open(void);

/**
     IPL close
     un-initial ipl module

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_Close(void);

/**
     check IPL module is open or not

     @return Description of data returned.
         - @b TRUE: ipl opened
         - @b FALSE: ipl closed
*/
extern BOOL IPL_IsOpen(void);

/**
     install IPL control id
*/
extern void Install_IPLCtrlID(void) _SECTION(".kercfg_text");

/**
     install IPL extend id
*/
extern void IPL_InstallID(void) _SECTION(".kercfg_text");

typedef ER(*IPL_CMD_CHGMODE_FP)(IPL_MODE_DATA *chg_mode, IPL_MODE_DATA *prv_mode);
typedef ER(*IPL_CMD_PROC_FP)(void *data);

typedef struct {
	CHAR *Name;					//Cmd String address
	IPL_CMD_PROC_FP ProcFp;    //IPL_CMD_PROC_FP address
}IPL_CMD_ITEM;

typedef struct {
	UINT32 TotalCnt;            	//total table count
	IPL_CMD_ITEM *items;			//command table pointer
} IPL_CMD_INFO;

/**

     IPL_GetCmdSETTab

     @note get command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetCmdSETTab(void);

/**

     IPL_GetCmdGETTab

     @note get command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetCmdGETTab(void);

/**

     IPL_GetExtCmdSETTab

     @note get extend command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetExtCmdSETTab(void) __attribute__((weak));

/**

     IPL_GetExtCmdGETTab

     @note get extend command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetExtCmdGETTab(void) __attribute__((weak));

/**

     IPL_SetDZoomFCB

     @note get digital zoom information
     @param[in] IPL_DZOOM(digital zoom information)
     @param[in/out] IPC_Dzoom_Info(no need assign memory)
     @return
         - @b void.
*/
extern void IPL_SetDZoomFCB(IPL_DZOOM *info, IPC_Dzoom_Info *set_info) __attribute__((weak));

/**

     IPL_SetSleepFCB

     @note enter sleep mode
     @param[in] IPL_SLEEP_INFO(enter sleep mode information)
     @return
         - @b void.
*/
extern void IPL_SetSleepFCB(IPL_SLEEP_INFO *info) __attribute__((weak));

/**

     IPL_SetWakeupFCB

     @note exit sleep mode
     @param[in] IPL_WAKEUP_INFO(exit sleep mode information)
     @return
         - @b void.
*/
extern void IPL_SetWakeupFCB(IPL_WAKEUP_INFO *info) __attribute__((weak));

/**
     IPL_GetCapRawFCB

     @note get cap raw flow
     @param[in] IPL_GET_CAP_RAW_DATA
     @return
         - @b void.
*/
extern void IPL_GetCapRawFCB(IPL_GET_CAP_RAW_DATA *info) __attribute__((weak));

/**
     IPL_SetPauseDMAFCB

     @note set pause dma
     @param[in] void
     @return
         - @b void.
*/
extern void IPL_SetPauseDMAFCB(IPL_PAUSEDMA_INFO *info) __attribute__((weak));

/**
     IPL_SetResumeDMAFCB

     @note set resume dma
     @param[in] void
     @return
         - @b void.
*/
extern void IPL_SetResumeDMAFCB(IPL_RESUMEDMA_INFO *info) __attribute__((weak));


/**

     IPL_SetImgRatioFCB

     @note set sensor crop & ime out information
     @param[in] IPL_SET_IMGINOUT_DATA
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @return
         - @b IPL_IME_PATH(modify path).
*/
extern IPL_IME_PATH IPL_SetImgInOutFCB(IPL_SET_IMGINOUT_DATA *info, IPC_IMEOut_Info *ipc_info_p1, IPC_IMEOut_Info *ipc_info_p2, IPC_IMEOut_Info *ipc_info_p3, IPC_IMEOut_Info *ipc_info_p4, IPC_IMEOut_Info *ipc_info_p5) __attribute__((weak));

/**

     IPL_SetVAFCB

     @note set va information
     @param[in] Info                va information
     @param[in, out] HalParaIdx     hal parameter index
     @param[in, out] Out            VA Out Addr
*/
extern void IPL_SetVAFCB(IPL_SET_VA_DATA *info, IPL_HAL_PARASET *hal_para_idx)__attribute__((weak));

/**

     IPL_GetIMEHalIdxFCB

     @note get ime Hal Idx
     @param[in] ime_info                ime information
     @param[in, out] HalParaIdx     hal parameter index
*/
extern void IPL_GetIMEHalIdxFCB(IPL_IME_INFOR *ime_info, IPL_HAL_PARASET *hal_para_idx)__attribute__((weak));

/**

     IPL_Stream2PauseFCB

     @note Set pause specific IPL
     @param[in] All IPL Current Mode Info
     @param[in] IPL Chg Mode Info
     @param[in] IPC Stop cmd for all Id
     @return
         - @b IPL Stop Flag.

*/
extern void IPL_Stream2PauseFCB(IPL_MODE_DATA **ipl_mode_info, IPL_MODE_DATA *chg_mode, UINT32 *trig_flg) __attribute__((weak));

/**

     IPL_Pause2StreamFCB

     @note Set Resume specific IPL
     @param[in] All IPL Current Mode Info
     @param[in] IPL Chg Mode Info
     @param[in] IPL Stop Flg
     @param[out] IPC cmd
     @return
         - @b void.

*/
extern void IPL_Pause2StreamFCB(IPL_MODE_DATA **ipl_mode_info, IPL_MODE_DATA *chg_mode, UINT32 *trig_flg, IPC_CTRL_CMD *proc_cmd) __attribute__((weak));

#endif //_IPL_CMD_H_
