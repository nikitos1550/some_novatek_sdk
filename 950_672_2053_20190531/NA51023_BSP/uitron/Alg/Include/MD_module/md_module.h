#ifndef MD_H
#define MD_H

#include    "ipl_header_infor_yuv.h"
#include    "ipl_hal_ime_info.h"
#include    "ErrorNo.h"

#define MD_MAX_LV_NUM 4
#define MD_MAX_REGION_NUM 4
#define MD_IMG_SIZE 48*68*4
#define MD_STRIPE_LEN 2592
#define MD_SIZE_FACTOR 10000

#define MD_FLOW_2 1


typedef struct _MD_DISTRICT {
	INT32  x;
	INT32  y;
	INT32  width;
	INT32  height;
	INT32  level;
	//UINT32 reserved[9];          ///< Reserved words
}MD_DISTRICT;

extern VOID md_install_id(VOID);
extern UINT32 md_set_lv_info(IME_HAL_MD_REGION* md_win_info, UINT32 id, UINT32 reserved);
//extern ER md_set_lv_info(HEADER_YUV_MD_INFO* md_win_info, UINT32 *lv_addr);
extern ER md_get_user_info(IME_HAL_MD_INFO *md_user_info);
extern ER md_get_src_size(UINT32 *src_w, UINT32 *src_h);
extern ER md_get_evt_rst(UINT32 input_addr, UINT32 output_addr, UINT32 dst_w, UINT32 dst_h, UINT32 type);
//  evt type: 0: learning based, 1: difference based
// post_type: 0: no post-processing
//            1: 1 erosion + 2 dilation
//            2: 2 dilation + 1 erosion
//            3: 2 dilation
//            4: 1 dilation
//            5: 1 erosion
//            6: 1 dilation + 1 erosion
//            7: 1 erosion + 1 dilation
extern ER md_get_evt_rst_v2(UINT32 input_addr, UINT32 output_addr, UINT32 dst_w, UINT32 dst_h, UINT32 evt_type, UINT32 post_type);
extern ER md_get_evt_rst_v3(UINT32 input_addr, UINT32 output_addr, UINT32 dst_w, UINT32 dst_h, UINT32 evt_type, UINT32 post_type, UINT32 fg_val, UINT32 ref_cell_en);
//extern ER md_set_lv_addr(UINT32 addr);
extern ER md_set_user_lv_info(MD_DISTRICT *region, UINT32 region_num);
extern ER md_set_param(VOID* p_param, BOOL usage);
extern ER md_set_sc_th(UINT32 val);
extern ER md_set_cellmap_addr(UINT32 addr);
//extern ER md_get_lv_addr(UINT32 addr);
extern ER md_chg_reg(INT32 type, UINT32 reg_val);
extern ER md_chg_param(VOID* p_param_info);
extern UINT32 md_calc_buf_size(VOID);
extern UINT32 md_get_evt_size(UINT32 max_height);
extern ER md_init(UINT32 buf);
extern ER md_uninit(VOID);


#endif