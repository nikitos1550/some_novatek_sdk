#ifndef _IQ_ALG_H_
#define _IQ_ALG_H_

#include "Type.h"
#include "ipl_hal_ime_info.h"

typedef struct _TMNR_AUTO_INFO {
    BOOL  enable;               //0~1, default=1
    INT32 coefa_Max;            //0~48, 32 is one; depends on sensor gain (default=48 or setting manually as sensor_gain/20*32)
    INT32 blkMeanY_minTh;       //Y_min is the lower-bound of Y value range
    INT32 blkMeanY_maxTh;       //Y_max is the upper-bound of Y value range
    INT32 blkEdgVarY_maxTh;     //default=20
    INT32 LearnRate;            //1~4, Noise profile learning rate (1:0.25, 2:0.5, 3:0.75, 4:1.0; default=2), higher is learning faster but less stable
} TMNR_AUTO_INFO;

typedef enum {
	TMNR_NOISE_MODEL_ALL,               ///< Calcuate noise model of Y, Cb, Cr
    TMNR_NOISE_MODEL_Y,                 ///< Calcuate noise model of Y
	TMNR_NOISE_MODEL_CB,                ///< Calcuate noise model of Cb
	TMNR_NOISE_MODEL_CR,                ///< Calcuate noise model of Cr
	TMNR_NOISE_MODEL_NONE,              ///< Calcuate bypass
	ENUM_DUMMY4WORD(TMNR_NOISE_MODEL)
} TMNR_NOISE_MODEL;

extern void iq_tmnr_set_auto_info(TMNR_AUTO_INFO *tmnr_auto_info_tmp);
extern void iq_tmnr_set_skip_factor(UINT32 value);
extern BOOL iq_tmnr_parse_stats_data(UINT32 addr, UINT32 x_blk_num, UINT32 y_blk_num);
extern BOOL iq_tmnr_get_auto_enable(void);
extern IME_HAL_TMNR_NOISE_MODEL_INFO *iq_tmnr_get_noise_model(void);
extern void iq_tmnr_set_noise_estimation_sad(INT32 x_blk_num, INT32 y_blk_num, TMNR_NOISE_MODEL noise_model);

#endif
