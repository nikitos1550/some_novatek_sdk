/**
    IPL_Ctrl module (ISR Event)

    @file       IPL_ISREvent.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_ISREVENT_H_
#define _IPL_ISREVENT_H_
#include "Type.h"
#include "ipl_utility.h"
#include "ipl_hal_op.h"
#include "ipl_ctrl_flow.h"
#include "dis_alg_fw.h"

/**
    Engine ISR enum
*/
typedef enum _IPL_ISR {
	IPL_ISR_UNKNOWN = IPL_HAL_UNKNOW,
	IPL_ISR_SIE1 = IPL_HAL_SIE1,   ///< SIE1
	IPL_ISR_SIE2 = IPL_HAL_SIE2,   ///< SIE2
	IPL_ISR_IFE  = IPL_HAL_IFE,    ///< IFE
	IPL_ISR_DCE  = IPL_HAL_DCE,    ///< DCE
	IPL_ISR_IPE  = IPL_HAL_IPE,    ///< IPE
	IPL_ISR_IME  = IPL_HAL_IME,    ///< IME
	IPL_ISR_IFE2 = IPL_HAL_IFE2,   ///< IFE2
	IPL_ISR_RHE  = IPL_HAL_RHE,    ///< RHE
	IPL_ISR_MAX_CNT = IPL_HAL_MAX,
	IPL_ISR_SIEx = 0x0000ffff, ///< for internal auto set
	ENUM_DUMMY4WORD(IPL_ISR)
} IPL_ISR;

typedef void (*IPL_ISR_EVENT_FP)(IPL_PROC_ID, UINT32, UINT32);
typedef void (*IPL_ISR_RAWHEADER_CB)(IPL_PROC_ID, UINT32);

/**
    ime datastamp callback

    param:
    IMESTAMP_INFO* info:    stamp info, set by callback function
    UINT32 ipl_id
    UINT32 width:           current ime input image width
    UINT32 height:          current ime input image height
    UINT32 reserved
*/
typedef void (*IPL_DATASTAMP_CB)(IMESTAMP_INFO *, UINT32, UINT32, UINT32, UINT32);

/**
    ime privacy mask callback

    param:
    IME_PRI_MASK_INFO* info:    privacy mask info, set by callback function,
                                PxlWidth, PxlHeight, PmPxlLofs will automatically calc by PmPxlBlkSize
    UINT32 ipl_id
    UINT32 width:               current ime input image width
    UINT32 height:              current ime input image height
    UINT32 reserved
*/
typedef void (*IPL_PM_CB)(IME_PRI_MASK_INFO *, UINT32, UINT32, UINT32, UINT32);

/**
    SIE RAW Post Process callback

    param:
    IPL_FRONTEND_IMG info:   raw info(addr, w, h...)
    UINT32 sie_id
    UINT32 reserved
*/
typedef void (*SIE_RAW_POSTPROC_CB)(IPL_FRONTEND_IMG, UINT32, UINT32);

/**
	ime motion detection callback

	param:
	IME_HAL_MD_REGION win_info:	md window information
	UINT32:						ipl_id
	UINT32:						reserved

	return value: MD LV Address
*/
typedef UINT32 (*IPL_MD_CB)(IME_HAL_MD_REGION*, UINT32, UINT32);


/**
	ipe eth end callback

	param:
	DISALG_PARAM dis_info:	eth size & addr
*/
typedef void (*IPL_DIS_CB)(DISALG_PARAM *);

typedef struct {
	IPL_ISR_EVENT_FP FP[IPL_ISR_MAX_CNT];
} IPL_ISR_EVENT_TAB;

typedef struct {
	IPL_ISR_EVENT_TAB EventTab;             ///<for IPL_lib inner modules
	IPL_ISR_EVENT_TAB CB_FP;                ///<for modules those are not in IPL_lib
	IPL_ISR_RAWHEADER_CB RAWHeader_CB;      ///<for raw hearder callback with ext_lib

	IPL_DATASTAMP_CB    datastamp_cb;       ///< for ime datastamp callback when trigger start
	IPL_PM_CB           pm_cb;              ///< for ime privacy mask callback when trigger start
	IPL_MD_CB			md_cb;				///< for ime motion detection lv address callback when trigger start

	SIE_RAW_POSTPROC_CB raw_postproc_cb;    ///< for sie raw post process callback when sie dramend
} IPL_ISR_OBJ_INFO;

/**
    IPL IMG out image info
*/
typedef struct _IPL_IMG_OUT_INFO {
	UINT32 Path_id;                     ///< ime path id
	UINT32 buf_handle;                  ///< buffer handle
	UINT32 img_out_tag;					///< tag for img out
	BOOL out_ycc_compress_en;           ///< ycc compress enable/disable, only for ime path1
	BOOL bsize_valid;           		///< output size valid, check this value when runtime change ime output size
	UINT32 raw_framecnt;				///< frame cnt of each raw(SIE)
	UINT64 vd_timestamp;         		///< vd timestamp,
	UINT64 dramend_timestamp;         	///< dram end timestamp,
	UINT64 ipl_timestamp_start;         ///< ipl timestamp, raw2yuv trigger start
	UINT64 ipl_timestamp_end;           ///< ipl timestamp ime_end
	UINT32 ipl_framecnt;				///< frame cnt of each ipl
	IPL_YCC_IMG_INFO img;
	UINT32 str_info_addr;
	UINT32 ime_md_addr;
} IPL_IMG_OUT_INFO;

/**
    IPL In/Out buffer config
*/
typedef enum _IPL_BUF_IO_CFG {
	IPL_BUF_IO_GET_NEXT_OUT_ADDR = 0,
	IPL_BUF_IO_PUSH_READY_ADDR,
	IPL_BUF_IO_LOCK_BUF,
	IPL_BUF_IO_UNLOCK_BUF,
	IPL_BUF_IO_START,
	IPL_BUF_IO_STOP,
	ENUM_DUMMY4WORD(IPL_BUF_IO_CFG)
} IPL_BUF_IO_CFG;
typedef UINT32(*IPL_OUT_BUF_CFG_CB)(IPL_BUF_IO_CFG, UINT32, IPL_IMG_OUT_INFO *);

ER IPL_BufIOCB_Init(UINT32 id, IPL_OUT_BUF_CFG_CB cb_fp);
UINT32 IPL_BufIOCB_Proc(UINT32 id, IPL_BUF_IO_CFG buf_io_cfg, UINT32 val, IPL_IMG_OUT_INFO *img_out_info);
extern void IPL_Set_Dis_CB(IPL_DIS_CB cb);

//@}
#endif //_IPL_ISREVENT_H_
