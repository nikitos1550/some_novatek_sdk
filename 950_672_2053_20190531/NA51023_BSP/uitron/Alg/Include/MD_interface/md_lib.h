#ifndef MD_LIB_H
#define MD_LIB_H

//#include    "ipl_header_infor_yuv.h"
//#include    "ipl_hal_ime_info.h"
#include    "ErrorNo.h"



#define MD_MAX_SUB_REGION_NUM 4
#define MD_USR_MAX_LV_NUM 4
#define MD_USR_MAX_OBJ_NUM 128
//#define MD_USR_MAX_EVT_SIZE 8192
#define MD_USR_MAX_EVT_SIZE 10800
typedef struct _MD_USR_OBJ_T {
	INT32 start_x;
	INT32 start_y;
	INT32 end_x;
	INT32 end_y;
	INT32 label;
	UINT32 reserved[6];
}MD_USR_OBJ_T;

typedef struct _MD_USR_RST {
	UINT32 motion_alarm;        ///< alarm
	UINT32 in_num;
	UINT32 out_num;
	UINT32 obj_num;
	UINT8 status;
	UINT32 sub_motion_alarm[MD_MAX_SUB_REGION_NUM]; ///< sub alarm
	MD_USR_OBJ_T obj[MD_USR_MAX_OBJ_NUM]; ///< obj info
	UINT8 evt_map_addr[MD_USR_MAX_EVT_SIZE];        ///< evt map
	UINT32 tp_result;
	UINT32 reserved[5];
}MD_USR_RST;

typedef struct _MD_USR_DISTRICT {
	UINT32  x;
	UINT32  y;
	UINT32  width;
	UINT32  height;
	UINT32  level;
}MD_USR_DISTRICT;

typedef struct _MD_USR_RECT {
	UINT32  is_enabled;
	UINT32  start_block_x;
	UINT32  start_block_y;
	UINT32  end_block_x;
	UINT32  end_block_y;
	UINT32  alarm_th;
	UINT32  alarm;
}MD_USR_RECT;

typedef struct _MD_USR_MD1_LV_INFO {
    UINT32	alpha;
	UINT32	tb;
	UINT32	sigma;
	UINT32	init_val;
	UINT32	tg;
	UINT32	lum_diff_th;
	UINT32	tex_diff_th;
	UINT32	tex_th;
	UINT32	tex_ratio_th;
	UINT32  reserved[6];
}MD_USR_MD1_LV_INFO;

typedef struct _MD_USR_MD1_INFO{
	UINT32	tbg;
    MD_USR_MD1_LV_INFO lv_info[MD_USR_MAX_LV_NUM];
	UINT32	reserved[6];
}MD_USR_MD1_INFO;

typedef struct {
	UINT32  time_period;
    UINT32	lv_gm_md2_th[MD_USR_MAX_LV_NUM];
	UINT32	reserved[6];
}MD_USR_MD2_INFO;

typedef struct {
	UINT32  update_prob;
    UINT32	update_neighbor_en;
	UINT32  deghost_en;
	UINT32  th_feature;
	UINT32  sensi;
	UINT32	reserved[6];
}MD_USR_MD3_INFO;

typedef struct _MD_USR_GLOBAL_INFO {
    MD_USR_RECT sub_region[MD_MAX_SUB_REGION_NUM];
	UINT32  sub_region_num;
	UINT32  scene_change_th;
	UINT32	u_height;           /*  user define height
										suggest not change default value    */
	UINT32	u_width;            /*  user define width
										suggest not change default value    */
	UINT32	u_mb_height;        /*  user define mb height (suggest value = k_mb_widthh * 2)
										too large would affect the accuracy of motion detection */
	UINT32	u_mb_width;         /*  user define mb width (suggest value = k_mb_widthh * 2)
										too large would affect the accuracy of motion detection */
	UINT32	frame_count;
	UINT32	training_time;      /*  Needed training frames number for motion detection 
										application  */
	
	UINT32	md_type;
	UINT32	rotate;
	UINT32	flip;

	UINT32  enable_type;
	UINT32	mb_w_num;
	UINT32	mb_h_num;
	UINT32  alarm_th;

	UINT32  object_size;        /*  object size threshold	*/	
	UINT32  cross_type;         /*  none:0 cross_Entrance:1 cross_region:2  */
	UINT32  overlap_th;         /*  object overlapping threshold  */		
	UINT32	blk1_x;
	UINT32	blk1_y;
	UINT32	blk2_x;
	UINT32	blk2_y;
	UINT32	blk3_x;
	UINT32	blk3_y;
	UINT32	blk4_x;
	UINT32	blk4_y;

	UINT32	single_people_size;
	UINT32	single_w;
	UINT32	single_h;
	UINT32  event_postprocessing; /* morph post processing(d:dilation, e:erosion)=> 0: no process, 1: 1e2d, 2: 2d1e, 3: 1d1e, 4: 1e1d, 5:1d, 6: 1e*/
	UINT32	tamper_th;
	UINT32  tamper_frm_num;
	UINT32  md_rst;
	UINT32  ref_cell_en;
	UINT8   cell_map[MD_USR_MAX_EVT_SIZE];
	//UINT8   cell_map[4450];
	UINT32	reserved[6];
}MD_USR_GLOBAL_INFO;

typedef struct _MD_USR_PARAM_INFO {
    MD_USR_GLOBAL_INFO  GLOBAL_INFO;
	MD_USR_MD1_INFO 	MD1_INFO;
	MD_USR_MD2_INFO 	MD2_INFO;
	MD_USR_MD3_INFO 	MD3_INFO;
	UINT32              reserved[6];
}MD_USR_PARAM_INFO;

typedef enum
{
	MD1_TBG,
	MD1_LV0_ALPHA,
	MD1_LV0_TB,
	MD1_LV0_SIGMA,
	MD1_LV0_INIT_VAL,
	MD1_LV0_TG,
	MD1_LV0_LUM_DIFF_TH,
	MD1_LV0_TEX_DIFF_TH,
	MD1_LV0_TEX_TH,
	MD1_LV0_TEX_RATIO_TH,
	MD1_LV1_ALPHA,
	MD1_LV1_TB,
	MD1_LV1_SIGMA,
	MD1_LV1_INIT_VAL,
	MD1_LV1_TG,
	MD1_LV1_LUM_DIFF_TH,
	MD1_LV1_TEX_DIFF_TH,
	MD1_LV1_TEX_TH,
	MD1_LV1_TEX_RATIO_TH,
	MD1_LV2_ALPHA,
	MD1_LV2_TB,
	MD1_LV2_SIGMA,
	MD1_LV2_INIT_VAL,
	MD1_LV2_TG,
	MD1_LV2_LUM_DIFF_TH,
	MD1_LV2_TEX_DIFF_TH,
	MD1_LV2_TEX_TH,
	MD1_LV2_TEX_RATIO_TH,
	MD1_LV3_ALPHA,
	MD1_LV3_TB,
	MD1_LV3_SIGMA,
	MD1_LV3_INIT_VAL,
	MD1_LV3_TG,
	MD1_LV3_LUM_DIFF_TH,
	MD1_LV3_TEX_DIFF_TH,
	MD1_LV3_TEX_TH,
	MD1_LV3_TEX_RATIO_TH,
	ENUM_DUMMY4WORD(MD_USR_MD1_PRMS)
}MD_USR_MD1_PRMS;

typedef enum
{
	MD2_TIME_PERIOD,
	MD2_LV0_GM_TH,
	MD2_LV1_GM_TH,
	MD2_LV2_GM_TH,
	MD2_LV3_GM_TH,
	ENUM_DUMMY4WORD(MD_USR_MD2_PRMS)
}MD_USR_MD2_PRMS;

typedef enum
{
	MD3_UPDATE_PROB,
	MD3_UPDATE_NEIGHBOR_EN,
	MD3_DEGHOST_EN,
	MD3_TH_FEATURE,
	MD3_SENSI,
	ENUM_DUMMY4WORD(MD_USR_MD3_PRMS)
}MD_USR_MD3_PRMS;

typedef enum
{
	MD_SUBREGION_NUM,
	MD_SCENE_CHG_TH,
	MD_USR_HEIGHT,
	MD_USR_WIDTH,
	MD_USR_MB_HEIGHT,
	MD_USR_MB_WIDTH,
	MD_USR_MB_H_NUM,
	MD_USR_MB_W_NUM,
	MD_FRM_CNT,
	MD_TRAINING_TIME,
	MD_TYPE,
	MD_ROTATE,
	MD_FLIP,
	MD_ENABLE_TYPE,
	MD_ALARM_TH,
	MD_OBJ_SIZE,
	MD_CROSS_TYPE,
	MD_OVERLAP_TH,
	MD_BLK1_X,
	MD_BLK1_Y,
	MD_BLK2_X,
	MD_BLK2_Y,
	MD_BLK3_X,
	MD_BLK3_Y,
	MD_BLK4_X,
	MD_BLK4_Y,
	MD_SINGLE_PEOPLE_SIZE,
	MD_SINGLE_W,
	MD_SINGLE_H,
	MD_EVT_POSTPROCESSING,
	MD_TP_TH,
	MD_TP_FRM_NUM,
	MD_RST,
	MD_SUBREGION_0_EN,
	MD_SUBREGION_0_START_BLK_X,
	MD_SUBREGION_0_START_BLK_Y,
	MD_SUBREGION_0_END_BLK_X,
	MD_SUBREGION_0_END_BLK_Y,
	MD_SUBREGION_0_ALARM_TH,
	MD_SUBREGION_1_EN,
	MD_SUBREGION_1_START_BLK_X,
	MD_SUBREGION_1_START_BLK_Y,
	MD_SUBREGION_1_END_BLK_X,
	MD_SUBREGION_1_END_BLK_Y,
	MD_SUBREGION_1_ALARM_TH,
	MD_SUBREGION_2_EN,
	MD_SUBREGION_2_START_BLK_X,
	MD_SUBREGION_2_START_BLK_Y,
	MD_SUBREGION_2_END_BLK_X,
	MD_SUBREGION_2_END_BLK_Y,
	MD_SUBREGION_2_ALARM_TH,
	MD_SUBREGION_3_EN,
	MD_SUBREGION_3_START_BLK_X,
	MD_SUBREGION_3_START_BLK_Y,
	MD_SUBREGION_3_END_BLK_X,
	MD_SUBREGION_3_END_BLK_Y,
	MD_SUBREGION_3_ALARM_TH,
	ENUM_DUMMY4WORD(MD_USR_GLOBAL_PRMS)
}MD_USR_GLOBAL_PRMS;

// set result & info from IPL
extern ER mdl_set_evt_addr(UINT32 evt_addr);
extern ER mdl_set_win_info(VOID* md_usr_info);
extern ER mdl_set_bc_info(UINT32 mb_w_num, UINT32 mb_h_num);
// get MD APP results
// func: 0 --> get event map only
//       1 --> get all results
extern ER mdl_get_result(MD_USR_RST* usr_result, UINT32 func);
extern ER mdl_get_param(MD_USR_PARAM_INFO *p_param_info);
extern UINT32 mdl_get_global_alarm(VOID);
extern UINT32 mdl_get_time_bound(VOID);
extern UINT32 mdl_get_evt_addr(VOID);
extern UINT32 mdl_get_global_prms(MD_USR_GLOBAL_PRMS name);
extern UINT32 mdl_get_md1_prms(MD_USR_MD1_PRMS name);
extern UINT32 mdl_get_md2_prms(MD_USR_MD2_PRMS name);
extern UINT32 mdl_get_md3_prms(MD_USR_MD3_PRMS name);
// set user parameters
extern ER mdl_set_cell_map(UINT32 input_addr, UINT32 mb_x_num, UINT32 mb_y_num);
extern ER mdl_set_user_lv_info(MD_USR_DISTRICT *region, UINT32 region_num);
extern ER mdl_set_param(MD_USR_PARAM_INFO *p_param_info);
extern VOID mdl_set_global_prms(MD_USR_GLOBAL_PRMS name, UINT32 val);
extern VOID mdl_set_md1_prms(MD_USR_MD1_PRMS name, UINT32 val);
extern VOID mdl_set_md2_prms(MD_USR_MD2_PRMS name, UINT32 val);
extern VOID mdl_set_md3_prms(MD_USR_MD3_PRMS name, UINT32 val);
extern VOID mdl_update_hw_prms(VOID);
// init md
extern ER mdl_init(UINT32 buf);
// uninit md
extern ER mdl_uninit(VOID);
// process md app
// cmd: 0b000001 --> check global motion alarm
//      0b000010 --> check sub region motion alarm
//      0b000100 --> size filter
//      0b001000 --> cross line detection
//      0b010000 --> forbidden zone detection
//      0b100000 --> tamper detection
// Note that: 
//   user can choose more than one functions & the process will do those functions simultaneously.
//   ex. 0b100001 --> check global motion alarm & tamper detection simultaneously.
//   but user should only choose cross line detection or forbidden zone detection at once.
extern ER mdl_process(UINT32 md_cmd);
// runtime change parameters
extern ER mdl_chg_param(MD_USR_PARAM_INFO *p_param_info);
// calculate buffer size
extern UINT32 mdl_calc_buf_size(void);


#endif