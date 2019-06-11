#ifndef _ALG_IPCAM_PHOTOFUNCINFO_H
#define _ALG_IPCAM_PHOTOFUNCINFO_H

#include "photo_task.h"
#include "vqa_lib.h"
#include "md_lib.h"
#include "pd_lib.h"
#include "bc_lib.h"
#include "nvtcodec_event.h"

//extern void IPCam_InitPhotoFunc(void);
extern void Photo_InstallCmd(void);

//extern PHOTO_FUNC_INFO PhotoFuncInfo_dummy;
//extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_motion;
//extern PHOTO_FUNC_INFO PhotoFuncInfo_vqa;
//extern PHOTO_FUNC_INFO PhotoFuncInfo_pd;
//extern PHOTO_FUNC_INFO PhotoFuncInfo_bc;
#define IPCAm_MAX_MD_REGION 4

#if IPCAM_FD_FUNC
#include "fd_lib.h"
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamfd;
#endif

//#NT#2017/12/26#Yuzhe Bian -begin
//#NT# Face Tracking & Grading
#if IPCAM_FTG_FUNC
#include "ftg_lib.h"
//extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_ftg;
//extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_ftgfd;
#endif
//#NT#2017/12/26#Yuzhe Bian -end

#if (IPCAM_MD_FUNC == ENABLE)
//extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_motion;
#endif

typedef struct {
	UINT32  is_enabled;
	UINT32  start_block_x;
	UINT32  start_block_y;
	UINT32  end_block_x;
	UINT32  end_block_y;
	UINT32  alarm_th;
	UINT32  alarm;
}USR_RECT;
typedef struct {
    MD_USR_RECT sub_region[IPCAm_MAX_MD_REGION];
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
	UINT32  event_postprocessing;
	UINT32	tamper_th;
	UINT32  tamper_frm_num;
	UINT32  md_rst;
	UINT32  ref_cell_en;
	UINT8   cell_map[MD_USR_MAX_EVT_SIZE];
	UINT32	reserved[6];
}USR_GLOBAL_INFO;

typedef struct {
	UINT32  x;
	UINT32  y;
	UINT32  width;
	UINT32  height;
	UINT32  level;
}USR_DISTRICT;

typedef struct {
    UINT32	alpha;
	UINT32	tb;
	UINT32	sigma;
	UINT32	init_val;
	UINT32	tg;
	UINT32	lum_diff_th;
	UINT32	tex_diff_th;
	UINT32	tex_th;
	UINT32	tex_ratio_th;
	UINT32	reserved[6];
}USR_MD1_LV_INFO;

typedef struct {
	UINT32	tbg;
    USR_MD1_LV_INFO lv_info[IPCAm_MAX_MD_REGION];
	UINT32	reserved[6];
}USR_MD1_INFO;

typedef struct {
	UINT32  time_period;
    UINT32	lv_gm_md2_th[IPCAm_MAX_MD_REGION];
	UINT32	reserved[6];
}USR_MD2_INFO;

typedef struct {
	UINT32  update_prob;
    UINT32	update_neighbor_en;
	UINT32  deghost_en;
	UINT32  th_feature;
	UINT32  sensi;

	UINT32	reserved[6];
}USR_MD3_INFO;

typedef struct {
    USR_GLOBAL_INFO  global_info;
	USR_MD1_INFO     md1_info;
	USR_MD2_INFO     md2_info;
    USR_MD3_INFO     md3_info;
	UINT32	         reserved[6];
}USR_PARAM_INFO;

typedef struct {
	UINT32 fps; //fps=0 means turn off
	UINT32 md_mode;
	UINT32           dist_region_nu;
	USR_DISTRICT     dist_region[IPCAm_MAX_MD_REGION];
	USR_PARAM_INFO param_info;	
	UINT32	reserved[6];
} MDParam;

typedef struct {
	UINT32 fps; //fps=0 means turn off
	UINT32 bc_mode;
	BC_USR_PARAM param_info;
	UINT32	reserved[6];
} BCParam;

void Movie_VqaSetFps(UINT32 fps);
vqa_res_t Movie_VqaGetResult(void);
void Movie_PdSetFps(UINT32 fps);
PD_RESULT *Movie_PdGetResult(void);
void Movie_set_motion_param(MDParam *param);
MDParam * Movie_get_motion_param(void);
#if (IPCAM_DIS_MV_FUNC == ENABLE)
//extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamdis;
#endif

#if (IPCAM_MD_SAMPLE_660_STYLE == ENABLE)
//extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_md_sample_660;

extern void Movie_MD660Lock(BOOL enable);
#endif

#if (IPCAM_BC_FUNC == ENABLE)
void Movie_SetBcParam(BCParam *param);
BC_RESULT *Movie_BcGetResult(void);

extern void IPCam_BCLock(BOOL enable);
extern void IPCam_BCYUVEnable(BOOL enable);
#endif


extern void IPCam_InitPhotoFunc(void);
#endif


