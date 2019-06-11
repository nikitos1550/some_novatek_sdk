/**
    Public header file for image setting information

    This file is the header file that define the API and data type for image information.

    @file       IPL_HeaderInfor_yuv.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_HEADER_YUV_INFOR_H_
#define _IPL_HEADER_YUV_INFOR_H_

#include "Type.h"
#include "ipl_utility.h"
#include "ipl_cmd.h"

/** \addtogroup mIIPL */
//@{

/**
    IPL yuv header information selection index
*/
typedef enum _IPL_HEADER_YUV_INFOR {
	IPL_HEADER_YUV_PROC_ID =            0,      ///< data type UINT32
	IPL_HEADER_YUV_PATH =               1,      ///< data type UINT32
	IPL_HEADER_YUV_FRAMECNT =           2,      ///< data type UINT32
	IPL_HEADER_YUV_TIMESTAMP_START =    3,      ///< data type UINT32
	IPL_HEADER_YUV_TIMESTAMP_END =      4,      ///< data type UINT32
	IPL_HEADER_YUV_RAW_INFO =           5,      ///< data type HEADER_YUV_RAW_INFO
	IPL_HEADER_YUV_ENCODE_INFO =        6,      ///< data type UINT32
	IPL_HEADER_YUV_STRIPE_INFO =        7,      ///< data type HEADER_YUV_STRIPE_INFO
	IPL_HEADER_YUV_IMG_INFO =           8,      ///< data type IPL_YCC_IMG_INFO
	IPL_HEADER_YUV_MD_INFO =            9,      ///< data type HEADER_YUV_MD_INFO
	IPL_HEADER_YUV_MD_EVT =             10,     ///< data type HEADER_YUV_MD_EVT
	IPL_HEADER_YUV_INFOR_MAX_CNT,
	ENUM_DUMMY4WORD(IPL_HEADER_YUV_INFOR)
} IPL_HEADER_YUV_INFOR;


typedef enum {
	HEADER_YUV_STS_PROC_ID			=	(1 << IPL_HEADER_YUV_PROC_ID),
	HEADER_YUV_STS_PATH				=	(1 << IPL_HEADER_YUV_PATH),
	HEADER_YUV_STS_FRAMECNT			=	(1 << IPL_HEADER_YUV_FRAMECNT),
	HEADER_YUV_STS_TIMESTAMP_START	=	(1 << IPL_HEADER_YUV_TIMESTAMP_START),
	HEADER_YUV_STS_TIMESTAMP_END	=	(1 << IPL_HEADER_YUV_TIMESTAMP_END),
	HEADER_YUV_STS_RAWINFO			=	(1 << IPL_HEADER_YUV_RAW_INFO),
	HEADER_YUV_STS_ENCODEINFO		=	(1 << IPL_HEADER_YUV_ENCODE_INFO),
	HEADER_YUV_STS_STRIPEINFO		=	(1 << IPL_HEADER_YUV_STRIPE_INFO),
	HEADER_YUV_STS_IMG_INFO			=	(1 << IPL_HEADER_YUV_IMG_INFO),
	HEADER_YUV_STS_MD_INFO			=	(1 << IPL_HEADER_YUV_MD_INFO),
	ENUM_DUMMY4WORD(HEADER_YUV_STATUS)
} HEADER_YUV_STATUS;

typedef struct _HEADER_YUV_RAW_INFO {
	UINT32 proc_raw_num;        ///< number of raw ipl processed
	UINT32 sie_id[4];           ///< src sie id
	UINT32 framecnt[4];         ///< frame cnt of each raw(SIE)
	UINT64 timestamp[4];        ///< VD time stamp of each raw(SIE)
	UINT64 dramend_timestamp[4];///< dram end time stamp of each raw(SIE)
} HEADER_YUV_RAW_INFO;

typedef struct _HEADER_YUV_ENCODE_INFO {
	UINT32 enable;          ///< encode enable/disable
	UINT32 hevc_mode;       ///< hevc mode/h264 mode
	UINT32 sideinfo_addr;   ///< sideinfo_addr
	UINT32 uiKt0;           ///< K-table information for codec
	UINT32 uiKt1;           ///< K-table information for codec
	UINT32 uiKt2;           ///< K-table information for codec
} HEADER_YUV_ENCODE_INFO;

typedef struct _HEADER_YUV_STRIPE_INFO {
	UINT32 strp_num;        ///< stripe number
	UINT32 strp_size[16];   ///< dce calculated stripe size array
	///< decode used stripe size need to apply below calculation
	///< first/last stripe: strp_size[i]-(ipe_overlap/2)-(ime_overlap/2)
	///< other stripe:      strp_size[i]-(ipe_overlap)-(ime_overlap)
	UINT32 ipe_overlap;     ///< ipe overlap
	UINT32 ime_overlap;     ///< ime overlap
} HEADER_YUV_STRIPE_INFO;

typedef struct _HEADER_YUV_MD_INFO {
	UINT32 evt_addr;

	UINT32 md_in_h;
	UINT32 md_in_v;
	// md window info
	UINT32 win_x[4];
	UINT32 win_y;
	UINT32 win_x_num[4];
	UINT32 win_y_num;
	UINT32 win_x_size;
	UINT32 win_y_size;
	UINT32 win_x_st[4];
	UINT32 win_width_num;
	UINT32 win_dxdy;

	// md event flag
	UINT32 tamper_evt;
	UINT32 scence_change_evt;
} HEADER_YUV_MD_INFO;

typedef struct _HEADER_YUV_MD_EVT {
	UINT32 tamper_evt;
	UINT32 scence_change_evt;
} HEADER_YUV_MD_EVT;

/**
     raw yuv structure

*/
typedef struct _HEADER_YUV_DATA {
	UINT32 start_tag;
	UINT32 status;
	UINT32 Proc_id;                     ///< ipl id
	UINT32 Path_id;                     ///< ime path id
	IPL_YCC_IMG_INFO img_info;          ///< image info

	UINT32 ycc_compress_en;             ///< encode information, for decode used
	HEADER_YUV_STRIPE_INFO stripe_info; ///< stripe information, for decode used
	HEADER_YUV_MD_INFO ime_md_info;     ///< ime md information, for md lib used

	UINT32 ipl_framecnt;                ///< ime frame counter
	UINT64 ipl_timestamp_start;         ///< ipl timestamp trigger start
	UINT64 ipl_timestamp_end;           ///< ipl timestamp ime_end

	HEADER_YUV_RAW_INFO raw_info;       ///< sie raw information
	UINT32 end_tag;

	struct _HEADER_YUV_DATA *p_next;    ///< next header address if exist
} HEADER_YUV_DATA;

/**
    IPL YUV Decode Information
*/
typedef struct _HEADER_YUV_DECODE_DATA {
	HEADER_YUV_ENCODE_INFO encode_info; ///< encode information, for decode used
	HEADER_YUV_STRIPE_INFO stripe_info; ///< stripe information, for decode used
} HEADER_YUV_DECODE_DATA;


/**
     IPL YUV header get header size

     @return Description of data returned.
         - @b UINT32:    header size
*/
extern UINT32 IPL_HeaderYuvGetBufSize(void);
//@}
#endif //_IPL_HEADER_YUV_INFOR_H_
